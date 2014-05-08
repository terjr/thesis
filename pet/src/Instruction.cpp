#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "Pet.hpp"
#include "Instruction.hpp"
#include "trim.hpp"

#define TICK_COL    0
#define PC_COL      2
#define INSTR_COL   3
#define TYPE_COL    4



using namespace boost;


/**
 * Create an empty Instruction instance.
 */
Instruction::Instruction() : SimEvent(InstEvent), op(std::vector<std::string>()) {}

/**
 * Create an Instruction instance parsed from input string.
 */
Instruction::Instruction(const std::string &line) : Instruction() {

    typedef tokenizer<char_separator<char> > tokenizer;
    const char_separator<char> sep(":");
    tokenizer tokens(line, sep);

    int column = 0;
    for (tokenizer::const_iterator it = tokens.begin();
            it != tokens.end(); ++it, ++column) {

        switch (column) {
            case TICK_COL:
                {
                    setTick(stoul(*it, NULL, 10));
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

/**
 * Return ID of CPU where the event took place
 */
unsigned int Instruction::getCPU() const {
    return cpu;
}

/**
 * Return of program counter value when the event took place
 */
unsigned long Instruction::getPC() const {
    return pc;
}

/**
 * Set the program counter value when the event took place
 */
void Instruction::setPC(unsigned long pc) {
    this->pc = pc;
}

/**
 * Set the instruction type
 */
void Instruction::setInstrType(const std::string &instrType) {
    this->instrType = instrTypeFromString(trim_str(instrType));
}

/**
 * Return the instruction type as InstrType
 */
InstrType Instruction::getInstrType() const {
    return instrType;
}

/**
 * Parse strings from simulator trace logs
 *
 * Example line:
 * "47000: system.cpu T0 : 0x89ec    :   mov   fp, #0             : IntAlu :  D=0x0000000000000000"
 */
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
        mnemonic = trim_str(assembly.substr(0, mnemLen));

        std::string args = assembly.substr(mnemLen);
        tokenizer tokens(args, sep);

        for (tokenizer::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->length()) {
                op.push_back(trim_str(*it));
            }
        }
    }
    return true;
}

/**
 * Destroy an Instruction instance
 */
Instruction::~Instruction() {

}

/**
 * Convert a string to a InstrType value
 */
InstrType instrTypeFromString(const std::string &instr) {
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


/**
 * Convert an InstrType to a string instance
 */
const std::string instrTypeToString(InstrType type) {
    switch (type)
    {
    case IntAlu:
        return "IntAlu";
    case IntMult:
        return "IntMult";
    case MemRead:
        return "MemRead";
    case MemWrite:
        return "MemWrite";
    case SimdFloatMisc:
        return "SimdFloatMisc";
    case ErrorType:
    default:
        return "ErrorType";
    }
}


/**
 * Return the number of operands in the current instruction
 */
unsigned int Instruction::getNumOp() const {
    return op.size();
}

/**
 * Return the mnemonic of the current instruction
 */
const std::string Instruction::getMnemonic() const {
    return mnemonic;
}

/**
 * Return operand number as defined by index
 */
const std::string Instruction::getOp(const unsigned int index) const {
    if (getNumOp() > index)
        return op[index];
    else
        return std::string();
}

/**
 * Convert Instruction instance to a string
 */
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

/**
 * Check if two Instruction instances are equal, which they are if mnemonic
 * and all operands are equal
 */
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

/**
 * Check if two Instruction instances are unequal. Same as inverted value of operator==
 * @see operator==
 */
bool operator!=(const Instruction& first, const Instruction& second) {
    return !(first==second);
}

 /**
  * Instruction is turned to string and streamed into the output stream
  * @see Instruction::toString
  */
std::ostream& operator <<(std::ostream& outputStream, const Instruction& instr) {
    return outputStream << instr.toString();
}
