/*
 * bitmap_type.h
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

#ifndef BITMAP_TYPE_H_
#define BITMAP_TYPE_H_

#include "exception.h"

namespace bitmap {

	enum {
		BITMAP_EXCEPTION_ID_NOT_FOUND = 0,
		BITMAP_EXCEPTION_UNINITIALIZED,
	};

	static const std::string BITMAP_EXCEPTION_STR[] = {
		"Bitmap Id not found",
		"Bitmap manager uninitialized",
	};

	#define BITMAP_EXCEPTION_HEADER "BITMAP"
	#define MAX_BITMAP_EXCEPTION BITMAP_EXCEPTION_UNINITIALIZED
	#define SHOW_BITMAP_EXCEPTION_HEADER true

	#define BITMAP_EXCEPTION_STRING(_T_)\
		(_T_ > MAX_BITMAP_EXCEPTION ? UNKNOWN_EXCEPTION : BITMAP_EXCEPTION_STR[_T_])
	#define THROW_BITMAP_EXCEPTION(_T_)\
		THROW_EXCEPTION(BITMAP_EXCEPTION_HEADER, SHOW_BITMAP_EXCEPTION_HEADER, BITMAP_EXCEPTION_STRING(_T_))
	#define THROW_BITMAP_EXCEPTION_MESSAGE(_T_, _M_)\
		THROW_EXCEPTION_MESSAGE(BITMAP_EXCEPTION_HEADER, SHOW_BITMAP_EXCEPTION_HEADER, BITMAP_EXCEPTION_STRING(_T_), _M_)
}

#endif