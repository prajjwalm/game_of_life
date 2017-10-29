/*
 * control.hpp
 *
 *  Created on: 16-Oct-2017
 *      Author: Prajjwal Mishra
 */

#ifndef SRC_CONTROL_HPP_
#define SRC_CONTROL_HPP_

#include "backend.hpp"
#include "renderer.hpp"
#include "logic.hpp"
#include <fstream>

namespace file {

	struct seed{
		bool matrix[size][size];
		std::string name;
	};

	void storeseed(seed);
	seed getseed(std::string);
	void storepattern(bool [][size], std::string, int nrows, int ncols);
	void getpattern(bool [][size], std::string, int nrows, int ncols);
}

namespace control {

	class seed_init {
		file::seed S1;
	public:
		bool setmatrix(bool [size][size]);
	};


	class controller  {
		renderer::speed pace;
		int tick;							//tick duration, in milliseconds, depends only on pace
		backend::universe life;
	public:
		controller ();
		void update(bool [size][size]);		// assignment of life::matrix during initialization or modification
		void setpace(renderer::speed);				// sets speed and tick, may use enum speed as its argument also
		bool rungame(bool [size][size],int&);			//returns whether paused
														//takes the matrix and gen to pass the paused values
														//of 'life' and life.gen into it
	};
}




#endif /* SRC_CONTROL_HPP_ */
