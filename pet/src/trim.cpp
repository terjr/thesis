#include <cstring>
#include "trim.hpp"

/**
 * The trim_str function is a trivial, but fast and simple string trimmer
 * function. It will remove all ' ' at both the start and at the end of cstr,
 * but will not remove other types of whitespace characters.
 */
std::string trim_str(std::basic_string<char> cstr)
{
    unsigned int a = 0;
    unsigned int b = cstr.length() - 1;
    while (cstr[a] == ' ')
        a++;
    while (cstr[b] == ' ') b--;
    return std::string(cstr.c_str()+a, b-a+1);
}
