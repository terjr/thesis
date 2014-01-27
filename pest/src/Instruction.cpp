#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "Instruction.hpp"

using namespace std;

Instruction::Instruction() : op(vector<string>()) {}

Instruction::Instruction(string assembly) : op(vector<string>())
{
    parseAssembly(assembly);
}

Instruction::Instruction(string assembly, string type) : Instruction(assembly)
{
        instrType = instrTypeFromString(type);
}

Instruction::Instruction(string assembly, InstrType type) : Instruction(assembly)
{
    instrType = type;
}

void Instruction::parseAssembly(string assembly)
{
    boost::regex pattern("([^ ]+) ([^,]*),?([^,]*),?([^,]*),?([^,]*),?");
    boost::smatch result;

    if (boost::regex_search(assembly, result, pattern))
    {
        if (result.size() > 1)
        {
            mnemonic = result[1];
            boost::trim(mnemonic);
        }
        for (unsigned i = 2; i < result.size(); i++)
        {
            if (result[i].length())
                op.push_back(result[i]);
        }
        for (unsigned i = 0; i < getNumOp(); i++)
                boost::trim(op[i]);
    }
}

Instruction::~Instruction()
{

}

InstrType Instruction::instrTypeFromString(const std::string &instr)
{
    if (instr == "IntAlu") return IntAlu;
    else if (instr == "IntMult") return IntMult;
    else if (instr == "MemRead") return MemRead;
    else if (instr == "MemWrite") return MemWrite;
    else if (instr == "SimdFloatMisc") return SimdFloatMisc;
    else {
        cerr << "Instruction type not implemented." << endl;
    }
}

unsigned int Instruction::getNumOp() const
{
    return op.size();
}

const string Instruction::getMnemonic() const
{
    return mnemonic;
}

const string Instruction::getOp(const unsigned int index) const
{
    if (getNumOp() > index)
        return op[index];
    else
        return string();
}

InstrType Instruction::getExecType() const
{
    return IntAlu;
}

string Instruction::toString() const {
    string output(mnemonic + " ");

    for (unsigned i = 0; i < getNumOp(); i++)
    {
        output += getOp(i);
        if (i + 1 < getNumOp())
            output += ",";
    }
    return output;
}

bool operator==(const Instruction& first, const Instruction& second)
{
    if (first.getMnemonic() == second.getMnemonic() &&
            first.getNumOp() == second.getNumOp() &&
            first.getExecType() == second.getExecType())
    {
        for (unsigned i = 0; i < first.getNumOp(); i++)
            if (first.getOp(i) != second.getOp(i))
                return false;
        return true;
    }
    else
        return false;
}

bool operator!=(const Instruction& first, const Instruction& second)
{
    return !(first==second);
}


ostream& operator <<(ostream& outputStream, const Instruction& instr)
{
    return outputStream << instr.toString();
}
