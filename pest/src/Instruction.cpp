#include <boost/regex.hpp>

#include <Instruction.hpp>

using namespace std;

Instruction::Instruction() : op(vector<string>()) {}

Instruction::Instruction(string assembly, InstrType type) : op(vector<string>())
{
    parseAssembly(assembly);
}

void Instruction::parseAssembly(string assembly)
{
    boost::regex pattern("([^ ]+) ([^,]*),?([^,]*),?([^,]*),?([^,]*),?");
    boost::smatch result;

    if (boost::regex_search(assembly, result, pattern))
    {
        if (result.size() > 1)
            mnemonic = result[1];
        for (unsigned i = 2; result.size() > i; i++)
            if (result[i].length())
                op.push_back(result[i]);
    }
}

Instruction::~Instruction()
{

}

InstrType Instruction::instrTypeFromString(std::string)
{
    return IntAlu;
}

unsigned int Instruction::getNumOp()
{
    return op.size();
}

const string Instruction::getMnemonic()
{
    return mnemonic;
}

const string Instruction::getOp(unsigned int index)
{
    if (getNumOp() > index)
        return op[index];
    else
        return string();
}

InstrType Instruction::getExecType()
{
    return IntAlu;
}
