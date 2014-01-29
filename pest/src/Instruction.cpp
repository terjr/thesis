#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "Instruction.hpp"

using namespace boost;

Instruction::Instruction() : op(std::vector<std::string>()) {}

Instruction::Instruction(std::string assembly) : op(std::vector<std::string>())
{
    parseAssembly(assembly);
}

Instruction::Instruction(std::string assembly, std::string type) : Instruction(assembly) {
    this->instrType = Instruction::instrTypeFromString(type);
}

Instruction::Instruction(std::string assembly, InstrType type) : Instruction(assembly) {
    this->instrType = type;
}

void Instruction::setInstrType(const std::string &instrType) {

    this->instrType = Instruction::instrTypeFromString(instrType);
}

InstrType Instruction::getInstrType() const
{
    return instrType;
}

bool Instruction::parseAssembly(std::string assembly)
{
    typedef tokenizer<char_separator<char> > tokenizer;
    const char_separator<char> sep(",");

    if (assembly.length() == 0)
        return false;
    trim(assembly);

    size_t mnemLen = assembly.find(' ');
    if (mnemLen == std::string::npos) {
        mnemonic = assembly;
    }
    else
    {
        mnemonic = assembly.substr(0, mnemLen);
        trim(mnemonic);

        std::string args = assembly.substr(mnemLen);
        tokenizer tokens(args, sep);

        for (tokenizer::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->length()) {
                op.push_back(*it);
                trim(op.back());
            }
        }
    }
    return true;
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
        std::cerr << "Instruction type " << instr << " not implemented." << std::endl;
        return ErrorType;
    }
}

unsigned int Instruction::getNumOp() const
{
    return op.size();
}

const std::string Instruction::getMnemonic() const
{
    return mnemonic;
}

const std::string Instruction::getOp(const unsigned int index) const
{
    if (getNumOp() > index)
        return op[index];
    else
        return std::string();
}

std::string Instruction::toString() const {
    std::string output(mnemonic + " ");

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
            first.getInstrType() == second.getInstrType())
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


std::ostream& operator <<(std::ostream& outputStream, const Instruction& instr)
{
    return outputStream << instr.toString();
}
