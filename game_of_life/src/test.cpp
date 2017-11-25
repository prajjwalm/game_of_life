/*
 * test.cpp
 *
 *  Created on: 10-Nov-2017
 *      Author: Prajjwal Mishra
 */

#include "test.hpp"
#include <iostream>

using namespace std;
using namespace test;

void test::assert (bool a){
	if (!a){
		throw 0;
	}
}

bool test::matrix_eq(int A[size][size], int B[size][size]){
	bool x=true;
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			if(A[i][j]!=B[i][j]){
				x=false;
			}
		}
	}
	return x;
}

bool test::matrix_eq(bool A[size][size], bool B[size][size]){
	bool x=true;
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			if(A[i][j]!=B[i][j]){
				x=false;
			}
		}
	}
	return x;
}
void cell_test::setup(){
	c03=new backend::cell(backend::dead,3);
	c12=new backend::cell(backend::live,2);
	c15=new backend::cell(backend::live,5);
	c02=new backend::cell(backend::dead,2);
	c11=new backend::cell(backend::live,1);
}

void cell_test::teardown(){
	delete c03;
	delete c12;
	delete c15;
	delete c02;
	delete c11;
}

void cell_test::test_setstate(){
//	int e;
	setup();
	try{
		assert(c03->setstate()==backend::live);
		assert(c12->setstate()==backend::live);
		assert(c15->setstate()==backend::dead);
		assert(c02->setstate()==backend::dead);
		assert(c11->setstate()==backend::dead);
		cout<<"setstate function passed all tests\n";
	}
	catch(int e){
		cerr<<"setstate function failed some test(s)!!\n";
	}
	teardown();
}

void univ_test::setup(int univ_no){

	file::seed S;
	string name;
	ptr = new utest_mem;
	switch (univ_no){
	case 0:
		name="glider";
		ptr->reqd_gen=4;
		break;
	case 1:
		name="blinker";
		ptr->reqd_gen=1;
		break;
	case 2:
		name="lwss";
		ptr->reqd_gen=4;
		break;
	case 3:
		name="edge";
		ptr->reqd_gen=2;
		break;
	}
	S=file::getseed(name,"TESTSEED.TXT");
	ptr->init.initialize(S.matrix);
	file::GetN(ptr->matrix, name);
	S=file::getseed(name, "TESTRES.TXT");
	for (int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			(ptr->final)[i][j]=S.matrix[i][j];
		}
	}
}

void univ_test::teardown(){
	delete ptr;
}

void univ_test::test_update_n(){
	int i;
	int nmat[size][size];
	try {
		for(i=0;i<4;i++){
			setup(i);
			ptr->init.next_gen();
			for (int j=0;j<size;j++){
				for(int k=0;k<size;k++){
					nmat[j][k]=ptr->init.getnxy(j, k);
				}
			}
			assert(matrix_eq(nmat,ptr->matrix));
			teardown();
		}
		cout<<"Update_n passed all tests\n";
	}
	catch (int e){
		teardown();
		cerr<<"Update_n failed some test(s)!!\n";
	}
}

void univ_test::test_next_gen(){
	bool mat[size][size];
	try {
		for (int i=0; i<4;i++){
			setup(i);
			for (int j = 0; j < ptr-> reqd_gen ; j++){
				ptr->init.next_gen();
			}
			for (int x=0 ; x<size; x++){
				for (int y=0;y<size; y++){
					mat[x][y]=ptr->init.getstatexy(x, y);
				}
			}
			assert(matrix_eq(ptr->final, mat));
			teardown();
		}
		cout<<"next_gen passed all tests\n";
	}
	catch (int e){
		teardown();
		cerr<<"next_gen failed some test(s)\n";
	}
}
