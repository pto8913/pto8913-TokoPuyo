
#include "Utils/String.h"
#include<format>
namespace Util
{
    std::string w2String(std::wstring oWString)
    {
        // wstring �� SJIS
        int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
            , -1, (char*)NULL, 0, NULL, NULL);

        // �o�b�t�@�̎擾
        CHAR* cpMultiByte = new CHAR[iBufferSize];

        // wstring �� SJIS
        WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
            , iBufferSize, NULL, NULL);

        // string�̐���
        std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

        // �o�b�t�@�̔j��
        delete[] cpMultiByte;

        // �ϊ����ʂ�Ԃ�
        return(oRet);
    }
    std::wstring s2WString(std::string oString)
    {
        // SJIS �� wstring
        int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
            , -1, (wchar_t*)NULL, 0);

        // �o�b�t�@�̎擾
        wchar_t* cpUCS2 = new wchar_t[iBufferSize];

        // SJIS �� wstring
        MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2
            , iBufferSize);

        // string�̐���
        std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

        // �o�b�t�@�̔j��
        delete[] cpUCS2;

        // �ϊ����ʂ�Ԃ�
        return(oRet);
    }
}