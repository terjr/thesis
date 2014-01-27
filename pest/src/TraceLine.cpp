#include <cstdlib>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "TraceLine.hpp"

using namespace std;
using boost::lexical_cast;

static const boost::regex pattern(" *([[:digit:]]+) *: *([^ :]+) T([[:digit:]]+) *: *([^:]+): *([^:]+): *([^ :]+) *: *(.*)");

TraceLine::TraceLine(const string line)
{
    boost::smatch result;

    if (boost::regex_search(line, result, pattern) && result.size() == 8 )
    {
        tick = lexical_cast<int>(result[1]);
        cpu = lexical_cast<int>(result[3]);
        pc = stoul(result[4], NULL, 16);
        instr = Instruction(result[5], Instruction::instrTypeFromString(result[6]));
    }
    else
    {
        cerr << "Could not match regex from: " << line << endl;
    }
}

TraceLine::~TraceLine()
{

}

int TraceLine::getTick() const
{
    return tick;
}

int TraceLine::getCPU() const
{
    return cpu;
}

int TraceLine::getPC() const
{
    return pc;
}

Instruction TraceLine::getInstr() const
{
    return instr;
}

ostream& operator <<(ostream& outputStream, const TraceLine& traceLine)
{
    return outputStream << traceLine.getInstr();
}
