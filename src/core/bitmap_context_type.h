/*
 * bitmap_context_type.h
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

#ifndef BITMAP_CONTEXT_TYPE_H_
#define BITMAP_CONTEXT_TYPE_H_

#include "exception.h"

namespace bitmap {

	namespace component {

		enum {
			BITMAP_CONTEXT_EXCEPTION_COORD_OUT_OF_RANGE = 0,
			BITMAP_CONTEXT_EXCEPTION_FILE_IO_FAILED,
			BITMAP_CONTEXT_EXCEPTION_FILE_NOT_FOUND,
			BITMAP_CONTEXT_EXCEPTION_INVALID_PARAMETER,
			BITMAP_CONTEXT_EXCEPTION_MALFORMED_FILE,
		};

		static const std::string BITMAP_CONTEXT_EXCEPTION_STR[] = {
			"Coordinates out-of-range",
			"Bitmap file io failed",
			"Bitmap file not found",
			"Invalid bitmap context parameter",
			"Malformed bitmap file",
		};

		static const std::string CHANNEL_STR[] = {
			"BLUE", "GREEN", "RED",
		};

		#define BITMAP_CONTEXT_EXCEPTION_HEADER "BITMAP_CONTEXT"
		#define MAX_BITMAP_CONTEXT_EXCEPTION BITMAP_CONTEXT_EXCEPTION_MALFORMED_FILE
		
		#define SHOW_BITMAP_CONTEXT_EXCEPTION_HEADER true

		#define BITMAP_CONTEXT_EXCEPTION_STRING(_T_)\
			((_T_) > MAX_BITMAP_CONTEXT_EXCEPTION ? UNKNOWN_EXCEPTION : BITMAP_CONTEXT_EXCEPTION_STR[_T_])
		#define CHANNEL_STRING(_T_)\
			((_T_) > MAX_CHANNEL ? UNKNOWN : CHANNEL_STR[_T_])
		#define THROW_BITMAP_CONTEXT_EXCEPTION(_T_)\
			THROW_EXCEPTION(\
				BITMAP_CONTEXT_EXCEPTION_HEADER,\
				SHOW_BITMAP_CONTEXT_EXCEPTION_HEADER,\
				BITMAP_CONTEXT_EXCEPTION_STRING(_T_))
		#define THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(_T_, _M_)\
			THROW_EXCEPTION_MESSAGE(\
				BITMAP_CONTEXT_EXCEPTION_HEADER,\
				SHOW_BITMAP_CONTEXT_EXCEPTION_HEADER,\
				BITMAP_CONTEXT_EXCEPTION_STRING(_T_),\
				_M_)

		class _bitmap_context;
		typedef _bitmap_context bitmap_context, *bitmap_context_ptr;
	}
}

#endif