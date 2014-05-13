#pragma once
#include "QueueType.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <atomic>
#include <istream>
#include <vector>
#include <string>

#include "PowerModel.hpp"
#include "ProgramArguments.hpp"

/**
 * The Pet class is the main driver in the PET program, and is created and
 * executed from the real main function of the program.
 */
class Pet
{
    private:
        unsigned int numThreads;
        std::atomic<bool> done;
        std::atomic<unsigned long> count;
        boost::asio::io_service ioService;
        boost::thread_group threadpool;
        std::vector<lfspscqueue> lineQueue;
        std::vector<unsigned long> results;
        std::vector<std::map<const std::string, unsigned long>> eventStats;
        options_t &options;

        std::vector<PowerModel*> pm;
        unsigned long long int nTicks;
        void sumBuckets(const std::vector<PowerModel*> &in, std::vector<unsigned long> &out);
    public:
        Pet(options_t &options);
        Pet(std::istream *input, unsigned int numThreads = 0, const std::string &output = std::string());
        virtual ~Pet();

        void processStreams();
        void produceOutput() const;

        static int main(int argc, char** argv);
};

// Utility functions used to collect and prepare data for presentation.
unsigned long findPowerModelMaxSize(const std::vector<PowerModel*> &modelworkers);
void sumBuckets(const std::vector<PowerModel*> &in, std::vector<unsigned long> &out);
void sumStats(const std::vector<PowerModel*> &in, std::vector<std::map<const std::string, unsigned long>> &eventStats);
void normalize(const unsigned long bucketSize, const unsigned long staticPowerDrain, std::vector<unsigned long> &results);
