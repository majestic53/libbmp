/*
 * unique_id_class.h
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

#ifndef UNIQUE_ID_CLASS_H_
#define UNIQUE_ID_CLASS_H_

#include <mutex>
#include "unique_id.h"

namespace bitmap {

	namespace component {

		namespace unique_id {

			typedef class _unique_id_class {

				public:

					_unique_id_class(
						bool register_id = true
						);

					_unique_id_class(
						const uid_t &unique_id,
						bool register_id = true
						);

					_unique_id_class(
						const _unique_id_class &other
						);

					virtual ~_unique_id_class(void);

					_unique_id_class &operator=(
						const _unique_id_class &other
						);

					uid_t get_unique_id(void);

					virtual std::string to_string(
						bool verbose
						);

				protected:

					uid_t _unique_id;

				private:

					std::recursive_mutex _unique_id_class_lock;

			} unique_id_class, *unique_id_class_ptr;
		}
	}
}

#endif