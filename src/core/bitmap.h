/*
 * bitmap.h
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

#ifndef BITMAP_H_
#define BITMAP_H_

#include <string>
#include "unique_id.h"

namespace bitmap {

	enum {
		CHANNEL_BLUE = 0,
		CHANNEL_GREEN,
		CHANNEL_RED,
	};

	#define MAX_CHANNEL CHANNEL_RED

	#define NULL_PIXEL {0}

#pragma pack(push)
#pragma pack(1)
	typedef struct _pixel {
		uint8_t channel[MAX_CHANNEL + 1];
	} pixel, *pixel_ptr;
#pragma pack(pop)

	bool operator==(
		const pixel &left,
		const pixel &right
		);

	bool operator!=(
		const pixel &left,
		const pixel &right
		);

	void cleanup(void);

	bool contains(
		const uid_t &id
		);

	std::string convert_pixel_to_string(
		const pixel &px
		);

	size_t count(void);

	uid_t create(
		uint32_t width,
		uint32_t height
		);

	pixel get_pixel(
		uint32_t x,
		uint32_t y,
		const uid_t &id
		);

	void initialize(void);

	uid_t read(
		const std::string &path
		);

	void remove(
		const uid_t &id
		);

	void remove_all(void);

	void set_pixel(
		uint32_t x,
		uint32_t y,
		const pixel &px,
		const uid_t &id
		);

	std::string version(
		bool verbose
		);

	void write(
		const std::string &path,
		const uid_t &id
		);
}

#endif