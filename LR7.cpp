// LR7.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "LR7.h"
#include <Windows.h>
#include <windowsx.h>
#include <vector>
using namespace std;

#define MAX_LOADSTRING 100
#define IDM_HOUSERBUTTON 1001
#define IDM_TREEBUTTON 1002
#define IDM_FENCEBUTTON 1003
#define IDM_CLEARBUTTON 1004

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

vector<pair<int, pair<int, int>>> points;
int drawFlag = 0;

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
    LoadStringW(hInstance, IDC_LR7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR7));

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
    wcex.lpfnWndProc    = WndProc;													// как будет обрабатываться сообщ
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR7);
    wcex.lpszClassName  = szWindowClass;											// имя нашего класса
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
   HWND hHouseButton = CreateWindow(_T("BUTTON"), _T("Домики"), WS_CHILD | WS_VISIBLE, 1390, 510, 130, 50, hWnd, (HMENU)IDM_HOUSERBUTTON, hInst, 0);
   HWND hTreeButton = CreateWindow(_T("BUTTON"), _T("Деревья"), WS_CHILD | WS_VISIBLE, 1390, 580, 130, 50, hWnd, (HMENU)IDM_TREEBUTTON, hInst, 0);
   HWND hFenceButton = CreateWindow(_T("BUTTON"), _T("Забор"), WS_CHILD | WS_VISIBLE, 1390, 650, 130, 50, hWnd, (HMENU)IDM_FENCEBUTTON, hInst, 0);
   HWND hClearButton = CreateWindow(_T("BUTTON"), _T("Очистить окно"), WS_CHILD | WS_VISIBLE, 1390, 720, 130, 50, hWnd, (HMENU)IDM_CLEARBUTTON, hInst, 0);
   
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_SHOWMAXIMIZED);
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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			case IDM_HOUSERBUTTON:
				drawFlag = 1;
				break;
			case IDM_TREEBUTTON:
				drawFlag = 2;
				break;
			case IDM_FENCEBUTTON:
				drawFlag = 3;
				break;
			case IDM_CLEARBUTTON:
				points.clear();
				InvalidateRect(hWnd, NULL, TRUE);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
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
			HPEN hYellowPen, hRedPen, hBluePen, hDarkBrownPen, hGreenPen, hBrownPen;
			hYellowPen = CreatePen(PS_SOLID, 1, RGB(255, 246, 115));
			hRedPen = CreatePen(PS_SOLID, 1, RGB(254, 32, 31));
			hBluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); 
			hDarkBrownPen = CreatePen(PS_SOLID, 1, RGB(91, 58, 43));
			hGreenPen = CreatePen(PS_SOLID, 1, RGB(118, 220, 20));
			hBrownPen = CreatePen(PS_SOLID, 1, RGB(190, 121, 62));
			HBRUSH hYellowBrush, hRedBrush, hBlueBrush, hDarkBrownBrush, hGreenBrush, hBrownBrush;
			hYellowBrush = CreateSolidBrush(RGB(255, 246, 115));
			hRedBrush = CreateSolidBrush(RGB(254, 32, 31));
			hBlueBrush = CreateSolidBrush(RGB(0, 176, 246));			
			hDarkBrownBrush = CreateSolidBrush(RGB(91, 58, 43));
			hGreenBrush = CreateSolidBrush(RGB(118, 220, 20));
			hBrownBrush = CreateSolidBrush(RGB(190, 121, 62));

			for (int i = 0; i < points.size(); i++)
			{
				int x = points[i].second.first;
				int y = points[i].second.second;
				if (points[i].first == 1)
				{
					SelectObject(hdc, hYellowPen);
					SelectObject(hdc, hYellowBrush);
					Rectangle(hdc, x - 100, y - 40, x + 100, y + 160);

					SelectObject(hdc, hRedPen);
					SelectObject(hdc, hRedBrush);
					POINT arr[3] = { { x - 140, y - 40 }, { x, y - 200 }, { x + 140, y - 40 } };
					Polygon(hdc, arr, 3);

					SelectObject(hdc, hBluePen);
					SelectObject(hdc, hBlueBrush);
					Rectangle(hdc, x - 40, y + 20, x + 40, y + 100);					
				}
				else if (points[i].first == 2)
				{
					SelectObject(hdc, hDarkBrownPen);
					SelectObject(hdc, hDarkBrownBrush);
					Rectangle(hdc, x - 20, y, x + 20, y + 160);

					SelectObject(hdc, hGreenPen);
					SelectObject(hdc, hGreenBrush);
					Ellipse(hdc, x - 70, y - 160, x + 70, y + 40);
				}
				else
				{
					SelectObject(hdc, hBrownPen);
					SelectObject(hdc, hBrownBrush);
					POINT arr[] = { { x - 20, y + 80 }, { x - 20, y + 40 }, { x - 60, y + 40 }, { x - 60, y }, { x - 20, y }, { x - 20, y - 40 }, { x, y - 80 }, 
									{ x + 20, y - 40 }, { x + 20, y }, { x + 60, y }, { x + 60, y + 40 }, { x + 20, y + 40 }, { x + 20, y + 80 } };
					Polygon(hdc, arr, 13);					
				}
			}

			DeleteObject(hYellowPen);
			DeleteObject(hRedPen);
			DeleteObject(hBluePen);
			DeleteObject(hDarkBrownPen);
			DeleteObject(hGreenPen);
			DeleteObject(hBrownPen);
			DeleteObject(hYellowBrush);
			DeleteObject(hRedBrush);
			DeleteObject(hBlueBrush);
			DeleteObject(hDarkBrownBrush);
			DeleteObject(hGreenBrush); 
			DeleteObject(hBrownBrush);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_LBUTTONDOWN:
		{
			if (drawFlag != 0)
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				points.push_back(pair<int, pair<int, int>>(drawFlag, pair<int, int>(xPos, yPos)));
				InvalidateRect(hWnd, 0, TRUE);
			}
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}