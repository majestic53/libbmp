/*
 * type.h
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

#ifndef TYPE_H_
#define TYPE_H_

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

namespace bitmap {

	#define BLOCK_SIZE (0x10)
	#define UNKNOWN "UNKNOWN"

	#define REFERENCE_PARAMETER(_P_) ((void) (_P_))
	#define SERIALIZE_CALL(_T_, _M_) std::lock_guard<_T_> __LOCK##_M_(_M_)
	#define VALUE_AS_HEX(_T_, _V_)\
		std::setw(2 * sizeof(_T_)) << std::setfill('0') << std::hex << ((unsigned) (_T_) (_V_)) << std::dec
}

#endif