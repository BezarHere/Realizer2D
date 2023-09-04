#pragma once
#include "_bite_utility.h"
#include <iostream>

namespace bite
{
	namespace dye
	{


		enum class ColorCode : uint8_t
		{
			Black,
			Blue,
			Green,
			Aqua,
			Red,
			Purple,
			Yellow,
			White,
			Gray,
			LightBlue,
			LightGreen,
			LightAqua,
			LightRed,
			LightPurple,
			LightYellow,
			BrightWhite
		};

		struct TerminalTheme {
			ColorCode fg, bg = ColorCode::Black;
		};

		extern TerminalTheme M_GetTheme();
		extern TerminalTheme M_GetDefaultTheme();
		extern void M_PutTheme(TerminalTheme theme);
		// will return to the same theme the program started with
		extern void M_ClearTheme();

		extern constexpr uint8_t M_UnpackTheme(TerminalTheme theme);
		extern constexpr TerminalTheme M_PackTheme(uint8_t theme);


		extern void M_Dye(const std::string &text,
											const TerminalTheme colors = TerminalTheme{ ColorCode::BrightWhite },
											std::ostream &stream = std::cout);
		extern void M_Dye(const char *text,
											const TerminalTheme colors = TerminalTheme{ ColorCode::BrightWhite },
											std::ostream &stream = std::cout);
		extern void M_Dye(const std::exception &exc,
											const TerminalTheme colors = TerminalTheme{ ColorCode::Red },
											std::ostream &stream = std::cout);
	}

}
