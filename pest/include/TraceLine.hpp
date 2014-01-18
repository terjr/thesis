#pragma once
#include <string>
#include "Instruction.hpp"

class TraceLine {
    public:
        TraceLine(std::string line);
        virtual ~TraceLine();

        int getTick();
        int getCPU();
        int getPC();

        Instruction* getInstr();
    private:
};
