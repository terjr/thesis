#include <cstdlib>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

#include "TraceLine.hpp"
#include "Instruction.hpp"
#include "Pest.hpp"
#include "UnknownEvent.hpp"

using namespace boost;

TraceLine::TraceLine(const std::string &line) {
    unsigned long from, to;
    from = line.find(":") + 1;
    to = line.find(":", from);
    if (from == std::string::npos || to == std::string::npos) {
        simEvent = new UnknownEvent();
        return;
    }

    string eventType = line.substr(from, to-from);
    trim(eventType);

    if (eventType == "system.cpu T0") {
        simEvent = new Instruction(line);
    } else {
        simEvent = new UnknownEvent();
        std::cerr << "Unknown event type: <<" << eventType << ">>" << std::endl;

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
