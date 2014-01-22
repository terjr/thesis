#pragma once

#include <vector>
#include <istream>

class Pest
{
    public:
        static int main(int argc, char** argv);
        Pest(std::vector<std::istream*> *inputs, int numThreads = -1);
};
