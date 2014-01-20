#pragma once
#include <vector>

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

        Instruction(std::string assembly, InstrType type);
        Instruction();
        virtual ~Instruction();
        void parseAssembly(std::string assembly);
        const std::string getMnemonic();
        const std::string getOp(unsigned int index);
        unsigned int getNumOp();
        InstrType getExecType();

        static InstrType instrTypeFromString(std::string instrType);


    private:
        InstrType instrType;
        std::vector<std::string> op;
        std::string mnemonic;

};

bool operator==(const Instruction& first, const Instruction& other);

