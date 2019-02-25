#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <vld.h>
#include "interface.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


int NG_SIGNAL;


HINSTANCE app_inst;
HANDLE unique_mutex;
extern BOOL CALLBACK mdlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlg_log(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
INIEdit config_dlg[] = {
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.rt_win.left,			"win_left"		},
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.rt_win.top,			"win_top"		},
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.rt_win.right,		"win_right"		},
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.rt_win.bottom,		"win_bottom"	},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.mdlg_x,				"mdlg_x"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.mdlg_y,				"mdlg_y"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.rt_log.left,			"log_left"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.rt_log.top,			"log_top"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.rt_log.right,		"log_right"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.rt_log.bottom,		"log_bottom"	},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.aadlg_x,				"aadlg_x"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.aadlg_y,				"aadlg_y"		},

	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.tcdlg_x,				"tcdlg_x"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.tcdlg_y,				"tcdlg_y"		},
	{NULL,	DLGTYPE_INPUT_INT,	&s_test::dinfo.show_log,			"show_log"		},
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.show_tc,				"show_tc"		},
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.show_aa_dlg,			"show_aa_dlg"	},
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.show_io1_dlg,		"show_io1_dlg"	},
	{NULL,  DLGTYPE_INPUT_INT,  &s_test::dinfo.show_io2_dlg,		"show_io2_dlg"	},
	{NULL,  NULL, NULL, NULL},
};

class Wnd
{
public:
	BOOL create(HWND hwnd, LPCREATESTRUCT lpcs);
	void size(HWND hwnd, UINT state, int cx, int cy);
	void hscroll(HWND hwnd, HWND hctrl, UINT code, int pos);
	void vscroll(HWND hwnd, HWND hctrl, UINT code, int pos);
	void command(HWND hwnd, int id, HWND hctrl, unsigned int code_notify);
	void paint(HWND hwnd);
//	void timer(HWND hwnd, unsigned int id);
	void close(HWND hwnd);

private:
	SCROLLINFO si;
	int pos_x;
	int pos_y;
/*	HANDLE unique_mutex;*/
};

ai_win::MoveROI mr_sfr_roi;
Wnd wnd;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE,    wnd.create			);
		HANDLE_MSG(hWnd, WM_SIZE,      wnd.size				);
		HANDLE_MSG(hWnd, WM_HSCROLL,   wnd.hscroll			);
		HANDLE_MSG(hWnd, WM_VSCROLL,   wnd.vscroll			);
		HANDLE_MSG(hWnd, WM_PAINT,     wnd.paint			);
		HANDLE_MSG(hWnd, WM_CLOSE,     wnd.close			);

	case WM_DROPFILES:
		{
			char img_path[MAX_PATH] = {0};
			HDROP hdrop = (HDROP)(HANDLE) wParam;
			int file_length = DragQueryFile(hdrop, 0, img_path, sizeof(img_path));
			if (file_length>0 && SELECT_IMG_SOURCE == IMG_SOURCE_FILE && IMG_CAPTURE_READY == FALSE){
				if (read_img_file(img_path) == TRUE){
					IMG_CAPTURE_READY = TRUE;
					ITEM_ENABLED(HWND_MAIN, IDC_START);
					ITEM_ENABLED(HWND_MAIN, IDC_STOP);
				}
				else{
					IMG_CAPTURE_READY = FALSE;
					ITEM_GRAYED(HWND_MAIN, IDC_START);
					ITEM_GRAYED(HWND_MAIN, IDC_STOP);
				}
			}
			DragFinish(hdrop);
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}



void dlg_size_reset(void)
{
	s_test::dinfo.rt_log.left = 1000;
	s_test::dinfo.rt_log.top = 250;
	s_test::dinfo.rt_log.right = s_test::dinfo.rt_log.left+305;
	s_test::dinfo.rt_log.bottom = s_test::dinfo.rt_log.top+545;

	s_test::dinfo.rt_win.left = 100;
	s_test::dinfo.rt_win.top = 100;
	s_test::dinfo.rt_win.right = 1000;
	s_test::dinfo.rt_win.bottom = 800;

	s_test::dinfo.mdlg_x = 106;
	s_test::dinfo.mdlg_y = 104;

	s_test::dinfo.tcdlg_x = 200;
	s_test::dinfo.tcdlg_y = 100;

	s_test::dinfo.aadlg_x = 100;
	s_test::dinfo.aadlg_y = 100;

	s_test::dinfo.show_img_info  = FALSE;
	s_test::dinfo.show_recipe	  = FALSE;
	s_test::dinfo.show_tc		  = FALSE;
	s_test::dinfo.show_hw_config = FALSE;
	s_test::dinfo.show_aa_dlg    = FALSE;
	s_test::dinfo.show_io1_dlg    = FALSE;
	s_test::dinfo.show_io2_dlg    = FALSE;

}

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	//HOME_STATUS=0;
	memset(s_test::dir, 0, sizeof(s_test::dir));
	ai_win::get_current_directory(s_test::dir);

	sprintf(s_test::config_path, "%s\\recipe\\config.ini", s_test::dir);

	int cx = GetSystemMetrics(SM_CXSCREEN);   
	int cy = GetSystemMetrics(SM_CYSCREEN);
	app_inst = hInstance;

	ai_win::load_keys(s_test::config_path, "dlg", NULL, config_dlg, sizeof(config_dlg)/sizeof(config_dlg[0]));

	if (s_test::dinfo.rt_log.left>cx || s_test::dinfo.rt_log.top>cy || s_test::dinfo.rt_log.left<0 || s_test::dinfo.rt_log.top<0 ||
		s_test::dinfo.rt_win.left>cx || s_test::dinfo.rt_win.top>cy || s_test::dinfo.rt_win.left<0 || s_test::dinfo.rt_win.top<0 ||
		s_test::dinfo.mdlg_x>cx || s_test::dinfo.mdlg_y>cy || s_test::dinfo.mdlg_x<0 || s_test::dinfo.mdlg_y<0 ||
		s_test::dinfo.tcdlg_x>cx || s_test::dinfo.tcdlg_y>cy || s_test::dinfo.tcdlg_x<0 || s_test::dinfo.tcdlg_y<0){
		dlg_size_reset();
	}

	static char app_name[] = "Image_Scenery";
	MSG msg;
	WNDCLASSEX wnd_class;

	wnd_class.cbSize		= sizeof(WNDCLASSEX);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
	wnd_class.lpfnWndProc   = WndProc;
	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = 0;
	wnd_class.hInstance     = hInstance;
	wnd_class.hIcon         = LoadIcon(hInstance, (LPCTSTR)IDI_LOGO);
	wnd_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wnd_class.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wnd_class.lpszClassName = app_name;
	wnd_class.lpszMenuName  = NULL;
	wnd_class.hIconSm       = LoadIcon(wnd_class.hInstance, (LPCTSTR)IDI_LOGO);

	if (!RegisterClassEx(&wnd_class)){
		MessageBox(NULL, "App Error!", app_name, 0);
		return 0;
	} 

	HWND_APP = CreateWindowEx(WS_EX_ACCEPTFILES, 
		app_name, "Image_Scenery", 
		WS_THICKFRAME ,
		s_test::dinfo.rt_win.left,
		s_test::dinfo.rt_win.top,
		(s_test::dinfo.rt_win.right-s_test::dinfo.rt_win.left),
		(s_test::dinfo.rt_win.bottom-s_test::dinfo.rt_win.top),
		NULL, 
		NULL, 
		hInstance, 
		NULL);


// 	ShowWindow(HWND_APP, nShowCmd);
//  	UpdateWindow(HWND_APP);

	HWND_MAIN = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)mdlg_process);
	RECT rt_mdlg;
	GetWindowRect(HWND_MAIN, &rt_mdlg);
	MoveWindow(HWND_MAIN, s_test::dinfo.mdlg_x, s_test::dinfo.mdlg_y, rt_mdlg.right-rt_mdlg.left, rt_mdlg.bottom-rt_mdlg.top, FALSE);
	while(GetMessage(&msg, NULL, 0, 0)){
		if(!IsDialogMessage(HWND_APP, &msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return	msg.wParam;
}

BOOL Wnd::create(HWND hwnd, LPCREATESTRUCT lpcs)
{
// 	//////////////////////////////////////////////////////////////////////////
	// make sure no duplicated program running at same time.
	unique_mutex = CreateMutex(NULL, FALSE, "s_aa");
	DWORD err_code = GetLastError();
	if (err_code == ERROR_ALREADY_EXISTS){
		Sleep(2000);
		CloseHandle(unique_mutex);
		unique_mutex = CreateMutex(NULL, FALSE, "s_aa");
		err_code = GetLastError();
		if (err_code == ERROR_ALREADY_EXISTS){
			MessageBox(hwnd, "s_aa is running!", "Warning", 0);
			CloseHandle(unique_mutex);
			PostQuitMessage(0);
		}
	}
// 	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void Wnd::size(HWND hwnd, UINT state, int cx, int cy)
{
	char path[MAX_PATH] = {0};

	if (s_test::myimg.get_full_sized()){
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS /*| SIF_DISABLENOSCROLL*/;
		si.nMin = 0;
		si.nMax = IMG_HEIGHT;
		si.nPage = cy;
		si.nPos = IMG_HEIGHT/4;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS/*| SIF_DISABLENOSCROLL*/;
		si.nMin = 0;
		si.nMax = IMG_WIDTH;
		si.nPage = cx;
		si.nPos = IMG_WIDTH/4;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
	}
	else{
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE ;
		si.nMin = 0;
		si.nMax = 0;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE ;
		si.nMin = 0;
		si.nMax = 0;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
	}
}

void Wnd::hscroll(HWND hwnd, HWND hctrl, UINT code, int pos)
{
	si.cbSize = sizeof(si);
	si.fMask  = SIF_ALL;
	GetScrollInfo(hwnd, SB_HORZ, &si);
	s_test::myimg.set_start_x(si.nPos);

	switch (code)
	{
	case SB_LEFT:		si.nPos  = si.nMin;		 break;
	case SB_RIGHT:		si.nPos  = si.nMax;		 break;
	case SB_LINELEFT:	si.nPos -= 1;			 break;
	case SB_LINERIGHT:	si.nPos += 1;			 break;
	case SB_PAGELEFT:	si.nPos -= si.nPage;	 break;
	case SB_PAGERIGHT:	si.nPos += si.nPage;	 break;
	case SB_THUMBTRACK:	si.nPos  = si.nTrackPos; break;
	default:	break;
	}

	si.fMask = SIF_POS;		
	SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
	GetScrollInfo(hwnd, SB_HORZ, &si);

	if (si.nPos != s_test::myimg.get_start_x()){
		InvalidateRect(hwnd, NULL, FALSE);
		UpdateWindow(hwnd);
	}
}

void Wnd::vscroll(HWND hwnd, HWND hctrl, UINT code, int pos)
{
	si.cbSize = sizeof(si);
	si.fMask  = SIF_ALL;
	GetScrollInfo(hwnd, SB_VERT, &si);
	s_test::myimg.set_start_y(si.nPos);

	switch (code) 
	{
	case SB_TOP:		si.nPos = si.nMin;		break;
	case SB_BOTTOM:		si.nPos = si.nMax;		break;
	case SB_LINEUP:		si.nPos -= 1;			break;
	case SB_LINEDOWN:	si.nPos += 1;			break;
	case SB_PAGEUP:		si.nPos -= si.nPage;	break;
	case SB_PAGEDOWN:	si.nPos += si.nPage;	break;
	case SB_THUMBTRACK:	si.nPos = si.nTrackPos;	break;
	default: break;
	}
	si.fMask = SIF_POS;
	SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
	GetScrollInfo(hwnd, SB_VERT, &si);

	if (si.nPos != s_test::myimg.get_start_y()){
		InvalidateRect(hwnd, NULL, FALSE);
		UpdateWindow(hwnd);
	}
}

void Wnd::paint(HWND hwnd)
{
	int flag = 0;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	si.cbSize = sizeof(si);
	si.fMask  = SIF_POS;
	GetScrollInfo(hwnd, SB_HORZ, &si);
	s_test::myimg.set_start_x(si.nPos);
	
	GetScrollInfo(hwnd, SB_VERT, &si);
	s_test::myimg.set_start_y(si.nPos);

	HBITMAP mybmp= LoadBitmap((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_TITLE));
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP old_bitmap = (HBITMAP)SelectObject(memdc, mybmp);

	BitBlt(hdc, 10, 10, 328, 60, memdc, 0, 0, SRCCOPY);

	DeleteObject(mybmp);
	DeleteDC(memdc);

	EndPaint(hwnd, &ps);
}

void Wnd::close(HWND hwnd)
{
	PostQuitMessage(0);
}

