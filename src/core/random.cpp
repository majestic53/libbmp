/*
 * random.cpp
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

#include <ctime>
#include <random>
#include "random.h"
#include "random_type.h"
#include "trace.h"

namespace bitmap {

	namespace component {

		namespace random {

			static std::mt19937 __random_generator;
			static bool __random_initialized = false;
			static std::recursive_mutex __random_lock;

			void 
			initialize(
				uint32_t seed
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::random::initialize");

				SERIALIZE_CALL(std::recursive_mutex, __random_lock);

				std::srand((uint32_t) std::time(NULL));

				if(seed == GENERATE_RANDOM_SEED) {
					__random_generator.seed(rand());
				} else {
					__random_generator.seed(seed);
				}
				__random_initialized = true;

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::random::initialize");
			}

			double 
			generate_normalized(
				double mean,
				double sigma,
				double crop_min,
				double crop_max
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::random::generate_normalized");

				SERIALIZE_CALL(std::recursive_mutex, __random_lock);

				double result;
				std::normal_distribution<double> normal_dist(mean, sigma);

				if(!__random_initialized) {
					THROW_RANDOM_EXCEPTION(RANDOM_EXCEPTION_UNINITIALIZED);
				}

				if(sigma < 0.0) {
					THROW_RANDOM_EXCEPTION_MESSAGE(
						RANDOM_EXCEPTION_INVALID_PARAMETER,
						"spread < 0.0"
						);
				}

				if(crop_min > crop_max) {
					THROW_RANDOM_EXCEPTION_MESSAGE(
						RANDOM_EXCEPTION_INVALID_PARAMETER,
						"crop_min > crop_max"
						);
				}
				result = normal_dist(__random_generator);

				if(result < crop_min) {
					result = crop_min;
				} else if(result > crop_max) {
					result = crop_max;
				}

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::random::generate_normalized, result. " << result);

				return result;
			}

			double 
			generate_uniform(
				double min,
				double max
				)
			{
				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "+sola::component::random::generate_uniform");

				SERIALIZE_CALL(std::recursive_mutex, __random_lock);

				double result;
				std::uniform_real_distribution<double> uniform_dist;

				if(!__random_initialized) {
					THROW_RANDOM_EXCEPTION(RANDOM_EXCEPTION_UNINITIALIZED);
				}

				if(min > max) {
					THROW_RANDOM_EXCEPTION_MESSAGE(
						RANDOM_EXCEPTION_INVALID_PARAMETER,
						"min > max"
						);
				}
				result = ((max - min) * uniform_dist(__random_generator)) + min;

				TRACE_MESSAGE(TRACE_LEVEL_VERBOSE, "-sola::component::random::generate_uniform, result. " << result);

				return result;
			}
		}
	}
}