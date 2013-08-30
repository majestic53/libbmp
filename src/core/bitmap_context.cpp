/*
 * bitmap_context.cpp
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

#include <fstream>
#include "bitmap.h"
#include "bitmap_context.h"
#include "bitmap_context_type.h"
#include "trace.h"

namespace bitmap {

	namespace component {

		#define PIXEL_COORDINATE(_X_, _Y_, _W_) (((_Y_) * (_W_)) + (_X_))

		_bitmap_context::_bitmap_context(
			const std::string &path
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::_bitmap_context");

			read(path);

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::_bitmap_context");
		}

		_bitmap_context::_bitmap_context(
			uint32_t width,
			uint32_t height
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::_bitmap_context");

			create(width, height);

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::_bitmap_context");
		}

		_bitmap_context::_bitmap_context(
			const _bitmap_context &other
			) :
				unique_id::unique_id_class(other),
				_header(other._header),
				_image(other._image)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::_bitmap_context");
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::_bitmap_context");
		}

		_bitmap_context::~_bitmap_context(void)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::~_bitmap_context");
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::~_bitmap_context");
		}

		_bitmap_context &
		_bitmap_context::operator=(
			const _bitmap_context &other
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::operator=");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			if(this != &other) {
				unique_id::unique_id_class::operator=(other);
				_header = other._header;
				_image = other._image;
			}

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::operator=");

			return *this;
		}

		void 
		_bitmap_context::clear(void)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::clear");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			memset(&_header, 0, sizeof(bitmap_header));
			_image.clear();

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::clear");
		}

		void 
		_bitmap_context::create(
			uint32_t width,
			uint32_t height
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::create");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			uint32_t padding;

			if(!width
					|| !height) {
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_INVALID_PARAMETER,
					"width == 0 || height == 0"
					);
			}
			clear();
			padding = (width * sizeof(pixel)) % 4;
			_image.resize(width * height);
			_header.file_type = 'MB';
			_header.file_size = (uint32_t) (sizeof(bitmap_header) + (_image.size() * sizeof(pixel)) + (padding * height));
			_header.image_offset = sizeof(bitmap_header);
			_header.information.header_size = sizeof(bitmap_info);
			_header.information.image_width = width;
			_header.information.image_height = height;
			_header.information.color_planes = 1;
			_header.information.bits_per_pixel = 24;
			_header.information.compression_type = 0;
			_header.information.image_size = (uint32_t) (_image.size() * sizeof(pixel) + (padding * height));
			_header.information.resolution_x = 0;
			_header.information.resolution_y = 0;
			_header.information.color_count = 0;
			_header.information.important_color_count = 0;

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::create");
		}

		bitmap_header &
		_bitmap_context::get_header(void)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::get_header");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::get_header");

			return _header;
		}

		pixel &
		_bitmap_context::get_pixel(
			uint32_t x,
			uint32_t y
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::get_pixel");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			if(x >= (uint32_t) _header.information.image_width) {
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_COORD_OUT_OF_RANGE,
					"x > width"
					);
			}

			if(y >= (uint32_t) _header.information.image_height) {
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_COORD_OUT_OF_RANGE,
					"y > height"
					);
			}

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::get_pixel");

			return _image.at(PIXEL_COORDINATE(x, y, _header.information.image_width));
		}

		void 
		_bitmap_context::read(
			const std::string &path
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::read");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			pixel pix;
			uint8_t sink;
			size_t i = 0, j;
			uint32_t padding;
			std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);

			if(!file) {
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_FILE_NOT_FOUND,
					"path. " << path
					);
			}
			clear();
			file.read((char *) &_header, sizeof(bitmap_header));

			if(_header.image_offset != sizeof(bitmap_header)
					|| _header.information.header_size != sizeof(bitmap_info)
					|| _header.information.image_width < 0
					|| _header.information.image_width < 0
					|| _header.information.color_planes != 1
					|| _header.information.bits_per_pixel != 24
					|| _header.information.compression_type != 0
					|| _header.information.resolution_x != 0
					|| _header.information.resolution_y != 0
					|| _header.information.color_count != 0
					|| _header.information.important_color_count != 0
					)
			{
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_MALFORMED_FILE,
					"path. " << path
					);
			}
			padding = (_header.information.image_width * sizeof(pixel)) % 4;

			if(_header.file_size != sizeof(bitmap_header) + ((_header.information.image_width * _header.information.image_height) * sizeof(pixel))
						+ (padding * _header.information.image_height)
					|| _header.information.image_size != ((_header.information.image_width * _header.information.image_height) * sizeof(pixel))
						+ (padding * _header.information.image_height)
					)
			{
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_MALFORMED_FILE,
					"path. " << path
					);
			}
			_image.resize((_header.information.image_width * _header.information.image_height));

			for(; i < ((_header.information.image_size - (padding * _header.information.image_height)) / (MAX_CHANNEL + 1)); ++i) {

				if(i
						&& !(i % (size_t) _header.information.image_width)) {
					
					for(j = 0; j < padding; ++j) {
						file >> (uint8_t) sink;
					}
				}
				file >> pix.channel[CHANNEL_BLUE];
				file >> pix.channel[CHANNEL_GREEN];
				file >> pix.channel[CHANNEL_RED];
				_image.at(i) = pix;	
			}
			file.close();

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::read");
		}

		void 
		_bitmap_context::set_pixel(
			const pixel &value,
			uint32_t x,
			uint32_t y
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::set_pixel");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			if(x >= (uint32_t) _header.information.image_width) {
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_COORD_OUT_OF_RANGE,
					"x > width"
					);
			}

			if(y >= (uint32_t) _header.information.image_height) {
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_COORD_OUT_OF_RANGE,
					"y > height"
					);
			}
			_image.at(PIXEL_COORDINATE(x, y, _header.information.image_width)) = value;

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::set_pixel");
		}

		std::string 
		_bitmap_context::to_string(
			bool verbose
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::to_string");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			uint32_t i;
			uint8_t *data;
			std::stringstream ss;

			ss << "bitmap_" << unique_id::unique_id_class::to_string(false) 
				<< " [ width: " << _header.information.image_width 
				<< ", height: " << _header.information.image_height
				<< ", size: " << _header.file_size << " bytes ]";

			if(verbose) {
				data = (uint8_t *) &_header;

				for(i = 0; i < sizeof(bitmap_header); ++i) {
					
					if(!(i % BLOCK_SIZE)) {
						ss << std::endl << "0x" << VALUE_AS_HEX(uint32_t, i) << " |";
					}
					ss << " " << VALUE_AS_HEX(uint8_t, data[i]);
				}
			}

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::to_string");

			return ss.str();
		}

		void 
		_bitmap_context::write(
			const std::string &path
			)
		{
			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+_bitmap_context::write");

			SERIALIZE_CALL(std::recursive_mutex, _bitmap_context_lock);

			size_t i, j;
			uint8_t *data;
			uint32_t padding;
			std::vector<pixel>::iterator pixel_iter;
			std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

			if(!file) {
				THROW_BITMAP_CONTEXT_EXCEPTION_MESSAGE(
					BITMAP_CONTEXT_EXCEPTION_FILE_IO_FAILED,
					"path. " << path
					);
			}
			data = (uint8_t *) &_header;
			padding = (_header.information.image_width * sizeof(pixel)) % 4;

			for(i = 0; i < sizeof(bitmap_header); ++i) {
				file << data[i];
			}

			for(i = 0, pixel_iter = _image.begin(); pixel_iter != _image.end(); ++i, ++pixel_iter) {

				if(i
						&& !(i % _header.information.image_width)) {
					
					for(j = 0; j < padding; ++j) {
						file << (uint8_t) 0;
					}
				}
				file << pixel_iter->channel[CHANNEL_BLUE];
				file << pixel_iter->channel[CHANNEL_GREEN];
				file << pixel_iter->channel[CHANNEL_RED];
			}

			for(j = 0; j < padding; ++j) {
				file << (uint8_t) 0;
			}
			file.close();

			TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-_bitmap_context::write");
		}
	}
}