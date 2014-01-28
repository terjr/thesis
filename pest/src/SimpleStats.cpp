#include "SimpleStats.hpp"

#include <string>
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>

#include "TraceLine.hpp"

using namespace boost;

int countAdds(lockfree::queue<std::string*> *q, atomic<bool> *done, atomic<unsigned long> *count) {
    std::string *s;
    unsigned long local = 0;
    while (!(*done)) {
        while (q->pop(s)) {
            if ("add" == TraceLine(*s).getInstr().getMnemonic()) {
                // TODO: Do I need a mutex here?
                local++;
            }
            delete s;
        }
        thread::yield();
    }
    *count += local;
    return 0;
}
