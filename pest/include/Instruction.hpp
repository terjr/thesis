#pragma once

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

class Instruction
{
    public:
        Instruction(std::string assembly);
        Instruction();
        Instruction(std::string assembly, InstrType type);
        Instruction(std::string assembly, std::string type);
        void setInstrType(const std::string &instrtype);
        virtual ~Instruction();
        bool  parseAssembly(std::string assembly);
        const std::string getMnemonic() const;
        InstrType getInstrType() const;
        const std::string getOp(unsigned int index) const;
        unsigned int getNumOp() const;
        std::string toString() const;

        static InstrType instrTypeFromString(const std::string &instrType);


    private:
        InstrType instrType;
        std::vector<std::string> op;
        std::string mnemonic;

};

bool operator==(const Instruction& first, const Instruction& other);
bool operator!=(const Instruction& first, const Instruction& other);
std::ostream& operator <<(std::ostream& outputStream, const Instruction& instr);
