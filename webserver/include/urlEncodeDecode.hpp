//
// Created by yjs on 2022/7/17.
//

#ifndef YJSTOOLS_URLENCODEDECODE_HPP
#define YJSTOOLS_URLENCODEDECODE_HPP


#include <iostream>
#include <assert.h>

using namespace std;


unsigned char ToHex(unsigned char x)
{
    // x + 55  10 进制到大写 HEX 字母 A B C D E F
    // x + 48  10 进制到数字 0 1 2 3 4 5 6 7 8 9
    return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}

std::string UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4); // 取出e8 的高位 e
            strTemp += ToHex((unsigned char)str[i] % 16); // 取出e8 的低位 8
        }
    }
    return strTemp;
}

std::string UrlDecode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]); // 高位
            unsigned char low = FromHex((unsigned char)str[++i]);  // 低位
            strTemp += high*16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}












#endif //YJSTOOLS_URLENCODEDECODE_HPP
