#include <cstring>
#include "trim.hpp"

std::string trim_str(std::basic_string<char> cstr)
{
    unsigned int a = 0;
    unsigned int b = cstr.length() - 1;
    while (cstr[a] == ' ')
        a++;
    while (cstr[b] == ' ') b--;
    return std::string(cstr.c_str()+a, b-a+1);
}
