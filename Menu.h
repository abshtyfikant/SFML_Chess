#ifndef MENU_H
#define MENU_H

#include "Game.h"
#include <string>
#include <regex>

namespace fs = std::filesystem;

class Menu {
	//Button chess_button, fischer_button, checkers_button;

	sf::RectangleShape button;
	sf::Sprite bg_image, chess_button, fischer_button, checkers_button, one_pc_button,
		two_pc_button, back_button;
public:
	fs::path img{ "Images" };
	Menu(int, int);
	void menuInit(int, int);
	void menuLoop(sf::RenderWindow&);
	void preGame(sf::RenderWindow&);
};

class Button {
	struct Button_struct {
		sf::RectangleShape button_square;
		sf::Font font;
		sf::Text name, explanation;
	};
	Button_struct button;
	/*RectangleShape button_square;
	Font font;
	Text name, explanation;*/

public:
	Button() {}
	Button(sf::RenderWindow& w, int size, int xpos, std::string n, std::string expl);
	void drawButton(sf::RenderWindow&);
};

#endif