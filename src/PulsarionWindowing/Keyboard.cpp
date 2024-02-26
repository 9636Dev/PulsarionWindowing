#include "Keyboard.hpp"

namespace Pulsarion::Windowing
{

    std::string KeyCodeToString(KeyCode key)
    {
        switch (key) {
            case KeyCode::Unknown: return "Unknown";

            case KeyCode::Space: return "Space";
            case KeyCode::Apostrophe: return "Apostrophe";
            case KeyCode::Comma: return "Comma";
            case KeyCode::Minus: return "Minus";
            case KeyCode::Period: return "Period";
            case KeyCode::Slash: return "Slash";

            case KeyCode::D0: return "0";
            case KeyCode::D1: return "1";
            case KeyCode::D2: return "2";
            case KeyCode::D3: return "3";
            case KeyCode::D4: return "4";
            case KeyCode::D5: return "5";
            case KeyCode::D6: return "6";
            case KeyCode::D7: return "7";
            case KeyCode::D8: return "8";
            case KeyCode::D9: return "9";

            case KeyCode::Semicolon: return "Semicolon";
            case KeyCode::Equal: return "Equal";

            case KeyCode::A: return "A";
            case KeyCode::B: return "B";
            case KeyCode::C: return "C";
            case KeyCode::D: return "D";
            case KeyCode::E: return "E";
            case KeyCode::F: return "F";
            case KeyCode::G: return "G";
            case KeyCode::H: return "H";
            case KeyCode::I: return "I";
            case KeyCode::J: return "J";
            case KeyCode::K: return "K";
            case KeyCode::L: return "L";
            case KeyCode::M: return "M";
            case KeyCode::N: return "N";
            case KeyCode::O: return "O";
            case KeyCode::P: return "P";
            case KeyCode::Q: return "Q";
            case KeyCode::R: return "R";
            case KeyCode::S: return "S";
            case KeyCode::T: return "T";
            case KeyCode::U: return "U";
            case KeyCode::V: return "V";
            case KeyCode::W: return "W";
            case KeyCode::X: return "X";
            case KeyCode::Y: return "Y";
            case KeyCode::Z: return "Z";

            case KeyCode::LeftBracket: return "Left bracket";
            case KeyCode::Backslash: return "Backslash";
            case KeyCode::RightBracket: return "Right bracket";
            case KeyCode::GraveAccent: return "Grave accent";

            case KeyCode::World1: return "World 1";
            case KeyCode::World2: return "World 2";

            case KeyCode::Escape: return "Escape";
            case KeyCode::Enter: return "Enter";
            case KeyCode::Tab: return "Tab";
            case KeyCode::Backspace: return "Backspace";
            case KeyCode::Insert: return "Insert";
            case KeyCode::Delete: return "Delete";
            case KeyCode::Right: return "Right";
            case KeyCode::Left: return "Left";
            case KeyCode::Down: return "Down";
            case KeyCode::Up: return "Up";
            case KeyCode::PageUp: return "Page up";
            case KeyCode::PageDown: return "Page down";
            case KeyCode::Home: return "Home";
            case KeyCode::End: return "End";
            case KeyCode::CapsLock: return "Caps lock";
            case KeyCode::ScrollLock: return "Scroll lock";
            case KeyCode::NumLock: return "Num lock";
            case KeyCode::PrintScreen: return "Print screen";
            case KeyCode::Pause: return "Pause";

            case KeyCode::F1: return "F1";
            case KeyCode::F2: return "F2";
            case KeyCode::F3: return "F3";
            case KeyCode::F4: return "F4";
            case KeyCode::F5: return "F5";
            case KeyCode::F6: return "F6";
            case KeyCode::F7: return "F7";
            case KeyCode::F8: return "F8";
            case KeyCode::F9: return "F9";
            case KeyCode::F10: return "F10";
            case KeyCode::F11: return "F11";
            case KeyCode::F12: return "F12";
            case KeyCode::F13: return "F13";
            case KeyCode::F14: return "F14";
            case KeyCode::F15: return "F15";
            case KeyCode::F16: return "F16";
            case KeyCode::F17: return "F17";
            case KeyCode::F18: return "F18";
            case KeyCode::F19: return "F19";
            case KeyCode::F20: return "F20";
            case KeyCode::F21: return "F21";
            case KeyCode::F22: return "F22";
            case KeyCode::F23: return "F23";
            case KeyCode::F24: return "F24";
            case KeyCode::F25: return "F25";

            case KeyCode::KP0: return "Keypad 0";
            case KeyCode::KP1: return "Keypad 1";
            case KeyCode::KP2: return "Keypad 2";
            case KeyCode::KP3: return "Keypad 3";
            case KeyCode::KP4: return "Keypad 4";
            case KeyCode::KP5: return "Keypad 5";
            case KeyCode::KP6: return "Keypad 6";
            case KeyCode::KP7: return "Keypad 7";
            case KeyCode::KP8: return "Keypad 8";
            case KeyCode::KP9: return "Keypad 9";
            case KeyCode::KPDecimal: return "Keypad decimal";
            case KeyCode::KPDivide: return "Keypad divide";
            case KeyCode::KPMultiply: return "Keypad multiply";
            case KeyCode::KPSubtract: return "Keypad subtract";
            case KeyCode::KPAdd: return "Keypad add";
            case KeyCode::KPEnter: return "Keypad enter";
            case KeyCode::KPEqual: return "Keypad equal";

            case KeyCode::LeftShift: return "Left shift";
            case KeyCode::LeftControl: return "Left control";
            case KeyCode::LeftAlt: return "Left alt";
            case KeyCode::LeftSuper: return "Left super";
            case KeyCode::RightShift: return "Right shift";
            case KeyCode::RightControl: return "Right control";
            case KeyCode::RightAlt: return "Right alt";
            case KeyCode::RightSuper: return "Right super";
            case KeyCode::Menu: return "Menu";

            default: return "Unknown";
        }
    }
}
