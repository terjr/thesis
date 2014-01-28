#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>

#include "Pest.hpp"
#include "ProgramArguments.hpp"
#include "SimpleStats.hpp"
#include "TraceLine.hpp"

using namespace std;

int printStream(istream* s)
{
    while (*s)
    {
        string str;
        std::getline(*s, str);
        cout << str << endl << TraceLine(str) << endl;
    }
    return 0;
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

Pest::Pest(istream *input, unsigned int numThreads) : done(false), count(0), lineQueue(128) {
    this->numThreads = numThreads;
    
    // Assign tasks to the thread pool
    boost::thread(&readLines, input, &lineQueue, &done);

    for (unsigned int i = 0; i < numThreads; ++i)
        this->ioService.post( boost::bind(countAdds, &lineQueue, &done, &count) );
}

void Pest::processStreams() {
    // Create worker threads
    for (unsigned int i = 0; i < this->numThreads; i++) {
        this->threadpool.create_thread( boost::bind(&boost::asio::io_service::run, &ioService) );
    }

    // Wait for threads to finish
    this->threadpool.join_all();
    cout << "There was " << this->count << " adds" << endl;
}


int Pest::main(int ac, char **av)
{
    unsigned int numThreads = boost::thread::hardware_concurrency();

    istream *inputStream;
    bool progOptParsed = processProgramOptions(ac, av, &inputStream, numThreads);

    Pest pest(inputStream, numThreads);
    pest.processStreams();

    return 0;
}
