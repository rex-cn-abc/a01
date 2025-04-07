#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

// ��������
int compare(const void* arg1, const void* arg2);

int main()
{
    setlocale(LC_ALL, "chs");   // ��_tprintf�����������Ҫ���ñ�������������

    //// ���岢��ʼ��һ���ַ�����
    //char c = 'A';
    //// ���岢��ʼ��һ��ָ���ַ�����ָ��
    //char *pStr = "Hello!";
    //// ���岢��ʼ��һ���ַ�����
    //char szStr[] = "Hello!";

    //// ���岢��ʼ��һ�����ַ�����
    //wchar_t wc = L'A';
    //// ���岢��ʼ��һ��ָ����ַ�����ָ��
    //wchar_t *pwStr = L"Hello!";
    //// ���岢��ʼ��һ�����ַ�����
    //wchar_t szwStr[] = L"Hello!";

    //////////////////////////////////////////////////////////////////////////
    //char ch = 'A';                // 1
    //wchar_t wch = L'A';           // 2
    //char str[] = "C����";         // 6
    //wchar_t wstr[] = L"C����";    // 8
    //printf("ch = %d, wch = %d, str = %d, wstr = %d\n",
    //    sizeof(ch), sizeof(wch), sizeof(str), sizeof(wstr));

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr[] = TEXT("C����");    // 3 �� 5
    //_tprintf(TEXT("_tcslen(szStr) = %d\n"), _tcslen(szStr));

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr[] = TEXT("WindowsAPI����Ϊǿ��ı�����ԣ�");
    //LPTSTR lp = _tcschr(szStr, TEXT('��'));
    //_tprintf(TEXT("szStr�ĵ�ַ��%p lp�ĵ�ַ��%p\n"), szStr, lp);
    //_tprintf(TEXT("szStr = %s lp = %s\n"), szStr, lp);
    //// Unicode
    //// szStr�ĵ�ַ��0014FCC0 lp�ĵ�ַ��0014FCD6
    //// szStr = WindowsAPI����Ϊǿ��ı�����ԣ� lp = ��Ϊǿ��ı�����ԣ�
    //// ���ֽڣ�
    //// szStr�ĵ�ַ��003EFE38 lp�ĵ�ַ��003EFE44
    //// szStr = WindowsAPI����Ϊǿ��ı�����ԣ� lp = ��Ϊǿ��ı�����ԣ�

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr[] = TEXT("WindowsAPI����Ϊǿ��ı�����ԣ�");
    ////_tprintf(TEXT("%s\n"), _tcsupr(szStr)); // WINDOWSAPI����Ϊǿ��ı�����ԣ�
    ////_tprintf(TEXT("%s\n"), _tcslwr(szStr)); // windowsapi����Ϊǿ��ı�����ԣ�
    //_tcsupr_s(szStr, _countof(szStr));
    //_tprintf(TEXT("%s\n"), szStr);
    //_tcslwr_s(szStr, _countof(szStr));
    //_tprintf(TEXT("%s\n"), szStr);

    ////////////////////////////////////////////////////////////////////////////
    //TCHAR szStrDest[64] = TEXT("WindowsAPI");
    //TCHAR szStrSour[] = TEXT("����Ϊǿ��ı�����ԣ�");
    //_tcscat_s(szStrDest, _countof(szStrDest), szStrSour);
    //_tprintf(TEXT("%s\n"), szStrDest);  // WindowsAPI����Ϊǿ��ı�����ԣ�

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStrDest[64];
    //TCHAR szStrSour[] = TEXT("WindowsAPI����Ϊǿ��ı�����ԣ�");
    //_tcscpy_s(szStrDest, _countof(szStrDest), szStrSour);
    //_tprintf(TEXT("%s\n"), szStrDest);  // WindowsAPI����Ϊǿ��ı�����ԣ�

    //////////////////////////////////////////////////////////////////////////
    //TCHAR szStr1[] = TEXT("ABCDE"); // E��ASCIIΪ0x45
    //TCHAR szStr2[] = TEXT("ABCDe"); // e��ASCIIΪ0x65
    //int n = _tcscmp(szStr1, szStr2);
    //if (n > 0)
    //    _tprintf(TEXT("szStr1 ���� szStr2\n"));
    //else if (n == 0)
    //    _tprintf(TEXT("szStr1 ���� szStr2\n"));
    //else
    //    _tprintf(TEXT("szStr1 С�� szStr2\n"));
    //// ��������szStr1 С�� szStr2

    //////////////////////////////////////////////////////////////////////////
    //setlocale(LC_ALL, "chs");   // LC_ALL����LC_COLLATE
    //TCHAR szStr1[] = TEXT("�Ұ�����");
    //// Unicode��11 62 31 72 01 80 8B 73 00 00  ���ֽڣ�CE D2 B0 AE C0 CF CD F5 00
    //TCHAR szStr2[] = TEXT("��������");
    //// Unicode��11 62 2F 66 01 80 8B 73 00 00  ���ֽڣ�CE D2 CA C7 C0 CF CD F5 00

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

    //// ��ȡ��һ���ַ���
    //lpToken = _tcstok_s(strToken, strDelimit, &lpTokenNext);

    //// ѭ������
    //while (lpToken != NULL)
    //{
    //    _tprintf(TEXT("%s\n"), lpToken);
    //    // ��ȡ��һ��
    //    lpToken = _tcstok_s(NULL, strDelimit, &lpTokenNext);
    //}
    ///*
    //��������
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
        TEXT("�ܹ����֮��Դ����.AVI"),
        TEXT("ģ�黯֮�����ھ�.AVI"),
        TEXT("�ܽ�.AVI"),
        TEXT("ģ�黯֮��������.AVI"),
        TEXT("ϵͳ�ܹ���Ƹ���.AVI"),
        TEXT("�ܹ����֮�ֲ�ʽ.AVI")
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
        /*TCHAR szStr[] = TEXT("Hello, Windows, Windows API program simple and ǿ��!");
        for (size_t i = 0; i < _tcslen(szStr); i++)
        {
            _tprintf(TEXT("%c"), _totupper(szStr[i]));
            // HELLO, WINDOWS, WINDOWS API PROGRAM SIMPLE AND ǿ��!
        }
        _tprintf(TEXT("\n"));
        for (size_t i = 0; i < _tcslen(szStr); i++)
        {
            _tprintf(TEXT("%c"), _totlower(szStr[i]));
            // hello, windows, windows api program simple and ǿ��!
        }*/

        //////////////////////////////////////////////////////////////////////////
        /*_tprintf(TEXT("char = %d, int = %d, float = %d, double = %d\n"),
            sizeof(char), sizeof(int), sizeof(float), sizeof(double));
        // char = 1, int = 4, float = 4, double = 8*/


    return 0;
}

int compare(const void* arg1, const void* arg2)
{
    // arg1��arg2������Ԫ�ص�ָ�룬������Ҫ*(LPTSTR *)
    return _tcscoll(*(LPTSTR*)arg1, *(LPTSTR*)arg2);
}




