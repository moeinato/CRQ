
// Codes pocessing data

#include "rh.h"

enum { PAD_BYTE_A = 236, PAD_BYTE_B = 17 };

int qrc(HWND hwnd, int oLen, WCHAR * oText, int& qrVer, char* sByte)
{
    const int VLBMQ[41] = { 0, 11, 20, 32, 46, 60, 74, 86, 108, 130, 151, 177, 203, 241, 258, 292, 322, 364, 394, 442, 482, 509, 565, 611, 661, 715, 751, 805, 868, 908, 982, 1030, 1112, 1168, 1228, 1283, 1351, 1423, 1499, 1579, 1663 };

    char countIndicatorL = 0;
    WCHAR countIndicatorH = 0;
    int rbECApDigit = 0, byteCount = 0, bPtrLo = 0, capaFillCount = 0;

    byteCount = WideCharToMultiByte(CP_UTF8, 0, oText, oLen, sByte + 4, Q40 - 5, NULL, NULL);

    if (byteCount > 1250 || byteCount == 0)
        return MessageBox(hwnd, TEXT("文本量超出限制。"), TEXT(""), MB_ICONERROR | MB_OK);

    for (qrVer = 1; qrVer <= 40; ++qrVer)
        if (byteCount <= VLBMQ[qrVer])
            break;

    /* 0100 << 4 */
    sByte[0] = 64;

    int byteLen = 0;
    if (qrVer <= 9)
    {
        rbECApDigit = 2;
        sByte[1] = 0xFF & byteCount;

        sByte[0] |= 0xF  & sByte[1] >> 4;
        sByte[1]  = 0xF  & sByte[1];
        sByte[1]  = 0xF0 & sByte[1] << 4;

        for (byteLen = 1; byteLen <= byteCount; ++byteLen)
            sByte[byteLen]    |= 0xF  & sByte[byteLen + 3] >> 4,
            sByte[byteLen + 1] = 0xF  & sByte[byteLen + 1],
            sByte[byteLen + 1] = 0xF0 & sByte[byteLen + 3] << 4;
        sByte[++byteLen] = 0;

    }
    else
    {
        rbECApDigit = 3;
        sByte[1] = byteCount >> 8;
        sByte[2] = byteCount;

        sByte[0] |= 0xF  & sByte[1] >> 4;
        sByte[1]  = 0xF  & sByte[1];
        sByte[1]  = 0xF0 & sByte[1] << 4;

        sByte[1] |= 0xF  & sByte[2] >> 4;
        sByte[2]  = 0xF  & sByte[2];
        sByte[2]  = 0xF0 & sByte[2] << 4;

        for (byteLen = 2; byteLen <= byteCount + 1; ++byteLen)
            sByte[byteLen]    |= 0xF  & sByte[byteLen + 2] >> 4,
            sByte[byteLen + 1] = 0xF  & sByte[byteLen + 1],
            sByte[byteLen + 1] = 0xF0 & sByte[byteLen + 2] << 4;
        sByte[++byteLen] = 0;
    }

    capaFillCount = VLBMQ[qrVer] + rbECApDigit - byteLen;
    for (int i = capaFillCount, fFlag = 0; i > 0; --i, fFlag = ~fFlag)
        sByte[byteLen++] = fFlag ? PAD_BYTE_B : PAD_BYTE_A;

    return 0;
}
