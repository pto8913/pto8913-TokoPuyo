#pragma once

#include <map>

namespace Util
{
    template<typename T>
    void SafeRelease(T*& p)
    {
        if (p != nullptr)
        {
            p->Release();
        }
        p = nullptr;
    }
}