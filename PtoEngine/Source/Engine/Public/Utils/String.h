#pragma once

#include <string>
#include <sstream>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <locale>
#include <Windows.h>
#include <vector>

namespace Util
{
    std::string w2String(std::wstring oWString);
    std::wstring s2WString(std::string oString);

    template<typename T, typename U>
    std::vector<T> Split(T str, U del);

    template<typename U>
    std::vector<std::string> Split(std::string str, U del)
    {
        std::vector<std::string> Out;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, del))
        {
            Out.push_back(token);
        }
        return Out;
    }
    template<typename U>
    std::vector<std::wstring> Split(std::wstring str, U del)
    {
        std::vector<std::wstring> Out;

        std::wstringstream ss(str);
        std::wstring token;
        while (std::getline(ss, token, del))
        {
            Out.push_back(token);
        }

        //std::wstring::size_type pos = 0;
        //std::wstring::size_type prev = 0;
        //while ((pos = str.find(del, prev)) != std::wstring::npos)
        //{
        //    Out.push_back(str.substr(prev, pos - prev));
        //    prev = pos + del.size();
        //}

        //// To get the last substring (or only, if delimiter is not found)
        //Out.push_back(str.substr(prev));

        return Out;
    }
}