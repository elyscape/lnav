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
 * @file auto_mem.hh
 */

#ifndef __auto_mem_hh
#define __auto_mem_hh

#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#include <exception>

/**
 * Resource management class for memory allocated by a custom allocator.
 *
 * @param T The object type.
 * @param auto_free The function to call to free the managed object.
 */
template<class T, void (*auto_free)(void *) = free>
class auto_mem {

public:
    auto_mem(T *ptr = NULL) : am_ptr(ptr) { };

    auto_mem(auto_mem &am) : am_ptr(am.release()) { };

    ~auto_mem() { this->reset(); };

    operator T *(void) const { return this->am_ptr; };

    auto_mem &operator=(T *ptr) {
	this->reset(ptr);
	return *this;
    };
    
    auto_mem &operator=(auto_mem &am) {
	this->reset(am.release());
	return *this;
    };

    T *release(void) {
	T *retval = this->am_ptr;

	this->am_ptr = NULL;
	return retval;
    };

    T *in(void) { return this->am_ptr; };
    
    T **out(void) {
	this->reset();
	return &this->am_ptr;
    };

    void reset(T *ptr = NULL) {
	if (this->am_ptr != ptr) {
	    auto_free(this->am_ptr);
	    this->am_ptr = ptr;
	}
    };
    
private:
    T *am_ptr;
    
};

#endif
