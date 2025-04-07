#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

// 函数声明
int compare(const void* arg1, const void* arg2);

int main()
{
    setlocale(LC_ALL, "chs");   // 用_tprintf函数输出，需要调用本函数设置区域

    //// 定义并初始化一个字符变量
    //char c = 'A';
    //// 定义并初始化一个指向字符串的指针
    //char *pStr = "Hello!";
    //// 定义并初始化一个字符数组
    //char szStr[] = "Hello!";

    //// 定义并初始化一个宽字符变量
    //wchar_t wc = L'A';
    //// 定义并初始化一个指向宽字符串的指针
    //wchar_t *pwStr = L"Hello!";
    //// 定义并初始化一个宽字符数组
    //wchar_t szwStr[] = L"Hello!";

    //////////////////////////////////////////////////////////////////////////
    //char ch = 'A';                // 1
    //wchar_t wch = L'A';           // 2
    //char str[] = "C语言";         // 6
    //wchar_t wstr[] = L"C语言";    // 8
    //printf("ch = %d, wch = %d, str = %d, wstr = %d\n",
    //    sizeof(ch), sizeof(wch), sizeof(str), sizeof(wstr));

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr[] = TEXT("C语言");    // 3 或 5
    //_tprintf(TEXT("_tcslen(szStr) = %d\n"), _tcslen(szStr));

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr[] = TEXT("WindowsAPI是最为强大的编程语言！");
    //LPTSTR lp = _tcschr(szStr, TEXT('最'));
    //_tprintf(TEXT("szStr的地址：%p lp的地址：%p\n"), szStr, lp);
    //_tprintf(TEXT("szStr = %s lp = %s\n"), szStr, lp);
    //// Unicode
    //// szStr的地址：0014FCC0 lp的地址：0014FCD6
    //// szStr = WindowsAPI是最为强大的编程语言！ lp = 最为强大的编程语言！
    //// 多字节：
    //// szStr的地址：003EFE38 lp的地址：003EFE44
    //// szStr = WindowsAPI是最为强大的编程语言！ lp = 最为强大的编程语言！

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr[] = TEXT("WindowsAPI是最为强大的编程语言！");
    ////_tprintf(TEXT("%s\n"), _tcsupr(szStr)); // WINDOWSAPI是最为强大的编程语言！
    ////_tprintf(TEXT("%s\n"), _tcslwr(szStr)); // windowsapi是最为强大的编程语言！
    //_tcsupr_s(szStr, _countof(szStr));
    //_tprintf(TEXT("%s\n"), szStr);
    //_tcslwr_s(szStr, _countof(szStr));
    //_tprintf(TEXT("%s\n"), szStr);

    ////////////////////////////////////////////////////////////////////////////
    //TCHAR szStrDest[64] = TEXT("WindowsAPI");
    //TCHAR szStrSour[] = TEXT("是最为强大的编程语言！");
    //_tcscat_s(szStrDest, _countof(szStrDest), szStrSour);
    //_tprintf(TEXT("%s\n"), szStrDest);  // WindowsAPI是最为强大的编程语言！

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStrDest[64];
    //TCHAR szStrSour[] = TEXT("WindowsAPI是最为强大的编程语言！");
    //_tcscpy_s(szStrDest, _countof(szStrDest), szStrSour);
    //_tprintf(TEXT("%s\n"), szStrDest);  // WindowsAPI是最为强大的编程语言！

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr1[] = TEXT("ABCDE"); // E的ASCII为0x45
    //TCHAR szStr2[] = TEXT("ABCDe"); // e的ASCII为0x65
    //int n = _tcscmp(szStr1, szStr2);
    //if (n > 0)
    //    _tprintf(TEXT("szStr1 大于 szStr2\n"));
    //else if (n == 0)
    //    _tprintf(TEXT("szStr1 等于 szStr2\n"));
    //else
    //    _tprintf(TEXT("szStr1 小于 szStr2\n"));
    //// 输出结果：szStr1 小于 szStr2

    //////////////////////////////////////////////////////////////////////////
    //setlocale(LC_ALL, "chs");   // LC_ALL包括LC_COLLATE
    //TCHAR szStr1[] = TEXT("我爱老王");
    //// Unicode：11 62 31 72 01 80 8B 73 00 00  多字节：CE D2 B0 AE C0 CF CD F5 00
    //TCHAR szStr2[] = TEXT("我是老王");
    //// Unicode：11 62 2F 66 01 80 8B 73 00 00  多字节：CE D2 CA C7 C0 CF CD F5 00

    //int n = _tcscmp(szStr1, szStr2);
    //if (n > 0)
    //    _tprintf(TEXT("szStr1 > szStr2\n"));
    //else if (n == 0)
    //    _tprintf(TEXT("szStr1 == szStr2\n"));
    //else
    //    _tprintf(TEXT("szStr1 < szStr2\n"));
    ////

    //n = _tcscoll(szStr1, szStr2);
    //if (n > 0)
    //    _tprintf(TEXT("szStr1 > szStr2\n"));
    //else if (n == 0)
    //    _tprintf(TEXT("szStr1 == szStr2\n"));
    //else
    //    _tprintf(TEXT("szStr1 < szStr2\n"));
    ////

    //////////////////////////////////////////////////////////////////////////
    //TCHAR strToken[] = TEXT("A string\tof ,,tokens\nand some  more tokens");
    //TCHAR strDelimit[] = TEXT(" ,\t\n");
    //LPTSTR lpToken = NULL;
    //LPTSTR lpTokenNext = NULL;

    //// 获取第一个字符串
    //lpToken = _tcstok_s(strToken, strDelimit, &lpTokenNext);

    //// 循环查找
    //while (lpToken != NULL)
    //{
    //    _tprintf(TEXT("%s\n"), lpToken);
    //    // 获取下一个
    //    lpToken = _tcstok_s(NULL, strDelimit, &lpTokenNext);
    //}
    ///*
    //输出结果：
    //A
    //string
    //of
    //tokens
    //and
    //some
    //more
    //tokens
    //*/

    //////////////////////////////////////////////////////////////////////////
    /*setlocale(LC_ALL, "chs");

    LPTSTR arrStr[] = {
        TEXT("架构风格之资源管理.AVI"),
        TEXT("模块化之合理内聚.AVI"),
        TEXT("总结.AVI"),
        TEXT("模块化之管理依赖.AVI"),
        TEXT("系统架构设计概述.AVI"),
        TEXT("架构风格之分布式.AVI")
    };
    qsort(arrStr, _countof(arrStr), sizeof(LPTSTR) , compare);

    for (int i = 0; i < _countof(arrStr); i++)
        _tprintf(TEXT("%s\n"), arrStr[i]);*/

        //////////////////////////////////////////////////////////////////////////
        /*TCHAR szStr[] = TEXT("Hello, Windows, Windows API program simple and powerful!");
        TCHAR szStrSearch[] = TEXT("Windows");

        _tprintf(TEXT("%s\n"), _tcsstr(szStr, szStrSearch));
        // Windows, Windows API program simple and powerful!*/

        //////////////////////////////////////////////////////////////////////////
        /*TCHAR szStr[] = TEXT("The 3 men and 2 boys ate 5 pigs");
        TCHAR szStrCharSet[] = TEXT("0123456789");
        LPTSTR lpSearch = NULL;

        _tprintf(TEXT("1: %s\n"), szStr);
        lpSearch = _tcspbrk(szStr, szStrCharSet);
        _tprintf(TEXT("2: %s\n"), lpSearch++);
        lpSearch = _tcspbrk(lpSearch, szStrCharSet);
        _tprintf(TEXT("3: %s\n"), lpSearch);*/

        //////////////////////////////////////////////////////////////////////////
        /*TCHAR szStr[] = TEXT("Hello, Windows, Windows API program simple and 强大!");
        for (size_t i = 0; i < _tcslen(szStr); i++)
        {
            _tprintf(TEXT("%c"), _totupper(szStr[i]));
            // HELLO, WINDOWS, WINDOWS API PROGRAM SIMPLE AND 强大!
        }
        _tprintf(TEXT("\n"));
        for (size_t i = 0; i < _tcslen(szStr); i++)
        {
            _tprintf(TEXT("%c"), _totlower(szStr[i]));
            // hello, windows, windows api program simple and 强大!
        }*/

        //////////////////////////////////////////////////////////////////////////
        /*_tprintf(TEXT("char = %d, int = %d, float = %d, double = %d\n"),
            sizeof(char), sizeof(int), sizeof(float), sizeof(double));
        // char = 1, int = 4, float = 4, double = 8*/


    return 0;
}

int compare(const void* arg1, const void* arg2)
{
    // arg1、arg2是数组元素的指针，所以需要*(LPTSTR *)
    return _tcscoll(*(LPTSTR*)arg1, *(LPTSTR*)arg2);
}




