#pragma once

#include "SimEvent.hpp"

/**
 * This class represents an unknown event,
 * which simply means any event that PET
 * does not recognize.
 */
class UnknownEvent : public SimEvent {
    public:
        UnknownEvent();
};
