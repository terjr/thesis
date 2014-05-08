#include "UnknownEvent.hpp"

/**
 * Instanciate a basic UnknownEvent, Tick is 0 and EventType is
 * EventType::Unknown
 * @see EventType
 */
UnknownEvent::UnknownEvent() : SimEvent(0, EventType::Unknown) {}


