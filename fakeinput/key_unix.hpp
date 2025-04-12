/**
 * This file is part of the FakeInput library (https://github.com/uiii/FakeInput)
 *
 * Copyright (C) 2011 by Richard Jedlicka <uiii.dev@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef FI_KEY_UNIX_HPP
#define FI_KEY_UNIX_HPP

#include "config.hpp"
#ifdef UNIX

#include <X11/Xlib.h>

#include <string>
#include <unordered_map>

#include "key_base.hpp"
#include "types.hpp"

namespace FakeInput
{
    inline unsigned int translateKey(KeyType type) 
    {
        static const std::unordered_map<KeyType, unsigned int> keyMap = 
        {
            { KeyType::Key_A, XK_A },
            { KeyType::Key_B, XK_B },
            { KeyType::Key_C, XK_C },
            { KeyType::Key_D, XK_D },
            { KeyType::Key_E, XK_E },
            { KeyType::Key_F, XK_F },
            { KeyType::Key_G, XK_G },
            { KeyType::Key_H, XK_H },
            { KeyType::Key_I, XK_I },
            { KeyType::Key_J, XK_J },
            { KeyType::Key_K, XK_K },
            { KeyType::Key_L, XK_L },
            { KeyType::Key_M, XK_M },
            { KeyType::Key_N, XK_N },
            { KeyType::Key_O, XK_O },
            { KeyType::Key_P, XK_P },
            { KeyType::Key_Q, XK_Q },
            { KeyType::Key_R, XK_R },
            { KeyType::Key_S, XK_S },
            { KeyType::Key_T, XK_T },
            { KeyType::Key_U, XK_U },
            { KeyType::Key_V, XK_V },
            { KeyType::Key_W, XK_W },
            { KeyType::Key_X, XK_X },
            { KeyType::Key_Y, XK_Y },
            { KeyType::Key_Z, XK_Z },
            { KeyType::Key_Enter, XK_Return },
            { KeyType::Key_Escape, XK_Escape },
            { KeyType::Key_Space, XK_space },
            { KeyType::Key_Backspace, XK_BackSpace },
            { KeyType::Key_Tab, XK_Tab },
            { KeyType::Key_VolumeUp, XF86XK_AudioRaiseVolume },
            { KeyType::Key_VolumeDown, XF86XK_AudioLowerVolume },
            { KeyType::Key_VolumeMute, XF86XK_AudioMute },
            { KeyType::Key_MediaPlayPause, XF86XK_AudioPlay },
            { KeyType::Key_MediaNext, XF86XK_AudioNext },
            { KeyType::Key_MediaPrev, XF86XK_AudioPrev },
            { KeyType::Key_MediaStop, XF86XK_AudioStop }
        };

        auto it = keyMap.find(type);
        return it != keyMap.end() ? it->second : 0;
    }

    auto CreateKeyFromKeyType(KeyType type)
    {
        Key k{};

        k.virtualKeycode_ = translateKey(type);
        k.code_ = XKeysymToKeycode(display(), keysym_);
        k.name_ = XKeysymToString(keysym_);
        return k;
    }

    auto CreateKeyFromEvent(XEvent* event)
    {
        if (event->type != KeyPress && event->type != KeyRelease)
            throw std::logic_error("Cannot get key from non-key event");

        Key k{};

        k.code_ = event->xkey.keycode;
        k.virtualKeycode_ = XKeycodeToKeysym(event->xkey.display, code_, 0);
        k.name_ = XKeysymToString(keysym_);
        return k;
    }

    auto CreateKeyFromKeycode(KeySym keySym)
    {
        Key k{};
        k.code_ = XKeysymToKeycode(display(), keysym);
        k.virtualKeycode_ = keysym;
        k.name_ = XKeysymToString(keysym);
        return k;
    }
}

#endif
#endif