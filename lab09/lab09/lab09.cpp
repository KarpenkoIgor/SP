// lab09.cpp : Определяет точку входа для приложения.
//
#include "framework.h"
#include "lab09.h"
#include <strsafe.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LAB09, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB09));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB09));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


#define EDIT_ID 1001
#define BUTTON_SEARCH_ID 1002
#define LISTBOX_ID 1003

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
      CW_USEDEFAULT, 0, 820, 570, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
int x = 1000;

void SearchRec(HWND hwnd, HKEY hKeyRoot, LPTSTR lpSubKey, TCHAR* s)
{
    if (--x <= 0) return;
    LPTSTR lpEnd;
    LONG lResult;
    DWORD dwSize;
    TCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;

    lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        return;
    }

    if (_tcsstr(lpSubKey, s) != NULL)
        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)lpSubKey);

    lpEnd = lpSubKey + lstrlen(lpSubKey);

    if (*(lpEnd - 1) != TEXT('\\'))
    {
        *lpEnd = TEXT('\\');
        lpEnd++;
        *lpEnd = TEXT('\0');
    }

    dwSize = MAX_PATH;
    lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
        NULL, NULL, &ftWrite);

    if (lResult == ERROR_SUCCESS)
    {
        int index = 0;
        do {
            index++;

            StringCchCopy(lpEnd, MAX_PATH * 2, szName);

            SearchRec(hwnd, hKeyRoot, lpSubKey, s);
            if (x <= 0) return;

            dwSize = MAX_PATH;

            lResult = RegEnumKeyEx(hKey, index, szName, &dwSize, NULL,
                NULL, NULL, &ftWrite);

        } while (lResult == ERROR_SUCCESS);
    }

    lpEnd--;
    *lpEnd = TEXT('\0');

    RegCloseKey(hKey);
}

void Search(HWND hwnd, TCHAR* s) {
    SendMessage(hwnd, LB_RESETCONTENT, 0, 0);

    LPTSTR lpSubKey = (LPTSTR)L"Software";
    TCHAR szSearchKey[MAX_PATH * 2];
    StringCchCopy(szSearchKey, MAX_PATH * 2, lpSubKey);

    x = 1000;
    SearchRec(hwnd, HKEY_LOCAL_MACHINE, szSearchKey, s);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    static HWND hButtonSearch;
    static HWND hListBox;
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_CREATE:
    {
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"", WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            0, 500, 400, 25, hWnd, (HMENU)EDIT_ID, hInst, NULL);

        hButtonSearch = CreateWindow(L"button", L"Search", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            400, 500, 200, 25, hWnd, (HMENU)BUTTON_SEARCH_ID, hInst, NULL);

        hListBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"listbox", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
            0, 0, 800, 500, hWnd, (HMENU)LISTBOX_ID, hInst, NULL);
    }
    break;
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case BUTTON_SEARCH_ID:
        {
            int len;
            len = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
            TCHAR buffer[256];
            SendMessage(hEdit, WM_GETTEXT, len + 1, (LPARAM)buffer);
            Search(hListBox, buffer);
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
