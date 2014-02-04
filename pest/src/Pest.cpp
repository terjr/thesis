#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>

#include "Pest.hpp"
#include "ProgramArguments.hpp"
#include "SimpleModel.hpp"
#include "TraceLine.hpp"
#include "OutputFormatter.hpp"

using namespace std;

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

Pest::Pest(istream *input, unsigned int numThreads, const string &output) : done(false), count(0), lineQueue(128), output(output) {
    this->numThreads = numThreads;

    // Assign tasks to the thread pool
    boost::thread(&readLines, input, &lineQueue, &done);

    this->pm = vector<PowerModel*>();

    for (unsigned int i = 0; i < numThreads; ++i)
    {
        SimpleModel *sm = new SimpleModel(&lineQueue, &done);
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


    OutputFormatter gnuplotter(results);
    if (this->output.empty())
        gnuplotter.showBarchart();
    else
        gnuplotter.saveBarchart(this->output);
}


int Pest::main(int ac, char **av)
{
    unsigned int numThreads = boost::thread::hardware_concurrency();

    istream *inputStream;
    string output;
    bool progOptParsed = processProgramOptions(ac, av, &inputStream, numThreads, output);
    if (!progOptParsed)
        return 1;

    Pest pest(inputStream, numThreads, output);
    pest.processStreams();

    if (inputStream != &cin)
        delete inputStream;

    return 0;
}
