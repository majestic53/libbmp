/*
 * unique_id.cpp
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

#include <map>
#include <set>
#include "random.h"
#include "trace.h"
#include "unique_id.h"
#include "unique_id_type.h"

namespace bitmap {

	bool 
	operator<(
		const uid_t &left,
		const uid_t &right
		)
	{
		return (left.block[0] < right.block[0])
				|| (left.block[0] == right.block[0]
						&& left.block[1] < right.block[1]);
	}

	bool 
	operator==(
		const uid_t &left,
		const uid_t &right
		)
	{
		return (left.block[0] == right.block[0])
				&& (left.block[1] == right.block[1]);
	}

	bool 
	operator!=(
		const uid_t &left,
		const uid_t &right
		)
	{
		return !(left == right);
	}

	namespace component {

		namespace unique_id {

			#define UNIQUE_UNIQUE_ID(_UUID_) (__unique_id_set.find(_UUID_) == __unique_id_set.end())

			static bool __unique_id_initialized = false;
			static std::recursive_mutex __unique_id_lock;
			static std::map<uid_t, size_t> __unique_id_ref_map;
			static std::set<uid_t> __unique_id_set;
			
			std::map<uid_t, size_t>::iterator
			__find_unique_id_ref(
				const uid_t &unique_id
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::thread_id::__find_unique_id_ref");

				std::map<uid_t, size_t>::iterator result;

				if(!VALID_UNIQUE_ID(unique_id)) {
					THROW_UNIQUE_ID_EXCEPTION_MESSAGE(
						UNIQUE_ID_EXCEPTION_ID_INVALID,
						UNIQUE_ID_AS_STRING(unique_id)
						);
				}
				result = __unique_id_ref_map.find(unique_id);

				if(result == __unique_id_ref_map.end()) {
					THROW_UNIQUE_ID_EXCEPTION_MESSAGE(
						UNIQUE_ID_EXCEPTION_ID_REGISTERED,
						UNIQUE_ID_AS_STRING(unique_id)
						);
				}

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::thread_id::__find_unique_id_ref");

				return result;
			}

			std::string 
			convert_to_string(
				const uid_t &unique_id
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::unique_id::convert_to_string");

				SERIALIZE_CALL(std::recursive_mutex, __unique_id_lock);

				std::stringstream ss;

				ss << UNIQUE_ID_AS_STRING(unique_id);

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::unique_id::convert_to_string");

				return ss.str();
			}

			uid_t 
			generate(
				bool register_id
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::unique_id::generate");

				SERIALIZE_CALL(std::recursive_mutex, __unique_id_lock);

				uid_t result = NULL_UNIQUE_ID;

				if(!__unique_id_initialized) {
					THROW_UNIQUE_ID_EXCEPTION(UNIQUE_ID_EXCEPTION_UNINITIALIZED);
				}

				do {
					result.block[0] = ((uint64_t) random::generate_uniform(1, UINT32_MAX) << 32) 
						| (uint32_t) random::generate_uniform(1, UINT32_MAX);
					result.block[1] = ((uint64_t) random::generate_uniform(1, UINT32_MAX) << 32) 
						| (uint32_t) random::generate_uniform(1, UINT32_MAX);

					if(register_id
							&& !UNIQUE_UNIQUE_ID(result)) {
						continue;
					}
				} while(!VALID_UNIQUE_ID(result));

				if(register_id) {

					if(UNIQUE_UNIQUE_ID(result)) {
						__unique_id_ref_map.insert(std::pair<uid_t, size_t>(result, 1));
						__unique_id_set.insert(result);
					} else {
						++__find_unique_id_ref(result)->second;
					}
				}

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::unique_id::generate, result. "
					<< UNIQUE_ID_AS_STRING(result));

				return result;
			}

			void 
			initialize(void)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::unique_id::initialize");

				SERIALIZE_CALL(std::recursive_mutex, __unique_id_lock);

				__unique_id_ref_map.clear();
				__unique_id_set.clear();
				__unique_id_initialized = true;

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::unique_id::initialize");
			}

			bool 
			is_unique(
				const uid_t &unique_id
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::unique_id::is_unique");

				SERIALIZE_CALL(std::recursive_mutex, __unique_id_lock);

				bool result;

				if(!__unique_id_initialized) {
					THROW_UNIQUE_ID_EXCEPTION(UNIQUE_ID_EXCEPTION_UNINITIALIZED);
				}

				if(!VALID_UNIQUE_ID(unique_id)) {
					THROW_UNIQUE_ID_EXCEPTION_MESSAGE(
						UNIQUE_ID_EXCEPTION_ID_INVALID,
						UNIQUE_ID_AS_STRING(unique_id)
						);
				}
				result = UNIQUE_UNIQUE_ID(unique_id);

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::unique_id::is_unique, result. "
					<< VALUE_AS_HEX(bool, result));

				return result;
			}

			void 
			register_id(
				const uid_t &unique_id
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::unique_id::register_id");

				SERIALIZE_CALL(std::recursive_mutex, __unique_id_lock);

				if(!__unique_id_initialized) {
					THROW_UNIQUE_ID_EXCEPTION(UNIQUE_ID_EXCEPTION_UNINITIALIZED);
				}

				if(!VALID_UNIQUE_ID(unique_id)) {
					THROW_UNIQUE_ID_EXCEPTION_MESSAGE(
						UNIQUE_ID_EXCEPTION_ID_INVALID,
						UNIQUE_ID_AS_STRING(unique_id)
						);
				}
				
				if(UNIQUE_UNIQUE_ID(unique_id)) {
					__unique_id_ref_map.insert(std::pair<uid_t, size_t>(unique_id, 1));
					__unique_id_set.insert(unique_id);
				} else {
					++__find_unique_id_ref(unique_id)->second;
				}

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::unique_id::register_id");
			}

			void 
			unregister_all_ids(void)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::unique_id::unregister_all_ids");

				SERIALIZE_CALL(std::recursive_mutex, __unique_id_lock);

				if(!__unique_id_initialized) {
					THROW_UNIQUE_ID_EXCEPTION(UNIQUE_ID_EXCEPTION_UNINITIALIZED);
				}
				__unique_id_ref_map.clear();
				__unique_id_set.clear();

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::unique_id::unregister_all_ids");
			}

			void 
			unregister_id(
				const uid_t &unique_id
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::unique_id::unregister_id");

				SERIALIZE_CALL(std::recursive_mutex, __unique_id_lock);

				std::map<uid_t, size_t>::iterator unique_id_ref_iter;

				if(!__unique_id_initialized) {
					THROW_UNIQUE_ID_EXCEPTION(UNIQUE_ID_EXCEPTION_UNINITIALIZED);
				}

				if(!VALID_UNIQUE_ID(unique_id)) {
					THROW_UNIQUE_ID_EXCEPTION_MESSAGE(
						UNIQUE_ID_EXCEPTION_ID_INVALID,
						UNIQUE_ID_AS_STRING(unique_id)
						);
				}
				
				if(UNIQUE_UNIQUE_ID(unique_id)) {
					THROW_UNIQUE_ID_EXCEPTION_MESSAGE(
						UNIQUE_ID_EXCEPTION_ID_NOT_FOUND,
						UNIQUE_ID_AS_STRING(unique_id)
						);
				}
				unique_id_ref_iter = __find_unique_id_ref(unique_id);

				if(unique_id_ref_iter->second <= 1) {
					__unique_id_ref_map.erase(unique_id);
					__unique_id_set.erase(unique_id);
				} else {
					--unique_id_ref_iter->second;
				}

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::unique_id::unregister_id");
			}
		}
	}
}