/*
 * exception.h
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

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>
#include "type.h"

namespace bitmap {

	#define SHOW_EXCEPTION_HEADER false
	#define UNKNOWN_EXCEPTION "Unknown exception"

	#define THROW_EXCEPTION(_H_, _HS_, _T_) {\
		std::stringstream __exc_ss;\
		bool __show_exc_header = (_HS_ && SHOW_EXCEPTION_HEADER);\
		if(__show_exc_header) { __exc_ss << "[" << _H_ << "] "; }\
		__exc_ss << _T_;\
		throw std::runtime_error(__exc_ss.str().c_str());\
		}
	#define THROW_EXCEPTION_MESSAGE(_H_, _HS_, _T_, _M_) {\
		std::stringstream __exc_ss;\
		bool __show_exc_header = (_HS_ && SHOW_EXCEPTION_HEADER);\
		if(__show_exc_header) { __exc_ss << "[" << _H_ << "] "; }\
		__exc_ss << _T_ << ": " << _M_;\
		throw std::runtime_error(__exc_ss.str().c_str());\
		}
}

#endif