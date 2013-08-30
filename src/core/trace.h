/*
 * trace.h
 * Copyright (C) 2013 David Jolly
 * ----------------------
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRACE_H_
#define TRACE_H_

#include "type.h"

namespace bitmap {

	enum {
		TRACE_LEVEL_ERROR = 0,
		TRACE_LEVEL_INFORMATION,
		TRACE_LEVEL_VERBOSE,
	};

	static const std::string TRACE_LEVEL_STR[] = {
		"ERROR", "INFORMATION", "VERBOSE",
	};

	#define MAX_TRACE_LEVEL TRACE_LEVEL_VERBOSE

	#define TRACE_LEVEL_STRING(_T_)\
		((_T_) > MAX_TRACE_LEVEL ? UNKNOWN : TRACE_LEVEL_STR[_T_])

#ifdef TRACE_ENABLED
	#include <Windows.h>
	#define SHOW_TO_TRACE_LEVEL TRACE_LEVEL_VERBOSE

	#define TRACE_MESSAGE(_L_, _M_) {\
		std::stringstream __trace_ss;\
		bool __show_trace = (_L_ <= SHOW_TO_TRACE_LEVEL);\
		if(__show_trace) {\
			__trace_ss << "[" << TRACE_LEVEL_STRING(_L_) << "] " << _M_ << std::endl;\
			OutputDebugString(__trace_ss.str().c_str());\
		}\
		}
#else
	#define TRACE_MESSAGE(_L_, _M_)
#endif
}

#endif