#include <fstream>

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <Pest.hpp>
#include <TraceLine.hpp>

namespace po = boost::program_options;
using namespace std;

int printStream(istream* s)
{
    while (*s)
    {
        string str;
        std::getline(*s,str);
        cout << str << '\n' << TraceLine(str) << endl << endl;
    }
    return 0;
}

Pest::Pest(vector<istream*> *inputs, int numThreads)
{
    /*
     * Create an asio::io_service and a thread_group (through pool in essence)
     */
    boost::asio::io_service ioService;
    boost::thread_group threadpool;

    /*
     * This will assign tasks to the thread pool. 
     * More about boost::bind: "http://www.boost.org/doc/libs/1_54_0/libs/bind/bind.html#with_functions"
     */
    for (vector<istream*>::iterator it = inputs->begin(); it != inputs->end(); ++it)
        ioService.post(boost::bind(printStream, *it));


    /*
     * This will start the ioService processing loop. All tasks 
     * assigned with ioService.post() will start executing. 
     */
    //    boost::asio::io_service::work work(ioService);

    /*
     * This will add 2 threads to the thread pool. (You could just put it in a for loop)
     */
    for (int i = 0; i < numThreads && i < inputs->size(); i++)
        threadpool.create_thread(
                boost::bind(&boost::asio::io_service::run, &ioService)
                );

    //    ioService.post(boost::bind(clearCache, "./cache"));
    //    ioService.post(boost::bind(getSocialUpdates, "twitter,gmail,facebook,tumblr,reddit"));

    //    ioService.poll();
    /*
     * This will stop the ioService processing loop. Any tasks
     * you add behind this point will not execute.
     */
    //    ioService.stop();

    /*
     * Will wait till all the treads in the thread pool are finished with 
     * their assigned tasks and 'join' them. Just assume the threads inside
     * the threadpool will be destroyed by this method.
     */
    threadpool.join_all();
}


/**
 * This is code copied and assembled from the boost::program_options library tutorial,
 * and it is only here for us to learn how the library works.
 * This content is neither well tested nor is it considered
 * to bee a poart of the final program
 **/
int Pest::main(int ac, char** av)
{
    unsigned numThreads = boost::thread::hardware_concurrency();
    string outputFile;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("input-file,i", po::value< vector<string> >(), "input file")
        ("max-threads,t", po::value<unsigned>(), "maximum number of threads")
        ("output-file,o", po::value<string>(&outputFile)->default_value("-"), "output file (defaults to stdout)")
        ("config-file", po::value<string>(), "config-file")
        ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).
                      options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("max-threads")) {
        numThreads = vm["max-threads"].as<unsigned>();
    } else {
        cout << "Automatically configuring number of threads.\n";
    }
    if (vm.count("config-file")) {
        // Load the file and tokenize it
        ifstream ifs(vm["config-file"].as<string>().c_str());
        if (!ifs) {
            cout << "Could no open the config file\n";
            return 1;
        }
        // Read the whole file into a string
        stringstream ss;
        ss << ifs.rdbuf();
        // Split the file content
        boost::char_separator<char> sep(" \n\r");
        boost::tokenizer<boost::char_separator<char> > tok(ss.str(), sep);
        vector<string> args;
        copy(tok.begin(), tok.end(), back_inserter(args));
        // Parse the file and store the options
        store(po::command_line_parser(args).options(desc).run(), vm);     
    }
    if (vm.count("include-path"))
    {
        string v = boost::algorithm::join(vm["include-path"].as< vector<string> >(), ", ");
        cout << "Include paths are: " << v << "\n";
    }

    if (vm.count("input-file"))
    {
        string v = boost::algorithm::join(vm["input-file"].as< vector<string> >(), ", ");
        cout << "Input files are: " << v << "\n";
    }
        cout << "Number of threads was set to " 
            << numThreads << ".\n";

    vector<string> v = vm["input-file"].as< vector<string> >();
    vector<istream*> input_streams;
    for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
        input_streams.push_back(new ifstream(*it));
    Pest pest(&input_streams, numThreads );

    return 0;
}
