/*
 * logic.cpp
 *
 *  Created on: 28-Oct-2017
 *      Author: Prajjwal Mishra
 */

#include "globalvar.hpp"
#include "logic.hpp"

using namespace std;

string logic::intro_screen::intro(){

	int selected_option=-1;

	//sfml objects...
	sf::RenderWindow rw(sf::VideoMode(width,height),"GAME OF LIFE: LOGIC GATES EXTENSION");
	sf::Font arial;
	sf::Vector2f point;
	arial.loadFromFile("arialbd.ttf");

	sf::RectangleShape cell(sf::Vector2f(side,side));
	cell.setOutlineColor(sf::Color(128,128,128));
	cell.setOutlineThickness(1);

	const int gun_x=width/2+50;
	const int gun_y=100;
	const int glider_x=width/2+50;
	const int glider_y=330;
	const int eater_x=width/2+50;
	const int eater_y=500;

	sf::Texture proceed0;
	if (!proceed0.loadFromFile("Proceed_button.png")){
		cerr<<"Unable to load proceed button terminated\n";
		exit(1);
	}
	proceed0.setSmooth(true);

	sf::Texture proceed1;
	if (!proceed1.loadFromFile("Proceed_pressed.png")){
		cerr<<"Unable to load proceed pressed terminated\n";
		exit(1);
	}
	proceed1.setSmooth(true);

	sf::Texture radio0;
	if (!radio0.loadFromFile("unselected_radio.png")){
		cerr<<"Unable to load unselected radio. terminated\n";
		exit(1);
	}
	radio0.setSmooth(true);

	sf::Texture radio1;
	if (!radio1.loadFromFile("selected_radio.png")){
		cerr<<"Unable to load selected radio. terminated\n";
		exit(1);
	}
	radio1.setSmooth(true);

	const int N_gates=3;
	sf::Sprite radioS[N_gates];
	sf::Sprite proceed;
	sf::FloatRect proceedbox, radioSbox[N_gates];

	sf::Text op[N_gates];
	string msg[N_gates]={"NOT", "OR", "AND"};

	const int ntxt=13;
	const int ngrid=3;
	sf::Text l[ntxt];
	sf::Text t[ngrid];


	l[0] .setString("USING THE GAME OF LIFE TO SIMULATE LOGIC GATES...");
	l[1] .setString("We use the following implementations...");
	l[2] .setString("Input and output electrical pulses >> Gliders.");
	l[3] .setString("Wires >> Trajectories of glider movements.");
	l[4] .setString("Processing devices >> Collision of gliders.");
	l[5] .setString("(guns are used as sources of gliders)");
	l[6] .setString("The 'gates'  presented would be closed by default");
	l[7] .setString("to open them  would require  destroying the eater");
	l[8] .setString("by activating the  highlighted cell, this act cannot");
	l[9] .setString("be undone and so the 0/0 state must be reopened");
	l[10].setString("(please  ensure that there is no glider that is  just");
	l[11].setString("about to collide with the eater while deativating it)");
	l[12].setString("Choose your gate...");

	for (int i=0;i<ntxt;i++){
		t[i].setFont(arial);
		t[i].setFillColor(sf::Color::Red);
		t[i].setCharacterSize(18);
	}

	for (int i=0;i<ntxt;i++){
		l[i].setFont(arial);
		l[i].setFillColor(sf::Color(64,64,255));
		l[i].setCharacterSize(24);
		l[i].setPosition(margin, margin+i*50+(i/6)*50);
	}
	t[0].setPosition(gun_x, gun_y-30);
	t[1].setPosition(glider_x, glider_y-30);
	t[2].setPosition(eater_x, eater_y-30);

	t[0].setString("Glider Gun : Pulse generator");
	t[1].setString("Glider : Pulse");
	t[2].setString("Eater : Off switch");


	rw.clear(color_bkgd);

	//draw the option buttons...
	for (int i=0; i<N_gates; i++){
		op[i].setFont(arial);
		op[i].setFillColor(sf::Color(64,64,255));
		op[i].setCharacterSize(24);
		radioS[i].setTexture(radio0);
		op[i].setString(msg[i]);
		radioS[i].setPosition(margin*2, -(3+i*2)*margin+height);
		radioS[i].setScale(0.3, 0.3);							// sets size of buttons very close to (but <) 10 pixels
		op[i].setPosition(margin*4, -(3+i*2)*margin-6+height);			// emperically set
		if (!msg[i].empty()){
			radioSbox[i]=radioS[i].getGlobalBounds();
			rw.draw(radioS[i]);
			rw.draw(op[i]);
		}
	}

	//draw the proceed option...
	proceed.setTexture(proceed0);
	proceed.setScale(0.5,0.5);
	proceed.setPosition(width-margin-200, height-100);			// emperically set
	proceedbox=proceed.getGlobalBounds();
	rw.draw(proceed);
	for (int i=0;i<ntxt;i++){
		rw.draw(l[i]);
	}
	for (int i=0;i<ngrid;i++){
		rw.draw(t[i]);
	}
	for (int i=0;i<gun_rows;i++){
		for (int j=0; j<gun_cols;j++){
			cell.setPosition(gun_x+j*side,gun_y+i*side);
			if (gun[i][j])
				cell.setFillColor(renderer::color_live);
			else
				cell.setFillColor(renderer::color_dead);
			rw.draw(cell);
		}
	}
	for (int i=0;i<gli_rows;i++){
		for (int j=0; j<gli_cols;j++){
			cell.setPosition(glider_x+j*side,glider_y+i*side);
			if (glider[i][j])
				cell.setFillColor(renderer::color_live);
			else
				cell.setFillColor(renderer::color_dead);
			rw.draw(cell);
		}
	}
	for (int i=0;i<eat_rows;i++){
			for (int j=0; j<eat_cols;j++){
				cell.setPosition(eater_x+j*side,eater_y+i*side);
				if (eater[i][j] && (i!=1 || j!=3))
					cell.setFillColor(renderer::color_live);
				else if ((i!=1 || j!=3))
					cell.setFillColor(renderer::color_dead);
				else
					cell.setFillColor(sf::Color::Red);
				rw.draw(cell);
			}
		}
	rw.display();

	while (rw.isOpen()){
		sf::Event event;

		//while there are pending events
		while (rw.pollEvent(event)){

			// check the type of the event
			switch (event.type) {
			case sf::Event::Closed:
				rw.close();
				exit(0);
				break;
			case sf::Event::MouseButtonPressed:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				for (int i=0;i<N_gates;i++){
					if (radioSbox[i].contains(point)){
						rw.clear(color_bkgd);
						radioS[i].setTexture(radio1);
						for (int i=0;i<ntxt;i++){
								rw.draw(l[i]);
							}
						for (int i=0;i<ngrid;i++){
								rw.draw(t[i]);
							}
						for (int i=0;i<gun_rows;i++){
							for (int j=0; j<gun_cols;j++){
								cell.setPosition(gun_x+j*side,gun_y+i*side);
								if (gun[i][j])
									cell.setFillColor(renderer::color_live);
								else
									cell.setFillColor(renderer::color_dead);
								rw.draw(cell);
							}
						}
						for (int i=0;i<gli_rows;i++){
							for (int j=0; j<gli_cols;j++){
								cell.setPosition(glider_x+j*side,glider_y+i*side);
								if (glider[i][j])
									cell.setFillColor(renderer::color_live);
								else
									cell.setFillColor(renderer::color_dead);
								rw.draw(cell);
							}
						}
						for (int i=0;i<eat_rows;i++){
								for (int j=0; j<eat_cols;j++){
									cell.setPosition(eater_x+j*side,eater_y+i*side);
									if (eater[i][j] && (i!=1 || j!=3))
										cell.setFillColor(renderer::color_live);
									else if ((i!=1 || j!=3))
										cell.setFillColor(renderer::color_dead);
									else
										cell.setFillColor(sf::Color::Red);
									rw.draw(cell);
								}
							}
						for (int j=0;j<N_gates;j++){
							if (j!=i)
								radioS[j].setTexture(radio0);
							if (!msg[j].empty()){
								rw.draw(radioS[j]);
								rw.draw(op[j]);
							}
						}
						rw.draw(proceed);
						rw.display();
						selected_option=i;
					}

				}
				if (proceedbox.contains(point)){
					proceed.setTexture(proceed1);
					proceed.setPosition(width-margin-195, height-95);			// emperically set
					rw.clear(color_bkgd);
					rw.draw(proceed);
					for (int j=0;j<N_gates;j++){
						if (!msg[j].empty()){
							rw.draw(radioS[j]);
							rw.draw(op[j]);
						}
					}

					for (int i=0;i<ntxt;i++){
							rw.draw(l[i]);
						}
					for (int i=0;i<ngrid;i++){
							rw.draw(t[i]);
						}
					for (int i=0;i<gun_rows;i++){
						for (int j=0; j<gun_cols;j++){
							cell.setPosition(gun_x+j*side,gun_y+i*side);
							if (gun[i][j])
								cell.setFillColor(renderer::color_live);
							else
								cell.setFillColor(renderer::color_dead);
							rw.draw(cell);
						}
					}
					for (int i=0;i<gli_rows;i++){
						for (int j=0; j<gli_cols;j++){
							cell.setPosition(glider_x+j*side,glider_y+i*side);
							if (glider[i][j])
								cell.setFillColor(renderer::color_live);
							else
								cell.setFillColor(renderer::color_dead);
							rw.draw(cell);
						}
					}
					for (int i=0;i<eat_rows;i++){
							for (int j=0; j<eat_cols;j++){
								cell.setPosition(eater_x+j*side,eater_y+i*side);
								if (eater[i][j] && (i!=1 || j!=3))
									cell.setFillColor(renderer::color_live);
								else if ((i!=1 || j!=3))
									cell.setFillColor(renderer::color_dead);
								else
									cell.setFillColor(sf::Color::Red);
								rw.draw(cell);
							}
						}
					rw.display();
				}
				break;
				// proceed is called only if the mouse is released there...
			case sf::Event::MouseButtonReleased:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				proceed.setTexture(proceed0);
				proceed.setPosition(width-margin-200, height-100);
				if (proceedbox.contains(point)){				//proceed clicked...
					if (selected_option>=0){						//with a selected option
						rw.close();
						return msg[selected_option];
					}
					else {
						rw.clear(color_bkgd);
						rw.draw(proceed);
						for (int j=0;j<N_gates;j++){
							if (!msg[j].empty()){
								rw.draw(radioS[j]);
								rw.draw(op[j]);
							}
						}
						for (int i=0;i<ntxt;i++){
							rw.draw(l[i]);
						}
						for (int i=0;i<ngrid;i++){
							rw.draw(t[i]);
						}
						for (int i=0;i<gun_rows;i++){
							for (int j=0; j<gun_cols;j++){
								cell.setPosition(gun_x+j*side,gun_y+i*side);
								if (gun[i][j])
									cell.setFillColor(renderer::color_live);
								else
									cell.setFillColor(renderer::color_dead);
								rw.draw(cell);
							}
						}
						for (int i=0;i<gli_rows;i++){
							for (int j=0; j<gli_cols;j++){
								cell.setPosition(glider_x+j*side,glider_y+i*side);
								if (glider[i][j])
									cell.setFillColor(renderer::color_live);
								else
									cell.setFillColor(renderer::color_dead);
								rw.draw(cell);
							}
						}
						for (int i=0;i<eat_rows;i++){
							for (int j=0; j<eat_cols;j++){
								cell.setPosition(eater_x+j*side,eater_y+i*side);
								if (eater[i][j] && (i!=1 || j!=3))
									cell.setFillColor(renderer::color_live);
								else if ((i!=1 || j!=3))
									cell.setFillColor(renderer::color_dead);
								else
									cell.setFillColor(sf::Color::Red);
								rw.draw(cell);
							}
						}
						rw.display();
					}
				}
				else {
						rw.clear(color_bkgd);
						rw.draw(proceed);
						for (int j=0;j<N_gates;j++){
							if (!msg[j].empty()){
								rw.draw(radioS[j]);
								rw.draw(op[j]);
							}
						}
						for (int i=0;i<ntxt;i++){
							rw.draw(l[i]);
						}
						for (int i=0;i<ngrid;i++){
							rw.draw(t[i]);
						}
						for (int i=0;i<gun_rows;i++){
							for (int j=0; j<gun_cols;j++){
								cell.setPosition(gun_x+j*side,gun_y+i*side);
								if (gun[i][j])
									cell.setFillColor(renderer::color_live);
								else
									cell.setFillColor(renderer::color_dead);
								rw.draw(cell);
							}
						}
						for (int i=0;i<gli_rows;i++){
							for (int j=0; j<gli_cols;j++){
								cell.setPosition(glider_x+j*side,glider_y+i*side);
								if (glider[i][j])
									cell.setFillColor(renderer::color_live);
								else
									cell.setFillColor(renderer::color_dead);
								rw.draw(cell);
							}
						}
						for (int i=0;i<eat_rows;i++){
							for (int j=0; j<eat_cols;j++){
								cell.setPosition(eater_x+j*side,eater_y+i*side);
								if (eater[i][j] && (i!=1 || j!=3))
									cell.setFillColor(renderer::color_live);
								else if ((i!=1 || j!=3))
									cell.setFillColor(renderer::color_dead);
								else
									cell.setFillColor(sf::Color::Red);
								rw.draw(cell);
							}
						}
						rw.display();
					}
				break;

			}
		}
	}
}


logic::intro_screen::intro_screen(){
	file::getpattern(_gun,"gun", gun_rows,gun_cols);
	for ( int i=0; i<gun_rows;i++) {
		for (int j=0; j<gun_cols;j++){
			gun[i][j]=_gun[i][j];
//			std::cout<<gun[i][j]<<" ";
		}
//		std::cout<<std::endl;
	}

	file::getpattern(_glider,"glider", gli_rows,gli_cols);
	for ( int i=0; i<gli_rows;i++) {
		for (int j=0; j<gli_cols;j++){
			glider[i][j]=_glider[i][j];
///			std::cout<<glider[i][j]<<" ";
		}
//		std::cout<<std::endl;
	}

	file::getpattern(_eater,"eater", eat_rows,eat_cols);
	for ( int i=0; i<eat_rows;i++) {
		for (int j=0; j<eat_cols;j++){
			eater[i][j]=_eater[i][j];
//			std::cout<<eater[i][j]<<" ";
		}
//		std::cout<<std::endl;
	}
}
