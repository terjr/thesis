#include <cstdlib>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "TraceLine.hpp"
#include "Instruction.hpp"
#include "Pest.hpp"

using namespace boost;

//static const boost::regex pattern(" *([[:digit:]]+) *: *([^ :]+) T([[:digit:]]+) *: *([^:]+): *([^:]+): *([^ :]+) *: *(.*)");

TraceLine::TraceLine() : tick(0), simEvent(new Instruction()) {};

TraceLine::TraceLine(const std::string &line) : TraceLine() {
    typedef tokenizer<char_separator<char> > tokenizer;

    const char_separator<char> sep(":");
    tokenizer tokens(line, sep);

    // TODO: For now, SimEvent is always an Instruction
    
    Instruction *inst = (Instruction *) simEvent;

    int index = 0;
    for (tokenizer::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
//        const int index = std::distance(tokens.begin(), it);
        switch (index++) {
            case 0:
                {
                    simEvent->setTick(stoul(*it, NULL, 10));
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
                    // TODO: Ignore cpu # for now
                    //this->cpu = 0;
                    break;
                }
            case 2:
                {
                    inst->setPC(stoul(*it, NULL, 16));
                    break;
                }
            case 3:
                {
                    inst->parseAssembly(*it);
                    break;

                }
            case 4:
                {
                    std::string s(*it);
                    trim(s);
                    inst->setInstrType(s);
                    break;
                }
            default:
                {
                }
        }
    }
}

TraceLine::~TraceLine() {

}

SimEvent* TraceLine::getSimEvent() const {
    return simEvent;
}

std::ostream& operator <<(std::ostream& outputStream, const TraceLine& traceLine) {
    // TODO: Handle all cases
    return outputStream << ((Instruction *)traceLine.getSimEvent())->toString();
}
