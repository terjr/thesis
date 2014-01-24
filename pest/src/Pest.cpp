#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "Pest.hpp"
#include "TraceLine.hpp"
#include "ProgramArguments.hpp"

using namespace std;

int printStream(istream* s)
{
    while (*s)
    {
        string str;
        std::getline(*s, str);
        cout << str << '\n' << TraceLine(str) << endl << endl;
    }
    return 0;
}

Pest::Pest(vector<istream*> *inputs, unsigned int numThreads) {
    this->inputStreams = inputs;
    this->numThreads = numThreads;
    // Assign tasks to the thread pool.
    for (vector<istream*>::iterator it = inputs->begin(); it != inputs->end(); ++it) {
        this->ioService.post( boost::bind(printStream, *it) );
    }

}

void Pest::processStreams() {
    // Create worker threads
    for (unsigned int i = 0; i < this->numThreads && i < this->inputStreams->size(); i++) {
        this->threadpool.create_thread( boost::bind(&boost::asio::io_service::run, &ioService) );
    }

    // Wait for threads to finish
    this->threadpool.join_all();
}


int Pest::main(int ac, char **av)
{
    unsigned int numThreads = boost::thread::hardware_concurrency();

    vector<istream *> inputStreams;
    bool progOptParsed = processProgramOptions(ac, av, inputStreams, numThreads);

    Pest pest(&inputStreams, numThreads);
    pest.processStreams();

    return 0;
}
