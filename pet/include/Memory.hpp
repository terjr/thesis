#pragma once
#include <string>
#include "SimEvent.hpp"

enum MemType {
    L1IR,
    L1IW,
    L1DR,
    L1DW,
    L2R,
    L2W,
    PhysR,
    PhysW,
    Null
};

class Memory : public SimEvent {
    public:
        Memory();
        Memory(const std::string &line);
        virtual ~Memory() {};
        MemType getMemType() const;
        bool isHit() const;
    private:
        MemType type;
        bool hit;
        bool read;
};

MemType parseType(const std::string &str);
const std::string memTypeToString(MemType type);
