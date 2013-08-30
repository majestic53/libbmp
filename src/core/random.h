/*
 * random.h
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

#ifndef RANDOM_H_
#define RANDOM_H_

#include <cstdint>
#include <string>

namespace bitmap {

	#define GENERATE_RANDOM_SEED (0)
	#define DEFAULT_NORMAL_RANDOM_CENTER (0.0)
	#define DEFAULT_NORMAL_RANDOM_SPREAD (1.0)
	#define DEFAULT_NORMAL_RANDOM_CROP_MAX (2.0)
	#define DEFAULT_NORMAL_RANDOM_CROP_MIN (-2.0)
	#define DEFAULT_UNIFORM_RANDOM_MAX (1.0)
	#define DEFAULT_UNIFORM_RANDOM_MIN (0.0)

	namespace component {

		namespace random {

			void initialize(
				uint32_t seed = GENERATE_RANDOM_SEED
				);

			double generate_normalized(
				double center = DEFAULT_NORMAL_RANDOM_CENTER,
				double spread = DEFAULT_NORMAL_RANDOM_SPREAD,
				double crop_min = -DEFAULT_NORMAL_RANDOM_CROP_MIN,
				double crop_max = DEFAULT_NORMAL_RANDOM_CROP_MAX
				);

			double generate_uniform(
				double min = DEFAULT_UNIFORM_RANDOM_MIN,
				double max = DEFAULT_UNIFORM_RANDOM_MAX
				);

			std::string to_string(
				bool verbose
				);
		}
	}
}

#endif