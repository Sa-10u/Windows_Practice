#include<windows.h>
#include<Gdiplus.h>
#include<iostream>
#include<chrono>

#pragma comment(lib,"Gdiplus.lib")
using namespace std;
using namespace Gdiplus;

RECT size = {};

static int top = 0;
static int bottom = 0;

void Changer(Rect* rect);

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

	auto cui = GetConsoleWindow();

	int x = 90; int y = 90;
	int sx = 1920 * 0.5;
	int sy = 1080 * 0.5;

	ShowWindow(cui, SW_NORMAL);
	{
		auto com_1 = [&]() {cout << '.'; Sleep(1000);};
		auto Command_1 = [&]() {com_1();com_1();com_1();};

		cout << "Loading";
		Command_1();

		cout << endl;
		for (int i = 0;i <= 40;i++) {

			cout << "ERROR";
			int time = 1000 - (i * 300);

			if (time < 0)	time = 10;

			Sleep(time);
		}
	}

	auto h_inst = GetModuleHandle(NULL);

	GdiplusStartupInput inp;
	ULONG_PTR token;

	GdiplusStartup(&token, &inp, nullptr);

	{
		WNDCLASSEX mainwnd = {};
		{
			mainwnd.cbSize = sizeof(mainwnd);
			mainwnd.hInstance = h_inst;
			mainwnd.lpszClassName = TEXT("backbone");
			mainwnd.style = CS_VREDRAW | CS_HREDRAW;
			mainwnd.lpfnWndProc = WndProc;
			mainwnd.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		}

		RegisterClassEx(&mainwnd);

		HWND h_wnd = {};
		{
			h_wnd = CreateWindowEx
			(
				WS_EX_LAYERED,
				TEXT("backbone"),
				L"DAAA!",
				WS_POPUP,
				x,
				y,
				sx,
				sy,
				nullptr,
				nullptr,
				h_inst,
				NULL
			);
		}

		GetClientRect(h_wnd, &::size);

		SetLayeredWindowAttributes(h_wnd, RGB(0, 0, 0), 255, LWA_ALPHA);
		ShowWindow(h_wnd, SW_NORMAL);

		//SetLayeredWindowAttributes(cui, NULL, 0, LWA_ALPHA);

		ShowWindow(cui, SW_HIDE);

		HDC h_dc = GetDC(h_wnd);
		Graphics gra(h_dc);
		Bitmap mp_bg(::size.right, ::size.bottom, PixelFormat32bppARGB);
		Graphics bg(&mp_bg);

		SolidBrush br_bg_1(Color(128, 128, 128));
		SolidBrush br_bg_2(Color(230, 230, 230));
		SolidBrush br_bg_3(Color(64, 64, 64));

		Rect rect = {};

		auto draw_1 = [&]()
		{
			Changer(&rect);
			bg.FillRectangle(&br_bg_1, rect);
			gra.DrawImage(&mp_bg, 0, 0);
		};

		auto draw_2 = [&]()
		{
			Changer(&rect);
			bg.FillRectangle(&br_bg_2, rect);
			gra.DrawImage(&mp_bg, 0, 0);
		};

		auto draw_3 = [&]()
		{
			Changer(&rect);
			bg.FillRectangle(&br_bg_3, rect);
			gra.DrawImage(&mp_bg, 0, 0);
		};

		auto DrawNoise = [&]()
		{
			gra.Clear(Color(0, 0, 0));
			bg.Clear(NULL);

			draw_1();
			draw_2();
			draw_3();
			draw_1();
			draw_2();
			draw_3();
			draw_3();
		};

		auto start = chrono::steady_clock::now();
		auto last = chrono::steady_clock::now();

		while(chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 2000){

			DrawNoise();

			last = chrono::steady_clock::now();
		}

		start = chrono::steady_clock::now();
		last = chrono::steady_clock::now();

		while(chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 2000) {
			DrawNoise();

			bool alp = rand() % 4;
			int alpha = alp*255;
			SetLayeredWindowAttributes(h_wnd, NULL, alpha, LWA_ALPHA);

			last = chrono::steady_clock::now();
		}
		gra.Clear(Color(0, 0, 0));
		SetLayeredWindowAttributes(h_wnd, NULL, 255, LWA_ALPHA);

		{
			Bitmap* logo = Bitmap::FromFile(L"Logo.png");
			Color col;
			col.SetValue(Color::MakeARGB(0, 0, 0, 0));
			
			for (int x = 0;x < logo->GetWidth();x++) {
				for (int y = 0;y < logo->GetHeight();y++) {

					logo->GetPixel(x, y, &col);

					if (col.GetR() == 0 && col.GetG() == 0 && col.GetB() == 0)
					{
						
						logo->SetPixel(x, y, col);
					}
				}
			}

			start = last = chrono::steady_clock::now();
			while(chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 3000) {
				
				gra.Clear(Color(0, 0, 0));
				bg.Clear(NULL);

				DrawNoise();

				gra.DrawImage(logo, -20, -200, logo->GetWidth(), logo->GetHeight());

				last = chrono::steady_clock::now();
			}

			gra.Clear(Color(0, 0, 0));
			bg.Clear(NULL);

			start = last = chrono::steady_clock::now();
			for (int i = 0;i < 150;i++) {
				
				int rx = -20 +(rand() % 12 * (i / 30)) -6 ;
				int ry = -200 + (rand() % 12 * (i / 30)) - 6 ;

				gra.Clear(Color(0, 0, 0));
				gra.DrawImage(logo, rx, ry, logo->GetWidth(), logo->GetHeight());

				last = chrono::steady_clock::now();

				//if (chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 3000)	break;
			}

			gra.SetCompositingMode(CompositingModeSourceCopy);

			start = last = chrono::steady_clock::now();
			while(chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 200) {

				gra.DrawImage(logo, -20, -200, logo->GetWidth(), logo->GetHeight());

				last = chrono::steady_clock::now();
			}
			Sleep(50);


			Bitmap* school = Bitmap::FromFile(L"School_Logo_2.png");
			for (int x = 0;x < school->GetWidth();x++) {
				for (int y = 0;y < school->GetHeight();y++) {

					school->GetPixel(x, y, &col);

					if (col.GetR() == 0 && col.GetG() == 0 && col.GetB() == 0)
					{
						school->SetPixel(x, y, col);
					}
				}
			}
			auto DrawWLOGO_Noise = [&]() 
			{
				bool alp = rand() % 9;
				
				if(!alp)	gra.Clear(NULL);
	
				DrawNoise();
				DrawNoise();

				gra.DrawImage(logo, 0, -300, logo->GetWidth(), logo->GetHeight());
				gra.DrawImage(school, 200, 200, 600, 400);
			};

			start = last = chrono::steady_clock::now();
			while (chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 600) {

				DrawWLOGO_Noise();
				last = chrono::steady_clock::now();
			}

			start = last = chrono::steady_clock::now();
			while (chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 500) {
				
				gra.Clear(NULL);

				gra.DrawImage(logo, 0, -300, logo->GetWidth(), logo->GetHeight());
				gra.DrawImage(school, 200, 200, 600, 400);

				last = chrono::steady_clock::now();
			}

			start = last = chrono::steady_clock::now();
			while (chrono::duration_cast<chrono::milliseconds>(last - start).count() <= 300) {
				last = chrono::steady_clock::now();
			}

			Sleep(150);

			gra.Clear(NULL);
			bg.Clear(NULL);

			DrawNoise();
			Sleep(30);

			DrawNoise();
			Sleep(30);

			DrawNoise();
			Sleep(30);

			delete logo;

	
			int* cnt = new int(1);
			gra.Clear(NULL);
			bg.Clear(NULL);

			{
				RECT rc;

				GetClientRect(h_wnd, &rc);

				Bitmap b_mem(rc.right, rc.bottom, PixelFormat32bppARGB);

			//	auto mem = CreateCompatibleDC(h_dc);

				Graphics g_mem(&b_mem);
				
				gra.SetCompositingMode(CompositingModeSourceCopy);
				int time = 10;
				int mx = rc.right / 2;
				int my = rc.bottom / 2;


				for (int i = 0; i <= time; i++) {

					double di = i;

					float coe = di / time;

					Rect ellip = { (int)(mx * coe),(int)(my * coe),(int)(rc.right - (sx * coe)),(int)(rc.bottom - (sy *coe)) };
					Rect line = { (int)((mx * coe) - 600 * (1.0 - coe)),(int)((my * coe) + 300* (1.0 - coe)),(int)((rc.right - (sx * coe)) + (1200 * (1.0 - coe))),(int)((rc.bottom - (sy * coe)) - (600 * (1.0 - coe))) };

					GraphicsPath size;
					size.AddEllipse(ellip);
					GraphicsPath hor;
					hor.AddEllipse(line);

					PathGradientBrush br(&size);
					br.SetCenterColor(Color(255, 255, 255, 255));
					PathGradientBrush horbr(&hor);
					horbr.SetCenterColor(Color(255, 255, 255, 255));

					Color sur(0, 0, 0, 0);

					br.SetSurroundColors(&sur, cnt);
					horbr.SetSurroundColors(&sur, cnt);

					;;
					bg.Clear(NULL);
					g_mem.Clear(NULL);

					bg.FillEllipse(&br,ellip);
					bg.FillEllipse(&horbr, line);

					g_mem.DrawImage(&mp_bg, 0, 0);

					gra.DrawImage(&b_mem,0, 0);

				}

				gra.Clear(NULL);

				Sleep(500);
			}

			delete cnt;
		}

		ReleaseDC(h_wnd, h_dc);

		
	
	}
	GdiplusShutdown(token);

	return 0;
}

void Changer(Rect* rect)
{
	top = rand() % ::size.bottom;
	bottom = (rand() % 150) + 1;
	*rect = { 0,top, ::size.right,bottom };
}