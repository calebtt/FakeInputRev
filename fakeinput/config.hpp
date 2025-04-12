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

#ifndef WC_CONFIG_HPP
#define WC_CONFIG_HPP
#include <string>

// Key base type
template<typename Vk_t = unsigned, typename Scan_t = unsigned>
struct Key_base
{
    Scan_t code_{}; // hardware scancode
    Vk_t virtualKey_{}; // virtual keycode
    std::string name_{ "<no key>" };
};

//#ifndef UNIX
//    #define UNIX
// #include "key_unix.hpp"
//// using Key = Key_unix;
// using Key = Key_base<KeySym, unsigned>;
//#endif

// Define platform macros manually since CMake is not being used
#ifndef WIN32
#define WIN32
#endif

#ifdef WIN32
using Key = Key_base<>;
#endif

#endif
