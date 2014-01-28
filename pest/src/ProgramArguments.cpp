#include <fstream>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

#include "ProgramArguments.hpp"

using namespace std;

namespace po = boost::program_options;

bool processProgramOptions(int ac, char **av, istream **inputStream, unsigned int& numThreads) {
    string outputFile;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "show help message")
        ("input-file,i", po::value<string>(), "input file")
        ("max-threads,t", po::value<unsigned>(), "maximum number of threads")
        ("output-file,o", po::value<string>(&outputFile)->default_value("-"), "output file (defaults to stdout)")
        ("config-file", po::value<string>(), "config-file")
        ("decompress,d", po::value<bool>()->default_value(false), "enable gzip decompression")
        ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).
            options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        return false;
    }

    if (vm.count("max-threads")) {
        numThreads = vm["max-threads"].as<unsigned int>();
    } else {
        cout << "Automatically configuring number of threads." << endl;
    }
    if (vm.count("config-file")) {
        // Load the file and tokenize it
        ifstream ifs(vm["config-file"].as<string>().c_str());
        if (!ifs) {
            cout << "Could no open the config file" << endl;
            return false;
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
    cout << "Number of threads was set to " << numThreads << endl;

    if (vm.count("input-file")) {
        // TODO: Add support for gzipped streams
        *inputStream = new ifstream(vm["input-file"].as<string>());
    }
    else {
        // Read from stdin
        *inputStream = &cin;
    }
    return true;

}
