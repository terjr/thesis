#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "Pest.hpp"
#include "Instruction.hpp"

#define TICK_COL    0
#define PC_COL      2
#define INSTR_COL   3
#define TYPE_COL    4



using namespace boost;



Instruction::Instruction() : op(std::vector<std::string>()) {}

Instruction::Instruction(const std::string &line) : op(std::vector<std::string>()) {
    typedef tokenizer<char_separator<char> > tokenizer;
    const char_separator<char> sep(":");
    tokenizer tokens(line, sep);

    int column = 0;
    for (tokenizer::const_iterator it = tokens.begin();
            it != tokens.end(); ++it, ++column) {

        switch (column) {
            case TICK_COL:
                {
                    setTick(stoul(*it, NULL, 16));
                    break;
                }
            case PC_COL:
                {
                    setPC(stoul(*it, NULL, 16));
                    break;
                }
            case INSTR_COL:
                {
                    parseAssembly(*it);
                    break;
                }
            case TYPE_COL:
                {
                    setInstrType(*it);
                    break;
                }
            default:
                {
                }
        }
    }

}

unsigned int Instruction::getCPU() const {
    return cpu;
}

unsigned long Instruction::getPC() const {
    return pc;
}

void Instruction::setPC(unsigned long pc) {
    this->pc = pc;
}

void Instruction::setInstrType(std::string instrType) {
    trim(instrType);
    this->instrType = Instruction::instrTypeFromString(instrType);
}

InstrType Instruction::getInstrType() const {
    return instrType;
}

bool Instruction::parseAssembly(std::string assembly) {
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

Instruction::~Instruction() {

}

InstrType Instruction::instrTypeFromString(const std::string &instr) {
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

unsigned int Instruction::getNumOp() const {
    return op.size();
}

const std::string Instruction::getMnemonic() const {
    return mnemonic;
}

const std::string Instruction::getOp(const unsigned int index) const {
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

bool operator==(const Instruction& first, const Instruction& second) {
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

bool operator!=(const Instruction& first, const Instruction& second) {
    return !(first==second);
}


std::ostream& operator <<(std::ostream& outputStream, const Instruction& instr) {
    return outputStream << instr.toString();
}
