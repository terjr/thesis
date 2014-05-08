#pragma once

/** En enumerative value that separates events into InstEvent, that is
 * Instruction type events, MemEvent, which is Memory related events, and
 * Unknown, which is all the others that we really don't care about
 */
enum EventType {
    InstEvent,
    MemEvent,
    Unknown /// for unimplemented events
};

/**
 * The SimEvent class is the base for all trace log line event types.
 * @see Instruction
 * @see Memory
 */
class SimEvent {
    public:
        SimEvent() {};
        SimEvent(EventType type) : type(type) {};
        SimEvent(unsigned long tick, EventType type) : tick(tick), type(type) {};
        virtual ~SimEvent() {};

        unsigned long getTick() const;
        EventType getType() const;

        void setTick(unsigned long tick);

    private:
        unsigned long tick;
        EventType type;


};
