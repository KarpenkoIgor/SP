// lab06.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab06.h"
#include "math.h"

#define MAX_LOADSTRING 100

#define IDB_START 1
#define IDB_STOP 2

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

DWORD THREAD_ID_1;
DWORD THREAD_ID_2;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD   CALLBACK    drawRectangle(LPVOID*);

typedef struct _params {
    HWND hWnd;
    unsigned int seed;
}PARAMS;

BOOL DrawLine(HDC, int, int, int, int);
DWORD   CALLBACK    drawLine(LPVOID*);

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
    LoadStringW(hInstance, IDC_LAB06, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB06));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB06));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//

BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    POINT pt;
    MoveToEx(hdc, x1, y1, &pt);
    return LineTo(hdc, x2, y2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hStart;
    static HWND hStop;

    static HANDLE hThread1;
    static HANDLE hThread2;

    switch (message)
    {
    case WM_CREATE:
    {
        hStart = CreateWindow(L"button", L"Start",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            0, 0, 200, 50, hWnd, (HMENU)IDB_START, hInst, NULL);
        hStop = CreateWindow(L"button", L"Stop",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            200, 0, 200, 50, hWnd, (HMENU)IDB_STOP, hInst, NULL);
        PARAMS* p1 = new PARAMS{ hWnd, static_cast<unsigned int>(100) };
        PARAMS* p2 = new PARAMS{ hWnd, static_cast<unsigned int>(200) };
        hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)drawLine, (LPVOID*)p1, CREATE_SUSPENDED, (LPDWORD)&THREAD_ID_1);
        hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)drawLine, (LPVOID*)p2, CREATE_SUSPENDED, (LPDWORD)&THREAD_ID_2);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDB_START:
            ResumeThread(hThread1);
            ResumeThread(hThread2);
            break;
        case IDB_STOP:
            SuspendThread(hThread1);
            SuspendThread(hThread2);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
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

DWORD CALLBACK drawLine(LPVOID* data) {
    PARAMS* pr = (PARAMS*)data;
    HWND hWnd = pr->hWnd;
    unsigned int seed = pr->seed;
    srand(seed);
    RECT rc;
    GetClientRect(hWnd, &rc);
    INT width = rc.right - rc.left;
    INT height = rc.bottom - rc.top;
    RECT rc2 = { 0, 50, rc.right, rc.bottom };
    INT x = rand() % width;
    INT y = rand() % height;
    INT length = rand() % (y - 50);
    INT speed = rand() % 10;
    FLOAT angle = 0;
    HDC hdc = GetWindowDC(hWnd);

    while (TRUE) {
        DrawLine(hdc, x - length * cos(angle), y - length * sin(angle), x + length*cos(angle), y + length * sin(angle));
        angle += 0.00004*speed;
        InvalidateRect(hWnd, &rc2, TRUE);
    }
    delete pr;
    return 0;
}