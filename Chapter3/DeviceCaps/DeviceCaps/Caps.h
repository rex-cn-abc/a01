#pragma once
GetDeviceCaps(hdc, LOGPIXELSX)
GetDeviceCaps(hdc, LOGPIXELSY)
struct
{
    int     m_nIndex;
    PTSTR   m_pLabel;
    PTSTR   m_pDesc;
}CAPS[] = {
    HORZRES,            TEXT("HORZRES"),            TEXT("屏幕的宽度，对于打印机则是页面可打印区域的宽度"),
    VERTRES,            TEXT("VERTRES"),            TEXT("屏幕的高度，对于打印机则是页面可打印区域的高度"),
    HORZSIZE,           TEXT("HORZSIZE"),           TEXT("物理屏幕的宽度(毫米)"),
    VERTSIZE,           TEXT("VERTSIZE"),           TEXT("物理屏幕的高度(毫米)"),
    LOGPIXELSX,         TEXT("LOGPIXELSX"),         TEXT("沿屏幕宽度每逻辑英寸的像素数"),
    LOGPIXELSY,         TEXT("LOGPIXELSY"),         TEXT("沿屏幕高度每逻辑英寸的像素数"),
    PLANES,             TEXT("PLANES"),             TEXT("颜色平面数"),
    BITSPIXEL,          TEXT("BITSPIXEL"),          TEXT("每个像素的颜色位数"),
    VREFRESH,           TEXT("VREFRESH"),           TEXT("设备的当前垂直刷新率"),
    ASPECTX,            TEXT("ASPECTX"),            TEXT("用于线条绘制的设备像素的相对宽度"),
    ASPECTY,            TEXT("ASPECTY"),            TEXT("用于线条绘制的设备像素的相对高度"),
    ASPECTXY,           TEXT("ASPECTXY"),           TEXT("用于线条绘制的设备像素的对角线宽度"),
    BLTALIGNMENT,       TEXT("BLTALIGNMENT"),       TEXT("首选水平绘图对齐方式，表示为像素的倍数"),
    CLIPCAPS,           TEXT("CLIPCAPS"),           TEXT("如果设备可以剪辑为矩形，则为1；否则，它是0"),
    COLORMGMTCAPS,      TEXT("COLORMGMTCAPS"),      TEXT("设备的颜色管理功能"),
    COLORRES,           TEXT("COLORRES"),           TEXT("设备的实际颜色分辨率"),
    CURVECAPS,          TEXT("CURVECAPS"),          TEXT("设备的曲线能力"),
    DRIVERVERSION,      TEXT("DRIVERVERSION"),      TEXT("设备驱动程序版本"),
    LINECAPS,           TEXT("LINECAPS"),           TEXT("设备的线路能力"),
    NUMBRUSHES,         TEXT("NUMBRUSHES"),         TEXT("设备专用画刷的数量"),
    NUMCOLORS,          TEXT("NUMCOLORS"),          TEXT("设备颜色表中的条目数，对于颜色深度较大的设备则返回1"),
    NUMFONTS,           TEXT("NUMFONTS"),           TEXT("设备专用字体的数量"),
    NUMPENS,            TEXT("NUMPENS"),            TEXT("设备专用画笔的数量"),
    NUMRESERVED,        TEXT("NUMRESERVED"),        TEXT("系统调色板中保留的条目数"),
    PDEVICESIZE,        TEXT("PDEVICESIZE"),        TEXT("保留"),
    PHYSICALHEIGHT,     TEXT("PHYSICALHEIGHT"),     TEXT("打印设备物理页的高度"),
    PHYSICALOFFSETX,    TEXT("PHYSICALOFFSETX"),    TEXT("打印设备从物理页的左边缘到可打印区域的左边缘的距离"),
    PHYSICALOFFSETY,    TEXT("PHYSICALOFFSETY"),    TEXT("打印设备从物理页的上边缘到可打印区域的上边缘的距离"),
    PHYSICALWIDTH,      TEXT("PHYSICALWIDTH"),      TEXT("打印设备物理页的宽度"),
    POLYGONALCAPS,      TEXT("POLYGONALCAPS"),      TEXT("设备的多边形功能"),
    RASTERCAPS,         TEXT("RASTERCAPS"),         TEXT("设备的光栅功能"),
    SCALINGFACTORX,     TEXT("SCALINGFACTORX"),     TEXT("打印机X轴的比例因子"),
    SCALINGFACTORY,     TEXT("SCALINGFACTORY"),     TEXT("打印机Y轴的比例因子"),
    SHADEBLENDCAPS,     TEXT("SHADEBLENDCAPS"),     TEXT("设备的着色和混合功能"),
    SIZEPALETTE,        TEXT("SIZEPALETTE"),        TEXT("系统调色板中的条目数"),
    TECHNOLOGY,         TEXT("TECHNOLOGY"),         TEXT("设备技术"),
    TEXTCAPS,           TEXT("TEXTCAPS"),           TEXT("设备的文本功能"),
};