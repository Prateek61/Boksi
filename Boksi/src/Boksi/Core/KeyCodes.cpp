#include "bkpch.h"
#include "KeyCodes.h"

namespace Boksi
{
	std::string KeyCodeToString(KeyCode keycode)
	{
		switch (keycode)
		{
		case Key::Space: return "Space";
		case Key::Apostrophe: return "Apostrophe";
		case Key::Comma: return "Comma";
		case Key::Minus: return "Minus";
		case Key::Period: return "Period";
		case Key::Slash: return "Slash";

		case Key::D0: return "D0";
		case Key::D1: return "D1";
		case Key::D2: return "D2";
		case Key::D3: return "D3";
		case Key::D4: return "D4";
		case Key::D5: return "D5";
		case Key::D6: return "D6";
		case Key::D7: return "D7";
		case Key::D8: return "D8";
		case Key::D9: return "D9";

		case Key::Semicolon: return "Semicolon";
		case Key::Equal: return "Equal";

		case Key::A: return "A";
		case Key::B: return "B";
		case Key::C: return "C";
		case Key::D: return "D";
		case Key::E: return "E";
		case Key::F: return "F";
		case Key::G: return "G";
		case Key::H: return "H";
		case Key::I: return "I";
		case Key::J: return "J";
		case Key::K: return "K";
		case Key::L: return "L";
		case Key::M: return "M";
		case Key::N: return "N";
		case Key::O: return "O";
		case Key::P: return "P";
		case Key::Q: return "Q";
		case Key::R: return "R";
		case Key::S: return "S";
		case Key::T: return "T";
		case Key::U: return "U";
		case Key::V: return "V";
		case Key::W: return "W";
		case Key::X: return "X";
		case Key::Y: return "Y";
		case Key::Z: return "Z";

		case Key::LeftBracket: return "LeftBracket";
		case Key::Backslash: return "Backslash";
		case Key::RightBracket: return "RightBracket";
		case Key::GraveAccent: return "GraveAccent";

		case Key::World1: return "World1";
		case Key::World2: return "World2";

		case Key::Escape: return "Escape";
		case Key::Enter: return "Enter";
		case Key::Tab: return "Tab";
		case Key::Backspace: return "Backspace";
		case Key::Insert: return "Insert";
		case Key::Delete: return "Delete";
		case Key::Right: return "Right";
		case Key::Left: return "Left";
		case Key::Down: return "Down";
		case Key::Up: return "Up";
		case Key::PageUp: return "PageUp";
		case Key::PageDown: return "PageDown";
		case Key::Home: return "Home";
		case Key::End: return "End";
		case Key::CapsLock: return "CapsLock";
		case Key::ScrollLock: return "ScrollLock";
		case Key::NumLock: return "NumLock";
		case Key::PrintScreen: return "PrintScreen";
		case Key::Pause: return "Pause";
		case Key::F1: return "F1";
		case Key::F2: return "F2";
		case Key::F3: return "F3";
		case Key::F4: return "F4";
		case Key::F5: return "F5";
		case Key::F6: return "F6";
		case Key::F7: return "F7";
		case Key::F8: return "F8";
		case Key::F9: return "F9";
		case Key::F10: return "F10";
		case Key::F11: return "F11";
		case Key::F12: return "F12";
		case Key::F13: return "F13";
		case Key::F14: return "F14";
		case Key::F15: return "F15";
		case Key::F16: return "F16";
		case Key::F17: return "F17";
		case Key::F18: return "F18";
		case Key::F19: return "F19";
		case Key::F20: return "F20";
		case Key::F21: return "F21";
		case Key::F22: return "F22";
		case Key::F23: return "F23";
		case Key::F24: return "F24";
		case Key::F25: return "F25";

		case Key::KP0: return "KP0";
		case Key::KP1: return "KP1";
		case Key::KP2: return "KP2";
		case Key::KP3: return "KP3";
		case Key::KP4: return "KP4";
		case Key::KP5: return "KP5";
		case Key::KP6: return "KP6";
		case Key::KP7: return "KP7";
		case Key::KP8: return "KP8";
		case Key::KP9: return "KP9";
		case Key::KPDecimal: return "KPDecimal";
		case Key::KPDivide: return "KPDivide";
		case Key::KPMultiply: return "KPMultiply";
		case Key::KPSubtract: return "KPSubtract";
		case Key::KPAdd: return "KPAdd";
		case Key::KPEnter: return "KPEnter";
		case Key::KPEqual: return "KPEqual";

		case Key::LeftShift: return "LeftShift";
		case Key::LeftControl: return "LeftControl";
		case Key::LeftAlt: return "LeftAlt";
		case Key::LeftSuper: return "LeftSuper";
		case Key::RightShift: return "RightShift";
		case Key::RightControl: return "RightControl";
		case Key::RightAlt: return "RightAlt";
		case Key::RightSuper: return "RightSuper";
		case Key::Menu: return "Menu";

		default: return "Unknown";
		}
	}
}
