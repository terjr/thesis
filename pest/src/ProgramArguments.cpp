#include <fstream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "ProgramArguments.hpp"

using namespace std;

namespace po = boost::program_options;

map<unsigned long, string>* parseAnnotationfile(const string &filename) {
    auto *m = new map<unsigned long, string>();

    ifstream ifs(filename);
    // 88aabb88 symbol
    string s;
    while (ifs) {
        getline(ifs, s);
        char* rest;
        unsigned long pc = strtol(s.c_str(), &rest, 16);
        string symbol(rest);
        boost::trim(symbol);
        m->insert( pair<unsigned long, string>(pc, symbol) );
    }

    return m;
}


options_t processProgramOptions(int ac, char **av) {
    // TODO: Wouldn't options become out of scope when this function returns?
    options_t options;
    string outputFile;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "show help message")
        ("input-file,i", po::value<string>(), "input file")
        ("max-threads,t", po::value<unsigned>(), "maximum number of threads")
        ("output-file,o", po::value<string>(&outputFile)->default_value(""), "output file")
        ("config-file,c", po::value<string>(), "config-file")
        ("annotation,a", po::value<string>(), "annotation-map")
        ("decompress,d", po::value<bool>()->default_value(false), "enable gzip decompression")
        ("num-buckets,b", po::value<unsigned long>(), "the number of buckets")
        ("bucket-size,B", po::value<unsigned long>(), "number of ticks in each bucket")
        ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).
            options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        options.help = true;
        cout << desc << endl;
        return options;
    } else {
        options.help = false;
    }

    if (vm.count("config-file")) {
        // Load the file and tokenize it
        ifstream ifs(vm["config-file"].as<string>().c_str());
        if (!ifs) {
            cout << "Could not open the config file" << endl;
            options.error = true;
            return options;
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

    if (vm.count("max-threads")) {
        options.numThreads = vm["max-threads"].as<unsigned int>();
    } else {
        options.numThreads = 0;
    }

    if (vm.count("output-file")) {
        options.output = vm["output-file"].as<string>();
    }

    if (vm.count("num-buckets")) {
        options.numBuckets = vm["num-buckets"].as<unsigned long>();
    } else {
        options.numBuckets = 0;
    }

    if (vm.count("bucket-size")) {
        options.bucketSize = vm["bucket-size"].as<unsigned long>();
    } else {
        options.bucketSize = 0;
    }

    if (vm.count("annotation")) {
        options.annotations = parseAnnotationfile(vm["annotation"].as<string>());
    } else {
        options.annotations = NULL;
    }

    if (vm.count("input-file")) {
        options.inputStream = new ifstream(vm["input-file"].as<string>());
        options.inputName = vm["input-file"].as<string>();
    } else {
        // Read from stdin
        options.inputStream = &cin;
        options.inputName = "-";
    }

    options.error = false;
    return options;

}
