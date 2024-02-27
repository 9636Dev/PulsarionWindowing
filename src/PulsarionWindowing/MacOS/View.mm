#include "View.h"
#include "PulsarionWindowing/Mouse.hpp"
#include <Carbon/Carbon.h> // Necessary for key code definitions on macOS

static Pulsarion::Windowing::MouseCode GetMouseCode(NSUInteger buttonNumber)
{
    if (buttonNumber <= 7)
        return static_cast<Pulsarion::Windowing::MouseCode>(buttonNumber);
    return Pulsarion::Windowing::MouseCode::Unknown;
}

Pulsarion::Windowing::KeyCode ConvertMacKeyCodeToKeyCode(unsigned short macKeyCode) {
    using KeyCode = Pulsarion::Windowing::KeyCode;
    switch (macKeyCode) {
        case kVK_Space: return KeyCode::Space;
        case kVK_ANSI_A: return KeyCode::A;
        case kVK_ANSI_B: return KeyCode::B;
        case kVK_ANSI_C: return KeyCode::C;
        case kVK_ANSI_D: return KeyCode::D;
        case kVK_ANSI_E: return KeyCode::E;
        case kVK_ANSI_F: return KeyCode::F;
        case kVK_ANSI_G: return KeyCode::G;
        case kVK_ANSI_H: return KeyCode::H;
        case kVK_ANSI_I: return KeyCode::I;
        case kVK_ANSI_J: return KeyCode::J;
        case kVK_ANSI_K: return KeyCode::K;
        case kVK_ANSI_L: return KeyCode::L;
        case kVK_ANSI_M: return KeyCode::M;
        case kVK_ANSI_N: return KeyCode::N;
        case kVK_ANSI_O: return KeyCode::O;
        case kVK_ANSI_P: return KeyCode::P;
        case kVK_ANSI_Q: return KeyCode::Q;
        case kVK_ANSI_R: return KeyCode::R;
        case kVK_ANSI_S: return KeyCode::S;
        case kVK_ANSI_T: return KeyCode::T;
        case kVK_ANSI_U: return KeyCode::U;
        case kVK_ANSI_V: return KeyCode::V;
        case kVK_ANSI_W: return KeyCode::W;
        case kVK_ANSI_X: return KeyCode::X;
        case kVK_ANSI_Y: return KeyCode::Y;
        case kVK_ANSI_Z: return KeyCode::Z;
        case kVK_ANSI_0: return KeyCode::D0;
        case kVK_ANSI_1: return KeyCode::D1;
        case kVK_ANSI_2: return KeyCode::D2;
        case kVK_ANSI_3: return KeyCode::D3;
        case kVK_ANSI_4: return KeyCode::D4;
        case kVK_ANSI_5: return KeyCode::D5;
        case kVK_ANSI_6: return KeyCode::D6;
        case kVK_ANSI_7: return KeyCode::D7;
        case kVK_ANSI_8: return KeyCode::D8;
        case kVK_ANSI_9: return KeyCode::D9;
        case kVK_ANSI_Semicolon: return KeyCode::Semicolon;
        case kVK_ANSI_Equal: return KeyCode::Equal;
        case kVK_ANSI_Comma: return KeyCode::Comma;
        case kVK_ANSI_Minus: return KeyCode::Minus;
        case kVK_ANSI_Period: return KeyCode::Period;
        case kVK_ANSI_Slash: return KeyCode::Slash;
        case kVK_ANSI_Grave: return KeyCode::GraveAccent;
        case kVK_ANSI_LeftBracket: return KeyCode::LeftBracket;
        case kVK_ANSI_Backslash: return KeyCode::Backslash;
        case kVK_ANSI_RightBracket: return KeyCode::RightBracket;
        case kVK_ANSI_Quote: return KeyCode::Apostrophe;
        case kVK_Return: return KeyCode::Enter;
        case kVK_Tab: return KeyCode::Tab;
        case kVK_Delete: return KeyCode::Backspace;
        case kVK_ForwardDelete: return KeyCode::Delete;
        case kVK_RightArrow: return KeyCode::Right;
        case kVK_LeftArrow: return KeyCode::Left;
        case kVK_DownArrow: return KeyCode::Down;
        case kVK_UpArrow: return KeyCode::Up;
        case kVK_Escape: return KeyCode::Escape;
        case kVK_Command: return KeyCode::LeftSuper;
        case kVK_Shift: return KeyCode::LeftShift;
        case kVK_CapsLock: return KeyCode::CapsLock;
        case kVK_Option: return KeyCode::LeftAlt;
        case kVK_Control: return KeyCode::LeftControl;
        case kVK_RightShift: return KeyCode::RightShift;
        case kVK_RightOption: return KeyCode::RightAlt;
        case kVK_RightControl: return KeyCode::RightControl;
        // F1-F12
        case kVK_F1: return KeyCode::F1;
        case kVK_F2: return KeyCode::F2;
        case kVK_F3: return KeyCode::F3;
        case kVK_F4: return KeyCode::F4;
        case kVK_F5: return KeyCode::F5;
        case kVK_F6: return KeyCode::F6;
        case kVK_F7: return KeyCode::F7;
        case kVK_F8: return KeyCode::F8;
        case kVK_F9: return KeyCode::F9;
        case kVK_F10: return KeyCode::F10;
        case kVK_F11: return KeyCode::F11;
        case kVK_F12: return KeyCode::F12;
        // Numpad
        case kVK_ANSI_Keypad0: return KeyCode::KP0;
        case kVK_ANSI_Keypad1: return KeyCode::KP1;
        case kVK_ANSI_Keypad2: return KeyCode::KP2;
        case kVK_ANSI_Keypad3: return KeyCode::KP3;
        case kVK_ANSI_Keypad4: return KeyCode::KP4;
        case kVK_ANSI_Keypad5: return KeyCode::KP5;
        case kVK_ANSI_Keypad6: return KeyCode::KP6;
        case kVK_ANSI_Keypad7: return KeyCode::KP7;
        case kVK_ANSI_Keypad8: return KeyCode::KP8;
        case kVK_ANSI_Keypad9: return KeyCode::KP9;
        case kVK_ANSI_KeypadDecimal: return KeyCode::KPDecimal;
        case kVK_ANSI_KeypadMultiply: return KeyCode::KPMultiply;
        case kVK_ANSI_KeypadPlus: return KeyCode::KPAdd;
        case kVK_ANSI_KeypadClear: return KeyCode::KPEnter;
        case kVK_ANSI_KeypadDivide: return KeyCode::KPDivide;
        case kVK_ANSI_KeypadEnter: return KeyCode::KPEnter;
        case kVK_ANSI_KeypadMinus: return KeyCode::KPSubtract;
        case kVK_ANSI_KeypadEquals: return KeyCode::KPEqual;
        default: return KeyCode::Unknown;
    }
}

static Pulsarion::Windowing::Modifier GetModifier(NSEvent* event)
{
    Pulsarion::Windowing::Modifier modifier = 0;
    if ([event modifierFlags] & NSEventModifierFlagShift)
        modifier |= 0x01;
    if ([event modifierFlags] & NSEventModifierFlagControl)
        modifier |= 0x02;
    if ([event modifierFlags] & NSEventModifierFlagOption)
        modifier |= 0x04;
    if ([event modifierFlags] & NSEventModifierFlagCommand)
        modifier |= 0x08;
    return modifier;
}

@implementation PulsarionView

- (instancetype)initWithState:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)initState
{
    self = [super init];
    if (!self)
        return nil;

    state = initState;
    NSTrackingAreaOptions options = NSTrackingActiveInKeyWindow | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved;
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
    return self;
}

- (void)viewWillMoveToWindow:(NSWindow *)newWindow {
    [super viewWillMoveToWindow:newWindow];
    if (newWindow) {
        [self updateTrackingAreas];
    }
}

- (void)updateTrackingAreas {
    [super updateTrackingAreas];
    [self removeTrackingArea:trackingArea];
    [trackingArea release];
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:NSTrackingActiveInKeyWindow | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyView {
    return YES;
}

- (BOOL)acceptsFirstMouse {
    return YES;
}

- (void)mouseEntered:(NSEvent *)event {
    if (state->OnMouseEnter)
        state->OnMouseEnter(state->UserData);

    [super mouseEntered:event];
}

- (void)mouseExited:(NSEvent *)event {
    if (state->OnMouseLeave)
        state->OnMouseLeave(state->UserData);

    [super mouseExited:event];
}

- (void)mouseDown:(NSEvent *)event {
    if (state->OnMouseDown)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseDown(state->UserData, point, Pulsarion::Windowing::MouseCode::Button0);
    }
}

- (void)mouseUp:(NSEvent *)event {
    if (state->OnMouseUp)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseUp(state->UserData, point, Pulsarion::Windowing::MouseCode::Button0);
    }
}

- (void)rightMouseDown:(NSEvent *)event {
    if (state->OnMouseDown)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseDown(state->UserData, point, Pulsarion::Windowing::MouseCode::Button1);
    }
}

- (void)rightMouseUp:(NSEvent *)event {
    if (state->OnMouseUp)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseUp(state->UserData, point, Pulsarion::Windowing::MouseCode::Button1);
    }
}

- (void)otherMouseDown:(NSEvent *)event {
    if (state->OnMouseDown)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseDown(state->UserData, point, GetMouseCode(event.buttonNumber));
    }
}

- (void)otherMouseUp:(NSEvent *)event {
    if (state->OnMouseUp)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseUp(state->UserData, point, GetMouseCode(event.buttonNumber));
    }

}

- (void)mouseMoved:(NSEvent *)event {
    if (state->OnMouseMove)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseMove(state->UserData, point);
    }

}

- (void)scrollWheel:(NSEvent *)event {
    if (state->OnMouseWheel)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseWheel(state->UserData, point, { static_cast<float>(event.scrollingDeltaX), static_cast<float>(event.scrollingDeltaY) });
    }

}

- (void)flagsChanged:(NSEvent *)event {
    // 1 = LShift, 2 = LCtrl, 4 = LAlt, 8 = LCmd, 16 = RShift, 32 = RCtrl, 64 = RAlt, 128 = RCmd
    static std::uint16_t lastModifier = 0;
    // lShift 56, rShift 60, lCtrl 59, lOpt 58, rOpt 61, lCmd 55, rCmd 54

    if (!state->OnKeyDown)
        return;

    switch (event.keyCode)
    {
    case 56: // lShift
        // Flip the bit for lastModifier
        lastModifier ^= 0x01;
        if (lastModifier & 0x01)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::LeftShift, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::LeftShift, 0);
        break;
    case 59: // lCtrl
        // Flip the bit for lastModifier
        lastModifier ^= 0x02;
        if (lastModifier & 0x02)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::LeftControl, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::LeftControl, 0);
        break;
    case 58: // lOpt
        // Flip the bit for lastModifier
        lastModifier ^= 0x04;
        if (lastModifier & 0x04)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::LeftAlt, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::LeftAlt, 0);
        break;
    case 55: // lCmd
        // Flip the bit for lastModifier
        lastModifier ^= 0x08;
        if (lastModifier & 0x08)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::LeftSuper, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::LeftSuper, 0);
        break;
    case 60: // rShift
        // Flip the bit for lastModifier
        lastModifier ^= 0x10;
        if (lastModifier & 0x10)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::RightShift, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::RightShift, 0);
        break;
    case 61: // rOpt
        // Flip the bit for lastModifier
        lastModifier ^= 0x40;
        if (lastModifier & 0x40)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::RightAlt, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::RightAlt, 0);
        break;
    case 54: // rCmd
        // Flip the bit for lastModifier
        lastModifier ^= 0x80;
        if (lastModifier & 0x80)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::RightSuper, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::RightSuper, 0);
        break;
    case 57: // caps lock
        if (event.modifierFlags & NSEventModifierFlagCapsLock)
            state->OnKeyDown(state->UserData, Pulsarion::Windowing::KeyCode::CapsLock, 0, false);
        else
            state->OnKeyUp(state->UserData, Pulsarion::Windowing::KeyCode::CapsLock, 0);
        break;
    default:
        break;
    }

}

- (void)keyDown:(NSEvent *)event {
    Pulsarion::Windowing::Modifier modifier = GetModifier(event);
    if (state->OnKeyTyped)
    {
        NSString* characters = [[event charactersIgnoringModifiers] lowercaseString];
        // There should be only one character in the string
        if ([characters length] == 1)
        {
            UnicodeScalarValue character = [characters characterAtIndex:0];
            auto c = static_cast<char>(character);

            state->OnKeyTyped(state->UserData, c, modifier);
        }
    }

    if (state->OnKeyDown)
        state->OnKeyDown(state->UserData, ConvertMacKeyCodeToKeyCode([event keyCode]), modifier, [event isARepeat]);
}

- (void)keyUp:(NSEvent *)event {
    Pulsarion::Windowing::Modifier modifier = GetModifier(event);
    if (state->OnKeyUp)
        state->OnKeyUp(state->UserData, ConvertMacKeyCodeToKeyCode([event keyCode]), modifier);
}

@end
