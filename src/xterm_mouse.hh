/**
 * Copyright (c) 2007-2012, Timothy Stack
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of Timothy Stack nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @file xterm_mouse.hh
 */

#ifndef __xterm_mouse_hh
#define __xterm_mouse_hh

#include <stdlib.h>
#include <curses.h>
#include <string.h>

#include <string>

/**
 * Base class for delegates of the xterm_mouse class.
 */
class mouse_behavior {

public:
	virtual ~mouse_behavior() { };

        /**
         * Callback used to process mouse events.
         * 
         * @param button The button that was pressed or released.  This will
         *   be one of the XT_BUTTON or XT_SCROLL constants in the xterm_mouse
         *   class.
         * @param x      The X coordinate where the event occurred.
         * @param y      The Y coordinate where the event occurred.
         */
	virtual void mouse_event(int button, int x, int y) = 0;
};

/**
 * Class that handles xterm mouse events coming through the ncurses interface.
 */
class xterm_mouse {

public:
	static const int XT_MAGIC = 32;

	static const int XT_BUTTON1 = 0;
	static const int XT_BUTTON2 = 1;
	static const int XT_BUTTON3 = 2;
	static const int XT_BUTTON_RELEASE = 3;

	static const int XT_SCROLL_WHEEL_FLAG = 64;
	static const int XT_SCROLL_UP =
		XT_SCROLL_WHEEL_FLAG | XT_BUTTON1;
	static const int XT_SCROLL_DOWN =
		XT_SCROLL_WHEEL_FLAG | XT_BUTTON2;

    	static const int XT_BUTTON__MASK =
    		XT_SCROLL_WHEEL_FLAG |
		XT_BUTTON1 |
		XT_BUTTON2 |
		XT_BUTTON3;

	static const char *XT_TERMCAP;
	static const char *XT_TERMCAP_TRACKING;

        /**
         * @return True if the user's terminal supports xterm-mouse events.
         */
	static bool is_available() {
		static const char *termname = getenv("TERM");
		bool retval = false;

		if (termname and strstr(termname, "xterm") != NULL) {
			retval = true;
		}
		return retval;
	};

	xterm_mouse() : xm_enabled(false), xm_behavior(NULL) {
	};

	~xterm_mouse() {
		set_enabled(false);
	};

        /**
         * @param enabled True if xterm mouse support should be enabled in the
         *   terminal.
         */
	void set_enabled(bool enabled) {
		if (is_available()) {
			putp(tparm((char *)XT_TERMCAP, enabled ? 1 : 0));
			putp(tparm((char *)XT_TERMCAP_TRACKING, enabled ? 1 : 0));
			this->xm_enabled = enabled;
		}
	};

        /**
         * @return True if xterm mouse support is enabled, false otherwise.
         */
	bool is_enabled() const {
		return this->xm_enabled;
	};

        /**
         * @param mb The delegate to send mouse events to.
         */
	void set_behavior(mouse_behavior *mb) {
		this->xm_behavior = mb;
	};

	mouse_behavior *get_behavior() { return this->xm_behavior; };

        /**
         * Handle a KEY_MOUSE character from ncurses.
         * @param ch unused
         */
	void handle_mouse(int ch) {
                /* The button event and coordinates follow the KEY_MOUSE char */
	    	int bstate = getch() - XT_MAGIC & XT_BUTTON__MASK;
	    	int x = getch() - XT_MAGIC - 1;
	    	int y = getch() - XT_MAGIC - 1;

	    	if (this->xm_behavior) {
	    		this->xm_behavior->mouse_event(bstate, x, y);
	    	}
	};

private:
	bool xm_enabled;
	mouse_behavior *xm_behavior;

};

#endif
