#include <Instruction.hpp>

using namespace std;

Instruction::Instruction(string assembly, InstrType type)
{

}

Instruction::~Instruction()
{

}

int Instruction::getNumOp()
{
    return 2;
}

const string* Instruction::getInstr()
{
    return new string("mov fp, #0");
}

const string* Instruction::getOp(int index)
{
    return new string("fp");
}

InstrType Instruction::getExecType()
{
    return IntAlu;
}
