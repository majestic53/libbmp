/*
 * random_type.h
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

#ifndef RANDOM_TYPE_H_
#define RANDOM_TYPE_H_

#include "exception.h"

namespace bitmap {

	namespace component {

		namespace random {

			enum {
				RANDOM_EXCEPTION_INVALID_PARAMETER = 0,
				RANDOM_EXCEPTION_UNINITIALIZED,
			};

			static const std::string RANDOM_EXCEPTION_STR[] = {
				"Invalid random generator parameter",
				"Random generator uninitialized",
			};

			#define MAX_RANDOM_EXCEPTION RANDOM_EXCEPTION_UNINITIALIZED
			#define RANDOM_EXCEPTION_HEADER "RANDOM"
			#define SHOW_RANDOM_EXCEPTION_HEADER true

			#define RANDOM_EXCEPTION_STRING(_T_)\
				(_T_ > MAX_RANDOM_EXCEPTION ? UNKNOWN_EXCEPTION : RANDOM_EXCEPTION_STR[_T_])
			#define THROW_RANDOM_EXCEPTION(_T_)\
				THROW_EXCEPTION(RANDOM_EXCEPTION_HEADER, SHOW_RANDOM_EXCEPTION_HEADER, RANDOM_EXCEPTION_STRING(_T_))
			#define THROW_RANDOM_EXCEPTION_MESSAGE(_T_, _M_)\
				THROW_EXCEPTION_MESSAGE(RANDOM_EXCEPTION_HEADER, SHOW_RANDOM_EXCEPTION_HEADER, RANDOM_EXCEPTION_STRING(_T_), _M_)
		}
	}
}

#endif