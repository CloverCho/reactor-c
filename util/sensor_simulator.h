/**
@file
@author Edward A. Lee (eal@berkeley.edu)

@section LICENSE
Copyright (c) 2020, The University of California at Berkeley.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

@section DESCRIPTION

When prototyping Lingua Franca programs on a laptop, it is convenient to use
the laptop keyboard to simulate asynchronous sensor input. This small library
provides a convenient way to do that.

To use this, include the following flags in your target properties:
<pre>
target C {
    cmake-include: "include/ncurses-cmake-extension.txt", // Adds support for ncurses
    files: ["/lib/C/util/sensor_simulator.c", "/lib/C/util/sensor_simulator.h"]
};
</pre>
This requires `ncurses`, a library providing somewhat portable keyboard access.

In addition, you need this in your Lingua Franca file:
<pre>
preamble {=
    #include "sensor_simulator.c"
=}
</pre>
To start the sensor simulator, call `start_sensor_simulator` passing it
an array of strings to print and the width of the window to use to display
characters using the `show_tick` function.

To print messages to the screen, rather than using printf(), you should use
the messaging functions in util.h, such as info_print(). Otherwise, your messages
will be printed over other information.
*/

#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

/**
 * Start the sensor simulator if it has not been already
 * started. This must be called at least once before any
 * call to register_sensor_key. If given, this will
 * put a message in the center of the terminal window.
 * The message will be left justified, with each string
 * in the specified array on a new line.
 * If a tick_window_width is given, then a second window
 * will be created that is intended to show activity,
 * typically using a single character per event.
 * @param message_lines The message lines or NULL for none.
 * @param number_of_lines The number of message lines or 0 for none.
 * @param tick_window_width The width of the tick window or 0 for none.
 * @param log_file If non-NULL, the name of a file to which to write logging messages.
 * @param log_level The level of log messages to redirect to the file.
 *  The level should be one of LOG_LEVEL_ERROR, LOG_LEVEL_WARNING,
 *  LOG_LEVEL_INFO, LOG_LEVEL_LOG, or LOG_LEVEL_DEBUG.
 */
int start_sensor_simulator(
		char* message_lines[],
		int number_of_lines,
		int tick_window_width,
		char* log_file,
		int log_level
);

/**
 * End ncurses control of the terminal.
 */
void end_sensor_simulator();

/**
 * Place a tick (usually a single character) in the tick window.
 * @param character The tick character.
 */
void show_tick(char* character);

/**
 * Register a keyboard key to trigger the specified action.
 * Printable ASCII characters (codes 32 to 127) are supported
 * plus '\n' and '\0', where the latter registers a trigger
 * to invoked when any key is pressed. If a specific key is
 * registered and any key ('\0') is also registered, the
 * any key trigger will be scheduled after the specific key
 * is scheduled. If these triggers belong to different reactors,
 * they could be invoked in parallel.
 * This will fail if the specified key has already been
 * registered (error code 1), or the key is not a supported key or a
 * newline ‘\n’ or any key '\0' (error code 2) or if the trigger is NULL
 * (error code 3).
 * @param key The key to register.
 * @param action The action to trigger when the key is pressed
 *  (a pointer to a trigger_t struct).
 * @return 0 for success, error code for failure.
 */
int register_sensor_key(char key, void* action);

#endif // SENSOR_SIMULATOR_H
