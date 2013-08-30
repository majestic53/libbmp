LIBBMP
========

LIBBMP is a C++ library that makes it easy to read, write and create
bitmap files on the fly.

###Version 1.0
Updated: 8/29/2013
Initial code release

######Bug fixes:
None Yet

Table of Contents
========

1. [Architecture](https://github.com/majestic53/libbmp#architecture)
	* [Types](https://github.com/majestic53/libbmp#types)
	* [Library Interface](https://github.com/majestic53/libbmp#library-interface)
2. [Usage](https://github.com/majestic53/libbmp#usage)
	* [Usage Example](https://github.com/majestic53/libbmp#usage-example)
3. [License](https://github.com/majestic53/libbmp#license)

Architecture
========

LIBBMP is made accessible through a C-style interface, where the functions are globally accessible.
Upon bitmap image creation or read, the caller is supplied with a unique identifier, which functions
as a handle to the bitmap image.

###Types

* ```uid_t```: 128-bit unique identifier used as a handle to a bitmap image
* ```pixel```: A 24-bit structure of containing the RGB saturation values

###Library Interface

####Library Functions
* ```initialize()```: Initialize the bitmap manager (This function should be called upon the application entry)
* ```cleanup```: Cleanup the bitmap manager (This function should be called upon the application exit)

####File Management
* ```contains(id)```: Check for the existance of a bitmap image at a given unique id
* ```count()```: Retrieve a count of the bitmap images held by the bitmap manager
* ```remove(id)```: Remove a bitmap image from the bitmap manager
* ```remove_all()```: Remove all bitmap images from the bitmap manager

####File Manipulation
* ```create(width, height)```: Create a new bitmap image with a given width and height in pixels
* ```read(path)```: Read a bitmap image from file at a given path
* ```write(path, id)```: Write a bitmap image to file at a given path

####Image Manipulation
* ```get_pixel(x, y, id)```: Retrieve a pixel within a bitmap image at a given unique id
* ```set_pixel(x, y, pixel, id)```: Set a pixel within a bitmap image at given unique id

####Misc Functions
* ```convert_pixel_to_string(pixel)```: Convert a pixel into a human-readable format
* ```version()```: Retrieve LIBBMP version string

Usage
========

LIBBMP can easily be imported into any C++ project. Simple compile the 
core project, and copy the resulting .lib file and associated "bitmap.h"
header file.

###Usage Example

```c++
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
	bitmap::pixel px = {0xff, 0xff, 0xff};
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
```

License
======

Copyright(C) 2013 David Jolly <majestic53@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
