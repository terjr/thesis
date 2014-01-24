#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <istream>
#include <vector>

using std::vector;
using std::istream;

class Pest
{
    private:
        unsigned int numThreads;
        vector<istream*> *inputStreams;
        boost::asio::io_service ioService;
        boost::thread_group threadpool;
    public:
        Pest(std::vector<std::istream*> *inputs, unsigned int numThreads = 0);


        void processStreams();

        static int main(int argc, char** argv);
};
