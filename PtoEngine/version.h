#pragma once
#define VERSION_YEAR        23  // 2 digit year
#define VERSION_MONTH       7   // 2 digit month
#define VERSION_DAY         21  // 2 digit day
#define VERSION_UNSAFE      0   // 0 = TESTED, 1 = UNSAFE / IN DEVELOPMENT

#define VER_COPYRIGHT_STR       "(C) 2023 pto8913"
#define VER_TRADEMARK_STR       "pto8913"
#define VER_COMPANY_STR     "pto8913"

#ifndef  VS_VERSION_INFO
#define  VS_VERSION_INFO                     1
#endif

#ifndef  VOS_NT_WINDOWS32
#define  VOS_NT_WINDOWS32                    0x00040004L
#endif

#ifndef  VFT_APP
#define  VFT_APP                             0x00000001L
#endif

#ifndef  VFT_DLL
#define  VFT_DLL                             0x00000002L
#endif

#define STRINGIZE2(s)               #s
#define STRINGIZE(s)                STRINGIZE2(s)
#if (VERSION_UNSAFE == 1)
#define MY_VERSION_NUM          STRINGIZE(VERSION_YEAR) STRINGIZE(VERSION_MONTH) "." STRINGIZE(VERSION_DAY) STRINGIZE(VERSION_UNSAFE)
#else
#define MY_VERSION_NUM          STRINGIZE(VERSION_YEAR) STRINGIZE(VERSION_MONTH) "." STRINGIZE(VERSION_DAY)
#endif
#define VER_FILE_VERSION            VERSION_YEAR, VERSION_MONTH, VERSION_DAY, VERSION_UNSAFE
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_YEAR) "." STRINGIZE(VERSION_MONTH) "." STRINGIZE(VERSION_DAY) "." STRINGIZE(VERSION_UNSAFE)
#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_STR
#ifdef _DEBUG
#define VER_VER_DEBUG           VS_FF_DEBUG
#else
#define VER_VER_DEBUG           0
#endif
#define VER_FILEOS                  VOS_NT_WINDOWS32
#define VER_FILEFLAGS               VER_VER_DEBUG
#define VER_FILETYPE                VFT_APP
