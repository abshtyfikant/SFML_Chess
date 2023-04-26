#include "Menu.h"

Menu::Menu(int x, int y) {
	menuInit(x, y);
}

void Menu::menuInit(int x, int y) {
	sf::RenderWindow window(sf::VideoMode(x, y), "JW_PK4_Projekt", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Texture t_bg;
	img /= "background.jpg";
	std::string path = img.generic_string();
	t_bg.loadFromFile(path);
	bg_image.setTexture(t_bg);

	bg_image.setScale((float)x / t_bg.getSize().x, (float)y / t_bg.getSize().y);

	//Button chess_button(window, 100, 100, "SZACHY", "gra");

	sf::RectangleShape sqr;
	sqr.setSize(sf::Vector2f(100, 100));
	sqr.setPosition(sf::Vector2f(300, 300));
	sqr.setFillColor(sf::Color::Green);
	sqr.setRotation(45);
	button = sqr;

	sf::Texture t[6];
	path = "Images/";
	int i = 0;
	std::string str_path;
	std::regex expr("Images/_[^w | ^r]\\w*_button.png");

	for (const auto& entry : fs::directory_iterator(path)) {
		str_path = entry.path().generic_string();
		if (std::regex_match(str_path, expr)) {
			t[i].loadFromFile(str_path);
			i++;
		}
	}
	one_pc_button.setTexture(t[0]);
	one_pc_button.setOrigin(one_pc_button.getLocalBounds().width / 2, one_pc_button.getLocalBounds().height / 2);
	one_pc_button.setScale(1, 1);
	one_pc_button.setPosition(sf::Vector2f(x / 2 - 150, y / 2));

	two_pc_button.setTexture(t[1]);
	two_pc_button.setOrigin(two_pc_button.getLocalBounds().width / 2, two_pc_button.getLocalBounds().height / 2);
	two_pc_button.setScale(1, 1);
	two_pc_button.setPosition(sf::Vector2f(x / 2 + 150, y / 2));

	t[2].setSmooth(1);
	back_button.setTexture(t[2]);
	back_button.setOrigin(back_button.getLocalBounds().width / 2, 0);
	back_button.setScale(0.7, 0.7);
	back_button.setPosition(bg_image.getGlobalBounds().height + (window.getSize().x - bg_image.getGlobalBounds().height) / 2, 50);

	checkers_button.setTexture(t[3]);
	checkers_button.setOrigin(checkers_button.getLocalBounds().width / 2, checkers_button.getLocalBounds().height / 2);
	checkers_button.setScale(0.7, 0.7);
	checkers_button.setPosition(sf::Vector2f(x / 4 + 500, y / 2));

	chess_button.setTexture(t[4]);
	fischer_button.setScale(1.2, 1.2);
	chess_button.setOrigin(chess_button.getLocalBounds().width / 2, chess_button.getLocalBounds().height / 2);
	chess_button.setPosition(sf::Vector2f(x / 4, y / 2));

	fischer_button.setTexture(t[5]);
	fischer_button.setOrigin(fischer_button.getLocalBounds().width / 2, fischer_button.getLocalBounds().height / 2);
	fischer_button.setScale(0.7, 0.7);
	fischer_button.setPosition(sf::Vector2f(x / 4 + 280, y / 2));

	menuLoop(window);
}

void Menu::menuLoop(sf::RenderWindow& w) {
	while (w.isOpen()) {
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(w);
		sf::Cursor hand, arrow;
		hand.loadFromSystem(sf::Cursor::Hand);
		arrow.loadFromSystem(sf::Cursor::Arrow);

		while (w.pollEvent(event)) {
			if (chess_button.getGlobalBounds().contains(pos.x, pos.y)
				|| fischer_button.getGlobalBounds().contains(pos.x, pos.y)
				|| checkers_button.getGlobalBounds().contains(pos.x, pos.y)) {
				w.setMouseCursor(hand);
			}
			else
				w.setMouseCursor(arrow);

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					if (chess_button.getGlobalBounds().contains(pos.x, pos.y)) {
						preGame(w);
					}
					else if (fischer_button.getGlobalBounds().contains(pos.x, pos.y)) {
						FischerGame partia;

						sf::Texture wood_chessboard;
						wood_chessboard.loadFromFile("Images/chessboard_2.png");
						sf::RectangleShape chboard(sf::Vector2f(w.getSize().y, w.getSize().y));
						chboard.setTexture(&wood_chessboard);

						partia.gameLoop(w, chboard, bg_image);
					}
					else if (checkers_button.getGlobalBounds().contains(pos.x, pos.y)) {
						CheckersGame partia;

						sf::Texture wood_chessboard;
						wood_chessboard.loadFromFile("Images/chessboard_2.png");
						sf::RectangleShape chboard(sf::Vector2f(w.getSize().y, w.getSize().y));
						chboard.setTexture(&wood_chessboard);

						partia.gameLoop(w, chboard, bg_image);
					}
				}
			}

			if (event.type == sf::Event::Closed)
				w.close();
		}

		w.clear();
		w.draw(bg_image);
		w.draw(chess_button);
		w.draw(fischer_button);
		w.draw(checkers_button);
		w.display();
	}
}

void Menu::preGame(sf::RenderWindow& w) {
	bool isGame = true;
	while (w.isOpen() && isGame) {
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(w);
		sf::Cursor hand, arrow;
		hand.loadFromSystem(sf::Cursor::Hand);
		arrow.loadFromSystem(sf::Cursor::Arrow);

		while (w.pollEvent(event)) {
			if (one_pc_button.getGlobalBounds().contains(pos.x, pos.y)
				|| two_pc_button.getGlobalBounds().contains(pos.x, pos.y)
				|| back_button.getGlobalBounds().contains(pos.x, pos.y)) {
				w.setMouseCursor(hand);
			}
			else
				w.setMouseCursor(arrow);

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					if (one_pc_button.getGlobalBounds().contains(pos.x, pos.y)) {
						ChessGame partia;

						sf::Texture wood_chessboard;
						wood_chessboard.loadFromFile("Images/chessboard_2.png");
						sf::RectangleShape chboard(sf::Vector2f(w.getSize().y, w.getSize().y));
						chboard.setTexture(&wood_chessboard);

						partia.gameLoop(w, chboard, bg_image);
					}
					else if (two_pc_button.getGlobalBounds().contains(pos.x, pos.y)) {
						ChessGame partia;

						sf::Texture wood_chessboard;
						wood_chessboard.loadFromFile("Images/chessboard_2.png");
						sf::RectangleShape chboard(sf::Vector2f(w.getSize().y, w.getSize().y));
						chboard.setTexture(&wood_chessboard);

						partia.gameLoopOnline(w, chboard, bg_image);
					}
					else if (back_button.getGlobalBounds().contains(pos.x, pos.y)) {
						isGame = false;
					}
				}

			}

			if (event.type == sf::Event::Closed)
				w.close();
		}

		w.clear();
		w.draw(bg_image);
		w.draw(one_pc_button);
		w.draw(two_pc_button);
		w.draw(back_button);
		w.display();
	}
}

Button::Button(sf::RenderWindow &w, int size, int xpos, std::string n, std::string expl) {
	sf::RectangleShape button_square;
	sf::Font font;
	sf::Text name, explanation;

	button_square.setSize(sf::Vector2f(size, size));
	button_square.setRotation(45);
	button_square.setOrigin(button_square.getSize().x / 2, button_square.getSize().y / 2);
	button_square.setPosition(sf::Vector2f(xpos, w.getSize().y / 2.5));

	font.loadFromFile("Images/Poppins-Regular.ttf");
	name.setFont(font); explanation.setFont(font);
	name.setString(n); explanation.setString(expl);
	name.setCharacterSize((float)size / 5); explanation.setCharacterSize((float)size / 8);

	sf::FloatRect textRect = name.getLocalBounds();
	name.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
	textRect = explanation.getLocalBounds();
	explanation.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);

	name.setPosition(button_square.getPosition());

	button.button_square = button_square;
	button.name = name;
}

void Button::drawButton(sf::RenderWindow& w) {
	w.draw(button.button_square);
	w.draw(button.name);
	//w.draw(button.explanation);
}

