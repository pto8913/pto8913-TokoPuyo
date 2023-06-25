#pragma once

#include <map>

namespace Util
{
    template<typename T>
    void SafeRelease(T* p)
    {
        if (p)
        {
            p->Release();
            p = nullptr;
        }
    }

    template<typename T>
    void SafeMapRelease(T& InMap)
    {
        for (auto itr = InMap.begin(); itr != InMap.end(); itr++)
        {
            SafeRelease(itr->second);
        }
    }

    template<typename T>
    void SafeDelete(T* p)
    {
        if (p)
        {
            delete p;
            p = nullptr;
        }
    }

    template<typename T>
    void SafeMapDelete(T& InMap)
    {
        for (auto itr = InMap.begin(); itr != InMap.end(); itr++)
        {
            SafeDelete(itr->second);
        }
    }
}