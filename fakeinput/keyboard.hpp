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

#ifndef FI_KEYBOARD_HPP
#define FI_KEYBOARD_HPP

#include "config.hpp"

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "key_win.hpp"
#endif

#ifdef UNIX
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include "display_unix.hpp"
#include "key_unix.hpp"
#endif

#include <iostream>

namespace FakeInput
{
    /** Represents keyboard device.
     *
     * Allows you to simulate key press.
     */
    class Keyboard
    {
    public:

        static void pressKey(Key key)
        {
            sendKeyEvent_(key, true);
        }

        static void releaseKey(Key key)
        {
            sendKeyEvent_(key, false);
        }

    private:
        /** Send fake key event to the system.
         *
         * @param key
         *      Key object representing real key to be pressed.
         * @param isPress
         *      Whether event is press or release.
         */
#ifdef WIN32
        static void sendKeyEvent_(Key key, bool isPress)
        {
            if (key.virtualKey_ == 0) {
                std::cerr << "Cannot send <no key> event" << std::endl;
                return;
            }

            INPUT input{};
            input.type = INPUT_KEYBOARD;

            // Fallback logic: use scan code if valid, otherwise use virtual key
            if (key.code_ != 0) {
                input.ki.wScan = key.code_;
                input.ki.dwFlags = KEYEVENTF_SCANCODE;
            }
            else {
                input.ki.wVk = key.virtualKey_;
                input.ki.dwFlags = 0; // fallback to virtual key
            }

            if (!isPress) {
                input.ki.dwFlags |= KEYEVENTF_KEYUP;
            }

            ::SendInput(1, &input, sizeof(INPUT));
        }

#endif
#ifdef UNIX
        void sendKeyEvent_(Key key, bool isPress)
        {
            if (key.keysym() == NoSymbol)
            {
                std::cerr << "Cannot send <no key> event" << std::endl;
            }
            else
            {
                XTestFakeKeyEvent(display(), key.code(), isPress, CurrentTime);
                XFlush(display());
            }
        }
#endif
    };
}

#endif
