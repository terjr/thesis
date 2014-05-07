#include "Memory.hpp"
#include "trim.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/tokenizer.hpp>

#define TICK_COL 0
#define EVENT_COL 1
#define DESC_COL 2

using namespace boost;

MemType parseType(const std::string &str) {
    if (str == "system.cpu.icache")
        return L1IR;
    else if (str == "system.cpu.dcache")
        return L1DR;
    else if (str ==  "system.l2")
        return L2R;
    else if (str == "system.physmem")
        return PhysR;
    else
        return Null;
}

const std::string memTypeToString(MemType type) {
    switch (type) {
        case L1IR:
            return "L1IR";
        case L1IW:
            return "L1IW";
        case L1DR:
            return "L1DR";
        case L1DW:
            return "L1DW";
        case L2R:
            return "L2R";
        case L2W:
            return "L2W";
        case PhysR:
            return "PhysR";
        case PhysW:
            return "PhysW";
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
                        std::string s = trim_str(*it);
                        type = parseType(s);
                    }
                    break;
                }
            case DESC_COL:
                {
                    std::string s = trim_str(*it);

                    switch (type) {
                        case L1IR:
                            {
                                read = starts_with(s, "ReadReq");
                                bool write = ends_with(s, "updated in Cache");
                                if (!read && !write)
                                {
                                    type = Null;
                                    break;
                                } else {
                                    hit = s.find("miss") == std::string::npos;
                                }
                                if (write)
                                    type=L1IW;
                                break;
                            }
                        case L1DR:
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
                                if (write)
                                    type=L1DW;
                                break;
                            }
                        case L2R:
                            {
                                read = starts_with(s, "ReadReq");
                                bool write = ends_with(s, "updated in Cache")
                                        || starts_with(s, "ReadExReq");
                                if (!read && !write)
                                {
                                    type = Null;
                                    break;
                                }
                                if (write)
                                    type=L2W;
                                hit = s.find("miss") == std::string::npos;
                                break;
                            }
                        case PhysR:
                            {
                            read = starts_with(s, "Read of size") || starts_with(s, "IFetch of size ");
                            bool write = starts_with(s, "Write of size");
                            if (!read && !write)
                            {
                                type = Null;
                                break;
                            }
                            if (write)
                                type=PhysW;
                            hit = true;
                            }
                        default:
                            hit = true;
                            break;
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
