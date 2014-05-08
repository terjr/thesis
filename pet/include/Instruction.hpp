#pragma once

#include "SimEvent.hpp"

#include <vector>
#include <iostream>
#include <boost/thread.hpp>

/**
 * The InstrType enum enumerates the different types of instruction types PET
 * will recognize from the parsed simulator trace log
 */
enum InstrType {
    IntAlu,
    IntMult,
    MemRead,
    MemWrite,
    SimdFloatMisc,
    ErrorType
};

/**
 * The Instruction class simply represents the event where an instruction is
 * taken through the simulated pipeline. It contains parser functions to
 * extract information from trace log lines detected as Instruction type events
 */
class Instruction : public SimEvent {
    public:
        Instruction();
        Instruction(const std::string &line);

        unsigned int getCPU() const;
        unsigned long getPC() const;
        void setPC(unsigned long pc);
        void setInstrType(const std::string &instrtype);
        virtual ~Instruction();
        bool  parseAssembly(std::string assembly);
        const std::string getMnemonic() const;
        virtual InstrType getInstrType() const;
        const std::string getOp(unsigned int index) const;
        unsigned int getNumOp() const;
        std::string toString() const;
    private:
        unsigned int cpu;
        unsigned long pc;
        InstrType instrType;
        std::vector<std::string> op;
        std::string mnemonic;

};

InstrType instrTypeFromString(const std::string &instrType);
const std::string instrTypeToString(InstrType type);


bool operator==(const Instruction& first, const Instruction& other);
bool operator!=(const Instruction& first, const Instruction& other);
std::ostream& operator <<(std::ostream& outputStream, const Instruction& instr);
