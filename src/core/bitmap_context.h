/*
 * bitmap_context_type.h
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

#ifndef BITMAP_CONTEXT_H_
#define BITMAP_CONTEXT_H_

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <vector>
#include "unique_id_class.h"

namespace bitmap {

	#define PIXEL_AS_STRING(_P_)\
		"{" << VALUE_AS_HEX(uint8_t, _P_.channel[CHANNEL_RED]) << ", "\
			<< VALUE_AS_HEX(uint8_t, _P_.channel[CHANNEL_GREEN]) << ", "\
			<< VALUE_AS_HEX(uint8_t, _P_.channel[CHANNEL_BLUE]) << "}"

#pragma pack(push)
#pragma pack(2)
	typedef struct _bitmap_info {
		uint32_t header_size;
		int32_t image_width;
		int32_t image_height;
		uint16_t color_planes;
		uint16_t bits_per_pixel;
		uint32_t compression_type;
		uint32_t image_size;
		int32_t resolution_x;
		int32_t resolution_y;
		uint32_t color_count;
		uint32_t important_color_count;
	} bitmap_info, *bitmap_info_ptr;

	typedef struct _bitmap_header {
		uint16_t file_type;
		uint32_t file_size;
		uint16_t reserved0;
		uint16_t reserved1;
		uint32_t image_offset;
		bitmap_info information;
	} bitmap_header, *bitmap_header_ptr;
#pragma pack(pop)

	namespace component {

		typedef class _bitmap_context :
				public unique_id::unique_id_class {

			public:

				_bitmap_context(
					const std::string &path
					);

				_bitmap_context(
					uint32_t width,
					uint32_t height
					);

				_bitmap_context(
					const _bitmap_context &other
					);

				virtual ~_bitmap_context(void);

				_bitmap_context &operator=(
					const _bitmap_context &other
					);

				void clear(void);

				void create(
					uint32_t width,
					uint32_t height
					);

				bitmap_header &get_header(void);

				pixel &get_pixel(
					uint32_t x,
					uint32_t y
					);

				void read(
					const std::string &path
					);

				void set_pixel(
					const pixel &value,
					uint32_t x,
					uint32_t y
					);

				virtual std::string to_string(
					bool verbose
					);

				void write(
					const std::string &path
					);

			protected:

				bitmap_header _header;

				std::vector<pixel> _image;

			private:

				std::recursive_mutex _bitmap_context_lock;

		} bitmap_context, *bitmap_context_ptr;

	}
}

#endif