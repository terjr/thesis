#include "SimEvent.hpp"

#include "Instruction.hpp"


unsigned long SimEvent::getTick() const {
    return tick;
}

EventType SimEvent::getType() const {
    return type;
}

void SimEvent::setTick(unsigned long tick) {
    this->tick = tick;
}


