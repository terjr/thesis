#pragma once

typedef struct {
    bool help;
    bool error;
    std::istream *inputStream;
    std::map<unsigned long, std::string> *annotations;
    std::string inputName;
    unsigned int numThreads;
    unsigned int numBuckets;
    unsigned int bucketSize;
    std::string output;
} options_t;

options_t processProgramOptions(int ac, char **av);
