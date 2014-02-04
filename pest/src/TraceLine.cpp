#include <cstdlib>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "TraceLine.hpp"
#include "Pest.hpp"

using namespace boost;

//static const boost::regex pattern(" *([[:digit:]]+) *: *([^ :]+) T([[:digit:]]+) *: *([^:]+): *([^:]+): *([^ :]+) *: *(.*)");

TraceLine::TraceLine() : tick(0), cpu(0), pc(0), instr()  {};

TraceLine::TraceLine(const std::string &line) : TraceLine()
{
    typedef tokenizer<char_separator<char> > tokenizer;

    muu.lock();
    const char_separator<char> sep(":");
    tokenizer tokens(line, sep);


    int index = 0;
    for (tokenizer::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
//        const int index = std::distance(tokens.begin(), it);
        switch (index++) {
            case 0:
                {
                    this->tick = stoul(*it, NULL, 10);
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
    muu.unlock();
}

TraceLine::~TraceLine()
{

}

unsigned long TraceLine::getTick() const
{
    return tick;
}

unsigned int TraceLine::getCPU() const
{
    return cpu;
}

unsigned long TraceLine::getPC() const
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
