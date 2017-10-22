/*
 * backend.hpp
 *
 *  Created on: 16-Oct-2017
 *      Author: Prajjwal Mishra
 */

#ifndef SRC_BACKEND_HPP_
#define SRC_BACKEND_HPP_

#include "globalvar.hpp"

namespace backend {

	enum state {dead, live};

	class cell {
		state curr, prev;
		int n;						// no of live neighbours
	public:
		cell();
		state getstate();			// gives current state, meant for universe::display
		void set_prev();			// assigns curr to prev, meant for universe::next_gen
		int set_n(int);				// assigning of n
		state init_state(state);	// forced assigning of state, meant for universe::initialize
		state setstate();			// sets the current state based on prev, n and returns it,
									// meant for universe::next_gen
	};

	class universe {
		int gen;						//generation  number, increments at call of next_gen
		void update_n();				// sets n for each cell, based on current states, for next_gen
		cell matrix[size][size];
	public:
		universe();
		int getgen() {return gen;}
		state getstatexy(int x, int y);
		void initialize(bool [size][size]);
		void next_gen();					//based on n and the previous states of the cells, generates the
											//current state of all the cells of the matrix, increments gen
		};

}

#endif /* SRC_BACKEND_HPP_ */
