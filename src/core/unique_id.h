/*
 * unique_id.h
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

#ifndef UNIQUE_ID_H_
#define UNIQUE_ID_H_

#include <cstdint>
#include <string>
#include "bitmap.h"

namespace bitmap {

	#define VALID_UNIQUE_ID(_UUID_) ((_UUID_).block[0] || (_UUID_).block[1])

	bool operator<(
		const uid_t &left,
		const uid_t &right
		);

	namespace component {

		namespace unique_id {

			std::string convert_to_string(
				const uid_t &unique_id
				);

			uid_t generate(
				bool register_id = true
				);

			void initialize(void);

			bool is_unique(
				const uid_t &unique_id
				);

			void register_id(
				const uid_t &unique_id
				);

			void unregister_all_ids(void);

			void unregister_id(
				const uid_t &unique_id
				);
		}
	}
}

#endif