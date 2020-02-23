
// Codes about mapping data

#include "rh.h"

void qmap(DFStruct** dfs, int qrVer, int width, unsigned char* byteFin)
{
    const int alignmentLo[290] = {
        0, 0, 0, 0, 0, 0, 0,
        6, 18, 0, 0, 0, 0, 0, /* ver. 2 */
        6, 22, 0, 0, 0, 0, 0,
        6, 26, 0, 0, 0, 0, 0,
        6, 30, 0, 0, 0, 0, 0,
        6, 34, 0, 0, 0, 0, 0,
        6, 22, 38, 0, 0, 0, 0,
        6, 24, 42, 0, 0, 0, 0,
        6, 26, 46, 0, 0, 0, 0,
        6, 28, 50, 0, 0, 0, 0,
        6, 30, 54, 0, 0, 0, 0,
        6, 32, 58, 0, 0, 0, 0,
        6, 34, 62, 0, 0, 0, 0,
        6, 26, 46, 66, 0, 0, 0,
        6, 26, 48, 70, 0, 0, 0,
        6, 26, 50, 74, 0, 0, 0,
        6, 30, 54, 78, 0, 0, 0,
        6, 30, 56, 82, 0, 0, 0,
        6, 30, 58, 86, 0, 0, 0,
        6, 34, 62, 90, 0, 0, 0,
        6, 28, 50, 72, 94, 0, 0,
        6, 26, 50, 74, 98, 0, 0,
        6, 30, 54, 78, 102, 0, 0,
        6, 28, 54, 80, 106, 0, 0,
        6, 32, 58, 84, 110, 0, 0,
        6, 30, 58, 86, 114, 0, 0,
        6, 34, 62, 90, 118, 0, 0,
        6, 26, 50, 74, 98, 122, 0,
        6, 30, 54, 78, 102, 126, 0,
        6, 26, 52, 78, 104, 130, 0,
        6, 30, 56, 82, 108, 134, 0,
        6, 34, 60, 86, 112, 138, 0,
        6, 30, 58, 86, 114, 142, 0,
        6, 34, 62, 90, 118, 146, 0,
        6, 30, 54, 78, 102, 128, 150,
        6, 24, 50, 76, 102, 128, 154,
        6, 28, 54, 80, 106, 132, 158,
        6, 32, 58, 84, 110, 136, 162,
        6, 26, 54, 82, 110, 138, 166,
        6, 30, 58, 86, 114, 142, 170,
        0 };

    const int verInfo[41] = {
        0, 0, 0, 0, 0, 0, 0,
        31892, /* Version 7 */
        34236,
        39577,
        42195,
        48118,
        51042,
        55367,
        58893,
        63784,
        68472,
        70749,
        76311,
        79154,
        84390,
        87683,
        92361,
        96236,
        102084,
        102881,
        110507,
        110734,
        117786,
        119615,
        126325,
        127568,
        133589,
        136944,
        141498,
        145311,
        150283,
        152622,
        158308,
        161089,
        167017
    };

    /* Patterns and Separators of Locator */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dfs[i][j] = { true, false },
            dfs[i][width - 1 - j] = { true, false },
            dfs[width - 1 - i][j] = { true, false };

    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
            dfs[i][j] = { true, QR_PATTERN },
            dfs[i][width - 1 - j] = { true, QR_PATTERN },
            dfs[width - 1 - i][j] = { true, QR_PATTERN };

    for (int i = 1; i < 6; ++i)
        for (int j = 1; j < 6; ++j)
            dfs[i][j] = { true, false },
            dfs[i][width - 1 - j] = { true, false },
            dfs[width - 1 - i][j] = { true, false };

    for (int i = 2; i < 5; ++i)
        for (int j = 2; j < 5; ++j)
            dfs[i][j] = { true, QR_PATTERN },
            dfs[i][width - 1 - j] = { true, QR_PATTERN },
            dfs[width - 1 - i][j] = { true, QR_PATTERN };

    /* alignmentLo */
    for (int i = 0; i < 7; ++i)
        if (!alignmentLo[((qrVer - 1) * 7) + i])
            break;
        else
            for (int j = 0; j <= i; ++j)
                if (!dfs[alignmentLo[((qrVer - 1) * 7) + i] - 2][alignmentLo[((qrVer - 1) * 7) + j] - 2].isPattern
                    && !dfs[alignmentLo[((qrVer - 1) * 7) + i] + 2][alignmentLo[((qrVer - 1) * 7) + j] - 2].isPattern
                    && !dfs[alignmentLo[((qrVer - 1) * 7) + i] - 2][alignmentLo[((qrVer - 1) * 7) + j] + 2].isPattern
                    && !dfs[alignmentLo[((qrVer - 1) * 7) + i] + 2][alignmentLo[((qrVer - 1) * 7) + j] + 2].isPattern
                    )
                {
                    for (int k = alignmentLo[((qrVer - 1) * 7) + i] - 2; k <= alignmentLo[((qrVer - 1) * 7) + i] + 2; ++k)
                        for (int l = alignmentLo[((qrVer - 1) * 7) + j] - 2; l <= alignmentLo[((qrVer - 1) * 7) + j] + 2; ++l)
                            dfs[k][l] = dfs[l][k] = { true, QR_PATTERN };

                    for (int k = alignmentLo[((qrVer - 1) * 7) + i] - 1; k <= alignmentLo[((qrVer - 1) * 7) + i] + 1; ++k)
                        for (int l = alignmentLo[((qrVer - 1) * 7) + j] - 1; l <= alignmentLo[((qrVer - 1) * 7) + j] + 1; ++l)
                            dfs[k][l] = dfs[l][k] = { true, false };

                    dfs[alignmentLo[((qrVer - 1) * 7) + i]][alignmentLo[((qrVer - 1) * 7) + j]] = dfs[alignmentLo[((qrVer - 1) * 7) + j]][alignmentLo[((qrVer - 1) * 7) + i]] = { true, QR_PATTERN };
                }

    /* Timing Patterns */
    bool tiktok = true;
    for (int i = 0; i < width; ++i, tiktok = tiktok ? false : true)
        if (!dfs[i][6].isPattern)
            dfs[i][6] = dfs[6][i] = { true, tiktok && QR_PATTERN };

    /* Dark Module and Other Reserved Areas */
    dfs[8][width - 1 - 7] = { true, QR_PATTERN };

    for (int i = 0; i < 8; ++i)
        dfs[i][8].isPattern = dfs[8][i].isPattern = true,
        dfs[width - 1 - i][8].isPattern = dfs[8][width - 1 - i].isPattern = true;
    dfs[8][8].isPattern = true;

    /* Version Information Areas for Ver. 7 and larger */
    if (qrVer >= 7)
        for (int i = width - 9; i > width - 12; --i)
            for (int j = 0; j < 6; ++j)
                dfs[j][i].isPattern = dfs[i][j].isPattern = true;

    /* Pre Q-0 Mask ID */
    dfs[8][width - 1].digit = 0,
    dfs[8][width - 2].digit = QR_PATTERN_INFO,
    dfs[8][width - 3].digit = QR_PATTERN_INFO,
    dfs[8][width - 4].digit = 0,
    dfs[8][width - 5].digit = QR_PATTERN_INFO,
    dfs[8][width - 6].digit = 0,
    dfs[8][width - 7].digit = QR_PATTERN_INFO,

    dfs[width - 8][8].digit = 0,

    dfs[width - 7][8].digit = QR_PATTERN_INFO,
    dfs[width - 6][8].digit = 0,
    dfs[width - 5][8].digit = QR_PATTERN_INFO,
    dfs[width - 4][8].digit = QR_PATTERN_INFO,
    dfs[width - 3][8].digit = QR_PATTERN_INFO,
    dfs[width - 2][8].digit = QR_PATTERN_INFO,
    dfs[width - 1][8].digit = QR_PATTERN_INFO,

    dfs[0][8].digit = 0,
    dfs[1][8].digit = QR_PATTERN_INFO,
    dfs[2][8].digit = QR_PATTERN_INFO,
    dfs[3][8].digit = 0,
    dfs[4][8].digit = QR_PATTERN_INFO,
    dfs[5][8].digit = 0,
    dfs[7][8].digit = QR_PATTERN_INFO,

    dfs[8][8].digit = 0,

    dfs[8][7].digit = QR_PATTERN_INFO,
    dfs[8][5].digit = 0,
    dfs[8][4].digit = QR_PATTERN_INFO,
    dfs[8][3].digit = QR_PATTERN_INFO,
    dfs[8][2].digit = QR_PATTERN_INFO,
    dfs[8][1].digit = QR_PATTERN_INFO,
    dfs[8][0].digit = QR_PATTERN_INFO;

    /* Ver. 7+ ver Areas */
    if (qrVer >= 7)
        dfs[0][width - 11].digit = dfs[width - 11][0].digit = verInfo[qrVer] & 1 & QR_PATTERN_INFO,
        dfs[0][width - 10].digit = dfs[width - 10][0].digit = verInfo[qrVer] >> 1 & 1 & QR_PATTERN_INFO,
        dfs[0][width - 9].digit = dfs[width - 9][0].digit = verInfo[qrVer] >> 2 & 1 & QR_PATTERN_INFO,

        dfs[1][width - 11].digit = dfs[width - 11][1].digit = verInfo[qrVer] >> 3 & 1 & QR_PATTERN_INFO,
        dfs[1][width - 10].digit = dfs[width - 10][1].digit = verInfo[qrVer] >> 4 & 1 & QR_PATTERN_INFO,
        dfs[1][width - 9].digit = dfs[width - 9][1].digit = verInfo[qrVer] >> 5 & 1 & QR_PATTERN_INFO,

        dfs[2][width - 11].digit = dfs[width - 11][2].digit = verInfo[qrVer] >> 6 & 1 & QR_PATTERN_INFO,
        dfs[2][width - 10].digit = dfs[width - 10][2].digit = verInfo[qrVer] >> 7 & 1 & QR_PATTERN_INFO,
        dfs[2][width - 9].digit = dfs[width - 9][2].digit = verInfo[qrVer] >> 8 & 1 & QR_PATTERN_INFO,

        dfs[3][width - 11].digit = dfs[width - 11][3].digit = verInfo[qrVer] >> 9 & 1 & QR_PATTERN_INFO,
        dfs[3][width - 10].digit = dfs[width - 10][3].digit = verInfo[qrVer] >> 10 & 1 & QR_PATTERN_INFO,
        dfs[3][width - 9].digit = dfs[width - 9][3].digit = verInfo[qrVer] >> 11 & 1 & QR_PATTERN_INFO,

        dfs[4][width - 11].digit = dfs[width - 11][4].digit = verInfo[qrVer] >> 12 & 1 & QR_PATTERN_INFO,
        dfs[4][width - 10].digit = dfs[width - 10][4].digit = verInfo[qrVer] >> 13 & 1 & QR_PATTERN_INFO,
        dfs[4][width - 9].digit = dfs[width - 9][4].digit = verInfo[qrVer] >> 14 & 1 & QR_PATTERN_INFO,

        dfs[5][width - 11].digit = dfs[width - 11][5].digit = verInfo[qrVer] >> 15 & 1 & QR_PATTERN_INFO,
        dfs[5][width - 10].digit = dfs[width - 10][5].digit = verInfo[qrVer] >> 16 & 1 & QR_PATTERN_INFO,
        dfs[5][width - 9].digit = dfs[width - 9][5].digit = verInfo[qrVer] >> 17 & 1 & QR_PATTERN_INFO;

    /* .DATA[column][row] */
    int overByteLo = 0, inByteLo = 0, currentColumn = 0;

    for (int columnIndex = width - 1; columnIndex >= 8; columnIndex -= 2)
        if (!((columnIndex / 2) & 1))
            for (int row = width - 1; row >= 0; --row)
            {
                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex][row].isPattern)
                    dfs[columnIndex][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;

                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex - 1][row].isPattern)
                    dfs[columnIndex - 1][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;
            }
        else
            for (int row = 0; row <= width - 1; ++row)
            {
                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex][row].isPattern)
                    dfs[columnIndex][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;

                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex - 1][row].isPattern)
                    dfs[columnIndex - 1][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;
            }

    for (int columnIndex = 5; columnIndex >= 0; columnIndex -= 2)
        if (columnIndex == 3)
            for (int row = width - 1; row >= 0; --row)
            {
                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex][row].isPattern)
                    dfs[columnIndex][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;

                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex - 1][row].isPattern)
                    dfs[columnIndex - 1][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;
            }
        else
            for (int row = 0; row <= width - 1; ++row)
            {
                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex][row].isPattern)
                    dfs[columnIndex][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;

                if (inByteLo == 8)
                    inByteLo = 0, ++overByteLo;
                if (!dfs[columnIndex - 1][row].isPattern)
                    dfs[columnIndex - 1][row].digit = byteFin[overByteLo] >> (7 - inByteLo++) & 1;
            }
}
