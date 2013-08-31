/*
 * bitmap.cpp
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
#include "bitmap.h"
#include "bitmap_context.h"
#include "bitmap_type.h"
#include "random.h"
#include "trace.h"
#include "unique_id.h"
#include "version.h"

namespace bitmap {

	static bool __bitmap_initialized = false;
	static std::recursive_mutex __bitmap_lock;
	static std::map<uid_t, component::bitmap_context> __bitmap_map;

	bool 
	operator==(
		const pixel_t &left,
		const pixel_t &right
		)
	{
		size_t i = 0;
		bool result = true;

		for(; i <= MAX_CHANNEL; ++i) {
			
			if(left.channel[i] != right.channel[i]) {
				result = false;
				break;
			}
		}

		return result;
	}

	bool 
	operator!=(
		const pixel_t &left,
		const pixel_t &right
		)
	{
		return !(left == right);
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

	void 
	cleanup(void)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::cleanup");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		__bitmap_map.clear();
		__bitmap_initialized = false;
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::cleanup");
	}

	bool 
	contains(
		const uid_t &id
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::contains");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		bool result;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		result = (__bitmap_map.find(id) != __bitmap_map.end());
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::contains, result. " 
			<< VALUE_AS_HEX(bool, result));

		return result;
	}

	std::string 
	convert_pixel_to_string(
		const pixel_t &px
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::convert_pixel_to_string");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		std::stringstream ss;

		ss << PIXEL_AS_STRING(px);
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::convert_pixel_to_string");

		return ss.str();
	}

	size_t 
	count(void)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::count");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		size_t result;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		result = __bitmap_map.size();
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::count, result. " 
			<< VALUE_AS_HEX(size_t, result));

		return result;
	}

	uid_t 
	create(
		uint32_t width,
		uint32_t height
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::create");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		uid_t result = NULL_UNIQUE_ID;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		component::bitmap_context context(width, height);
		__bitmap_map.insert(std::pair<uid_t, component::bitmap_context>(
			context.get_unique_id(), 
			context
			));
		result = context.get_unique_id();

		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::create, result. " 
			<< component::unique_id::convert_to_string(result));

		return result;
	}

	pixel_t 
	get_pixel(
		uint32_t x,
		uint32_t y,
		const uid_t &id
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::get_pixel");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		pixel_t px;
		std::map<uid_t, component::bitmap_context>::iterator bitmap_iter;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		bitmap_iter = __bitmap_map.find(id);

		if(bitmap_iter == __bitmap_map.end()) {
			THROW_BITMAP_EXCEPTION_MESSAGE(
				BITMAP_EXCEPTION_ID_NOT_FOUND,
				component::unique_id::convert_to_string(id)
				);
		}
		px = bitmap_iter->second.get_pixel(x, y);
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::get_pixel, result. " 
			<< PIXEL_AS_STRING(px));

		return px;
	}

	void 
	initialize(void)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::initialize");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		if(__bitmap_initialized) {
			cleanup();
		}
		component::random::initialize();
		component::unique_id::initialize();
		__bitmap_initialized = true;
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::initialize");
	}

	uid_t 
	read(
		const std::string &path
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::read");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		uid_t result = NULL_UNIQUE_ID;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		component::bitmap_context context(path);
		__bitmap_map.insert(std::pair<uid_t, component::bitmap_context>(
			context.get_unique_id(), 
			context
			));
		result = context.get_unique_id();

		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::read, result. " 
			<< component::unique_id::convert_to_string(result));

		return result;
	}

	void 
	remove(
		const uid_t &id
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::remove");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		std::map<uid_t, component::bitmap_context>::iterator bitmap_iter;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		bitmap_iter = __bitmap_map.find(id);

		if(bitmap_iter == __bitmap_map.end()) {
			THROW_BITMAP_EXCEPTION_MESSAGE(
				BITMAP_EXCEPTION_ID_NOT_FOUND,
				component::unique_id::convert_to_string(id)
				);
		}
		__bitmap_map.erase(bitmap_iter);

		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::remove");
	}

	void 
	remove_all(void)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::remove_all");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		__bitmap_map.clear();
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::remove_all");
	}

	void 
	set_pixel(
		uint32_t x,
		uint32_t y,
		const pixel_t &px,
		const uid_t &id
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::set_pixel");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		std::map<uid_t, component::bitmap_context>::iterator bitmap_iter;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		bitmap_iter = __bitmap_map.find(id);

		if(bitmap_iter == __bitmap_map.end()) {
			THROW_BITMAP_EXCEPTION_MESSAGE(
				BITMAP_EXCEPTION_ID_NOT_FOUND,
				component::unique_id::convert_to_string(id)
				);
		}
		bitmap_iter->second.set_pixel(x, y, px);
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::set_pixel");
	}

	std::string version(
		bool verbose
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::version");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		std::stringstream ss;

		if(verbose) {
			ss << VERBOSE_VERSION_STRING;
		} else {
			ss << VERSION_STRING;
		}
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::version");

		return ss.str();
	}

	void 
	write(
		const std::string &path,
		const uid_t &id
		)
	{
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+bitmap::write");

		SERIALIZE_CALL(std::recursive_mutex, __bitmap_lock);

		std::map<uid_t, component::bitmap_context>::iterator bitmap_iter;

		if(!__bitmap_initialized) {
			THROW_BITMAP_EXCEPTION(BITMAP_EXCEPTION_UNINITIALIZED);
		}
		bitmap_iter = __bitmap_map.find(id);

		if(bitmap_iter == __bitmap_map.end()) {
			THROW_BITMAP_EXCEPTION_MESSAGE(
				BITMAP_EXCEPTION_ID_NOT_FOUND,
				component::unique_id::convert_to_string(id)
				);
		}
		bitmap_iter->second.write(path);
		
		TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-bitmap::write");
	}
}