/*
 * version.h
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

#ifndef VERSION_H_
#define VERSION_H_

namespace sola {

	#define VERSION_DELIM "."
	#define VERSION_MAJOR 1
	#define VERSION_MINOR 0
	#define VERSION_YEARWEEK 1335
	#define VERSION_REVISION 2

	#define VERSION_STRING\
		VERSION_MAJOR << VERSION_DELIM << VERSION_MINOR << VERSION_DELIM << VERSION_YEARWEEK
	#define VERBOSE_VERSION_STRING\
		VERSION_MAJOR << VERSION_DELIM << VERSION_MINOR << VERSION_DELIM << VERSION_YEARWEEK\
			<< " (rev. " << VERSION_REVISION << ", " << __DATE__ << ", " << __TIME__ << ")"
}

#endif