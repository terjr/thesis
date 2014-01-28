#include <cstdlib>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "TraceLine.hpp"

using namespace boost;

//static const boost::regex pattern(" *([[:digit:]]+) *: *([^ :]+) T([[:digit:]]+) *: *([^:]+): *([^:]+): *([^ :]+) *: *(.*)");

TraceLine::TraceLine() {};

static const char_separator<char> sep(":");
TraceLine::TraceLine(const std::string line)
{
    typedef tokenizer<char_separator<char> > tokenizer;

    tokenizer tokens(line, sep);


    int index = 0;
    for (tokenizer::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
//        const int index = std::distance(tokens.begin(), it);
        switch (index++) {
            case 0:
                {
                    std::string s(*it);
                    trim(s);
                    this->tick = lexical_cast<int>(s);
                    break;
                }
            case 1:
                {
                    /*
                    static const regex pattern("system.cpu T([[:digit:]]+)");
                    smatch result;
                    std::string s(*it);
                    trim(s);
                    if (regex_search(s, result, pattern) && result.size() == 1) {
                        this->cpu = lexical_cast<int>(result[0]);
                    }
                    */
                    this->cpu = 0;
                    break;
                }
            case 2:
                {
                    this->pc = stoul(*it, NULL, 16);
                    break;
                }
            case 3:
                {
                    this->instr = Instruction(*it);
                    break;

                }
            case 4:
                {
                    std::string s(*it);
                    trim(s);
                    this->instr.setInstrType(s);
                    break;
                }
            default:
                {
                }
        }
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

std::ostream& operator <<(std::ostream& outputStream, const TraceLine& traceLine)
{
    return outputStream << traceLine.getInstr();
}
