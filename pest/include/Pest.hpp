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

class Pest
{
    private:
        unsigned int numThreads;
        boost::atomic<bool> done;
        boost::atomic<unsigned long> count;
        boost::asio::io_service ioService;
        boost::thread_group threadpool;
        boost::lockfree::queue<std::string*> lineQueue;
        std::string output;
        options_t &options;

        std::vector<PowerModel*> pm;
    public:
        Pest(options_t &options);
        Pest(std::istream *input, unsigned int numThreads = 0, const std::string &output = std::string());
        virtual ~Pest();

        void processStreams();

        static int main(int argc, char** argv);
};
