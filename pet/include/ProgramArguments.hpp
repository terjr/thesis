#pragma once

enum OutputFormat {
    Graph,
    Plain,
    Table
};

typedef struct {
    bool help;
    bool error;
    std::istream *inputStream;
    std::map<unsigned long, std::string> *annotations;
    std::map<std::string, unsigned long> *weights;
    std::string inputName;
    unsigned int numThreads;
    unsigned int numBuckets;
    unsigned int bucketSize;
    std::string output;
    OutputFormat outputFormat;
} options_t;

options_t processProgramOptions(int ac, char **av);
void vPrint(const std::string str);
bool isVerbose();
