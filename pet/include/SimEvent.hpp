#pragma once

enum EventType {
    InstEvent,
    MemEvent,
    Unknown // for unimplemented events
};

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
