#pragma once
#include <vector>
#include <iostream>

enum InstrType {
    IntAlu,
    IntMult,
    MemRead,
    MemWrite,
    SimdFloatMisc
};

class Instruction
{
    public:
        Instruction(std::string assembly, std::string type);
        Instruction(std::string assembly, InstrType type);
        Instruction();
        virtual ~Instruction();
        void parseAssembly(std::string assembly);
        const std::string getMnemonic() const;
        const std::string getOp(unsigned int index) const;
        unsigned int getNumOp() const;
        InstrType getExecType() const;
        std::string toString() const;

        static InstrType instrTypeFromString(std::string instrType);


    private:
        Instruction(std::string assembly);
        InstrType instrType;
        std::vector<std::string> op;
        std::string mnemonic;

};

bool operator==(const Instruction& first, const Instruction& other);
bool operator!=(const Instruction& first, const Instruction& other);
std::ostream& operator <<(std::ostream& outputStream, const Instruction& instr);
