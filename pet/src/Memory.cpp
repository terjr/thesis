#include "Memory.hpp"
#include "trim.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/tokenizer.hpp>

#define TICK_COL 0
#define EVENT_COL 1
#define DESC_COL 2

using namespace boost;

MemType memTypeFromString(const std::string &str) {
    if (str == "system.cpu.icache" || str == "system.cpu.dcache")
        return L1R;
    else if (str ==  "system.l2")
        return L2R;
    else if (str == "system.physmem")
        return PhysR;
    else
        return Null;
}

std::string memTypeToString(MemType type) {
    switch (type) {
        case L1R:
            return "L1R";
        case L1W:
            return "L1W";
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
                        type = memTypeFromString(s);
                    }
                    break;
                }
            case DESC_COL:
                {
                    std::string s = trim_str(*it);

                    switch (type) {
                        case L1R:
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
                                    type=L1W;
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
