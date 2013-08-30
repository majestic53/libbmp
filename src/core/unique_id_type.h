/*
 * unique_id_type.h
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

#ifndef UNIQUE_ID_TYPE_H_
#define UNIQUE_ID_TYPE_H_

#include <cstdint>
#include "exception.h"

namespace bitmap {

	namespace component {

		namespace unique_id {

			#define UNIQUE_ID_AS_STRING(_UUID_)\
				"{" << VALUE_AS_HEX(uint32_t, ((_UUID_).block[0] >> 32))\
				<< "-"	<< VALUE_AS_HEX(uint16_t, ((_UUID_).block[0] >> 16))\
				<< "-"	<< VALUE_AS_HEX(uint16_t, (_UUID_).block[0])\
				<< "-"	<< VALUE_AS_HEX(uint16_t, ((_UUID_).block[1] >> 48))\
				<< "-"	<< VALUE_AS_HEX(uint16_t, ((_UUID_).block[1] >> 32))\
				<< "-"	<< VALUE_AS_HEX(uint32_t, (_UUID_).block[1]) << "}"

			enum {
				
				UNIQUE_ID_EXCEPTION_ID_INVALID = 0,
				UNIQUE_ID_EXCEPTION_ID_NOT_FOUND,
				UNIQUE_ID_EXCEPTION_ID_REGISTERED,
				UNIQUE_ID_EXCEPTION_UNINITIALIZED,
			};

			static const std::string UNIQUE_ID_EXCEPTION_STR[] = {
				"Invalid unique ID",
				"Unique ID not found",
				"Unique ID previously registered",
				"Unique ID generator uninitialized",
			};

			#define MAX_UNIQUE_ID_EXCEPTION UNIQUE_ID_EXCEPTION_UNINITIALIZED
			#define SHOW_UNIQUE_ID_EXCEPTION_HEADER true
			#define UNIQUE_ID_EXCEPTION_HEADER "UNIQUE_ID"

			#define UNIQUE_ID_EXCEPTION_STRING(_T_)\
				(_T_ > MAX_UNIQUE_ID_EXCEPTION ? UNKNOWN_EXCEPTION : UNIQUE_ID_EXCEPTION_STR[_T_])
			#define THROW_UNIQUE_ID_EXCEPTION(_T_)\
				THROW_EXCEPTION(UNIQUE_ID_EXCEPTION_HEADER, SHOW_UNIQUE_ID_EXCEPTION_HEADER, UNIQUE_ID_EXCEPTION_STRING(_T_))
			#define THROW_UNIQUE_ID_EXCEPTION_MESSAGE(_T_, _M_)\
				THROW_EXCEPTION_MESSAGE(UNIQUE_ID_EXCEPTION_HEADER, SHOW_UNIQUE_ID_EXCEPTION_HEADER, UNIQUE_ID_EXCEPTION_STRING(_T_), _M_)
		}
	}
}

#endif