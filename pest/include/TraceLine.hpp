#pragma once
#include <string>
#include "Instruction.hpp"

class TraceLine {
    public:
        TraceLine();
        TraceLine(std::string line);
        virtual ~TraceLine();

        int getTick() const;
        int getCPU() const;
        int getPC() const;
        Instruction getInstr() const;

    private:
        int tick;
        int cpu;
        int pc;
        Instruction instr;
};

std::ostream& operator <<(std::ostream& outputStream, const TraceLine& traceLine);
