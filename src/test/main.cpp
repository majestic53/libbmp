/*
 * main.cpp
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

#include <iostream>
#include <string>
#include "bitmap.h"

#define IMAGE_HEIGHT (100)
#define IMAGE_WIDTH (100)
#define IMAGE_PATH "..\\..\\..\\test\\out.bmp"

int 
main(void)
{
	int i = 0, j, result = 0;
	bitmap::pixel_t px = {0xff, 0xff, 0xff};
	bitmap::uid_t bmp_id = NULL_UNIQUE_ID;

	try {

		// initialize bitmap manager
		bitmap::initialize();

		// create a new bitmap image
		bmp_id = bitmap::create(IMAGE_WIDTH, IMAGE_HEIGHT);

		for(; i < IMAGE_WIDTH; ++i) {
			
			for(j = 0; j < IMAGE_HEIGHT; ++j) {

				// fill the image with white pixels
				bitmap::set_pixel(j, i, px, bmp_id);
			}
		}

		// write bitmap image to file
		bitmap::write(IMAGE_PATH, bmp_id);

		bitmap::remove(bmp_id);

		// read bitmap image from file
		bmp_id = bitmap::read(IMAGE_PATH);
		px = bitmap::get_pixel(IMAGE_WIDTH / 2, IMAGE_HEIGHT / 2, bmp_id);
		std::cout << bitmap::convert_pixel_to_string(px) << std::endl;

		// cleanup bitmap manager
		bitmap::cleanup();
	} catch(std::runtime_error &exc) {
		std::cerr << "Exception: " << exc.what() << std::endl;
		result = 1;
	}
	std::cin.get();

	return result;
}