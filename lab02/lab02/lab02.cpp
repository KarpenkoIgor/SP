// lab02.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab02.h"

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
    LoadStringW(hInstance, IDC_LAB02, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB02));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB02));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
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
      CW_USEDEFAULT, 0, 420, 410, nullptr, nullptr, hInstance, nullptr);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;

    static HWND hFirstListBox;
    static HWND hSecondListBox;
    static HWND hAddButton;
    static HWND hClearButton;
    static HWND hToRightButton;
    static HWND hDeleteButton;
    static HWND hEdit;

    switch (message)
    {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &rc);
        hFirstListBox = CreateWindow(L"listbox", NULL,
            WS_CHILD | WS_VISIBLE | LBS_STANDARD |
            LBS_WANTKEYBOARDINPUT,
            rc.left + 5, 70, 195, 300,
            hWnd, (HMENU)ID_LIST_LEFT, hInst, NULL);

        hSecondListBox = CreateWindow(L"listbox", NULL,
            WS_CHILD | WS_VISIBLE | LBS_STANDARD |
            LBS_WANTKEYBOARDINPUT,
            rc.left + 205, 70, 192, 300,
            hWnd, (HMENU)ID_LIST_RIGHT, hInst, NULL);

        hAddButton = CreateWindow(L"button", L"Add",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            0, 0, 100, 30,
            hWnd, (HMENU)ID_ADD, hInst, NULL);
        hClearButton = CreateWindow(L"button", L"Clear",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            100, 0, 100, 30,
            hWnd, (HMENU)ID_CLEAR, hInst, NULL);

        hToRightButton = CreateWindow(L"button", L"ToRight",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            200, 0, 100, 30,
            hWnd, (HMENU)ID_TO_RIGHT, hInst, NULL);

        hDeleteButton = CreateWindow(L"button", L"Delete",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            300, 0, 100, 30,
            hWnd, (HMENU)ID_DELETE, hInst, NULL);

        hEdit = CreateWindow(L"edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            ES_LEFT,
            5, 35, 390, 30,
            hWnd, (HMENU)ID_EDIT, hInst, NULL);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_ADD:
            TCHAR chBuff[80];
            WORD cbText;

            cbText = SendMessage(hEdit, EM_GETLINE, 0, (LPARAM)(LPSTR)chBuff);

            chBuff[cbText] = '\0';
            if (SendMessage(hFirstListBox, LB_FINDSTRINGEXACT, 0, (LPARAM)(LPSTR)chBuff) == LB_ERR)
                SendMessage(hFirstListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)chBuff);
            break;
        case ID_CLEAR:
            SendMessage(hFirstListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hSecondListBox, LB_RESETCONTENT, 0, 0L);
            break;
        case ID_TO_RIGHT:
            INT pos;

            pos = SendMessage(hFirstListBox, LB_GETCURSEL, 0, 0L);
            if (pos == LB_ERR)
                break;

            *(WORD*)chBuff = sizeof(chBuff) - 1;

            SendMessage(hFirstListBox, LB_GETTEXT, (WPARAM)pos, (LPARAM)(LPSTR)chBuff);
            if (SendMessage(hSecondListBox, LB_FINDSTRINGEXACT, 0, (LPARAM)(LPSTR)chBuff) == LB_ERR)
                SendMessage(hSecondListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)chBuff);
            break;
        case ID_DELETE:
            pos = SendMessage(hFirstListBox, LB_GETCURSEL, 0, 0L);
            if (pos != LB_ERR) {
                SendMessage(hFirstListBox, LB_DELETESTRING, (WPARAM)pos, 0L);
            }
            pos = SendMessage(hSecondListBox, LB_GETCURSEL, 0, 0L);
            if (pos != LB_ERR) {
                SendMessage(hSecondListBox, LB_DELETESTRING, (WPARAM)pos, 0L);
            }
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
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
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


