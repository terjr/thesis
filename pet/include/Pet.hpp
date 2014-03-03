#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <istream>
#include <vector>
#include <string>

#include "PowerModel.hpp"
#include "ProgramArguments.hpp"

using std::vector;
using std::istream;

class Pet
{
    private:
        unsigned int numThreads;
        boost::atomic<bool> done;
        boost::atomic<unsigned long> count;
        boost::asio::io_service ioService;
        boost::thread_group threadpool;
        boost::lockfree::queue<std::string*, boost::lockfree::fixed_sized<FIXED> > lineQueue;
        options_t &options;

        std::vector<PowerModel*> pm;
    public:
        Pet(options_t &options);
        Pet(std::istream *input, unsigned int numThreads = 0, const std::string &output = std::string());
        virtual ~Pet();

        void processStreams();

        static int main(int argc, char** argv);
};

// Utility functions used to collect and prepare data for presentation.
unsigned long findWorkerMaxSize(vector<PowerModel*> &modelworkers);
void sumBuckets(const vector<PowerModel*> &in, vector<unsigned long> &out);
void normalize(const unsigned long bucketSize, vector<unsigned long> &results);