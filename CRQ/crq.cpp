
// WinMain
// I'm extremely grateful to this tutorial: https://www.thonky.com/qr-code-tutorial/
// Some parameters are fixed for developing this program (my final assignment) sooner and easier.
// Attention: for quite few inputs, this program generates wrong QR code.
// Example: EEEEEEEEEEEEEEEEEEEE
// The output could be EEEEEEEEEEEEEEEEEUEE
// I guess that those fixed parameters caused this problem.

#include "rh.h"
#include "xpstyle.h"

#define BTN_ONE     3301
#define EDT_ONE     3501

enum { CW = 750, CH = 500, MAXLEN = 1250, FINL = 3710 };

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szAppName[] = TEXT("CRQ");
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    RECT     rect;

    SetRect(&rect, 0, 0, CW, CH);
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, false);

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)/*GetStockObject(WHITE_BRUSH)*/(COLOR_BTNFACE + 1);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
        return MessageBox(NULL, TEXT("窗口类注册失败。"), szAppName, MB_ICONERROR);

    hwnd = CreateWindow(szAppName,
        TEXT("CRQA"),
        WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC           hdc;
    PAINTSTRUCT   ps;
    static HPEN   hPenB, hPenF, hPenP;
    static HBRUSH hBrushB, hBrushF, hBrushP;
    static RECT   screenRect, clientRect, setPosRect, qrRect, dataRect;
    static HWND   hEncodeButton, hTextBox;
    static HFONT  hFontW, hFontL;

    int screenWidth = 0, screenHeight = 0;
    static int cxChar = 0, cyChar = 0;

    static int unit = 2;
    static int rfu = 0;
    static int unitLength = 0;
    static int qrVer = 0;
    static bool drawable = false;

    WCHAR txtBuff[MAXLEN] = { 0 };
    int chrLen = 0;
    char sByte[Q40] = { 0 };
    unsigned char byteFin[FINL] = { 0 };

    static DFStruct** dfs = nullptr;

    switch (message)
    {
    case WM_CREATE:
        hPenB = CreatePen(PS_SOLID, 1, RGB(255, 242, 231));
        hPenF = CreatePen(PS_SOLID, 1, RGB(0, 20, 147));
        hPenP = CreatePen(PS_SOLID, 1, RGB(255, 116, 116));
        hBrushB = CreateSolidBrush(RGB(255, 242, 231));
        hBrushF = CreateSolidBrush(RGB(0, 20, 147));
        hBrushP = CreateSolidBrush(RGB(255, 116, 116));
        hFontW = CreateFont(30, 0, 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Arial Unicode MS"));
        hFontL = CreateFont(22, 0, 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Arial Unicode MS"));
        GetClientRect(hwnd, &clientRect);
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);
        screenRect.top = screenRect.left = 0;
        screenRect.right = screenWidth;
        screenRect.bottom = screenHeight;

        qrRect.top = clientRect.top;
        qrRect.left = clientRect.right - clientRect.bottom;
        qrRect.right = clientRect.right;
        qrRect.bottom = clientRect.bottom;

        dataRect.top = qrRect.top + 25;
        dataRect.left = qrRect.left + 25;
        dataRect.right = qrRect.right - 25;
        dataRect.bottom = qrRect.bottom - 25;

        cxChar = LOWORD(GetDialogBaseUnits());
        cyChar = HIWORD(GetDialogBaseUnits());

        hEncodeButton = CreateWindow(TEXT("button"),
            TEXT("Generate"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT,// | BS_OWNERDRAW,
            5, 450,
            240, 45,
            hwnd,
            (HMENU)BTN_ONE,
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hEncodeButton, WM_SETFONT, (WPARAM)hFontW, NULL);

        hTextBox = CreateWindow(TEXT("edit"), NULL,
            WS_CHILD | WS_VISIBLE | ES_MULTILINE |
            ES_WANTRETURN,
            5, 5,
            240, 440,
            hwnd,
            (HMENU)EDT_ONE,
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hTextBox, WM_SETFONT, (WPARAM)hFontL, NULL);
        SetFocus(hTextBox);

        GetClientRect(hwnd, &clientRect);
        setPosRect.left = (screenRect.right - screenRect.left) / 2 - (clientRect.right - clientRect.left) / 2;
        setPosRect.right = (screenRect.right - screenRect.left) / 2 + (clientRect.right - clientRect.left) / 2;
        setPosRect.top = (screenRect.bottom - screenRect.top) / 2 - (clientRect.bottom - clientRect.top) / 2;
        setPosRect.bottom = (screenRect.bottom - screenRect.top) / 2 + (clientRect.bottom - clientRect.top) / 2;

        return ::SetWindowPos(hwnd, HWND_TOPMOST, setPosRect.left, setPosRect.top, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW) & 0;

    case WM_ACTIVATE:
        SetFocus(hTextBox);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        SelectObject(hdc, hPenB);
        SelectObject(hdc, hBrushB);
        Rectangle(hdc, qrRect.left, qrRect.top, qrRect.right, qrRect.bottom);
        SelectObject(hdc, hPenF);
        SelectObject(hdc, hBrushF);
        unitLength = (dataRect.bottom - dataRect.top) / unit;
        rfu = (dataRect.bottom - dataRect.top - unitLength * unit) / 2;

        if (drawable)
            for (int j = 0; j < unit; ++j)
                for (int i = 0; i < unit; ++i)
                    if (dfs[i][j].digit)
                        if (dfs[i][j].isPattern)
                        {
                            SelectObject(hdc, hPenF);
                            SelectObject(hdc, hBrushF);
                            Rectangle(hdc, rfu + dataRect.left + i * unitLength, rfu + dataRect.top + j * unitLength, rfu + dataRect.left + (1 + i) * unitLength, rfu + dataRect.top + (1 + j) * unitLength);
                        }
                        else
                        {
                            SelectObject(hdc, hPenP);
                            SelectObject(hdc, hBrushP);
                            Rectangle(hdc, rfu + dataRect.left + i * unitLength, rfu + dataRect.top + j * unitLength, rfu + dataRect.left + (1 + i) * unitLength, rfu + dataRect.top + (1 + j) * unitLength);
                        }
        return EndPaint(hwnd, &ps) & 0;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case BTN_ONE:
            memset(byteFin, 0, FINL);
            drawable = false;
            if (dfs)
            {
                for (int i = 0; i < qrVer; ++i)
                {
                    delete[](dfs[i]);
                    dfs[i] = nullptr;
                }
                delete[] dfs;
                dfs = nullptr;
            }

            chrLen = GetDlgItemText(hwnd, EDT_ONE, txtBuff, MAXLEN);
            if (!chrLen)
            {
                InvalidateRect(hwnd, NULL, true);
                UpdateWindow(hwnd);
                SetFocus(hTextBox);
                return 0;
            }
            if (qrc(hwnd, chrLen, txtBuff, qrVer, sByte))
                SetDlgItemText(hwnd, EDT_ONE, TEXT(""));
            else
            {
                unit = 21 + (qrVer - 1) * 4;
                dfs = new DFStruct*[unit]();

                for (int i = 0; i < unit; ++i)
                    dfs[i] = new DFStruct[unit]();

                if (qec(byteFin, sByte, qrVer, hwnd))
                    SetDlgItemText(hwnd, EDT_ONE, TEXT(""));
                else
                {
                    qmap(dfs, qrVer, unit, byteFin);
                    if (QR_FILLDATA)
                        for (int j = 0; j < unit; ++j)
                            for (int i = 0; i < unit; ++i)
                                if (!dfs[j][i].isPattern)
                                    dfs[j][i].digit = 1;
                                else;
                    else
                        if (QR_MASK)
                            /* Q Mask 0 */
                            for (int j = 0; j < unit; ++j)
                                for (int i = 0; i < unit; ++i)
                                    if (!dfs[j][i].isPattern)
                                        dfs[j][i].digit ^= !((i + j) & 1);

                    drawable = true;
                }
            }
            memset(sByte, 0, Q40);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
            SetFocus(hTextBox);
            break;
        }
        return 0;
    }

    case WM_DESTROY:
        DeleteObject(hPenB);
        DeleteObject(hPenF);
        DeleteObject(hPenP);
        DeleteObject(hBrushB);
        DeleteObject(hBrushF);
        DeleteObject(hBrushP);
        DeleteObject(hFontW);
        DeleteObject(hFontL);

        if (dfs)
        {
            for (int i = 0; i < qrVer; ++i)
            {
                delete[](dfs[i]);
                dfs[i] = nullptr;
            }
            delete[] dfs;
            dfs = nullptr;
        }

        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
