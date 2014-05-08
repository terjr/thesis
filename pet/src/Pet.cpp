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

int run(PowerModel *pm) {
    return pm->run();
}

Pet::Pet(options_t &options) :
    numThreads(options.numThreads),
    done(false),
    count(0),
    lineQueue(options.numThreads),
    options(options) {

        if (options.error) {
            cerr << "Error while parsing program arguments." << endl;
            return;
        }

        if (options.numBuckets && options.bucketSize) {
            cerr << "Cannot have both bucket size and number of buckets defined at once!" << endl;
            return;
        }
        unsigned long long int nTicks = numTicks(options.inputStream);
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

        //        for (unsigned int i = 0; i < numThreads; ++i)
        //            lineQueue[i] = new boost::lockfree::spsc_queue<std::string*, boost::lockfree::capacity<8192>>();

        this->ioService.post( boost::bind(readLines, options.inputStream, &lineQueue, lineQueue.size(), &done) );


        this->pm = vector<PowerModel*>();

        // Assign tasks to the thread pool
        for (unsigned int i = 0; i < lineQueue.size(); ++i) {
            SimpleModel *sm = new SimpleModel(&(lineQueue[i]), &done, options.annotations, options.weights, options.bucketSize, nTicks, options.stats);
            nTicks = 0;
            this->ioService.post( boost::bind(run, sm) );
            pm.push_back(sm);
        }
    }

Pet::~Pet() {
    for (unsigned long i = 0; i < this->pm.size(); ++i) {
        delete pm[i];
    }
    delete options.annotations;
}


void Pet::processStreams() {
    // Create worker threads
    for (unsigned int i = 0; i < this->numThreads; i++) {
        this->threadpool.create_thread( boost::bind(&boost::asio::io_service::run, &ioService) );
    }

    // Wait for threads to finish
    this->threadpool.join_all();
    cout << "\nDone! Processing graphs..." << endl;

    // Collect and massage data
    unsigned long maxSize = findWorkerMaxSize(this->pm);
    vector<unsigned long> results(maxSize);
    map<const string, unsigned long> eventStats;

    sumBuckets(this->pm, results);
    sumStats(this->pm, eventStats);
    normalize(options.bucketSize, this->pm[0]->getWeight("Static"), results);

    OutputFormatter gnuplotter(results, &options);
    for (unsigned long i = 0; i < this->pm.size(); ++i) {
        auto annot = this->pm[i]->getAnnotations();
        gnuplotter.addAnnotations(annot);
    }
    gnuplotter.produceOutput();

    // Will only print anything if map is filled, not done when stats = false
    for (auto it = eventStats.begin(); it != eventStats.end(); ++it)
        cout << it->first << ": " << it->second << endl;

}


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

    Pet pest(progOptParsed);
    pest.processStreams();

    if (progOptParsed.inputStream != &cin)
        delete progOptParsed.inputStream;

    return 0;
}

unsigned long findWorkerMaxSize(vector<PowerModel*> &modelworkers) {
    // Find max size among all PowerModel workers
    unsigned long maxSize = 0;
    for (unsigned long i = 0; i < modelworkers.size(); ++i) {
        if (modelworkers[i]->getOutput().size() > maxSize)
            maxSize = modelworkers[i]->getOutput().size();
    }
    return maxSize;
}

void setEqualSize(const vector<PowerModel*> &in)
{
    unsigned long maxSize = 0;
    for (unsigned long i = 0; i < in.size(); ++i)
        if (in[i]->getOutput().size() > maxSize)
            maxSize = in[i]->getOutput().size();

    for (unsigned long i = 0; i < in.size(); ++i)
        if (in[i]->getOutput().size() < maxSize)
        {
            in[i]->setOutputSize(maxSize);
        }
}

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

void sumStats(const vector<PowerModel*> &in, map<const string, unsigned long> &eventStats) {
    for (unsigned long i = 0; i < in.size(); ++i) {
        auto stats = in[i]->getStats();
        for (auto pit = stats.begin(); pit != stats.end(); ++pit) {
            eventStats[pit->first] += pit->second;
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

