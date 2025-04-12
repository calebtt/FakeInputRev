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

#ifndef FI_SYSTEM_HPP
#define FI_SYSTEM_HPP

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
#include <stdlib.h>
#include <time.h>
#endif
#include <string>

namespace FakeInput
{
    /** Handler of some system operations.
     *
     * Allows you to run command-line commands
     * and wait for specified time
     */
    class System
    {
    public:
#ifdef WIN32
        /** Executes command-line command.
         *
         * @param cmd
         *     %Command to run.
         */
        static void run(const std::string& cmd)
        {
            std::string command = "start " + cmd;

            system(command.c_str());
        }

        /** Sleeps the current thread and wait for specified time.
         *
         * @param milisec
         *     time to wait in miliseconds
         */
        static void wait(unsigned int milisec)
        {
            Sleep(milisec);
        }
#endif

#ifdef UNIX
        static void run(const std::string& cmd)
        {
            std::string command = cmd + " &";

            system(command.c_str());
        }

        static void wait(unsigned int milisec)
        {
            timespec tm;
            tm.tv_sec = milisec / 1000;
            tm.tv_nsec = (milisec % 1000) * 1000000;

            nanosleep(&tm, NULL);
        }
#endif

    };
}

#endif
