#include "Memory.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/tokenizer.hpp>

#define TICK_COL 0
#define EVENT_COL 1
#define DESC_COL 2

using namespace boost;

MemType memTypeFromString(const std::string &str) {
    if (str == "system.cpu.icache")
        return L1I;
    else if (str == "system.cpu.dcache")
        return L1D;
    else if (str ==  "system.l2")
        return L2;
    else if (str == "system.physmem")
        return Phys;
    else
        return Null;
}

std::string memTypeToString(MemType type) {
    switch (type) {
        case L1I:
            return "L1I";
        case L1D:
            return "L1D";
        case L2:
            return "L2";
        case Phys:
            return "Phys";
        case Null:
        default:
            return "Null";
    }
}

Memory::Memory() : SimEvent(MemEvent) {}

Memory::Memory(const std::string &line) : Memory() {
    typedef tokenizer<char_separator<char> > tokenizer;
    const char_separator<char> sep(":");
    tokenizer tokens(line, sep);

    int column = 0;
    for (tokenizer::const_iterator it = tokens.begin();
            it != tokens.end(); ++it, ++column) {

        switch (column) {
            case TICK_COL:
                {
                    setTick(stoul(*it, NULL, 10));
                    break;
                }
            case EVENT_COL:
                {
                    if (getTick() == 0) {
                        type = Null;
                    } else {
                        std::string s(*it);
                        trim(s);
                        type = memTypeFromString(s);
                    }
                    break;
                }
            case DESC_COL:
                {
                    std::string s(*it);
                    trim(s);

                    switch (type) {
                        case L1I:
                        case L1D:
                            {
                                read = starts_with(s, "ReadReq");
                                bool write = starts_with(s, "WriteReq");
                                if (!read && !write)
                                {
                                    type = Null;
                                    break;
                                } else {
                                    hit = s.find("miss") == std::string::npos;
                                }
                                break;
                            }
                        case L2:
                            {
                                read = starts_with(s, "ReadReq");
                                bool write = ends_with(s, "updated in Cache")
                                        || starts_with(s, "ReadExReq");
                                if (!read && !write)
                                {
                                    type = Null;
                                    break;
                                }
                                hit = s.find("miss") == std::string::npos;
                                break;
                            }
                        case Phys:
                        case Null:
                            {
                                hit = true;
                                break;
                            }
                    }

                }
                break;
            default:
                break;

        }

    }
}

MemType Memory::getMemType() const {
    return type;
}

bool Memory::isHit() const {
    return hit;
}
