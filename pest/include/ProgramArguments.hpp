#pragma once

using namespace std;

typedef struct {
    bool help;
    bool error;
    istream *inputStream;
    std::string inputName;
    unsigned int numThreads;
    unsigned int numBuckets;
    unsigned int bucketSize;
    std::string output;
} options_t;

options_t processProgramOptions(int ac, char **av);
