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

#ifndef FI_KEY_WIN_HPP
#define FI_KEY_WIN_HPP

#include "config.hpp"

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>


#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "types.hpp"

namespace FakeInput
{
    inline WORD translateKey(KeyType type) 
    {
        static const std::unordered_map<KeyType, WORD> keyMap = 
        {
            { KeyType::Key_A, 'A' },
            { KeyType::Key_B, 'B' },
            { KeyType::Key_C, 'C' },
            { KeyType::Key_D, 'D' },
            { KeyType::Key_E, 'E' },
            { KeyType::Key_F, 'F' },
            { KeyType::Key_G, 'G' },
            { KeyType::Key_H, 'H' },
            { KeyType::Key_I, 'I' },
            { KeyType::Key_J, 'J' },
            { KeyType::Key_K, 'K' },
            { KeyType::Key_L, 'L' },
            { KeyType::Key_M, 'M' },
            { KeyType::Key_N, 'N' },
            { KeyType::Key_O, 'O' },
            { KeyType::Key_P, 'P' },
            { KeyType::Key_Q, 'Q' },
            { KeyType::Key_R, 'R' },
            { KeyType::Key_S, 'S' },
            { KeyType::Key_T, 'T' },
            { KeyType::Key_U, 'U' },
            { KeyType::Key_V, 'V' },
            { KeyType::Key_W, 'W' },
            { KeyType::Key_X, 'X' },
            { KeyType::Key_Y, 'Y' },
            { KeyType::Key_Z, 'Z' },
            { KeyType::Key_Return, VK_RETURN },
            { KeyType::Key_Escape, VK_ESCAPE },
            { KeyType::Key_Space, VK_SPACE },
            { KeyType::Key_Backspace, VK_BACK },
            { KeyType::Key_Tab, VK_TAB },
            { KeyType::Key_VolumeUp, VK_VOLUME_UP },
            { KeyType::Key_VolumeDown, VK_VOLUME_DOWN },
            { KeyType::Key_VolumeMute, VK_VOLUME_MUTE },
            { KeyType::Key_MediaPlayPause, VK_MEDIA_PLAY_PAUSE },
            { KeyType::Key_MediaNext, VK_MEDIA_NEXT_TRACK },
            { KeyType::Key_MediaPrev, VK_MEDIA_PREV_TRACK },
            { KeyType::Key_MediaStop, VK_MEDIA_STOP }
        };

        auto it = keyMap.find(type);
        return it != keyMap.end() ? it->second : 0;
    }

    auto CreateKeyFromKeycode(WORD virtualKey) -> Key
    {
        Key k{};
        k.virtualKey_ = virtualKey;

        k.code_ = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

        // If MapVirtualKey returns 0 OR the key is one of the known multimedia keys,
        // force the input to use only virtual key.
        bool useVirtualKeyOnly =
            k.code_ == 0 ||
            virtualKey == VK_VOLUME_UP ||
            virtualKey == VK_VOLUME_DOWN ||
            virtualKey == VK_VOLUME_MUTE ||
            virtualKey == VK_MEDIA_PLAY_PAUSE ||
            virtualKey == VK_MEDIA_NEXT_TRACK ||
            virtualKey == VK_MEDIA_PREV_TRACK ||
            virtualKey == VK_MEDIA_STOP;

        if (useVirtualKeyOnly) {
            k.code_ = 0; // disable scancode path
            k.name_ = "<virtual key only>";
            return k;
        }

        LONG lParam = k.code_;
        switch (virtualKey)
        {
        case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
        case VK_PRIOR: case VK_NEXT:
        case VK_END: case VK_HOME:
        case VK_INSERT: case VK_DELETE:
        case VK_DIVIDE: case VK_NUMLOCK:
            lParam |= 0x100; // set extended bit
            break;
        }

        char name[129]{};
        if (GetKeyNameTextA(lParam << 16, name, 128)) {
            k.name_ = std::string(name);
        }
        else {
            k.name_ = "<unknown>";
        }

        return k;
    }

    auto CreateKeyFromKeyType(KeyType type) -> Key
    {
        if (type == Key_NoKey)
        {
            return {};
        }
        else
        {
            auto virtualKey = (WORD)translateKey(type);
            return CreateKeyFromKeycode(virtualKey);
        }
    }

    auto CreateKeyFromMessage(MSG* message)
    {
        WPARAM tempVk{};
        switch (message->message)
        {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            tempVk = message->wParam;
            break;
        default:
            throw std::logic_error("Cannot get key from non-key message");
            break;
        }
        return CreateKeyFromKeycode(tempVk);
    }

}
#endif
#endif
