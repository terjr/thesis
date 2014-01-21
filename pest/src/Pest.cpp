#include <fstream>

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/join.hpp>

#include <Pest.hpp>

namespace po = boost::program_options;
using namespace std;

/**
 * This is code copied and assembled from the boost::program_options library tutorial,
 * and it is only here for us to learn how the library works.
 * This content is neither well tested nor is it considered
 * to bee a poart of the final program
**/
int Pest::main(int ac, char** av)
{
    int opt;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("optimization", po::value<int>(&opt)->default_value(10), 
         "optimization level")
        ("include-path,I", po::value< vector<string> >(), 
         "include path")
        ("input-file", po::value< vector<string> >(), "input file")
        ;
    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("compression")) {
        cout << "Compression level was set to " 
            << vm["compression"].as<int>() << ".\n";
    } else {
        cout << "Compression level was not set.\n";
    }
    if (vm.count("response-file")) {
        // Load the file and tokenize it
        ifstream ifs(vm["response-file"].as<string>().c_str());
        if (!ifs) {
            cout << "Could no open the response file\n";
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

    cout << "Optimization level is " << opt << "\n";

    return 0;
}
