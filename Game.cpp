#include "Game.h"

//Chess
ChessGame::ChessGame() {
	gameReset();
	gameInit();
}

void ChessGame::storePos(ChessPiece* arr[8][8]) {
	arrPosition foo;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			foo.aux_arr[i][j] = arr[i][j];
	}
	game_storage.push_back(foo);
}

void ChessGame::gameReset() {
	for (int row = 2; row < 6; row++) {
		for (int col = 0; col < 8; col++)
			game_pos[col][row] = 0;
	}
	// WHITE pieces initialization
	game_pos[0][0] = new Rook('W');
	game_pos[1][0] = new Knight('W');
	game_pos[2][0] = new Bishop('W');
	game_pos[3][0] = new Queen('W');
	game_pos[4][0] = new King('W');
	game_pos[5][0] = new Bishop('W');
	game_pos[6][0] = new Knight('W');
	game_pos[7][0] = new Rook('W');

	for (int i = 0; i < 8; i++)
		game_pos[i][1] = new Pawn('W');

	// BLACK pieces initialization
	game_pos[0][7] = new Rook('B');
	game_pos[1][7] = new Knight('B');
	game_pos[2][7] = new Bishop('B');
	game_pos[3][7] = new Queen('B');
	game_pos[4][7] = new King('B');
	game_pos[5][7] = new Bishop('B');
	game_pos[6][7] = new Knight('B');
	game_pos[7][7] = new Rook('B');

	for (int i = 0; i < 8; i++)
		game_pos[i][6] = new Pawn('B');

	whoseMove = 'W';
	storePos(game_pos);
}

void ChessGame::gameInit() {

	//texture init
	std::string path = "Images/Chess/", str_path;
	std::regex expr(path+"[w|b]\\w{1}.png");
	int i = 0;

	for (const auto& entry : fs::directory_iterator(path)) {
		str_path = entry.path().generic_string();
		if (std::regex_match(str_path, expr)) {
			t[i].loadFromFile(str_path);
			i++;
		}
	}

	t[12].loadFromFile("Images/_back_button.png");
	t[13].loadFromFile("Images/_rewind_button.png");
	t[14].loadFromFile("Images/_rotate_button.png");

	r[0].loadFromFile("Images/_result_white.png");
	r[1].loadFromFile("Images/_result_black.png");
	r[2].loadFromFile("Images/_result_draw.png");

	//Sprite init
	for (int i = 0; i < 12; i++) {
		t[i].setSmooth(1);
		fig[i].setTexture(t[i]);
	}
	t[12].setSmooth(1);
	back_button.setTexture(t[12]);
	back_button.setOrigin(back_button.getLocalBounds().width / 2, 0);
	back_button.setScale(0.7, 0.7);

	t[13].setSmooth(1);
	rewind_button.setTexture(t[13]);
	rewind_button.setOrigin(rewind_button.getLocalBounds().width / 2, 0);
	rewind_button.setScale(0.5, 0.5);

	t[14].setSmooth(1);
	rotate_button.setTexture(t[14]);
	rotate_button.setOrigin(rotate_button.getLocalBounds().width / 2, 0);
	rotate_button.setScale(0.5, 0.5);

	for (int i = 0; i < 3; i++) {
		r[i].setSmooth(1);
		res_info[i].setTexture(r[i]);
		res_info[i].setOrigin(res_info[i].getLocalBounds().width / 2, 0);
		res_info[i].setScale(0.7, 0.7);
	}
}

sf::Sprite ChessGame::drawPiece(ChessPiece* arr[8][8], int b_size, int i, int j, bool rot) {
	float sq_size = (float)b_size / 8;

	sf::Sprite temp;
	if (arr[i][j]->whatColor() == 'B') {
		switch (arr[i][j]->pieceName()) {
		case 'B':
			temp = fig[0];
			break;
		case 'K':
			temp = fig[1];
			break;
		case 'N':
			temp = fig[2];
			break;
		case 'P':
			temp = fig[3];
			break;
		case 'Q':
			temp = fig[4];
			break;
		case 'R':
			temp = fig[5];
			break;
		}
	}
	else {
		switch (arr[i][j]->pieceName()) {
		case 'B':
			temp = fig[6];
			break;
		case 'K':
			temp = fig[7];
			break;
		case 'N':
			temp = fig[8];
			break;
		case 'P':
			temp = fig[9];
			break;
		case 'Q':
			temp = fig[10];
			break;
		case 'R':
			temp = fig[11];
			break;
		}
	}

	sf::Vector2f p_size = sf::Vector2f(temp.getTexture()->getSize());
	float s = (sq_size / p_size.y) * 0.8;
	sf::Vector2f sprite_size; sprite_size.x = s * p_size.x; sprite_size.y = s * p_size.y;

	temp.setScale(s, s);
	if (rot) {
		j = 7 - j;
		i = 7 - i;
	}
	temp.setPosition(sq_size* i + ((sq_size - sprite_size.x) / 2), sq_size* (7 - j) + ((sq_size - sprite_size.y) / 2));

	return temp;
}

void ChessGame::gameLoop(sf::RenderWindow& w, sf::RectangleShape& board, sf::Sprite bg) {
	bool move = false, isGame = true, back_pressed = false, rewind_pressed = false,
		rotate_pressed = false, rotation = false;
	int f_row = -2, f_col = -2, new_row, new_col, dx, dy, res = 2;
	char castl;
	sf::Sprite f_moving;
	sf::RectangleShape movesqr;
	sf::Cursor hand, arrow;
	hand.loadFromSystem(sf::Cursor::Hand);
	arrow.loadFromSystem(sf::Cursor::Arrow);

	back_button.setPosition(board.getSize().x + (w.getSize().x - board.getSize().x) / 2, 50);
	rewind_button.setPosition(back_button.getPosition().x, back_button.getPosition().y + back_button.getGlobalBounds().height + 70);
	rotate_button.setPosition(rewind_button.getPosition().x, rewind_button.getPosition().y + rewind_button.getGlobalBounds().height + 30);
	movesqr.setSize(sf::Vector2f(rotate_button.getGlobalBounds().width, rotate_button.getGlobalBounds().height));
	movesqr.setOrigin(movesqr.getSize().x / 2, 0);
	movesqr.setPosition(rotate_button.getPosition().x, rotate_button.getPosition().y + rotate_button.getGlobalBounds().height + 60);
	for (int i = 0; i < 3; i++) {
		res_info[i].setPosition(movesqr.getPosition().x, movesqr.getPosition().y + movesqr.getGlobalBounds().height + 60);
	}

	while (w.isOpen()) {
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(w);

		if (whoseMove == 'W')
			movesqr.setFillColor(sf::Color::White);
		else
			movesqr.setFillColor(sf::Color::Black);

		while (w.pollEvent(event)) {
			if (back_button.getGlobalBounds().contains(pos.x, pos.y)
				|| rewind_button.getGlobalBounds().contains(pos.x, pos.y)
				|| rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
				w.setMouseCursor(hand);
			}
			else
				w.setMouseCursor(arrow);

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					if (board.getGlobalBounds().contains(pos.x, pos.y)) {
						f_row = pos.x / (board.getSize().x / 8);
						f_col = 8 - (pos.y / (board.getSize().y / 8));
						if (rotation) {
							f_col = pos.y / (board.getSize().y / 8);
							f_row = 8 - pos.x / (board.getSize().x / 8);
						}

						if (game_pos[f_row][f_col] != 0) {
							w.setMouseCursor(hand);
							move = true;
							f_moving = drawPiece(game_pos, board.getSize().x, f_row, f_col, rotation);
							sf::Vector2f sprite_size;
							sprite_size.x = (f_moving.getScale().x) * ((float)f_moving.getTexture()->getSize().x);
							sprite_size.y = (f_moving.getScale().y) * ((float)f_moving.getTexture()->getSize().y);
							dx = sprite_size.x / 2;
							dy = sprite_size.y / 2;
						}
					}
					else if (back_button.getGlobalBounds().contains(pos.x, pos.y)) {
						back_pressed = true;
					}
					else if (rewind_button.getGlobalBounds().contains(pos.x, pos.y)) {
						rewind_pressed = true;
					}
					else if (rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
						rotate_pressed = true;
					}
				}

			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.key.code == sf::Mouse::Left) {
					w.setMouseCursor(arrow);
					if (board.getGlobalBounds().contains(pos.x, pos.y)) {
						if (move) {
							new_row = pos.x / (board.getSize().x / 8);
							new_col = 8 - (pos.y / (board.getSize().y / 8));
							if (rotation) {
								new_row = 8 - pos.x / (board.getSize().x / 8);
								new_col = pos.y / (board.getSize().y / 8);
							}
							castl = game_pos[f_row][f_col]->pieceName();
							if (moveJudge(f_row, f_col, new_row, new_col)) {
								moveMaker(f_row, f_col, new_row, new_col);
								if (!wasCastle) {
									game_pos[new_row][new_col] = game_pos[f_row][f_col];
									game_pos[f_row][f_col] = 0;
								}
								if (whoseMove == 'W')
									whoseMove = 'B';
								else
									whoseMove = 'W';
								storePos(game_pos);

								if (isCheckmate()) {
									if (whoseMove == 'B')
										res = 0;
									else
										res = 1;
									isGame = false;
								}
								else if (isStalemate()) {
									res = 2;
									isGame = false;
								}
								wasCastle = false;
							}
						}
					}
					else if (back_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (back_pressed)
							return;
						back_pressed = false;
					}
					else if (rewind_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (rewind_pressed)
							previousPos();
						rewind_pressed = false;
					}
					else if (rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (rotate_pressed)
							rotation = !rotation;
						rotate_pressed = false;
					}
					move = false;
					f_row = -2, f_col = -2;
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::BackSpace)
					previousPos();
			}
			else if (event.type == sf::Event::Closed)
				w.close();
		}
			
		w.draw(bg);
		w.draw(board);
		w.draw(back_button);
		w.draw(rewind_button);
		w.draw(rotate_button);
		w.draw(movesqr);
		if (!isGame)
			w.draw(res_info[res]);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (game_pos[i][j] != 0) {
					if (f_row > -2) {
						if (f_row == i && f_col == j)
							continue;
					}
					w.draw(drawPiece(game_pos, board.getSize().x, i, j, rotation));
				}
			}
		}

		if (move) {
			f_moving.setPosition(pos.x - dx, pos.y - dy);
			w.draw(f_moving);
		}
		
		w.display();
	}
}

std::binary_semaphore a(1), b(1), c(1);

void setSemaphore(std::binary_semaphore& s){
	s.try_acquire();
	s.release();
}

void sconnect(sf::TcpSocket &sock, sf::IpAddress ip, int p) {
	a.acquire();
	if (sock.connect(ip, p) != sf::Socket::Done) {
		sf::TcpListener listener;
		if (listener.listen(p) != sf::Socket::Done)
			std::cout << "Blad nasluchiwania\n";
		std::cout << "Serwer nasluchuje na porcie: " << p << std::endl;

		if (listener.accept(sock) != sf::Socket::Done)
			std::cout << "Blad polaczenia z klientem\n";
		std::cout << "Polaczono z klientem: " << sock.getRemoteAddress() << std::endl;
	}
	else {
		std::cout << "Polaczono\n";
		a.release();
	}
	b.acquire();
	c.acquire();
	return;
}

void drawLoad(sf::RenderWindow &w, sf::Sprite bg, sf::Sprite w1, sf::Sprite w2) {
	if (b.try_acquire()) {
		w.clear();
		w.draw(bg);
		w.draw(w1);
		w.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		w.clear();
		w.draw(bg);
		w.draw(w2);
		w.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		b.release();
	}
	return;
}

void ChessGame::gameLoopOnline(sf::RenderWindow& w, sf::RectangleShape& board, sf::Sprite bg) {
	bool move = false, isGame = true, back_pressed = false, rewind_pressed = false,
		client = false, rotate_pressed = false, rotation = false;
	int f_row = -2, f_col = -2, new_row, new_col, dx, dy, res = 2;
	char castl;
	sf::Sprite f_moving;
	sf::RectangleShape movesqr;
	sf::Cursor hand, arrow;
	hand.loadFromSystem(sf::Cursor::Hand);
	arrow.loadFromSystem(sf::Cursor::Arrow);

	sf::TcpSocket socket;
	sf::IpAddress server = "localhost"; int port = 50001;
	std::string out, move_str; char in[4];
	size_t received = 0;

	sf::Texture w1, w2; sf::Sprite wait1, wait2;
	w1.loadFromFile("Images/_wait1_button.png");
	w2.loadFromFile("Images/_wait2_button.png");
	
	w1.setSmooth(1);
	wait1.setTexture(w1);
	wait1.setScale(1.2, 1.2);
	wait1.setOrigin(wait1.getLocalBounds().width / 2, wait1.getLocalBounds().height / 2);
	wait1.setPosition(w.getSize().x / 2, w.getSize().y / 2);

	w2.setSmooth(1);
	wait2.setTexture(w2);
	wait2.setScale(wait1.getScale());
	wait2.setOrigin(wait1.getOrigin());
	wait2.setPosition(wait1.getPosition());

	setSemaphore(a);
	setSemaphore(b);
	setSemaphore(c);

	std::thread t1(sconnect, std::ref(socket), server, port);
	sf::Event e;

	while (c.try_acquire()) {
		while (w.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				w.close();
		}
		drawLoad(w, bg, wait1, wait2);
		c.release();
	}
	t1.join();

	if (a.try_acquire()) {
		client = true;
		std::cout << "Polaczono z serwerem: " << server << std::endl;
	}

	bool wait4move = false;
	if (client == true)
		wait4move = true;
	socket.setBlocking(false);

	back_button.setPosition(board.getSize().x + (w.getSize().x - board.getSize().x) / 2, 50);
	rotate_button.setPosition(back_button.getPosition().x, back_button.getPosition().y + back_button.getGlobalBounds().height + 70);
	movesqr.setSize(sf::Vector2f(rotate_button.getGlobalBounds().width, rotate_button.getGlobalBounds().height));
	movesqr.setOrigin(movesqr.getSize().x / 2, 0);
	movesqr.setPosition(rotate_button.getPosition().x, rotate_button.getPosition().y + rotate_button.getGlobalBounds().height + 60);
	for (int i = 0; i < 3; i++) {
		res_info[i].setPosition(rotate_button.getPosition().x, rotate_button.getPosition().y + rotate_button.getGlobalBounds().height + 150);
	}

	while (w.isOpen()) {
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(w);

		if (wait4move == true) {
			if (socket.receive(in, sizeof(in), received) != sf::Socket::Done) {
				"Blad w receive\n";
			}

			if (received > 0 && in[0] > 96 && in[0] < 105) {
				makeStringMove(in);
				wait4move = false;
				if (isCheckmate()) {
					if (whoseMove == 'B')
						res = 0;
					else
						res = 1;
					isGame = false;
				}
				else if (isStalemate()) {
					res = 2;
					isGame = false;
				}
			}
		}
		if (whoseMove == 'W')
			movesqr.setFillColor(sf::Color::White);
		else
			movesqr.setFillColor(sf::Color::Black);

		while (w.pollEvent(event)) {
			if (back_button.getGlobalBounds().contains(pos.x, pos.y)
				|| rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
				w.setMouseCursor(hand);
			}
			else
				w.setMouseCursor(arrow);

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					if (board.getGlobalBounds().contains(pos.x, pos.y)) {
						f_row = pos.x / (board.getSize().x / 8);
						f_col = 8 - (pos.y / (board.getSize().y / 8));
						if (rotation)
							f_col = pos.y / (board.getSize().y / 8);

						if (game_pos[f_row][f_col] != 0) {
							w.setMouseCursor(hand);
							move = true;
							f_moving = drawPiece(game_pos, board.getSize().x, f_row, f_col, rotation);
							sf::Vector2f sprite_size;
							sprite_size.x = (f_moving.getScale().x) * ((float)f_moving.getTexture()->getSize().x);
							sprite_size.y = (f_moving.getScale().y) * ((float)f_moving.getTexture()->getSize().y);
							dx = sprite_size.x / 2;
							dy = sprite_size.y / 2;
						}
					}
					else if (back_button.getGlobalBounds().contains(pos.x, pos.y)) {
						back_pressed = true;
					}
					else if (rewind_button.getGlobalBounds().contains(pos.x, pos.y)) {
						rewind_pressed = true;
					}
					else if (rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
						rotate_pressed = true;
					}
				}

			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.key.code == sf::Mouse::Left) {
					w.setMouseCursor(arrow);
					if (board.getGlobalBounds().contains(pos.x, pos.y)) {
						if (move) {
							new_row = pos.x / (board.getSize().x / 8);
							new_col = 8 - (pos.y / (board.getSize().y / 8));
							if (rotation)
								new_col = pos.y / (board.getSize().y / 8);
							castl = game_pos[f_row][f_col]->pieceName();
							if (!wait4move && moveJudge(f_row, f_col, new_row, new_col)) {
								moveMaker(f_row, f_col, new_row, new_col);
								if (!wasCastle) {
									game_pos[new_row][new_col] = game_pos[f_row][f_col];
									game_pos[f_row][f_col] = 0;
								}
								if (whoseMove == 'W')
									whoseMove = 'B';
								else
									whoseMove = 'W';

								if (isCheckmate()) {
									if (whoseMove == 'B')
										res = 0;
									else
										res = 1;
									isGame = false;
								}
								else if (isStalemate()) {
									res = 2;
									isGame = false;
								}
								move_str = moveString(f_row, f_col, new_row, new_col);

								if (socket.send(move_str.c_str(), sizeof(move_str) + 1) != sf::Socket::Done)
									std::cout << "Nie mozna wyslac\n";
								wasCastle = false; wait4move = true;
							}
						}
					}
					else if (back_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (back_pressed) {
							isGame = false;
							socket.disconnect();
							return;
						}
						back_pressed = false;
					}
					else if (rewind_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (rewind_pressed)
							previousPos();
						rewind_pressed = false;
					}
					else if (rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (rotate_pressed)
							rotation = !rotation;
						rotate_pressed = false;
					}
					move = false;
					f_row = -2, f_col = -2;
				}
			}
			if (event.type == sf::Event::Closed)
				w.close();
		}

		w.clear();
		w.draw(bg);
		w.draw(board);
		w.draw(back_button);
		w.draw(rotate_button);
		w.draw(movesqr);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (game_pos[i][j] != 0) {
					if (f_row > -2) {
						if (f_row == i && f_col == j)
							continue;
					}
					w.draw(drawPiece(game_pos, board.getSize().x, i, j, rotation));
				}
			}
		}

		if (move) {
			f_moving.setPosition(pos.x - dx, pos.y - dy);
			w.draw(f_moving);
		}
		if (!isGame) {
			w.draw(res_info[res]);
		}

		w.display();
	}
}

bool ChessGame::isCheck(ChessPiece* arr[8][8]) {
	int i_king = -1, j_king = -1;

	//finding the king
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (arr[i][j] != 0) {
				if ((arr[i][j]->pieceName() == 'K') && (arr[i][j]->whatColor() == whoseMove)) {
					i_king = i; j_king = j;
				}
			}
		}
	}
	//checking if the king could be captured by opponent's piece
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (arr[i][j] != 0) {
				if (arr[i][j]->whatColor() != whoseMove) {
					if (arr[i][j]->correctMove(i, j, i_king, j_king, arr)) {
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

bool ChessGame::isStalemate() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (game_pos[i][j] != 0) {
				if (game_pos[i][j]->whatColor() == whoseMove) {
					for (int k = 0; k < 8; k++) {
						for (int l = 0; l < 8; l++) {
							if (i != k || j != l) {
								if (game_pos[i][j]->correctMove(i, j, k, l, game_pos)) {
									if (moveJudge(i, j, k, l)) {
										return 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 1;
}

bool ChessGame::isCheckmate() {
	if (isStalemate() == 1 && isCheck(game_pos) == 1)
		return 1;
	else
		return 0;
}

void ChessGame::moveMaker(int r1, int c1, int r2, int c2) {
	if (game_pos[r1][c1] != 0) {
		if ((whoseMove == 'W' && c2 == 7) || (whoseMove == 'B' && c2 == 0)) {
			if (game_pos[r1][c1]->pieceName() == 'P')
				game_pos[r1][c1] = new Queen(whoseMove);
		}
		bool fm_changed = false;
		if (game_pos[r1][c1]->pieceName() == 'K' || game_pos[r1][c1]->pieceName() == 'R') {
			game_pos[r1][c1]->firstmove = false;
			fm_changed = true;
		}
		if (game_pos[r2][c2] != 0) {
			if (game_pos[r2][c2]->pieceName() == 'R' && game_pos[r2][c2]->whatColor() == whoseMove) {
				ChessPiece* arr[8][8];
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++)
						arr[i][j] = game_pos[i][j];
				}
				arr[r1][c1] = 0;
				arr[r2][c2] = 0;
				if (r2 - r1 < 0) {
					arr[2][c1] = new King(whoseMove);
					arr[2][c1]->firstmove = false;
					arr[3][c1] = new Rook(whoseMove);
					arr[3][c1]->firstmove = false;
				}
				else if (r2 - r1 > 0) {
					arr[6][c1] = new King(whoseMove);
					arr[6][c1]->firstmove = false;
					arr[5][c1] = new Rook(whoseMove);
					arr[5][c1]->firstmove = false;
				}
				if (!isCheck(arr)) {
					game_pos[r1][c1] = 0;
					game_pos[r2][c2] = 0;
					game_pos[2][c1] = arr[2][c1];
					game_pos[3][c1] = arr[3][c1];
					game_pos[5][c1] = arr[5][c1];
					game_pos[6][c1] = arr[6][c1];
				}
				else {
					if (whoseMove == 'W')
						whoseMove = 'B';
					else
						whoseMove = 'W';
					if (fm_changed)
						game_pos[r1][c1]->firstmove = true;
				}
				wasCastle = true;
			}
		}
	}
}

bool ChessGame::moveJudge(const int r1, const int c1, const int r2, const int c2) {
	if (game_pos[r1][c1] != 0 &&whoseMove == game_pos[r1][c1]->whatColor()) {
		if (game_pos[r1][c1]->correctMove(r1, c1, r2, c2, game_pos)) {
			ChessPiece* arr[8][8];
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++)
					arr[i][j] = game_pos[i][j];
			}
			arr[r2][c2] = arr[r1][c1];
			arr[r1][c1] = 0;

			if (isCheck(arr))
				return 0;
			else
				return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

void ChessGame::previousPos() {
	if (game_storage.size() < 2)
		return;
	else {
		game_storage.pop_back();
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				game_pos[i][j] = game_storage.back().aux_arr[i][j];
			}
		}
		if (whoseMove == 'W')
			whoseMove = 'B';
		else
			whoseMove = 'W';
	}
}

void ChessGame::makeCompMove(int r1, int c1, int r2, int c2) {
	std::string player_move = "aaaa", comp_move;
	player_move[0] = r1 + 97;
	player_move[1] = c1 + 49;
	player_move[2] = r2 + 97;
	player_move[3] = c2 + 49;

	//cout << player_move << endl;

	//comp_move = getNextMove(player_move);
}

std::string ChessGame::moveString(int r1, int c1, int r2, int c2) {
	std::string player_move = "aaaa";
	player_move[0] = r1 + 97;
	player_move[1] = c1 + 49;
	player_move[2] = r2 + 97;
	player_move[3] = c2 + 49;

	return player_move;
}

void ChessGame::makeStringMove(std::string m) {
	int r1 = (int)(m[0] - 97);
	int c1 = (int)(m[1] - 49);
	int r2 = (int)(m[2] - 97);
	int c2 = (int)(m[3] - 49);

	game_pos[r2][c2] = game_pos[r1][c1];
	game_pos[r1][c1] = 0;

	if (whoseMove == 'W')
		whoseMove = 'B';
	else
		whoseMove = 'W';
}

//Fischer Chess
FischerGame::FischerGame() {
	gameInit();
	gameReset();
}

void FischerGame::gameReset() {
	srand(time(NULL));

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++)
			game_pos[col][row] = 0;
	}

	//Pawns
	for (int i = 0; i < 8; i++)
		game_pos[i][1] = new Pawn('W');

	for (int i = 0; i < 8; i++)
		game_pos[i][6] = new Pawn('B');

	//Rooks
	int r1 = rand() % 7;
	game_pos[r1][0] = new Rook('W');
	game_pos[r1][7] = new Rook('B');

	int r2 = 0;
	int dif = 0;
	while (1) {
		r2 = rand() % 7;
		dif = (abs)(r2 - r1);
		if ((game_pos[r2][0] == 0) && (dif >= 2))
			break;
	}
	game_pos[r2][0] = new Rook('W');
	game_pos[r2][7] = new Rook('B');

	//Kings
	if (r1 > r2) {
		int temp = r2;
		r2 = r1;
		r1 = temp;
	}
	int k = 0;
	while (1) {
		k = rand() % 7;
		if ((k > r1) && (k < r2))
			break;
	}

	game_pos[k][0] = new King('W');
	game_pos[k][7] = new King('B');

	//Bishops
	while (1) {
		r1 = rand() % 7;
		if (game_pos[r1][0] == 0)
			break;
	}

	game_pos[r1][0] = new Bishop('W');
	game_pos[r1][7] = new Bishop('B');

	while (1) {
		r2 = rand() % 7;
		dif = (abs)(r2 - r1);
		if (game_pos[r2][0] == 0 && dif % 2 == 1)
			break;
	}

	game_pos[r2][0] = new Bishop('W');
	game_pos[r2][7] = new Bishop('B');

	//Knights
	int count = 0;
	while (count < 2) {
		k = rand() % 7;
		if (game_pos[k][0] == 0) {
			game_pos[k][0] = new Knight('W');
			game_pos[k][7] = new Knight('B');
			count++;
		}
	}

	//Queens
	for (int i = 0; i < 8; i++) {
		if (game_pos[i][0] == 0) {
			game_pos[i][0] = new Queen('W');
			game_pos[i][7] = new Queen('B');
			break;
		}
	}

	whoseMove = 'W';
	storePos(game_pos);
}

//Checkers
CheckersGame::CheckersGame() {
	gameReset();
	gameInit();
}

void CheckersGame::gameInit() {
	//texture init
	t[0].loadFromFile("Images/Checkers/wp.png");
	t[1].loadFromFile("Images/Checkers/wk.png");
	t[2].loadFromFile("Images/Checkers/bp.png");
	t[3].loadFromFile("Images/Checkers/bk.png");

	t[4].loadFromFile("Images/_back_button.png");
	t[5].loadFromFile("Images/_rewind_button.png");
	t[6].loadFromFile("Images/_rotate_button.png");

	//Sprite init
	for (int i = 0; i < 4; i++) {
		t[i].setSmooth(1);
		fig[i].setTexture(t[i]);
	}

	t[4].setSmooth(1);
	back_button.setTexture(t[4]);
	back_button.setOrigin(back_button.getLocalBounds().width / 2, 0);
	back_button.setScale(0.7, 0.7);

	t[5].setSmooth(1);
	rewind_button.setTexture(t[5]);
	rewind_button.setOrigin(rewind_button.getLocalBounds().width / 2, 0);
	rewind_button.setScale(0.5, 0.5);

	t[6].setSmooth(1);
	rotate_button.setTexture(t[6]);
	rotate_button.setOrigin(rotate_button.getLocalBounds().width / 2, 0);
	rotate_button.setScale(0.5, 0.5);
}

void CheckersGame::gameReset() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			game_pos[i][j] = 0;
		}
	}
	int i = 0, j;
	for (j = 0; j < 3; j++) {
		for (i; i < 8; i += 2) {
			game_pos[i][j] = new Man('W');
		}
		if (j % 2 == 0)
			i = 1;
		else
			i = 0;
	}

	i = 1;
	for (j = 5; j < 8; j++) {
		for (i; i < 8; i += 2) {
			game_pos[i][j] = new Man('B');
		}
		if (j % 2 == 0)
			i = 1;
		else
			i = 0;
	}

	whoseMove = 'W';
	is_capture = false;
}

sf::Sprite CheckersGame::drawPiece(CheckersPiece* arr[8][8], int b_size, int i, int j, bool rot) {
	float sq_size = (float)b_size / 8;

	sf::Sprite temp;
	if (arr[i][j]->whatColor() == 'W') {
		switch (arr[i][j]->pieceName()) {
		case 'M':
			temp = fig[0];
			break;
		case 'K':
			temp = fig[1];
			break;
		}
	}
	else {
		switch (arr[i][j]->pieceName()) {
		case 'M':
			temp = fig[2];
			break;
		case 'K':
			temp = fig[3];
			break;
		}
	}

	sf::Vector2f p_size = sf::Vector2f(temp.getTexture()->getSize());
	float s = (sq_size / p_size.y) * 0.8;
	sf::Vector2f sprite_size; sprite_size.x = s * p_size.x; sprite_size.y = s * p_size.y;

	temp.setScale(s, s);
	if (rot) {
		j = 7 - j;
		i = 7 - i;
	}
	temp.setPosition(sq_size * i + ((sq_size - sprite_size.x) / 2), sq_size * (7 - j) + ((sq_size - sprite_size.y) / 2));

	return temp;
}

void CheckersGame::gameLoop(sf::RenderWindow& w, sf::RectangleShape& board, sf::Sprite bg) {
	bool move = false, isGame = true, back_pressed = false, rewind_pressed = false,
		rotate_pressed = false, rotation = false;
	int f_row = -2, f_col = -2, new_row, new_col, dx, dy;
	sf::Sprite f_moving;
	sf::RectangleShape movesqr;
	sf::Cursor hand, arrow;
	hand.loadFromSystem(sf::Cursor::Hand);
	arrow.loadFromSystem(sf::Cursor::Arrow);

	back_button.setPosition(board.getSize().x + (w.getSize().x - board.getSize().x) / 2, 50);
	rotate_button.setPosition(back_button.getPosition().x, back_button.getPosition().y + back_button.getGlobalBounds().height + 70);
	movesqr.setSize(sf::Vector2f(rotate_button.getGlobalBounds().width, rotate_button.getGlobalBounds().height));
	movesqr.setOrigin(movesqr.getSize().x / 2, 0);
	movesqr.setPosition(rotate_button.getPosition().x, rotate_button.getPosition().y + rotate_button.getGlobalBounds().height + 60);

	while (w.isOpen() && isGame) {
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(w);

		if (whoseMove == 'W')
			movesqr.setFillColor(sf::Color::White);
		else
			movesqr.setFillColor(sf::Color::Black);

		while (w.pollEvent(event)) {
			if (back_button.getGlobalBounds().contains(pos.x, pos.y)
				|| rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
				w.setMouseCursor(hand);
			}
			else
				w.setMouseCursor(arrow);

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					if (board.getGlobalBounds().contains(pos.x, pos.y)) {
						f_row = pos.x / (board.getSize().x / 8);
						f_col = 8 - (pos.y / (board.getSize().y / 8));
						if (rotation) {
							f_col = pos.y / (board.getSize().y / 8);
							f_row = 8 - (pos.x / (board.getSize().x / 8));
						}

						if (game_pos[f_row][f_col] != 0) {
							w.setMouseCursor(hand);
							move = true;
							f_moving = drawPiece(game_pos, board.getSize().x, f_row, f_col, rotation);
							sf::Vector2f sprite_size;
							sprite_size.x = (f_moving.getScale().x) * ((float)f_moving.getTexture()->getSize().x);
							sprite_size.y = (f_moving.getScale().y) * ((float)f_moving.getTexture()->getSize().y);
							dx = sprite_size.x / 2;
							dy = sprite_size.y / 2;
						}
					}
					else if (back_button.getGlobalBounds().contains(pos.x, pos.y)) {
						back_pressed = true;
					}
					else if (rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
						rotate_pressed = true;
					}
				}

			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.key.code == sf::Mouse::Left) {
					w.setMouseCursor(arrow);
					if (board.getGlobalBounds().contains(pos.x, pos.y)) {
						if (move) {
							setIsCapture();
							new_row = pos.x / (board.getSize().x / 8);
							new_col = 8 - (pos.y / (board.getSize().y / 8));
							if (rotation) {
								new_row = 8 - (pos.x / (board.getSize().x / 8));
								new_col = pos.y / (board.getSize().y / 8);
							}
							if (moveJudge(f_row, f_col, new_row, new_col)) {
								setPosition(f_row, f_col, new_row, new_col);
								if (isGameOver()) {
									Sleep(4000);/* isGame = false;*/
								}
							}
						}
					}
					else if (back_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (back_pressed) {
							isGame = false;
							back_pressed = false;
						}
					}
					else if (rotate_button.getGlobalBounds().contains(pos.x, pos.y)) {
						if (rotate_pressed) {
							rotation = !rotation;
							rotate_pressed = false;
						}
					}
					move = false;
					f_row = -2, f_col = -2;
				}
			}

			else if (event.type == sf::Event::Closed)
				w.close();
		}

		w.clear();
		w.draw(bg);
		w.draw(board);
		w.draw(back_button);
		w.draw(rotate_button);
		w.draw(movesqr);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (game_pos[i][j] != 0) {
					if (f_row > -2) {
						if (f_row == i && f_col == j)
							continue;
					}
					w.draw(drawPiece(game_pos, board.getSize().x, i, j, rotation));
				}
			}
		}

		if (move) {
			f_moving.setPosition(pos.x - dx, pos.y - dy);
			w.draw(f_moving);
		}

		w.display();
	}
}

bool CheckersGame::moveJudge(const int r1, const int c1, const int r2, const int c2) {
	if (r2 >= 0 && r2 <= 7 && c2 >= 0 && c2 <= 7) {
		if (whoseMove == game_pos[r1][c1]->whatColor()) {
			int zmp = (abs)(r2 - r1) + (abs)(c2 - c1);
			if (zmp == 2 && is_capture == true)
				return 0;

			if (game_pos[r1][c1]->correctMove(r1, c1, r2, c2, game_pos))
				return 1;
			else
				return 0;
		}

	}
	return 0;
}

void CheckersGame::setIsCapture(int r, int c) {
	if (r != -1) {
		for (int k = -2; k <= 2; k += 4) {
			for (int l = -2; l <= 2; l += 4) {
				if (game_pos[r][c]->correctMove(r, c, r + k, c + l, game_pos)) {
					is_capture = true; return;
				}
			}
		}
		is_capture = false; return;
	}

	int i = 0, j;
	for (j = 0; j < 8; j++) {
		for (i; i < 8; i += 2) {
			if (game_pos[i][j] != 0 && (game_pos[i][j]->whatColor() == whoseMove)) {
				for (int k = -2; k <= 2; k += 4) {
					for (int l = -2; l <= 2; l += 4) {
						if (game_pos[i][j]->correctMove(i, j, i + k, j + l, game_pos)) {
							is_capture = true; return;
						}
					}
				}
			}
		}
		if (j % 2 == 0)
			i = 1;
		else
			i = 0;
	}

	is_capture = false;
}

void CheckersGame::setPosition(int f_row, int f_col, int new_row, int new_col) {
	if ((new_col == 7 && whoseMove == 'W') ||
		(new_col == 0 && whoseMove == 'B'))
		game_pos[f_row][f_col] = new CheckerKing(whoseMove);

	int zmp = new_row - f_row;
	if (game_pos[f_row][f_col]->pieceName() == 'M' && (abs)(zmp) == 2) {
		int clrAddend = -1;
		if (whoseMove == 'B')
			clrAddend = 1;
		game_pos[f_row + (1 * (zmp / 2))][new_col + clrAddend] = 0;
	}
	if (game_pos[f_row][f_col]->pieceName() == 'K' && (abs)(zmp) == 2) {
		int r1 = f_row, r2 = new_row, c1 = f_col, c2 = new_col;
		if (r2 < r1) {
			int temp = r2;
			r2 = r1; r1 = temp;
		}
		if (c2 < c1) {
			int temp = c2;
			c2 = c1; c1 = temp;
		}
		game_pos[r2 - 1][c2 - 1] = 0;
	}
	game_pos[new_row][new_col] = game_pos[f_row][f_col];
	game_pos[f_row][f_col] = 0;

	if ((abs)(zmp) == 1)
		is_capture = false;
	else
		setIsCapture(new_row, new_col);
	if (!is_capture) {
		if (whoseMove == 'W')
			whoseMove = 'B';
		else
			whoseMove = 'W';
	}
}

bool CheckersGame::isGameOver() {
	int i = 0, j;
	for (j = 0; j < 8; j++) {
		for (i; i < 8; i += 2) {
			if (game_pos[i][j] != 0 && game_pos[i][j]->whatColor() == whoseMove) {
				for (int k = -2; k <= 2; k++) {
					if (k == 0)
						continue;
					for (int l = -2; l <= 2; l++) {
						if (l == 0)
							continue;
						if ((l == k || l == -k) && game_pos[i][j]->correctMove(i, j, i + k, j + l, game_pos))
							return 0;
					}
				}
			}
		}
		if (j % 2 == 0)
			i = 1;
		else
			i = 0;
	}
	return 1;

}
