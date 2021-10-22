// lab08.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "framework.h"
#include "lab08.h"
#include <TlHelp32.h>
#include <windowsx.h>

#define MAX_LOADSTRING 100
#define LISTBOX1_ID 1001
#define LISTBOX2_ID 1002

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HMENU hPopupMenu;

#define N 500

int procCount = 0;
PROCESSENTRY32 procs[N];

int moduleCount = 0;
MODULEENTRY32 modules[N];

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    MSG msg;
    HACCEL hAccelTable;

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB08, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB08));


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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB08));
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
      CW_USEDEFAULT, 0, 600, 600, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void ClearListBox(HWND listBox) {
    while (SendMessage(listBox, LB_GETCOUNT, 0, NULL))
        SendMessage(listBox, LB_DELETESTRING, 0, NULL);
}

void ShowProcs(HWND listBox)
{
    procCount = 0;
    procs[0].dwSize = sizeof(PROCESSENTRY32);

    HANDLE toolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool result = Process32First(toolHelp, &procs[0]);
    while (result)
    {
        procCount++;
        procs[procCount].dwSize = sizeof(PROCESSENTRY32);
        result = Process32Next(toolHelp, &procs[procCount]);
    }
    CloseHandle(toolHelp);

    ClearListBox(listBox);
    for (int i = 0; i < procCount; i++)
        SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)procs[i].szExeFile);
}

void GetModuleListByPID(DWORD pid, HWND listBox)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (snapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 module;
        module.dwSize = sizeof(MODULEENTRY32);
        Module32First(snapshot, &module);

        do {
            SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)module.szModule);
        } while (Module32Next(snapshot, &module));// find it
    }

    CloseHandle(snapshot);
}

#define ID_IDLE 10
#define ID_NORMAL 11
#define ID_HIGH 12
#define ID_REAL_TIME 13

void SetPriority(DWORD priorityClass, HWND listBox) {
    int itemId = SendMessage(listBox, LB_GETCURSEL, 0, NULL);
    if (itemId != -1) {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
        SetPriorityClass(hProcess, priorityClass);
        CloseHandle(hProcess);
    }
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
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    static HWND listBox1, listBox2;

    switch (message)
    {
    case WM_CREATE:
    {
        listBox1 = CreateWindow(L"LISTBOX", L"Processes", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            20, 20, 250, 500, hWnd, (HMENU)LISTBOX1_ID, hInst, NULL);
        listBox2 = CreateWindow(L"LISTBOX", L"Modules", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
            300, 20, 250, 500, hWnd, (HMENU)LISTBOX2_ID, hInst, NULL);

        ShowProcs(listBox1);
    }
    break;
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case LISTBOX1_ID:
            if (wmEvent == LBN_SELCHANGE)
            {
                int itemId = SendMessage(listBox1, LB_GETCURSEL, 0, NULL);
                if (itemId != -1) {
                    SendMessage(listBox2, LB_RESETCONTENT, 0, NULL);
                    GetModuleListByPID(procs[itemId].th32ProcessID, listBox2);
                }
            }
            break;
        case ID_IDLE:
            SetPriority(IDLE_PRIORITY_CLASS, listBox1);
            break;
        case ID_NORMAL:
            SetPriority(NORMAL_PRIORITY_CLASS, listBox1);
            break;
        case ID_HIGH:
            SetPriority(HIGH_PRIORITY_CLASS, listBox1);
            break;
        case ID_REAL_TIME:
            SetPriority(REALTIME_PRIORITY_CLASS, listBox1);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_CONTEXTMENU:
        if ((HWND)wParam == listBox1) {
            int itemId = SendMessage(listBox1, LB_GETCURSEL, 0, NULL);
            if (itemId != -1) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
                int pC = GetPriorityClass(hProcess);
                CloseHandle(hProcess);

                hPopupMenu = CreatePopupMenu();
                AppendMenu(hPopupMenu, pC == IDLE_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_IDLE, L"Idle");
                AppendMenu(hPopupMenu, pC == NORMAL_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_NORMAL, L"Normal");
                AppendMenu(hPopupMenu, pC == HIGH_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_HIGH, L"High");
                AppendMenu(hPopupMenu, pC == REALTIME_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_REAL_TIME, L"Real time");
                TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hWnd, NULL);
            }
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