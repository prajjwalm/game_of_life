/*
 * logic.hpp
 *
 *  Created on: 27-Oct-2017
 *      Author: Prajjwal Mishra
 */

#ifndef SRC_LOGIC_HPP_
#define SRC_LOGIC_HPP_

#include "renderer.hpp"


namespace logic{

	using renderer::height;
	using renderer::margin;
	using renderer::side;
	using renderer::width;
	using renderer::color_bkgd;
	using renderer::color_dead;
	using renderer::color_live;


	const int gun_rows =11;
	const int gun_cols =38;
	const int gli_rows = 5;
	const int gli_cols = 5;
	const int eat_rows = 6;
	const int eat_cols = 6;

	class intro_screen{
		bool gun[gun_rows][gun_cols];
		bool glider[gli_rows][gli_cols];
		bool eater[eat_rows][eat_cols];
		bool _gun[gun_rows][size];
		bool _glider[gli_rows][size];
		bool _eater[eat_rows][size];
	public:
		intro_screen();						//constructor only
		std::string intro();						// display the introductory screen...
	};
}


#endif /* SRC_LOGIC_HPP_ */
