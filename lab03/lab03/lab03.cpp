// lab03.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab03.h"
#include <stdlib.h>

#define MAX_LOADSTRING 100

#define ID_BUTTON_1 1
#define ID_BUTTON_2 2

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL DrawLine(HDC, int, int, int, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB03, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB03));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB03));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 600, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//

BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    POINT pt;
    MoveToEx(hdc, x1, y1, &pt);
    return LineTo(hdc, x2, y2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;
    static HWND hButton1;
    static HWND hButton2;
    switch (message)
    {
    case WM_CREATE:
        hButton1 = CreateWindow(L"Button", L"Draw",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            0, 0, 300, 50,
            hWnd, (HMENU)ID_BUTTON_1, hInst, NULL);
        hButton2 = CreateWindow(L"Button", L"Clear",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            300, 0, 300, 50,
            hWnd, (HMENU)ID_BUTTON_2, hInst, NULL);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_BUTTON_1:
            GetClientRect(hWnd, &rc);
            INT width;
            INT height;
            width = rc.right - rc.left;
            height = rc.bottom - rc.top;
            hdc = GetWindowDC(hWnd);
            Ellipse(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Ellipse(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Ellipse(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Ellipse(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            DrawLine(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            DrawLine(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            DrawLine(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Rectangle(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Rectangle(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Rectangle(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            POINT arr1[3];
            arr1[0] = { rand() % width, rand() % (height - 100) + 100 };
            arr1[1] = { rand() % width, rand() % (height - 100) + 100 };
            arr1[2] = { rand() % width, rand() % (height - 100) + 100 };
            Polyline(hdc, arr1, 3);
            POINT arr2[3];
            arr2[0] = { rand() % width, rand() % (height - 100) + 100 };
            arr2[1] = { rand() % width, rand() % (height - 100) + 100 };
            arr2[2] = { rand() % width, rand() % (height - 100) + 100 };
            Polyline(hdc, arr2, 3);
            POINT arr3[3];
            arr3[0] = { rand() % width, rand() % (height - 100) + 100 };
            arr3[1] = { rand() % width, rand() % (height - 100) + 100 };
            arr3[2] = { rand() % width, rand() % (height - 100) + 100 };
            Polyline(hdc, arr3, 3);
            Arc(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Arc(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Arc(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            Arc(hdc, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100, rand() % width, rand() % (height - 100) + 100);
            break;
        case ID_BUTTON_2:
            GetClientRect(hWnd, &rc);
            InvalidateRect(hWnd, &rc, TRUE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DRAWITEM:
        LPDRAWITEMSTRUCT lpInfo;
        lpInfo = (LPDRAWITEMSTRUCT)lParam;
        if (lpInfo->CtlID == ID_BUTTON_1) {
            RECT rcItem;
            POINT arr[4];
            INT width, height;
            RECT rcIm;
            rcItem = lpInfo->rcItem;
            FillRect(lpInfo->hDC, &rcItem, CreateSolidBrush(RGB(0, 91, 187)));
            width = rcItem.right - rcItem.left;
            height = rcItem.bottom - rcItem.top;
            rcIm = { width / 3 - width / 6, height / 3 - height / 6, width - width / 6, height - height / 6 };
            Rectangle(lpInfo->hDC, rcIm.left, rcIm.top, rcIm.right, rcIm.bottom);
            FillRect(lpInfo->hDC, &rcIm, CreateSolidBrush(RGB(255, 213, 0)));
        }
        if (lpInfo->CtlID == ID_BUTTON_2) {
            RECT rcItem;
            POINT arr[4];
            INT width, height;
            rcItem = lpInfo->rcItem;
            FillRect(lpInfo->hDC, &rcItem, CreateSolidBrush(RGB(255, 213, 0)));
            width = rcItem.right - rcItem.left;
            height = rcItem.bottom - rcItem.top;
            Ellipse(lpInfo->hDC, width / 3 - width / 6, height / 3 - height / 6, width - width / 6, height - height / 6);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}