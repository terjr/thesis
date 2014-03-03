#include <fstream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "ProgramArguments.hpp"

using namespace std;

namespace po = boost::program_options;
bool verbose = false;

bool isVerbose() {
    return verbose;
}

inline void vPrint(const string str) {
    if (verbose) cout << str;
}

map<string, unsigned long>* parseWeightFile(const string &filename) {
    auto *m = new map<string, unsigned long>();

    if (verbose)
        cout << "Loading weights:" << endl;

    ifstream ifs(filename);
    if (!ifs) {
        cout << "ERROR: No such weight file: " << filename << endl;
        return NULL;
    }

    string s;
    while (ifs) {
        getline(ifs, s);
        boost::trim(s);
        if (s.empty() || s[0] == '#') continue;
        auto idx = s.find(' ');
        string symbol = s.substr(0, idx);
        string weight_str = s.substr(idx);
        boost::trim(weight_str);
        unsigned long weight = strtol(weight_str.c_str(), NULL, 10);
        vPrint(weight_str + " " + symbol + "\n");
        m->insert( pair<string, unsigned long>(symbol, weight) );
    }

    return m;
}


map<unsigned long, string>* parseAnnotationFile(const string &filename) {
    auto *m = new map<unsigned long, string>();

    ifstream ifs(filename);
    if (!ifs)
    {
        cout << "ERROR: No such annotation file: " << filename << endl;
        return NULL;
    }

    vPrint("Loading annotation map\n");

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
    options_t options;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "show help message")
        ("verbose,v", "be verbose")
        ("input-file,i", po::value<string>(), "input file")
        ("max-threads,t", po::value<unsigned>(), "maximum number of threads")
        ("output-file,o", po::value<string>(&options.output)->default_value(""), "output file")
        ("output-format,f", po::value<string>()->default_value("graph"), "output format, \"graph\", \"plain\" or \"table\"")
        ("config-file,c", po::value<string>(), "config-file")
        ("weights,w", po::value<string>()->default_value("weights.conf"), "weight-file")
        ("annotations,a", po::value<string>(), "annotation-map")
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

    if (vm.count("verbose")) {
        verbose = true;
    }

    if (vm.count("max-threads")) {
        options.numThreads = vm["max-threads"].as<unsigned int>();
        vPrint("Max threads set to " + to_string(options.numThreads) + "\n");
    } else {
        options.numThreads = 0;
    }

    if (vm.count("output-file")) {
        //options.output = vm["output-file"].as<string>();
        vPrint("Output file set to " + options.output + "\n");
    }
    if (vm.count("output-format")) {
        string format = vm["output-format"].as<string>();
        if (format == "graph")
            options.outputFormat = Graph;
        else if (format == "plain")
            options.outputFormat = Plain;
        else if (format == "table")
            options.outputFormat = Table;
        else {
            cout << "Unknown output format " << format << "\n" << " Use 'graph', 'plain' or 'table'" << endl;
            options.error = true;
            return options;
        }
        vPrint("Output format set to " + format + "\n");
    }

    if (vm.count("num-buckets")) {
        options.numBuckets = vm["num-buckets"].as<unsigned long>();
        vPrint("Number of buckets set to " + to_string(options.numBuckets) + "\n");
    } else {
        options.numBuckets = 0;
    }

    if (vm.count("bucket-size")) {
        options.bucketSize = vm["bucket-size"].as<unsigned long>();
        vPrint("Number of buckets set to " + to_string(options.numBuckets) + "\n");
    } else {
        options.bucketSize = 0;
    }

    if (vm.count("annotations")) {
        options.annotations = parseAnnotationFile(vm["annotations"].as<string>());
        if (options.annotations == NULL)
        {
            options.error = true;
            return options;
        }
    } else {
        options.annotations = NULL;
    }

    if (vm.count("weights")) {
        options.weights = parseWeightFile(vm["weights"].as<string>());
        if (options.weights == NULL) {
            options.error = true;
            return options;
        }

    } else {
        options.weights = NULL;
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
