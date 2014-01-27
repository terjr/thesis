#include "SimpleStats.hpp"

#include <iostream>

#include "TraceLine.hpp"

using namespace std;

static unsigned long count = 0;

int countAdds(istream *s) {
    while (*s) {
        string str;
        std::getline(*s, str);
        TraceLine tl(str);
        
        if ("add" == tl.getInstr().getMnemonic()) {
            // TODO: Do I need a mutex here?
            count++;
        }
    }
    cout << "count is now: " << count << endl;
    return 0;
}
