#pragma once
#pragma comment(lib, "winmm.lib")

#ifndef UNICODE
#error Turn UNICODE on for this system 
#endif

#include <windows.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>

using namespace std;

enum COLOUR
{
	FG_BLACK		= 0x0000,
	FG_DARK_BLUE    = 0x0001,	
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY			= 0x0007, // Thanks MS :-/
	FG_DARK_GREY    = 0x0008,
	FG_BLUE			= 0x0009,
	FG_GREEN		= 0x000A,
	FG_CYAN			= 0x000B,
	FG_RED			= 0x000C,
	FG_MAGENTA		= 0x000D,
	FG_YELLOW		= 0x000E,
	FG_WHITE		= 0x000F,
	BG_BLACK		= 0x0000,
	BG_DARK_BLUE	= 0x0010,
	BG_DARK_GREEN	= 0x0020,
	BG_DARK_CYAN	= 0x0030,
	BG_DARK_RED		= 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW	= 0x0060,
	BG_GREY			= 0x0070,
	BG_DARK_GREY	= 0x0080,
	BG_BLUE			= 0x0090,
	BG_GREEN		= 0x00A0,
	BG_CYAN			= 0x00B0,
	BG_RED			= 0x00C0,
	BG_MAGENTA		= 0x00D0,
	BG_YELLOW		= 0x00E0,
	BG_WHITE		= 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};


class clayGameEngine{
    public: 
        clayGameEngine(){
			m_nScreenWidth = 80;
            m_nScreenHeight = 30;

            m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

           // std::memset(m_keyNewState, 0, 256 * sizeof(short));
           // std::memset(m_keyOldState, 0, 256 * sizeof(short));
           // std::memset(m_keys, 0, 256 * sizeof(sKeyState));
           // m_mousePosX = 0;
           // m_mousePosY = 0;

            m_sAppName = L"Default";
        }

        int ConstructConsole(int width, int height, int fontw, int fonth)
        {
            if (m_hConsole == INVALID_HANDLE_VALUE)
                return Error(L"Bad Handle");

            m_nScreenWidth = width;
            m_nScreenHeight = height;

            // Change console visual size to a minimum so ScreenBuffer can shrink
            // below the actual visual size
            m_rectWindow = { 0, 0, 1, 1 };
            SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);

            // Set the size of the screen buffer
            COORD coord = { (short)m_nScreenWidth, (short)m_nScreenHeight };
			cout << coord.X << " " << coord.Y << endl;
            if (!SetConsoleScreenBufferSize(m_hConsole, coord))
                Error(L"SetConsoleScreenBufferSize");

            // Assign screen buffer to the console
            if (!SetConsoleActiveScreenBuffer(m_hConsole))
                return Error(L"SetConsoleActiveScreenBuffer");
		

            // Set the font size now that the screen buffer has been assigned to the console
            CONSOLE_FONT_INFOEX cfi;
            cfi.cbSize = sizeof(cfi);
            cfi.nFont = 0;
            cfi.dwFontSize.X = fontw;
            cfi.dwFontSize.Y = fonth;
            cfi.FontFamily = FF_DONTCARE;
            cfi.FontWeight = FW_NORMAL;
            wcscpy_s(cfi.FaceName, L"Consolas");
            if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi))
                return Error(L"SetCurrentConsoleFontEx");

            // Get screen buffer info and check the maximum allowed window size. Return
            // error if exceeded, so user knows their dimensions/fontsize are too large
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi))
                return Error(L"GetConsoleScreenBufferInfo");
            if (m_nScreenHeight > csbi.dwMaximumWindowSize.Y)
                return Error(L"Screen Height / Font Height Too Big");
            if (m_nScreenWidth > csbi.dwMaximumWindowSize.X)
                return Error(L"Screen Width / Font Width Too Big");

            // Set Physical Console Window Size
            m_rectWindow = { 0, 0, (short)m_nScreenWidth - 1, (short)m_nScreenHeight - 1 };
            if (!SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow))
                return Error(L"SetConsoleWindowInfo");

            // Set flags to allow mouse input		
            if (!SetConsoleMode(m_hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
                return Error(L"SetConsoleMode");

            // Allocate memory for screen buffer
            m_bufScreen = new CHAR_INFO[m_nScreenWidth*m_nScreenHeight];
            memset(m_bufScreen, 0, sizeof(CHAR_INFO) * m_nScreenWidth * m_nScreenHeight);

            //SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
            return 1;
	    }


		protected:
			int Error(const wchar_t *msg)
			{
				wchar_t buf[256];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
				SetConsoleActiveScreenBuffer(m_hOriginalConsole);
				wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
				return 0;
			}
			/*
			static BOOL CloseHandler(DWORD evt)
			{
				if (evt == CTRL_CLOSE_EVENT)
				{
					m_bAtomActive = false;

					// Wait for thread to be exited
					std::unique_lock<std::mutex> ul(m_muxGame);
					m_cvGameFinished.wait(ul);
				}
				return true;
			}
			*/


		protected:
			int m_nScreenWidth;
			int m_nScreenHeight;
			CHAR_INFO *m_bufScreen;
			std::wstring m_sAppName;
			HANDLE m_hOriginalConsole;
			CONSOLE_SCREEN_BUFFER_INFO m_OriginalConsoleInfo;
			HANDLE m_hConsole;
			HANDLE m_hConsoleIn;
			SMALL_RECT m_rectWindow;
			short m_keyOldState[256] = { 0 };
			short m_keyNewState[256] = { 0 };
			bool m_mouseOldState[5] = { 0 };
			bool m_mouseNewState[5] = { 0 };
			bool m_bConsoleInFocus = true;
			bool m_bEnableSound = false;

			std::atomic<bool> m_bAtomActive;
			std::condition_variable m_cvGameFinished;
			std::mutex m_muxGame;
		
};

