#pragma once

/** En enumeration value for the different output types available in PET
 */
enum OutputFormat {
    Graph,
    Plain,
    Table
};

/** This is a struct containg all user-set options that PET uses during it's
 * runtime
 */
typedef struct {
    bool help;
    bool error;
    bool stats;
    std::istream *inputStream;
    std::map<unsigned long, std::string> *annotations;
    std::map<std::string, unsigned long> *weights;
    std::string inputName;
    unsigned int numThreads;
    unsigned int numBuckets;
    unsigned int bucketSize;
    unsigned int ticksInCycle;
    std::string output;
    std::string title;
    OutputFormat outputFormat;
} options_t;

options_t processProgramOptions(int ac, char **av);
void vPrint(const std::string str);
bool isVerbose();
