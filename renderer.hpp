/*
 * renderer.hpp
 *
 *  Created on: 16-Oct-2017
 *      Author: Prajjwal Mishra
 */

#ifndef SRC_RENDERER_HPP_
#define SRC_RENDERER_HPP_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sfml/graphics.hpp>
#include "globalvar.hpp"


namespace renderer {

	enum command { STOP, RESUME, MODIFY, QUIT};
	enum speed { vslow, slow, med, fast, vfast};

	const sf::Color color_live=sf::Color(0,255,0);
	const sf::Color color_dead=sf::Color(0,0,0);
	const sf::Color color_bkgd=sf::Color(16,16,16);

	const int N_options=5;
	const int side=8;
	const int height=900;
	const int width=1200;
	const int margin=20;

	std::string init_screen();
	void user_input(bool [size][size],bool);				//boolean value checks if the game has just started
	speed pause_screen(bool [size][size], int, command&);
	speed lpause_screen(bool [size][size], int, command&);
};


#endif /* SRC_RENDERER_HPP_ */
