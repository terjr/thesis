#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>

#include "Pest.hpp"
#include "SimpleModel.hpp"
#include "TraceLine.hpp"
#include "OutputFormatter.hpp"

using namespace std;

unsigned long numTicks(istream *is)
{
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
    return strtoul(str.c_str(), NULL, 10);
}

int readLines(istream *s, boost::lockfree::queue<string*> *q, boost::atomic<bool> *done)
{
    cout << "Reading lines" << endl;
    while (*s)
    {
        string* str = new string();
        std::getline(*s, *str);
        while(!q->push(str));
    }
    cout << "Done reading input file" << endl;
    *done = true;
    return 0;
}

int run(PowerModel *pm)
{
    return pm->run();
}

Pest::Pest(options_t &options) :
      numThreads(options.numThreads),
      done(false),
      count(0),
      lineQueue(128),
      output(options.output),
      options(options)
{

    if (options.error) {
        cerr << "Error while parsing program arguments." << endl;
    }

    if (options.numBuckets && options.bucketSize) {
        cerr << "Cannot have both bucket size and number of buckets defined at once!" << endl;
        return;
    }
    else if (options.numBuckets && !options.bucketSize)
        options.bucketSize = numTicks(options.inputStream)/options.numBuckets;
    else if (!options.numBuckets && !options.bucketSize)
        options.bucketSize = numTicks(options.inputStream)/1000;

    //boost::thread(&readLines, options.inputStream, &lineQueue, &done);
    this->ioService.post( boost::bind(readLines, options.inputStream, &lineQueue, &done) );

    this->pm = vector<PowerModel*>();

    // Assign tasks to the thread pool
    for (unsigned int i = 0; i < numThreads; ++i) { // one thread is reading the file
        SimpleModel *sm = new SimpleModel(&lineQueue, &done, options.bucketSize);
        this->ioService.post( boost::bind(run, sm) );
        pm.push_back(sm);
    }
}

Pest::~Pest() {
    for (unsigned long i = 0; i < this->pm.size(); ++i)
        delete pm[i];
}

void Pest::processStreams() {
    // Create worker threads
    for (unsigned int i = 0; i < this->numThreads; i++) {
        this->threadpool.create_thread( boost::bind(&boost::asio::io_service::run, &ioService) );
    }

    // Wait for threads to finish
    this->threadpool.join_all();

    unsigned long max_size = 0;
    for (unsigned long i = 0; i < this->pm.size(); ++i)
        if (this->pm[i]->getOutput().size() > max_size)
            max_size = this->pm[i]->getOutput().size();

    vector<unsigned long> results(max_size);
    for (unsigned long i = 0; i < this->pm.size(); ++i)
        for (unsigned long j = 0; j < this->pm[i]->getOutput().size(); ++j)
            results[j] += this->pm[i]->getOutput()[j];

    unsigned long normalize;
    if (options.bucketSize < 500000)
    {
        if (options.bucketSize >= 500)
        {
            normalize = options.bucketSize/500;
        }
        else if (options.bucketSize >= 5)
        {
            normalize = options.bucketSize/5;
        }
        else
        {
            std::cerr << "Unable to normalize results, " << std::endl;
                normalize = 1;
        }
    }
    else
    {
        normalize = options.bucketSize/500000;
    }

    for (unsigned long i = 0; i < results.size(); ++i)
        results[i] /= normalize;


    OutputFormatter gnuplotter(results);
    if (this->output.empty())
        gnuplotter.showBarchart();
    else
        gnuplotter.saveBarchart(this->output);
}


int Pest::main(int ac, char **av)
{
    unsigned int numThreads = boost::thread::hardware_concurrency();

    options_t progOptParsed = processProgramOptions(ac, av);
    if (progOptParsed.error)
        return 1;
    if (progOptParsed.help)
        return 0;

    if (progOptParsed.numThreads < 1)
        progOptParsed.numThreads = numThreads;

    Pest pest(progOptParsed);
    pest.processStreams();

    if (progOptParsed.inputStream != &cin)
        delete progOptParsed.inputStream;

    return 0;
}
