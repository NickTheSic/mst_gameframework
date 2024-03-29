#ifndef _MST_KEYS_H_
#define _MST_KEYS_H_

#if defined __EMSCRIPTEN__ || defined PLATFORM_WEB
#include <emscripten/key_codes.h> // To use the DOM_PK codes
#endif

namespace mst
{
    #if defined _WIN64
    enum class Key : unsigned short
    {
        UNKOWN = 0,
    
        BACKSPACE = 0x08,
        TAB = 0x09,

        SHIFT_LEFT = 0X10, CONTROL_LEFT, ALT_LEFT, PAUSE, CAPS_LOCK,
    
        ESCAPE = 0x1B,
        SPACE = 0x20,
    
        LEFT_ARROW = 0x25, UP_ARROW, RIGHT_ARROW, DOWN_ARROW,
    
        ZERO = 0X30, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
    
        A = 0X41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
        F1 = 0X70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
    };
    #endif
    
    #if defined __EMSCRIPTEN__ || defined PLATFORM_WEB
    enum class Key : unsigned short
    {
        UNKOWN = DOM_PK_UNKNOWN,
        ESCAPE = DOM_PK_ESCAPE,
        ZERO = DOM_PK_0,
        ONE = DOM_PK_1,
        TWO = DOM_PK_2,
        THREE = DOM_PK_3,
        FOUR = DOM_PK_4,
        FIVE = DOM_PK_5,
        SIX = DOM_PK_6,
        SEVEN = DOM_PK_7,
        EIGHT = DOM_PK_8,
        NINE = DOM_PK_9,
        MINUS = DOM_PK_MINUS,
        EQUAL = DOM_PK_EQUAL,
        BACKSPACE = DOM_PK_BACKSPACE,
        TAB = DOM_PK_TAB,
        Q = DOM_PK_Q,
        W = DOM_PK_W,
        E = DOM_PK_E,
        R = DOM_PK_R,
        T = DOM_PK_T,
        Y = DOM_PK_Y,
        U = DOM_PK_U,
        I = DOM_PK_I,
        O = DOM_PK_O,
        P = DOM_PK_P,
        BRACKET_LEFT = DOM_PK_BRACKET_LEFT,
        BRACKET_RIGHT = DOM_PK_BRACKET_RIGHT,
        ENTER = DOM_PK_ENTER,
        CONTROL_LEFT = DOM_PK_CONTROL_LEFT,
        A = DOM_PK_A,
        S = DOM_PK_S,
        D = DOM_PK_D,
        F = DOM_PK_F,
        G = DOM_PK_G,
        H = DOM_PK_H,
        J = DOM_PK_J,
        K = DOM_PK_K,
        L = DOM_PK_L,
        SEMICOLON = DOM_PK_SEMICOLON,
        QUOTE = DOM_PK_QUOTE,
        BACKQUOTE = DOM_PK_BACKQUOTE,
        SHIFT_LEFT = DOM_PK_SHIFT_LEFT,
        BACKSLASH = DOM_PK_BACKSLASH,
        Z = DOM_PK_Z,
        X = DOM_PK_X,
        C = DOM_PK_C,
        V = DOM_PK_V,
        B = DOM_PK_B,
        N = DOM_PK_N,
        M = DOM_PK_M,
        COMMA = DOM_PK_COMMA,
        PERIOD = DOM_PK_PERIOD,
        SLASH = DOM_PK_SLASH,
        SHIFT_RIGHT = DOM_PK_SHIFT_RIGHT,
        NUMPAD_MULTIPLY = DOM_PK_NUMPAD_MULTIPLY,
        ALT_LEFT = DOM_PK_ALT_LEFT,
        SPACE = DOM_PK_SPACE,
        CAPS_LOCK = DOM_PK_CAPS_LOCK,
        F1 = DOM_PK_F1,
        F2 = DOM_PK_F2,
        F3 = DOM_PK_F3,
        F4 = DOM_PK_F4,
        F5 = DOM_PK_F5,
        F6 = DOM_PK_F6,
        F7 = DOM_PK_F7,
        F8 = DOM_PK_F8,
        F9 = DOM_PK_F9,
        F10 = DOM_PK_F10,
        PAUSE = DOM_PK_PAUSE,
        SCROLL_LOCK = DOM_PK_SCROLL_LOCK,
        NUMPAD_7 = DOM_PK_NUMPAD_7,
        NUMPAD_8 = DOM_PK_NUMPAD_8,
        NUMPAD_9 = DOM_PK_NUMPAD_9,
        NUMPAD_SUBTRACT = DOM_PK_NUMPAD_SUBTRACT,
        NUMPAD_4 = DOM_PK_NUMPAD_4,
        NUMPAD_5 = DOM_PK_NUMPAD_5,
        NUMPAD_6 = DOM_PK_NUMPAD_6,
        NUMPAD_ADD = DOM_PK_NUMPAD_ADD,
        NUMPAD_1 = DOM_PK_NUMPAD_1,
        NUMPAD_2 = DOM_PK_NUMPAD_2,
        NUMPAD_3 = DOM_PK_NUMPAD_3,
        NUMPAD_0 = DOM_PK_NUMPAD_0,
        NUMPAD_DECIMAL = DOM_PK_NUMPAD_DECIMAL,
        PRINT_SCREEN = DOM_PK_PRINT_SCREEN,
        INTL_BACKSLASH = DOM_PK_INTL_BACKSLASH,
        F11 = DOM_PK_F11,
        F12 = DOM_PK_F12,
        NUMPAD_EQUAL = DOM_PK_NUMPAD_EQUAL,
        F13 = DOM_PK_F13,
        F14 = DOM_PK_F14,
        F15 = DOM_PK_F15,
        F16 = DOM_PK_F16,
        F17 = DOM_PK_F17,
        F18 = DOM_PK_F18,
        F19 = DOM_PK_F19,
        F20 = DOM_PK_F20,
        F21 = DOM_PK_F21,
        F22 = DOM_PK_F22,
        F23 = DOM_PK_F23,
        KANA_MODE = DOM_PK_KANA_MODE,
        LANG_2 = DOM_PK_LANG_2,
        LANG_1 = DOM_PK_LANG_1,
        INTL_RO = DOM_PK_INTL_RO,
        F24 = DOM_PK_F24,
        CONVERT = DOM_PK_CONVERT,
        NON_CONVERT = DOM_PK_NON_CONVERT,
        INTL_YEN = DOM_PK_INTL_YEN,
        NUMPAD_COMMA = DOM_PK_NUMPAD_COMMA,
        PASTE = DOM_PK_PASTE,
        MEDIA_TRACK_PREVIOUS = DOM_PK_MEDIA_TRACK_PREVIOUS,
        CUT = DOM_PK_CUT,
        COPY = DOM_PK_COPY,
        MEDIA_TRACK_NEXT = DOM_PK_MEDIA_TRACK_NEXT,
        NUMPAD_ENTER = DOM_PK_NUMPAD_ENTER,
        CONTROL_RIGHT = DOM_PK_CONTROL_RIGHT,
        AUDIO_VOLUME_MUTE = DOM_PK_AUDIO_VOLUME_MUTE,
        LAUNCH_APP_2 = DOM_PK_LAUNCH_APP_2,
        MEDIA_PLAY_PAUSE = DOM_PK_MEDIA_PLAY_PAUSE,
        MEDIA_STOP = DOM_PK_MEDIA_STOP,
        EJECT = DOM_PK_EJECT,
        AUDIO_VOLUME_DOWN = DOM_PK_AUDIO_VOLUME_DOWN,
        AUDIO_VOLUME_UP = DOM_PK_AUDIO_VOLUME_UP,
        BROWSER_HOME = DOM_PK_BROWSER_HOME,
        NUMPAD_DIVIDE = DOM_PK_NUMPAD_DIVIDE,
        ALT_RIGHT = DOM_PK_ALT_RIGHT,
        HELP = DOM_PK_HELP,
        NUM_LOCK = DOM_PK_NUM_LOCK,
        HOME = DOM_PK_HOME,
        UP_ARROW = DOM_PK_ARROW_UP,
        PAGE_UP = DOM_PK_PAGE_UP,
        LEFT_ARROW = DOM_PK_ARROW_LEFT,
        RIGHT_ARROW = DOM_PK_ARROW_RIGHT,
        END = DOM_PK_END,
        DOWN_ARROW = DOM_PK_ARROW_DOWN,
        PAGE_DOWN = DOM_PK_PAGE_DOWN,
        INSERT = DOM_PK_INSERT,
        DELETE = DOM_PK_DELETE,
        META_LEFT = DOM_PK_META_LEFT,
        OS_LEFT = DOM_PK_OS_LEFT,
        META_RIGHT = DOM_PK_META_RIGHT,
        OS_RIGHT = DOM_PK_OS_RIGHT,
        CONTEXT_MENU = DOM_PK_CONTEXT_MENU,
        POWER = DOM_PK_POWER,
        BROWSER_SEARCH = DOM_PK_BROWSER_SEARCH,
        BROWSER_FAVORITES = DOM_PK_BROWSER_FAVORITES,
        BROWSER_REFRESH = DOM_PK_BROWSER_REFRESH,
        BROWSER_STOP = DOM_PK_BROWSER_STOP,
        BROWSER_FORWARD = DOM_PK_BROWSER_FORWARD,
        BROWSER_BACK = DOM_PK_BROWSER_BACK,
        LAUNCH_APP_1 = DOM_PK_LAUNCH_APP_1,
        LAUNCH_MAIL = DOM_PK_LAUNCH_MAIL,
        LAUNCH_MEDIA_PLAYER = DOM_PK_LAUNCH_MEDIA_PLAYER,
        MEDIA_SELECT = DOM_PK_MEDIA_SELECT
    };
    #endif
}
#endif // _MST_KEYS_H

#if defined MST_KEY_OPERATOR_IMPLEMENTATION
#include <ostream>
namespace mst
{
    std::ostream& operator<<(std::ostream& buff, const mst::Key& k);
}

std::ostream& mst::operator<<(std::ostream& buff, const mst::Key& k)
{
#define AS_KEY_CODE(k) case mst::Key::k:{ buff << #k;} break;
switch (k)
{
    AS_KEY_CODE(UNKOWN);
    AS_KEY_CODE(ESCAPE);
    AS_KEY_CODE(ZERO);
    AS_KEY_CODE(ONE);
    AS_KEY_CODE(TWO);
    AS_KEY_CODE(THREE);
    AS_KEY_CODE(FOUR);
    AS_KEY_CODE(FIVE);
    AS_KEY_CODE(SIX);
    AS_KEY_CODE(SEVEN);
    AS_KEY_CODE(EIGHT);
    AS_KEY_CODE(NINE);
    AS_KEY_CODE(BACKSPACE);
    AS_KEY_CODE(TAB);
    AS_KEY_CODE(Q);
    AS_KEY_CODE(W);
    AS_KEY_CODE(E);
    AS_KEY_CODE(R);
    AS_KEY_CODE(T);
    AS_KEY_CODE(Y);
    AS_KEY_CODE(U);
    AS_KEY_CODE(I);
    AS_KEY_CODE(O);
    AS_KEY_CODE(P);
    AS_KEY_CODE(CONTROL_LEFT);
    AS_KEY_CODE(A);
    AS_KEY_CODE(S);
    AS_KEY_CODE(D);
    AS_KEY_CODE(F);
    AS_KEY_CODE(G);
    AS_KEY_CODE(H);
    AS_KEY_CODE(J);
    AS_KEY_CODE(K);
    AS_KEY_CODE(L);
    AS_KEY_CODE(SHIFT_LEFT);
    AS_KEY_CODE(Z);
    AS_KEY_CODE(X);
    AS_KEY_CODE(C);
    AS_KEY_CODE(V);
    AS_KEY_CODE(B);
    AS_KEY_CODE(N);
    AS_KEY_CODE(M);
    AS_KEY_CODE(ALT_LEFT);
    AS_KEY_CODE(SPACE);
    AS_KEY_CODE(CAPS_LOCK);
    AS_KEY_CODE(F1);
    AS_KEY_CODE(F2);
    AS_KEY_CODE(F3);
    AS_KEY_CODE(F4);
    AS_KEY_CODE(F5);
    AS_KEY_CODE(F6);
    AS_KEY_CODE(F7);
    AS_KEY_CODE(F8);
    AS_KEY_CODE(F9);
    AS_KEY_CODE(F10);
    AS_KEY_CODE(PAUSE);
    AS_KEY_CODE(F11);
    AS_KEY_CODE(F12);
    AS_KEY_CODE(UP_ARROW);
    AS_KEY_CODE(LEFT_ARROW);
    AS_KEY_CODE(RIGHT_ARROW);
    AS_KEY_CODE(DOWN_ARROW);

    //AS_KEY_CODE(MINUS);
    //AS_KEY_CODE(EQUAL);
    //AS_KEY_CODE(BRACKET_LEFT);
    //AS_KEY_CODE(BRACKET_RIGHT);
    //AS_KEY_CODE(ENTER);
    //AS_KEY_CODE(SEMICOLON);
    //AS_KEY_CODE(QUOTE);
    //AS_KEY_CODE(BACKQUOTE);
    //AS_KEY_CODE(BACKSLASH);
    //AS_KEY_CODE(COMMA);
    //AS_KEY_CODE(PERIOD);
    //AS_KEY_CODE(SLASH);
    //AS_KEY_CODE(SHIFT_RIGHT);
    //AS_KEY_CODE(NUMPAD_MULTIPLY);
    //AS_KEY_CODE(SCROLL_LOCK);
    //AS_KEY_CODE(NUMPAD_7);
    //AS_KEY_CODE(NUMPAD_8);
    //AS_KEY_CODE(NUMPAD_9);
    //AS_KEY_CODE(NUMPAD_SUBTRACT);
    //AS_KEY_CODE(NUMPAD_4);
    //AS_KEY_CODE(NUMPAD_5);
    //AS_KEY_CODE(NUMPAD_6);
    //AS_KEY_CODE(NUMPAD_ADD);
    //AS_KEY_CODE(NUMPAD_1);
    //AS_KEY_CODE(NUMPAD_2);
    //AS_KEY_CODE(NUMPAD_3);
    //AS_KEY_CODE(NUMPAD_0);
    //AS_KEY_CODE(NUMPAD_DECIMAL);
    //AS_KEY_CODE(PRINT_SCREEN);
    //AS_KEY_CODE(INTL_BACKSLASH);
    //AS_KEY_CODE(NUMPAD_EQUAL);
    //AS_KEY_CODE(F13);
    //AS_KEY_CODE(F14);
    //AS_KEY_CODE(F15);
    //AS_KEY_CODE(F16);
    //AS_KEY_CODE(F17);
    //AS_KEY_CODE(F18);
    //AS_KEY_CODE(F19);
    //AS_KEY_CODE(F20);
    //AS_KEY_CODE(F21);
    //AS_KEY_CODE(F22);
    //AS_KEY_CODE(F23);
    //AS_KEY_CODE(KANA_MODE);
    //AS_KEY_CODE(LANG_2);
    //AS_KEY_CODE(LANG_1);
    //AS_KEY_CODE(INTL_RO);
    //AS_KEY_CODE(F24);
    //AS_KEY_CODE(CONVERT);
    //AS_KEY_CODE(NON_CONVERT);
    //AS_KEY_CODE(INTL_YEN);
    //AS_KEY_CODE(NUMPAD_COMMA);
    //AS_KEY_CODE(PASTE);
    //AS_KEY_CODE(MEDIA_TRACK_PREVIOUS);
    //AS_KEY_CODE(CUT);
    //AS_KEY_CODE(COPY);
    //AS_KEY_CODE(MEDIA_TRACK_NEXT);
    //AS_KEY_CODE(NUMPAD_ENTER);
    //AS_KEY_CODE(CONTROL_RIGHT);
    //AS_KEY_CODE(AUDIO_VOLUME_MUTE);
    //AS_KEY_CODE(AUDIO_VOLUME_MUTE);
    //AS_KEY_CODE(LAUNCH_APP_2);
    //AS_KEY_CODE(MEDIA_PLAY_PAUSE);
    //AS_KEY_CODE(MEDIA_STOP);
    //AS_KEY_CODE(EJECT);
    //AS_KEY_CODE(AUDIO_VOLUME_DOWN);
    //AS_KEY_CODE(AUDIO_VOLUME_DOWN);
    //AS_KEY_CODE(AUDIO_VOLUME_UP);
    //AS_KEY_CODE(AUDIO_VOLUME_UP);
    //AS_KEY_CODE(BROWSER_HOME);
    //AS_KEY_CODE(NUMPAD_DIVIDE);
    //AS_KEY_CODE(ALT_RIGHT);
    //AS_KEY_CODE(HELP);
    //AS_KEY_CODE(NUM_LOCK);
    //AS_KEY_CODE(HOME);
    //AS_KEY_CODE(PAGE_UP);
    //AS_KEY_CODE(END);
    //AS_KEY_CODE(PAGE_DOWN);
    //AS_KEY_CODE(INSERT);
    //AS_KEY_CODE(DELETE);
    //AS_KEY_CODE(META_LEFT);
    //AS_KEY_CODE(OS_LEFT);
    //AS_KEY_CODE(META_RIGHT);
    //AS_KEY_CODE(OS_RIGHT);
    //AS_KEY_CODE(CONTEXT_MENU);
    //AS_KEY_CODE(POWER);
    //AS_KEY_CODE(BROWSER_SEARCH);
    //AS_KEY_CODE(BROWSER_FAVORITES);
    //AS_KEY_CODE(BROWSER_REFRESH);
    //AS_KEY_CODE(BROWSER_STOP);
    //AS_KEY_CODE(BROWSER_FORWARD);
    //AS_KEY_CODE(BROWSER_BACK);
    //AS_KEY_CODE(LAUNCH_APP_1);
    //AS_KEY_CODE(LAUNCH_MAIL);
    //AS_KEY_CODE(LAUNCH_MEDIA_PLAYER);
    //AS_KEY_CODE(MEDIA_SELECT);
default:
    buff << "Unknown Key: " << (int)k;
    break;
}
#undef AS_KEY_CODE
return buff;
}
#endif