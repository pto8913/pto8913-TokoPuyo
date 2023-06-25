#pragma once

#include <string>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <locale>
#include <Windows.h>

namespace Util
{
    std::string w2String(std::wstring oWString);
    std::wstring s2WString(std::string oString);
}