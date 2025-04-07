#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

// ��������
int compare(const void* arg1, const void* arg2);

int main()
{
    setlocale(LC_ALL, "chs");

    LPCTSTR arrStr[] = {
        TEXT("4��ԭ���������֮��Դ����.AVI"),
        TEXT("11��ԭ���ܽ�.AVI"),
        TEXT("8��ԭ��ģ�黯֮��������.AVI"),
        TEXT("6��ԭ���ܹ����֮��������չ.AVI"),
        TEXT("1��ԭ��ϵͳ�ܹ���Ƹ���.AVI"),
        TEXT("7��ԭ��ģ�黯֮�������ھ�.AVI"),
        TEXT("10��ԭ��ģ�黯֮ȷ����չ.AVI"),
        TEXT("3��ԭ���ܹ����֮�ֲ�ʽ.AVI"),
        TEXT("9��ԭ��ģ�黯֮���ֿ���.AVI"),
        TEXT("2��ԭ���ܹ����֮ϵͳ�ṹ.AVI"),
        TEXT("5��ԭ���ܹ����֮�¼�����.AVI"),
        TEXT("4��ԭ���ܹ����֮��Դ����.AVI")
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
//    LPTSTR p2 = NULL;   // p1 p2���ص��������ַ�������ַ���
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