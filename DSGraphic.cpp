/********************************************************************
file base:	DSGraphic
file ext:	cpp
author:		邵华(shaohua@njtech.edu.cn)
purpose:	图形库实现
version:    1.4
*********************************************************************/

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include "DSGraphic.h"
#include <iostream>
#include <algorithm>
using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")

struct WindowSize
{
	int left, top, width, height;
};

vector<GraphicObject*> gRenderObjects;
CRITICAL_SECTION g_cs;
HWND g_hWnd = 0;
unsigned gPlayingSpeed = 500;
WindowSize g_ws = { 100,100,800,600 };

VOID OnPaint(HDC hdc, HWND hWnd)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	graphics.Clear(Color(255, 220, 220, 220));
	Pen linePen(Color(255, 0, 255, 0), 1);
	SolidBrush pointBrush(Color(255, 255, 0, 0));
	FontFamily  fontFamily(L"楷体");
	Font* pFont = 0;
	SolidBrush  textBrush(Color(255, 0, 0, 255));
	wstring test;
	for (size_t i = 0; i < gRenderObjects.size(); ++i)
	{
		GraphicObject* pObject = gRenderObjects[i];
		DSPoint* pPoint;
		DSLine* pLine;
		DSText* pText;
		switch (pObject->getGraphicType())
		{
		case gtPoint:
			pPoint = static_cast<DSPoint*>(pObject);
			pointBrush.SetColor(pPoint->mColor);
			graphics.FillEllipse(&pointBrush, pPoint->mx - pPoint->mSize, pPoint->my - pPoint->mSize,
				pPoint->mSize * 2, pPoint->mSize * 2);
			break;
		case gtLine:
			pLine = static_cast<DSLine*>(pObject);
			linePen.SetWidth(pLine->mWidth);
			linePen.SetColor(pLine->mColor);
			graphics.DrawLine(&linePen, pLine->mx1, pLine->my1, pLine->mx2, pLine->my2);
			break;
		case gtText:
			pText = static_cast<DSText*>(pObject);
			pFont = new Font(&fontFamily, pText->mSize, FontStyleRegular, UnitPixel);
			textBrush.SetColor(pText->mColor);
			graphics.DrawString(pText->mText.c_str(), -1, pFont, PointF(pText->mx, pText->my), &textBrush);
			delete pFont;
			break;
		default:
			break;
		}
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI drawThread(LPVOID lp)
{
	EnterCriticalSection(&g_cs);
	HINSTANCE hInstance;
	hInstance = GetModuleHandle(NULL);
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);
	RECT rc = { g_ws.left,g_ws.top,g_ws.left+ g_ws.width,g_ws.top+ g_ws.height };// client 
	AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, NULL);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("数据结构练习"),     // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		//CW_USEDEFAULT,       // default horizontal position 
		//CW_USEDEFAULT,       // default vertical position 
		//CW_USEDEFAULT,       // default width 
		//CW_USEDEFAULT,       // default height 
		rc.left,            // initial x position
		rc.top,            // initial y position
		rc.right - rc.left,            // initial x size
		rc.bottom - rc.top,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	g_hWnd = hWnd;
	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//InvalidateRect(hWnd, &rc, TRUE);
	}

	GdiplusShutdown(gdiplusToken);
	LeaveCriticalSection(&g_cs);
	return msg.wParam;
}  // WinMain

int showWindow(int left, int top, int width, int height)
{
	HANDLE handle = CreateThread(NULL, 0, drawThread, NULL, 0, NULL);
	InitializeCriticalSection(&g_cs);
	g_ws.left = left;
	g_ws.top = top;
	g_ws.width = width;
	g_ws.height = height;
	return 1;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc, hWnd);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc

void flushWindow()
{
	if (g_hWnd != 0)
	{
		RECT rect;
		GetClientRect(g_hWnd, &rect);
		InvalidateRect(g_hWnd, &rect, TRUE);
	}
}

GraphicObject* drawPoint(float x, float y, float size, Gdiplus::Color color)
{
	GraphicObject* p = new DSPoint(x, y, size, color);
	gRenderObjects.push_back(p);
	flushWindow();
	Sleep(gPlayingSpeed);
	return p;
}

GraphicObject* drawLine(float x1, float y1, float x2, float y2, float width, Gdiplus::Color color)
{
	GraphicObject* p = new DSLine(x1, y1, x2, y2, width, color);
	gRenderObjects.push_back(p);
	flushWindow();
	Sleep(gPlayingSpeed);
	return p;
}

GraphicObject* drawText(string s, float x, float y, float size, Gdiplus::Color color)
{
	GraphicObject* p = new DSText(s, x, y, size, color);
	gRenderObjects.push_back(p);
	flushWindow();
	Sleep(gPlayingSpeed);
	return p;
}

GraphicObject* drawText(int i, float x, float y, float size, Gdiplus::Color color)
{
	string s = to_string(static_cast<long long>(i));
	return drawText(s, x, y, size, color);
}

void clearWindow()
{
	for (unsigned i = 0; i < gRenderObjects.size(); ++i)
		delete gRenderObjects[i];
	gRenderObjects.clear();
}

void setPlayingSpeed(unsigned speed)
{
	gPlayingSpeed = speed;
}

void clearObject(void* p)
{
	vector<GraphicObject*>::iterator it = find(gRenderObjects.begin(), gRenderObjects.end(), p);
	if(it != gRenderObjects.end())
		gRenderObjects.erase(it);
	flushWindow();
	Sleep(gPlayingSpeed);
}