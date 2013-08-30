/*
 * unique_id_class.cpp
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

#include "unique_id_class.h"
#include "unique_id_class_type.h"
#include "trace.h"

namespace bitmap {

	namespace component {

		namespace unique_id {

			_unique_id_class::_unique_id_class(
				bool register_id
				) :
					_unique_id(unique_id::generate(register_id))
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_unique_id_class::_unique_id_class");
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_unique_id_class::_unique_id_class");
			}

			_unique_id_class::_unique_id_class(
				const uid_t &unique_id,
				bool register_id
				) :
					_unique_id(unique_id)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_unique_id_class::_unique_id_class");

				if(!VALID_UNIQUE_ID(unique_id)) {
					_unique_id = unique_id::generate(register_id);
				} else if(register_id) {
					unique_id::register_id(_unique_id);
				}

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_unique_id_class::_unique_id_class");
			}

			_unique_id_class::_unique_id_class(
				const _unique_id_class &other
				) :
					_unique_id(other._unique_id)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_unique_id_class::_unique_id_class");

				unique_id::register_id(_unique_id);

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_unique_id_class::_unique_id_class");
			}

			_unique_id_class::~_unique_id_class(void)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_unique_id_class::~_unique_id_class");

				if(!unique_id::is_unique(_unique_id)) {
					unique_id::unregister_id(_unique_id);
				}
				memset(&_unique_id, 0, sizeof(uid_t));

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_unique_id_class::~_unique_id_class");
			}

			_unique_id_class &
			_unique_id_class::operator=(
				const _unique_id_class &other
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_unique_id_class::operator=");

				SERIALIZE_CALL(std::recursive_mutex, _unique_id_class_lock);

				if(this != &other) {
					_unique_id = other._unique_id;
					unique_id::register_id(_unique_id);
				}

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_unique_id_class::operator=");

				return *this;
			}

			uid_t 
			_unique_id_class::get_unique_id(void)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_unique_id_class::get_unique_id");

				SERIALIZE_CALL(std::recursive_mutex, _unique_id_class_lock);

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_unique_id_class::get_unique_id, result. "
					<< unique_id::convert_to_string(_unique_id));

				return _unique_id;
			}

			std::string 
			_unique_id_class::to_string(
				bool verbose
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_unique_id_class::to_string");
				
				REFERENCE_PARAMETER(verbose);

				SERIALIZE_CALL(std::recursive_mutex, _unique_id_class_lock);

				std::stringstream ss;

				ss << unique_id::convert_to_string(_unique_id);

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_unique_id_class::to_string");

				return ss.str();
			}
		}
	}
}