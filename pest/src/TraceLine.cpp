#include <cstdlib>
#include <boost/regex.hpp>

#include <TraceLine.hpp>

using namespace std;

TraceLine::TraceLine(const string line)
{
    const boost::regex pattern(" *([[:digit:]]+) *: *([^ :]+) T([[:digit:]]+) *: *([^:]+): *([^:]+): *([^ :]+) *: *(.*)");
    boost::smatch result;

    if (boost::regex_search(line, result, pattern) && result.size() == 8 )
    {
        tick = strtol(string(result[1]).c_str(), NULL, 10);
        cpu =  strtol(string(result[3]).c_str(), NULL, 10);
        pc = strtol(string(result[4]).c_str(), NULL, 16);
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
