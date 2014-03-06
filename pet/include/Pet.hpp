#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <atomic>
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
        std::atomic<bool> done;
        std::atomic<unsigned long> count;
        boost::asio::io_service ioService;
        boost::thread_group threadpool;
        std::vector<boost::lockfree::spsc_queue<std::string*, boost::lockfree::capacity<8192>>> lineQueue;
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
void normalize(const unsigned long bucketSize, const unsigned long staticPowerDrain, vector<unsigned long> &results);
