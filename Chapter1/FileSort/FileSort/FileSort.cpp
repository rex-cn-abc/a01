#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

// 函数声明
int compare(const void* arg1, const void* arg2);

int main()
{
    setlocale(LC_ALL, "chs");

    LPCTSTR arrStr[] = {
        TEXT("4、原理—开发风格之资源管理.AVI"),
        TEXT("11、原理—总结.AVI"),
        TEXT("8、原理—模块化之管理依赖.AVI"),
        TEXT("6、原理—架构风格之适配与扩展.AVI"),
        TEXT("1、原理—系统架构设计概述.AVI"),
        TEXT("7、原理—模块化之重用与内聚.AVI"),
        TEXT("10、原理—模块化之确保扩展.AVI"),
        TEXT("3、原理—架构风格之分布式.AVI"),
        TEXT("9、原理—模块化之保持可用.AVI"),
        TEXT("2、原理—架构风格之系统结构.AVI"),
        TEXT("5、原理—架构风格之事件驱动.AVI"),
        TEXT("4、原理—架构风格之资源管理.AVI")
    };
    qsort(arrStr, _countof(arrStr), sizeof(LPTSTR), compare);

    for (int i = 0; i < _countof(arrStr); i++)
        _tprintf(TEXT("%s\n"), arrStr[i]);

    system("pause");
    return 0;
}

//int compare(const void *arg1, const void *arg2)
//{
//    LPTSTR p1 = NULL;
//    LPTSTR p2 = NULL;   // p1 p2返回的是数字字符后面的字符串
//    double d1 = _tcstod(*(LPTSTR *)arg1, &p1);
//    double d2 = _tcstod(*(LPTSTR *)arg2, &p2);
//    if (d1 != d2)
//    {
//        if (d1 > d2)
//            return 1;
//        else
//            return -1;
//    }
//    else
//    {
//        return _tcscoll(p1, p2);
//    }
//}

int compare(const void* arg1, const void* arg2)
{
    return CompareStringEx(LOCALE_NAME_USER_DEFAULT, SORT_DIGITSASNUMBERS,
        *(LPTSTR*)arg1, -1, *(LPTSTR*)arg2, -1, NULL, NULL, NULL) - 2;
}