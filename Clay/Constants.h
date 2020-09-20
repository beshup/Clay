#pragma once

class Constants {
	public:
		Constants() {

		}
		int ScreenWidth()
		{
			return m_nScreenWidth;
		}

		int ScreenHeight()
		{
			return m_nScreenHeight;
		}

		void setScreenWidth(int width) {
			m_nScreenWidth = width;
		}

		void setScreenHeight(int height) {
			m_nScreenHeight = height;
		}

        enum COLOUR
        {
            FG_BLACK = 0x0000,
            FG_DARK_BLUE = 0x0001,
            FG_DARK_GREEN = 0x0002,
            FG_DARK_CYAN = 0x0003,
            FG_DARK_RED = 0x0004,
            FG_DARK_MAGENTA = 0x0005,
            FG_DARK_YELLOW = 0x0006,
            FG_GREY = 0x0007, // Thanks MS :-/
            FG_DARK_GREY = 0x0008,
            FG_BLUE = 0x0009,
            FG_GREEN = 0x000A,
            FG_CYAN = 0x000B,
            FG_RED = 0x000C,
            FG_MAGENTA = 0x000D,
            FG_YELLOW = 0x000E,
            FG_WHITE = 0x000F,
            BG_BLACK = 0x0000,
            BG_DARK_BLUE = 0x0010,
            BG_DARK_GREEN = 0x0020,
            BG_DARK_CYAN = 0x0030,
            BG_DARK_RED = 0x0040,
            BG_DARK_MAGENTA = 0x0050,
            BG_DARK_YELLOW = 0x0060,
            BG_GREY = 0x0070,
            BG_DARK_GREY = 0x0080,
            BG_BLUE = 0x0090,
            BG_GREEN = 0x00A0,
            BG_CYAN = 0x00B0,
            BG_RED = 0x00C0,
            BG_MAGENTA = 0x00D0,
            BG_YELLOW = 0x00E0,
            BG_WHITE = 0x00F0,
        };

        enum PIXEL_TYPE
        {
            PIXEL_SOLID = 0x2588,
            PIXEL_THREEQUARTERS = 0x2593,
            PIXEL_HALF = 0x2592,
            PIXEL_QUARTER = 0x2591,
        };

        //shading config taken from onelonecoder.com
        CHAR_INFO GetColor(float lumination)
        {
            lumination = abs(lumination);
            short bg_col, fg_col;
            wchar_t sym;
            int pixel_bw = (int)(13.0f * lumination);
            switch (pixel_bw)
            {
            case 0:
                bg_col = BG_BLACK;
                fg_col = FG_BLACK;
                sym = PIXEL_SOLID;
                break;

            case 1:
                bg_col = BG_BLACK;
                fg_col = FG_DARK_GREY;
                sym = PIXEL_QUARTER;
                break;
            case 2:
                bg_col = BG_BLACK;
                fg_col = FG_DARK_GREY;
                sym = PIXEL_HALF;
                break;
            case 3:
                bg_col = BG_BLACK;
                fg_col = FG_DARK_GREY;
                sym = PIXEL_THREEQUARTERS;
                break;
            case 4:
                bg_col = BG_BLACK;
                fg_col = FG_DARK_GREY;
                sym = PIXEL_SOLID;
                break;

            case 5:
                bg_col = BG_DARK_GREY;
                fg_col = FG_GREY;
                sym = PIXEL_QUARTER;
                break;
            case 6:
                bg_col = BG_DARK_GREY;
                fg_col = FG_GREY;
                sym = PIXEL_HALF;
                break;
            case 7:
                bg_col = BG_DARK_GREY;
                fg_col = FG_GREY;
                sym = PIXEL_THREEQUARTERS;
                break;
            case 8:
                bg_col = BG_DARK_GREY;
                fg_col = FG_GREY;
                sym = PIXEL_SOLID;
                break;

            case 9:
                bg_col = BG_GREY;
                fg_col = FG_WHITE;
                sym = PIXEL_QUARTER;
                break;
            case 10:
                bg_col = BG_GREY;
                fg_col = FG_WHITE;
                sym = PIXEL_HALF;
                break;
            case 11:
                bg_col = BG_GREY;
                fg_col = FG_WHITE;
                sym = PIXEL_THREEQUARTERS;
                break;
            case 12:
                bg_col = BG_GREY;
                fg_col = FG_WHITE;
                sym = PIXEL_SOLID;
                break;
            default:
                bg_col = BG_BLACK;
                fg_col = FG_BLACK;
                sym = PIXEL_SOLID;
            }

            CHAR_INFO c;
            c.Attributes = bg_col | fg_col;
            c.Char.UnicodeChar = sym;
            return c;
        }

	private:
		int m_nScreenWidth = 500;
		int m_nScreenHeight = 500;
};