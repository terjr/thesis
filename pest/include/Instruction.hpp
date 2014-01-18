#pragma once
#include <string>

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
        virtual ~Instruction();
        const std::string *getInstr();
        const std::string *getOp(int index);
        int getNumOp();
        InstrType getExecType();

        static InstrType instrTypeFromString(std::string instrType);


    private:
        InstrType instrType;

};

bool operator==(const Instruction& first, const Instruction& other);

