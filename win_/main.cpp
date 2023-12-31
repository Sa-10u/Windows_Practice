#include<windows.h>
#include<cstdlib>
#include<chrono>

#include <iostream>

LPCWSTR name = TEXT("SAMPLE");
LPCWSTR white = TEXT("BACKGROUND");

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:PostQuitMessage(0);     break;

	default: return DefWindowProc(hwnd, msg, wp, lp);           break;

	}
}

int main()
{
	srand(time(NULL));

	HINSTANCE h_ins = GetModuleHandle(NULL);

	WNDCLASSEX wnd = {};
	{
		wnd.cbSize = sizeof(wnd);
		wnd.hInstance = h_ins;
		wnd.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		wnd.lpfnWndProc = WndProc;
		wnd.lpszClassName = name;
		wnd.lpszMenuName = nullptr;
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.hCursor = LoadCursor(h_ins, IDC_ARROW);
		wnd.hIcon = LoadIcon(h_ins, IDI_ASTERISK);
		wnd.hIconSm = LoadIcon(h_ins, IDI_ASTERISK);
	}

	WNDCLASSEX bg= {};
	{
		bg.cbSize = sizeof(bg);
		bg.hInstance = h_ins;
		bg.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		bg.lpfnWndProc = WndProc;
		bg.lpszClassName = white;
		bg.lpszMenuName = nullptr;
		bg.style = CS_HREDRAW | CS_VREDRAW;
		bg.hCursor = LoadCursor(h_ins, IDC_ARROW);
		bg.hIcon = LoadIcon(h_ins, IDI_ASTERISK);
	}

	RegisterClassEx(&wnd);
	RegisterClassEx(&bg);

	HWND h_wnd = CreateWindowEx
	(
		WS_EX_LAYERED,
		name,
		TEXT("aaa"),
		WS_POPUP,
		800,
		800,
		150,
		90,
		nullptr,
		nullptr,
		h_ins,
		0
	);

	UINT winx = 900;
	UINT winy = 300;

	HWND h_bg = CreateWindowEx
	(
		WS_EX_LAYERED,
		white,
		TEXT("aaa"),
		WS_POPUP,
		(GetSystemMetrics(SM_CXSCREEN) - winx)/2,
		(GetSystemMetrics(SM_CYSCREEN) - winy)/2,
		winx,
		winy,
		nullptr,
		nullptr,
		h_ins,
		0
	);

	ShowWindow(h_wnd , SW_NORMAL);
	ShowWindow(h_bg, SW_NORMAL);

	UpdateWindow(h_wnd);
	UpdateWindow(h_bg);

	SetFocus(h_wnd); 

	SetLayeredWindowAttributes(h_bg, RGB(0, 0, 0), 255, LWA_ALPHA);
	SetFocus(h_bg);

	{

		int alpha = 255;
		MSG msg = {};
		auto dc_bg= GetDC(h_bg);
		auto dc_mem = GetDC(NULL);
		HBITMAP h_bm = CreateCompatibleBitmap(dc_mem, winx, winy);

		static auto st = std::chrono::system_clock::now();

		for (int i = 0;i < GetSystemMetrics(SM_CYSCREEN);i++){
			std::cout << i;
			for (int j = 0;j < GetSystemMetrics(SM_CXSCREEN);j++) {
		
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
			RECT rc = { 0, 0, winx, winy };
			FillRect(dc_mem, &rc, hBrush);

			BitBlt(GetDC(h_wnd), 0, 0, winx, winy,dc_mem, 0, 0, SRCCOPY);
			}
			ShowWindow(h_wnd, SW_NORMAL);
		}

		ReleaseDC(h_wnd, dc_bg);

		while (msg.message != WM_QUIT) {

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			else
			{
				
				auto now = std::chrono::system_clock::now();
				static auto last = now;

				if (((now - last).count()) * 15.0 <= 1000*10000.0f)
				{
					continue;
				}

				if ((now - st).count() >= 1000 * 10000)
				{
					st = now;
				}
				last = now;
				{
					int x = rand() % 1600;
					int y = rand() % 1000;
					int sx = rand() % 200 + 50;
					int sy = rand() % 200 + 50;
					alpha-=6;

					SetLayeredWindowAttributes(h_wnd, RGB(0,0,0), alpha, LWA_ALPHA);
					SetWindowPos(h_wnd, NULL, x, y, sx, sy, SWP_NOZORDER);
					RedrawWindow(h_wnd, NULL, NULL, RDW_FRAME);

					if (alpha <= 0)	break;
				}
			}
		}

		SetLayeredWindowAttributes(h_wnd, RGB(0, 0, 0), 0, LWA_ALPHA);

		while (true) {
			static int alpha = 255;

			auto now = std::chrono::system_clock::now();
			static auto last = now;

			if (((now - last).count()) * 15.0 <= 1000 * 10000.0f)
			{
				continue;
			}


			if ((now - st).count() >= 1000 * 10000)
			{
				st = now;
			}
			last = now;

			alpha-= 5;
			

			SetLayeredWindowAttributes(h_bg, RGB(0, 0, 0), alpha, LWA_ALPHA);
			

			if (alpha <= 0)	break;
		}

	}

	DestroyWindow(h_bg);
	DestroyWindow(h_wnd);

	return 0;
}