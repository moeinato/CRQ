
// Codes about EC info.

#include "rh.h"
#include <cstring>

enum { DATA_GEN = 1, EC_GEN = 1 };

const int qECperBLOCK[41] = {
    0,
    13, 22, 18, 26, 18, 24, 18, 22, 20, 24,
    28, 26, 24, 20, 30, 24, 28, 28, 26, 30,
    28, 30, 30, 30, 30, 28, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30
};

const int polAlp13[13] = { 78, 140, 206, 218, 130, 104, 106, 100, 86, 100, 176, 152, 74 };
const int polAlp18[18] = { 153, 96, 98, 5, 179, 252, 148, 152, 187, 79, 170, 118, 97, 184, 94, 158, 234, 215 };
const int polAlp20[20] = { 190, 188, 212, 212, 164, 156, 239, 83, 225, 221, 180, 202, 187, 26, 163, 61, 50, 79, 60, 17 };
const int polAlp22[22] = { 231, 165, 105, 160, 134, 219, 80, 98, 172, 8, 74, 200, 53, 221, 109, 14, 230, 93, 242, 247, 171, 210 };
const int polAlp24[24] = { 21, 227, 96, 87, 232, 117, 0, 111, 218, 228, 226, 192, 152, 169, 180, 159, 126, 251, 117, 211, 48, 135, 121, 229 };
const int polAlp26[26] = { 70, 218, 145, 153, 227, 48, 102, 13, 142, 245, 21, 161, 53, 165, 28, 111, 201, 145, 17, 118, 182, 103, 2, 158, 125, 173 };
const int polAlp28[28] = { 123, 9, 37, 242, 119, 212, 195, 42, 87, 245, 43, 21, 201, 232, 27, 205, 147, 195, 190, 110, 180, 108, 234, 224, 104, 200, 223, 168 };
const int polAlp30[30] = { 180, 192, 40, 238, 216, 251, 37, 156, 130, 224, 193, 226, 173, 42, 125, 222, 96, 239, 86, 110, 48, 50, 182, 179, 31, 216, 152, 145, 173, 41 };

const int numOfBlocksInG1[41] = { 0, 1, 1, 2, 2, 2, 4, 2, 4, 4, 6, 4, 4, 8, 11, 5, 15, 1, 17, 17, 15, 17, 7, 11, 11, 7, 28, 8, 4, 1, 15, 42, 10, 29, 44, 39, 46, 49, 48, 43, 34 };
const int numOfCwInG1B[41] = { 0, 13, 22, 17, 24, 15, 19, 14, 18, 16, 19, 22, 20, 20, 16, 24, 19, 22, 22, 21, 24, 22, 24, 24, 24, 24, 22, 23, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 };

const int numOfBlocksInG2[41] = { 0, 0, 0, 0, 0, 2, 0, 4, 2, 4, 2, 4, 6, 4, 5, 7, 2, 15, 1, 4, 5, 6, 16, 14, 16, 22, 6, 26, 31, 37, 25, 1, 35, 19, 7, 14, 10, 10, 14, 22, 34 };
const int numOfCwInG2B[41] = { 0, 0, 0, 0, 0, 16, 0, 15, 19, 17, 20, 23, 21, 21, 17, 25, 20, 23, 23, 22, 25, 23, 25, 25, 25, 25, 23, 24, 25, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 };

int calcEC(int qrVer, unsigned char* codeWord, unsigned char* ecWord, int group, HWND hwnd)
{
    int alpha[256] = { 1 };

    for (int i = 1; i < 256; ++i)
        if ((alpha[i] = 2 * alpha[i - 1]) >= 256)
            alpha[i] ^= 285;

    /* Message in integer */
    unsigned char messagePolynominal[70] = { 0 };

    /* Generator in alpha */
    int generatorPolynominal[70] = { 0 };
    int codeWordLength = (group == 1 ? numOfCwInG1B[qrVer] : numOfCwInG2B[qrVer]);
    for (int i = codeWordLength - 1, j = 0; i >= 0; --i, ++j)
        messagePolynominal[i + qECperBLOCK[qrVer]] = codeWord[j];

    int genPolyLo = codeWordLength + qECperBLOCK[qrVer] - 1;

    // -> 
    for (; genPolyLo > qECperBLOCK[qrVer] - 1;)
    {
        int j = 0;
        for (j; j < qECperBLOCK[qrVer]; ++j)
        {
            switch (qECperBLOCK[qrVer])
            {
            case 13:
                generatorPolynominal[j] = polAlp13[j];
                break;
            case 18:
                generatorPolynominal[j] = polAlp18[j];
                break;
            case 20:
                generatorPolynominal[j] = polAlp20[j];
                break;
            case 22:
                generatorPolynominal[j] = polAlp22[j];
                break;
            case 24:
                generatorPolynominal[j] = polAlp24[j];
                break;
            case 26:
                generatorPolynominal[j] = polAlp26[j];
                break;
            case 28:
                generatorPolynominal[j] = polAlp28[j];
                break;
            case 30:
                generatorPolynominal[j] = polAlp30[j];
                break;
            default:
                return MessageBox(hwnd, TEXT("ERROR: EC.C Failed."), TEXT("ERROR"), MB_OK | MB_ICONERROR);
            }
        }
        generatorPolynominal[j] = 0;

        if (genPolyLo > qECperBLOCK[qrVer])
            for (int i = qECperBLOCK[qrVer]; i >= 0; --i)
                generatorPolynominal[i + genPolyLo - qECperBLOCK[qrVer]] = generatorPolynominal[i],
                generatorPolynominal[i] = 0;

        for (int i = 0, quitFlag = 1; i < 256 && quitFlag; ++i)
            if (alpha[i] == messagePolynominal[genPolyLo])
            {
                /* replace genPolyLo to ... and ... */
                for (int j = genPolyLo - qECperBLOCK[qrVer]; j <= genPolyLo; ++j)
                    generatorPolynominal[j] = (generatorPolynominal[j] + i) % 255;
                quitFlag = 0;
            }

        --genPolyLo;

        int lowestLo = 0;
        for (int s = 0; s <= genPolyLo; ++s)
            if (messagePolynominal[s] || generatorPolynominal[s])
            {
                lowestLo = s;
                break;
            }

        for (int j = lowestLo; j <= genPolyLo; ++j)
            messagePolynominal[j] = alpha[generatorPolynominal[j]] ^ messagePolynominal[j];
    }
    // <-

    for (int p = 0, q = qECperBLOCK[qrVer] - 1; p < qECperBLOCK[qrVer]; ++p, --q)
        ecWord[p] = messagePolynominal[q];
    return 0;
}

int qec(unsigned char* byteFin, char* sByte, int qrVer, HWND hwnd)
{
    unsigned char ecByte[Q40] = { 0 };
    int currentLo = 0, ecLo = 0, finLo = 0;

    /* Group 1 */
    for (int i = 0; i < numOfBlocksInG1[qrVer]; ++i)
    {
        unsigned char ecWord[32] = { 0 };
        unsigned char codeWord[32] = { 0 };
        memcpy(codeWord, sByte + currentLo, numOfCwInG1B[qrVer]);
        currentLo += numOfCwInG1B[qrVer];
        if (calcEC(qrVer, codeWord, ecWord, 1, hwnd))
            return 1;

        memcpy(ecByte + ecLo, ecWord, qECperBLOCK[qrVer]);
        ecLo += qECperBLOCK[qrVer];
    }

    /* Group 2, if */
    for (int i = 0; i < numOfBlocksInG2[qrVer]; ++i)
    {
        unsigned char ecWord[32] = { 0 };
        unsigned char codeWord[32] = { 0 };
        memcpy(codeWord, sByte + currentLo, numOfCwInG2B[qrVer]);
        currentLo += numOfCwInG2B[qrVer];
        if (calcEC(qrVer, codeWord, ecWord, 2, hwnd))
            return 1;

        memcpy(ecByte + ecLo, ecWord, qECperBLOCK[qrVer]);
        ecLo += qECperBLOCK[qrVer];
    }

    for (int cwid = 0; cwid < 32; ++cwid)
        for (int grpid = 1; grpid <= 2; ++grpid)
            for (int blkid = 0,
                     bNum = (grpid == 1 ? numOfBlocksInG1[qrVer] : numOfBlocksInG2[qrVer]),
                     wNum = (grpid == 1 ? numOfCwInG1B[qrVer] : numOfCwInG2B[qrVer]);
                 blkid < bNum; ++blkid)
            {
                if (cwid >= wNum)
                    break;

                if (grpid == 1)
                    byteFin[finLo++] = QR_DATA ? sByte[blkid * wNum + cwid] : 0;
                else  /* grpid == 2 */
                    byteFin[finLo++] = QR_DATA ? sByte[numOfBlocksInG1[qrVer] * numOfCwInG1B[qrVer] + blkid * wNum + cwid] : 0;
            }

    for (int ecwid = 0, blockNum = numOfBlocksInG1[qrVer] + numOfBlocksInG2[qrVer]; ecwid < qECperBLOCK[qrVer]; ++ecwid)
        for (int blkid = 0; blkid < blockNum; ++blkid)
            byteFin[finLo++] = QR_DATA_EC ? ecByte[blkid * qECperBLOCK[qrVer] + ecwid] : 0;

    return 0;
}
