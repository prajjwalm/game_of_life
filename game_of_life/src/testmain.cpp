/*
 * testmain.cpp
 *
 *  Created on: 23-Nov-2017
 *      Author: Prajjwal Mishra
 */

#include "control.hpp"
#include "test.hpp"

/************************** testing code


int main(){

	test::cell_test ct1;
	test::univ_test ut1;

	ct1.test_setstate();
	ut1.test_next_gen();
	ut1.test_update_n();

	return 0;
}

/********************************************/
