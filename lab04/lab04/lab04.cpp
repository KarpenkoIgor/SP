// lab04.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab04.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawBitmap(HDC, int, int, HBITMAP);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

typedef struct _object {
    HBITMAP hBitmap;
    int x;
    int y;
}OBJECT;

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
    LoadStringW(hInstance, IDC_LAB04, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB04));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB04));
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

int current = 0;

HBITMAP next(HBITMAP* stages) {
    current = (current + 1) % 4;
    return stages[current];
}

HBITMAP prev(HBITMAP* stages) {
    if (current == 0) current = 3;
    else current = (current - 1) % 4;
    return stages[current];
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rc;
    static HBITMAP hStages[4];
    static OBJECT  obj;
    static WORD destX, destY;
    HDC hdc;
    switch (message)
    {
    case WM_CREATE:
    {
        hStages[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP10));
        hStages[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP11));
        hStages[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP12));
        hStages[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP13));
        obj = { hStages[0], 100, 100 };
    }
    break;
    case WM_COMMAND:
        break;
    case WM_TIMER:
    {
        GetClientRect(hWnd, &rc);
        if (destX == obj.x && obj.y == destY) {
            KillTimer(hWnd, 1);
            InvalidateRect(hWnd, &rc, TRUE);
            break;
        }
        if (destX == obj.x) {
            if (destY < obj.y) {
                obj.y--;
                obj.hBitmap = prev(hStages);
            }
            if (destY > obj.y) {
                obj.y++;
                obj.hBitmap = next(hStages);
            }
        }
        if (destX  < obj.x) {
            obj.hBitmap = prev(hStages);
            obj.x--;
            if (destY < obj.y) obj.y--;

            if (destY > obj.y) obj.y++;
        }
        else if (destX > obj.x) {
            obj.hBitmap = next(hStages);
            obj.x++;
            if (destY < obj.y) obj.y--;

            if (destY > obj.y) obj.y++;
        }
        InvalidateRect(hWnd, &rc, TRUE);
    }
    break;
    case WM_LBUTTONUP:
        if (HIWORD(lParam) > 16 && HIWORD(lParam) > 16) {
            destX = LOWORD(lParam) - 16;
            destY = HIWORD(lParam) - 18;
        }
        else {
            destX = 0;
            destY = 0;
        }
        SetTimer(hWnd, 1, 50, NULL);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawBitmap(hdc, obj.x, obj.y, obj.hBitmap);
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

void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;
    POINT  ptSize, ptOrg;
    hMemDC = CreateCompatibleDC(hDC);
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);
    if (hOldbm)
    {
        SetMapMode(hMemDC, GetMapMode(hDC));
        GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);

        ptSize.x = bm.bmWidth;
        ptSize.y = bm.bmHeight;
        DPtoLP(hDC, &ptSize, 1);

        ptOrg.x = 0;
        ptOrg.y = 0;
        DPtoLP(hMemDC, &ptOrg, 1);
        BitBlt(hDC, x, y, ptSize.x, ptSize.y,
            hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);
        SelectObject(hMemDC, hOldbm);
    }
    DeleteDC(hMemDC);
}
