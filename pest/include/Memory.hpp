#pragma once
#include <string>
#include "SimEvent.hpp"

enum MemType {
    L1I,
    L1D,
    L2,
    Phys,
    Null
};

class Memory : public SimEvent {
    public:
        Memory();
        Memory(const std::string &line);
        MemType getMemType() const;
        bool isHit() const;
    private:
        MemType type;
        bool hit;
        bool read;
};

MemType getTypeFromString(const std::string &str);
