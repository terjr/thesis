#pragma once

#include "SimEvent.hpp"

#include <vector>
#include <iostream>
#include <boost/thread.hpp>

enum InstrType {
    IntAlu,
    IntMult,
    MemRead,
    MemWrite,
    SimdFloatMisc,
    ErrorType
};

class Instruction : public SimEvent {
    public:
        Instruction(std::string assembly);
        Instruction();
        Instruction(std::string assembly, InstrType type);
        Instruction(std::string assembly, std::string type);

        unsigned int getCPU() const;
        unsigned long getPC() const;
        void setPC(unsigned long pc);
        void setInstrType(const std::string &instrtype);
        virtual ~Instruction();
        bool  parseAssembly(std::string assembly);
        const std::string getMnemonic() const;
        virtual InstrType getInstrType() const;
        const std::string getOp(unsigned int index) const;
        unsigned int getNumOp() const;
        std::string toString() const;

        static InstrType instrTypeFromString(const std::string &instrType);


    private:
        unsigned int cpu;
        unsigned long pc;
        InstrType instrType;
        std::vector<std::string> op;
        std::string mnemonic;

};

bool operator==(const Instruction& first, const Instruction& other);
bool operator!=(const Instruction& first, const Instruction& other);
std::ostream& operator <<(std::ostream& outputStream, const Instruction& instr);
