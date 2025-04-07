#pragma once
GetDeviceCaps(hdc, LOGPIXELSX)
GetDeviceCaps(hdc, LOGPIXELSY)
struct
{
    int     m_nIndex;
    PTSTR   m_pLabel;
    PTSTR   m_pDesc;
}CAPS[] = {
    HORZRES,            TEXT("HORZRES"),            TEXT("��Ļ�Ŀ�ȣ����ڴ�ӡ������ҳ��ɴ�ӡ����Ŀ��"),
    VERTRES,            TEXT("VERTRES"),            TEXT("��Ļ�ĸ߶ȣ����ڴ�ӡ������ҳ��ɴ�ӡ����ĸ߶�"),
    HORZSIZE,           TEXT("HORZSIZE"),           TEXT("������Ļ�Ŀ��(����)"),
    VERTSIZE,           TEXT("VERTSIZE"),           TEXT("������Ļ�ĸ߶�(����)"),
    LOGPIXELSX,         TEXT("LOGPIXELSX"),         TEXT("����Ļ���ÿ�߼�Ӣ���������"),
    LOGPIXELSY,         TEXT("LOGPIXELSY"),         TEXT("����Ļ�߶�ÿ�߼�Ӣ���������"),
    PLANES,             TEXT("PLANES"),             TEXT("��ɫƽ����"),
    BITSPIXEL,          TEXT("BITSPIXEL"),          TEXT("ÿ�����ص���ɫλ��"),
    VREFRESH,           TEXT("VREFRESH"),           TEXT("�豸�ĵ�ǰ��ֱˢ����"),
    ASPECTX,            TEXT("ASPECTX"),            TEXT("�����������Ƶ��豸���ص���Կ��"),
    ASPECTY,            TEXT("ASPECTY"),            TEXT("�����������Ƶ��豸���ص���Ը߶�"),
    ASPECTXY,           TEXT("ASPECTXY"),           TEXT("�����������Ƶ��豸���صĶԽ��߿��"),
    BLTALIGNMENT,       TEXT("BLTALIGNMENT"),       TEXT("��ѡˮƽ��ͼ���뷽ʽ����ʾΪ���صı���"),
    CLIPCAPS,           TEXT("CLIPCAPS"),           TEXT("����豸���Լ���Ϊ���Σ���Ϊ1����������0"),
    COLORMGMTCAPS,      TEXT("COLORMGMTCAPS"),      TEXT("�豸����ɫ������"),
    COLORRES,           TEXT("COLORRES"),           TEXT("�豸��ʵ����ɫ�ֱ���"),
    CURVECAPS,          TEXT("CURVECAPS"),          TEXT("�豸����������"),
    DRIVERVERSION,      TEXT("DRIVERVERSION"),      TEXT("�豸��������汾"),
    LINECAPS,           TEXT("LINECAPS"),           TEXT("�豸����·����"),
    NUMBRUSHES,         TEXT("NUMBRUSHES"),         TEXT("�豸ר�û�ˢ������"),
    NUMCOLORS,          TEXT("NUMCOLORS"),          TEXT("�豸��ɫ���е���Ŀ����������ɫ��Ƚϴ���豸�򷵻�1"),
    NUMFONTS,           TEXT("NUMFONTS"),           TEXT("�豸ר�����������"),
    NUMPENS,            TEXT("NUMPENS"),            TEXT("�豸ר�û��ʵ�����"),
    NUMRESERVED,        TEXT("NUMRESERVED"),        TEXT("ϵͳ��ɫ���б�������Ŀ��"),
    PDEVICESIZE,        TEXT("PDEVICESIZE"),        TEXT("����"),
    PHYSICALHEIGHT,     TEXT("PHYSICALHEIGHT"),     TEXT("��ӡ�豸����ҳ�ĸ߶�"),
    PHYSICALOFFSETX,    TEXT("PHYSICALOFFSETX"),    TEXT("��ӡ�豸������ҳ�����Ե���ɴ�ӡ��������Ե�ľ���"),
    PHYSICALOFFSETY,    TEXT("PHYSICALOFFSETY"),    TEXT("��ӡ�豸������ҳ���ϱ�Ե���ɴ�ӡ������ϱ�Ե�ľ���"),
    PHYSICALWIDTH,      TEXT("PHYSICALWIDTH"),      TEXT("��ӡ�豸����ҳ�Ŀ��"),
    POLYGONALCAPS,      TEXT("POLYGONALCAPS"),      TEXT("�豸�Ķ���ι���"),
    RASTERCAPS,         TEXT("RASTERCAPS"),         TEXT("�豸�Ĺ�դ����"),
    SCALINGFACTORX,     TEXT("SCALINGFACTORX"),     TEXT("��ӡ��X��ı�������"),
    SCALINGFACTORY,     TEXT("SCALINGFACTORY"),     TEXT("��ӡ��Y��ı�������"),
    SHADEBLENDCAPS,     TEXT("SHADEBLENDCAPS"),     TEXT("�豸����ɫ�ͻ�Ϲ���"),
    SIZEPALETTE,        TEXT("SIZEPALETTE"),        TEXT("ϵͳ��ɫ���е���Ŀ��"),
    TECHNOLOGY,         TEXT("TECHNOLOGY"),         TEXT("�豸����"),
    TEXTCAPS,           TEXT("TEXTCAPS"),           TEXT("�豸���ı�����"),
};