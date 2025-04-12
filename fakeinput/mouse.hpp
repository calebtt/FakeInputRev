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

#ifndef FI_MOUSE_HPP
#define FI_MOUSE_HPP

#include "config.hpp"

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#ifdef UNIX
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

#include <unordered_map>
#include "types.hpp"

namespace FakeInput
{

    struct Mouse 
    {
        static DWORD translateMouseButton(MouseButton button) 
        {
#ifdef WIN32
            static const std::unordered_map<MouseButton, DWORD> buttonMap = {
                { MouseButton::Mouse_Left, MOUSEEVENTF_LEFTDOWN },
                { MouseButton::Mouse_Right, MOUSEEVENTF_RIGHTDOWN },
                { MouseButton::Mouse_Middle, MOUSEEVENTF_MIDDLEDOWN }
            };

            auto it = buttonMap.find(button);
            return it != buttonMap.end() ? it->second : 0;
#elif defined(UNIX)
            // X11 buttons: 1 = left, 2 = middle, 3 = right
            static const std::unordered_map<MouseButton, unsigned int> buttonMap = {
                { MouseButton::Mouse_Left, 1 },
                { MouseButton::Mouse_Middle, 2 },
                { MouseButton::Mouse_Right, 3 }
            };

            auto it = buttonMap.find(button);
            return it != buttonMap.end() ? it->second : 0;
#endif
        }

        static void move(int dx, int dy) {
#ifdef WIN32
            INPUT input{};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            input.mi.dx = dx;
            input.mi.dy = dy;
            ::SendInput(1, &input, sizeof(INPUT));
#elif defined(UNIX)
            Display* display = XOpenDisplay(nullptr);
            if (display) {
                XTestFakeRelativeMotionEvent(display, dx, dy, CurrentTime);
                XFlush(display);
                XCloseDisplay(display);
            }
#endif
        }

        static void pressButton(MouseButton button) {
#ifdef WIN32
            INPUT input{};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = translateMouseButton(button);  // e.g., MOUSEEVENTF_LEFTDOWN
            ::SendInput(1, &input, sizeof(INPUT));
#elif defined(UNIX)
            Display* display = XOpenDisplay(nullptr);
            if (display) {
                XTestFakeButtonEvent(display, static_cast<unsigned int>(button) + 1, True, CurrentTime);
                XFlush(display);
                XCloseDisplay(display);
            }
#endif
        }

        static void releaseButton(MouseButton button) {
#ifdef WIN32
            INPUT input{};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = translateMouseButton(button) << 1;  // e.g., MOUSEEVENTF_LEFTUP
            ::SendInput(1, &input, sizeof(INPUT));
#elif defined(UNIX)
            Display* display = XOpenDisplay(nullptr);
            if (display) {
                XTestFakeButtonEvent(display, static_cast<unsigned int>(button) + 1, False, CurrentTime);
                XFlush(display);
                XCloseDisplay(display);
            }
#endif
        }

        static void moveTo(int x, int y) {
#ifdef WIN32
            INPUT input{};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

            // Normalize to 0–65535
            input.mi.dx = static_cast<LONG>(x * (65535.0f / GetSystemMetrics(SM_CXSCREEN)));
            input.mi.dy = static_cast<LONG>(y * (65535.0f / GetSystemMetrics(SM_CYSCREEN)));
            ::SendInput(1, &input, sizeof(INPUT));
#elif defined(UNIX)
            Display* display = XOpenDisplay(nullptr);
            if (display) {
                Window root = DefaultRootWindow(display);
                XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
                XFlush(display);
                XCloseDisplay(display);
            }
#endif
        }

        static void wheelUp()
        {
#ifdef WIN32
            INPUT input{};
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.mouseData = WHEEL_DELTA;

            SendInput(1, &input, sizeof(INPUT));
#elif defined(UNIX)
            XTestFakeButtonEvent(display(), 4, true, CurrentTime);
            XSync(display(), false);
            XTestFakeButtonEvent(display(), 4, false, CurrentTime);
            XFlush(display());
#endif
        }

        static void wheelDown()
        {
#ifdef WIN32
            INPUT input{};
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.mouseData = -WHEEL_DELTA;

            SendInput(1, &input, sizeof(INPUT));
#elif defined(UNIX)
            XTestFakeButtonEvent(display(), 5, true, CurrentTime);
            XSync(display(), false);
            XTestFakeButtonEvent(display(), 5, false, CurrentTime);
            XFlush(display());
#endif
        }
    };
}

#endif
