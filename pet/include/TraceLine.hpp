#pragma once
#include <string>
#include "SimEvent.hpp"

class TraceLine {
    public:
        TraceLine(const std::string &line);
        virtual ~TraceLine();

        SimEvent* getSimEvent() const;

    private:
        SimEvent *simEvent;
};

std::ostream& operator <<(std::ostream& outputStream, const TraceLine& traceLine);
