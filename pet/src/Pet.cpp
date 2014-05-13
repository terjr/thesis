#include <fstream>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>

#include "Pet.hpp"
#include "SimpleModel.hpp"
#include "TraceLine.hpp"
#include "OutputFormatter.hpp"

using namespace std;

unsigned long long int numTicks(istream *is) {
    if (!dynamic_cast<ifstream*>(is)) return 0; // check that stream is from file

    is->seekg(0, is->end);
    unsigned long length = is->tellg();
    length-=3;
    while(is->peek() != '\n') {
        is->seekg(length--);
    }
    is->seekg(length+2);

    string str;
    getline(*is, str);
    is->seekg(0, is->beg);
    return strtoull(str.c_str(), NULL, 10);
}

int readLines(istream *s,
        std::vector<lfspscqueue> *q,
        unsigned int numQueues,
        std::atomic<bool> *done)
{
    cout << "Reading lines" << endl;
    unsigned int i = 0;
    while (*s)
    {
        string * str = new string();
        std::getline(*s, *str);
        while (!(*q)[i].push(str))
        {
            i++;
            i %= numQueues;
        }
    }
    cout << "\nDone reading input file" << endl;
    *done = true;
    return 0;
}

int doWork(PowerModel *pm) {
    return pm->run();
}

/**
 * Create a Pet instance and run it with a configuration set.
 * @see options_t
 */
Pet::Pet(options_t &options) :
    numThreads(options.numThreads),
    done(false),
    count(0),
    lineQueue(options.numThreads),
    results(),
    eventStats(),
    options(options) {

        if (options.error) {
            cerr << "Error while parsing program arguments." << endl;
            return;
        }

        if (options.numBuckets && options.bucketSize) {
            cerr << "Cannot have both bucket size and number of buckets defined at once!" << endl;
            return;
        }
        nTicks = numTicks(options.inputStream);
        if (!options.bucketSize) {
            if (!nTicks)
                options.bucketSize = 100000;
            else if (options.numBuckets)
                options.bucketSize = nTicks/options.numBuckets;
            else
                options.bucketSize = nTicks/1000;
        }

        if (isVerbose())
        {
            printf("Found %lld ticks in input file, using %lld buckets of size %d\n", nTicks, nTicks/options.bucketSize, options.bucketSize);
            printf("Buckets pr. sec: %f\n", 1000000000000.0/options.bucketSize);
        }

        this->ioService.post( boost::bind(readLines, options.inputStream, &lineQueue, lineQueue.size(), &done) );


        this->pm = vector<PowerModel*>();
    }

/**
 * Clean up all PowerModels and
 * annotations used in Pet along with
 * destroying the Pet object itself
 */
Pet::~Pet() {
    if (options.inputStream != &cin)
        delete options.inputStream;

    for (unsigned long i = 0; i < this->pm.size(); ++i) {
        delete pm[i];
    }
    delete options.annotations;
}

/**
 * Add worker threads to the thread pool and
 * start parsing input
 */
void Pet::processStreams() {
    // Assign tasks to the thread pool
    for (unsigned int i = 0; i < lineQueue.size(); ++i) {
        SimpleModel *sm = new SimpleModel(&(lineQueue[i]), &done, options.annotations, options.weights, options.bucketSize, nTicks, options.stats);
        nTicks = 0;
        this->ioService.post( boost::bind(doWork, sm) );
        pm.push_back(sm);
    }

    // Create worker threads
    for (unsigned int i = 0; i < this->numThreads; i++) {
        this->threadpool.create_thread( boost::bind(&boost::asio::io_service::run, &ioService) );
    }

    // Wait for threads to finish
    this->threadpool.join_all();
    cout << "\nDone! Processing graphs..." << endl;

    // Collect and massage data
    unsigned long maxSize = findPowerModelMaxSize(this->pm);
    results = vector<unsigned long>(maxSize);

    sumBuckets(this->pm, results);
    sumStats(this->pm, eventStats);
    normalize(options.bucketSize, this->pm[0]->getWeight("Static"), results);
}

/**
 * Produce output using an instance of OutputFormatter
 */
void Pet::produceOutput() const {
    OutputFormatter gnuplotter(results, &options);
    for (unsigned long i = 0; i < this->pm.size(); ++i) {
        auto annot = this->pm[i]->getAnnotations();
        gnuplotter.addAnnotations(annot);
    }
    gnuplotter.produceOutput();

    // Will only print anything if map is filled, not done when stats = false
    if (options.stats) {
        ostream *stats_out;
        if (options.statsOutput.empty())
            stats_out = &cout;
        else
            stats_out = new ofstream(options.statsOutput);
        *stats_out << options.bucketSize << endl;

        *stats_out << "[";
        for (unsigned long i = 0; i < eventStats.size(); i++) {
            auto it = options.weights->begin();
            *stats_out << "{";
            while (it != options.weights->end()) {
                auto stat = eventStats[i].find(it->first);
                *stats_out << "'" << it->first << "':";
                if (stat != eventStats[i].end())
                    *stats_out << stat->second;
                else
                    *stats_out << "0";
                if (++it != options.weights->end())
                    *stats_out << ", ";
            }
            if (i < eventStats.size()-1)
                *stats_out << "},";
            else
                *stats_out << "}";
        }
        *stats_out << "]";
        if (stats_out != &cout)
            delete stats_out;
    }
}

/**
 * This method will kick off PET along
 * with the program arguments supplied to
 * the real main function
 */
int Pet::main(int ac, char **av)
{
    unsigned int numThreads = boost::thread::hardware_concurrency();

    options_t progOptParsed = processProgramOptions(ac, av);
    if (progOptParsed.error)
        return 1;
    if (progOptParsed.help)
        return 0;

    if (progOptParsed.numThreads < 1)
        progOptParsed.numThreads = numThreads;

    Pet pet(progOptParsed);
    pet.processStreams();
    pet.produceOutput();

    return 0;
}

/**
 * Find the maximum vector size amongst a set of PowerModels
 */
unsigned long findPowerModelMaxSize(const vector<PowerModel*> &modelworkers) {
    unsigned long maxSize = 0;
    for (unsigned long i = 0; i < modelworkers.size(); ++i) {
        if (modelworkers[i]->getOutput().size() > maxSize)
            maxSize = modelworkers[i]->getOutput().size();
    }
    return maxSize;
}

/**
 * Let all PowerModel output vectors be of the same size,
 * that is, increase everyone to the size of the largest
 */
void setEqualSize(const vector<PowerModel*> &in)
{
    unsigned long maxSize = findPowerModelMaxSize(in);

    for (unsigned long i = 0; i < in.size(); ++i)
        if (in[i]->getOutput().size() < maxSize)
        {
            in[i]->setOutputSize(maxSize);
        }
}

/**
 * Concatenate all PowerModel bucket vectors into a single vector.  Each bucket
 * in the new vector is equal to the sum of the same bucket index of all the
 * input vectors
 */
void Pet::sumBuckets(const vector<PowerModel*> &in, vector<unsigned long> &out) {
    setEqualSize(in);
    for (unsigned long j = 0; j < in[0]->getOutput().size(); ++j)
    {
        unsigned long numEvents = 0;
        for (unsigned long i = 0; i < in.size(); ++i)
        {
            out[j] += in[i]->getOutput()[j].sum;
            numEvents += in[i]->getOutput()[j].num;
        }
        long idleTicks = (options.bucketSize / options.ticksInCycle) - numEvents;
        if (idleTicks > 0)
            out[j] +=  in[0]->getWeight("Idle")*idleTicks;
    }
}

void sumStats(const vector<PowerModel*> &in, vector<map<const string, unsigned long>> &eventStats) {
    if (in.size() > 0)
        while (eventStats.size() < in[0]->getStats().size())
            eventStats.push_back(std::map<const std::string, unsigned long>());

    for (unsigned long i = 0; i < in.size(); ++i) {
        unsigned long bucket = 0;
        auto stats = in[i]->getStats();
        for (auto vit = stats.begin(); vit != stats.end(); ++vit) {
            for (auto mit = vit->begin(); mit != vit->end(); ++mit) {
                eventStats[bucket][mit->first] += mit->second;
            }
            bucket++;
        }
    }
}

void normalize(const unsigned long bucketSize, const unsigned long staticPowerDrain, vector<unsigned long> &results) {
    double staticDrain;
    double normalize;
    if (bucketSize < 500) {
        if (bucketSize >= 5) {
            cerr << "Very small buckets found, scale is amps x 10^-5" << std::endl;
            normalize = bucketSize/5;
            staticDrain = staticPowerDrain / 100;
        } else {
            cerr << "Unreliably small bucket size, unable to normalize results, output is " << std::endl;
            normalize = 1;
            staticDrain = staticPowerDrain / 500;
        }
    } else {
        normalize = bucketSize/500;
        staticDrain = staticPowerDrain;
    }

    for (unsigned long i = 0; i < results.size(); ++i)
        results[i] = (results[i] / normalize) + staticPowerDrain;
}

