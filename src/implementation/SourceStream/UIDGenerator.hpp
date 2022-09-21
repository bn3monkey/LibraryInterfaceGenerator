#ifndef __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_UID_GENERATOR__
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_UID_GENERATOR__

#include <vector>
#include <string>
#include <cstring>

inline std::string makeGUID(const std::vector<std::string>& strs)
{
    constexpr static char characters[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr size_t length = sizeof(characters);

    size_t slots[12] = { 0 };
    size_t idx = 0;

    constexpr size_t upper_length = 'A' - 'Z' + 1;
    constexpr size_t lower_length = 'z' - 'a' + 1;

    for (auto& str : strs)
    {
        for (auto& ch : str)
        {
            auto& slot = slots[idx++];
            if ('a' <= ch && ch <= 'z')
            {
                slot += ch - 'a' + upper_length;
            }
            else if ('A' <= ch && ch <= 'Z')
            {
                slot += ch - 'A';
            }
            else if ('0' <= ch && ch <= '9')
            {
                slot += ch - '0' + lower_length + upper_length;
            }
        }
    }

    for (auto& slot : slots)
    {
        slot %= length;
    }

    std::string ret{""};
    ret += slots[0];
    ret += slots[1];
    ret += slots[2];
    ret += slots[3];
    ret += "_";
    ret += slots[4];
    ret += slots[5];
    ret += slots[6];
    ret += slots[7];
    ret += "_" ;
    ret += slots[8];
    ret += slots[9];
    ret += slots[10];
    ret += slots[11];
    return ret;
}

#endif
