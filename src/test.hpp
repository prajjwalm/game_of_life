/*
 * test.hpp
 *
 *  Created on: 10-Nov-2017
 *      Author: Prajjwal Mishra
 */

#ifndef SRC_TEST_HPP_
#define SRC_TEST_HPP_

#include "control.hpp"

namespace test{

	void assert(bool);				// tests the boolean value passed to it, expected to be true
									// if true, nothing happens else throws an exception: 0 (int)

	bool matrix_eq(int  [][size],int  [][size]);
	bool matrix_eq(bool [][size],bool [][size]);

	class cell_test{
	private:
		backend::cell *c03, *c12, *c15, *c02, *c11;		// ptr names are given according to the cells they would point to
														// as c<curr><n>;
	public:
		void setup();
		void teardown();
		void test_setstate();
	};

	struct utest_mem {
		backend::universe init;
		bool final[size][size];			//0 : glider, 1 : blinker, 2 : lwss
		int reqd_gen;					//gen no at which the initial matrix ideally transforms into the final {4,1,4}
		int matrix [size][size];
	};

	class univ_test{
	private:
		utest_mem *ptr;
	public:
		void setup (int univ_no);
		void teardown ();
		void test_update_n();
		void test_next_gen();
	};
}
#endif /* SRC_TEST_HPP_ */
