#pragma once
#include <string>
#include "Instruction.hpp"

class TraceLine {
    public:
        TraceLine();
        TraceLine(const std::string &line);
        virtual ~TraceLine();

        unsigned long getTick() const;
        unsigned int getCPU() const;
        unsigned long getPC() const;
        Instruction getInstr() const;

    private:
        unsigned long tick;
        unsigned int cpu;
        unsigned long pc;
        Instruction instr;
};

std::ostream& operator <<(std::ostream& outputStream, const TraceLine& traceLine);
