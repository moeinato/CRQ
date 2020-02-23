
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef RH_H_
#define RH_H_

#include <windows.h>

enum QR_CONTROL {
    QR_PATTERN = 1,
    QR_PATTERN_INFO = 1 & QR_PATTERN,

    QR_DATA = 1,
    QR_DATA_EC = 1,

    QR_FILLDATA = 0,
    QR_MASK = 1
};

enum { Q40 = 1696 };

struct DFStruct
{
    bool isPattern;
    bool digit;
};

int qrc(HWND, int, wchar_t*, int&, char*);

int qec(unsigned char*, char*, int, HWND);

void qmap(DFStruct**, int, int, unsigned char*);

#endif // RH_H_
