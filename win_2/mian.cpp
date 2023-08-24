#include<windows.h>
#include<cassert>
#include<gdiplus.h>
#include <iostream>

#pragma comment(lib,"Gdiplus.lib")

using namespace std;

LRESULT Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:PostQuitMessage(0);     break;
/*
	case WM_PAINT :
	{
		static PAINTSTRUCT ps = {};

		//static HBITMAP bm = static_cast<HBITMAP>(LoadImage(NULL, L".//sample.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		//assert(bm != nullptr);

		static LPCWSTR text = L"aaaaaaaaaaaa";
		static HDC h_dc = {};
		static HDC m_hdc = CreateCompatibleDC(h_dc);

		//SelectObject(m_hdc, bm);
		
		h_dc = BeginPaint(hwnd, &ps);

		Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		Gdiplus::Bitmap* map = Gdiplus::Bitmap::FromFile(L"glacean.png");
		Gdiplus::Graphics graphics(h_dc);
		graphics.DrawImage(map, 0, 0, 1000, 1000);
		delete map;

		SetTextColor(h_dc, RGB(25, 0, 255));
		TextOutW(h_dc, 10, 10, text, lstrlen(text));

		static RECT rect = {};
		GetClientRect(hwnd, &rect);

		DRAWTEXTPARAMS tparam = {};
		{
			tparam.cbSize = sizeof(DRAWTEXTPARAMS);
			tparam.iLeftMargin = 1;
			tparam.iRightMargin = 1;
			tparam.iTabLength = 1;
		}
		HFONT font = CreateFont
		(
			80,0,0,0,
			FW_DONTCARE,
			false,false,false,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			L"HGçsèëëÃ"
		);

		SelectObject(h_dc, font);

		DrawTextExW(h_dc, const_cast<LPWSTR>(text), -1, &rect, DT_WORDBREAK, &tparam);

		BitBlt(h_dc, 400, 0, 1000, 1000, m_hdc, 0, 0, SRCCOPY);

		ReleaseDC(hwnd, h_dc);

		DeleteObject(font);
		EndPaint(hwnd, &ps);
		//DeleteDC(m_hdc);

	}break;
*/
	default: return DefWindowProc(hwnd, msg, wp, lp);           break;

	}
}


LPCWSTR name = TEXT("sample");

int main(void)
{
	
	auto CUI = GetConsoleWindow();
	ShowWindow(CUI, SW_HIDE);

	HINSTANCE h_ins = GetModuleHandle(NULL);
	HRESULT hr = 0;

	WNDCLASSEX wnd = {};
	{
		wnd.hInstance = h_ins;
		wnd.cbSize = sizeof(wnd);
		wnd.lpszClassName = name;
		wnd.hbrBackground = CreateSolidBrush(RGB(255,255,255));
		wnd.style =  CS_HREDRAW | CS_VREDRAW;
		wnd.hCursor = LoadCursor(h_ins, IDC_ARROW);
		wnd.hIcon = LoadIcon(h_ins, IDI_APPLICATION);
		wnd.hIconSm = LoadIcon(h_ins, IDI_APPLICATION);
		wnd.lpfnWndProc = Proc;
	}
	hr = RegisterClassEx(&wnd);
	if (FAILED(hr))
	{
		exit(0);
	}

	HWND h_wnd = CreateWindowEx
	(
		WS_EX_LAYERED,
		name,
		TEXT("aaa"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		nullptr,
		nullptr,
		h_ins,
		NULL
	);

	SetLayeredWindowAttributes(h_wnd, RGB(0,0,0), 255, LWA_ALPHA);

	ShowWindow(h_wnd, SW_NORMAL);
	UpdateWindow(h_wnd);

	auto h_dc = GetDC(h_wnd);
	RECT rect = { 0,0,100,100 };
	auto brush = CreateSolidBrush(RGB(0, 0, 0));
	

	FillRect(h_dc,&rect,brush);

	{
		MSG msg = {};

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			else
			{

			}
		}

	}


	UnregisterClass(name, h_ins);
	return 0;
}
