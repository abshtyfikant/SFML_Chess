#ifndef GAME_H
#define GAME_H

#include "Piece.h"
//#include "StockfishConnector.h"
//#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <time.h>
#include <iostream>
#include <windows.h>
#include <atlconv.h>
#include <thread>
#include <semaphore>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

void setSemaphore(std::binary_semaphore&);

void sconnect(sf::TcpSocket& sock, sf::IpAddress ip, int p);

void drawLoad(sf::RenderWindow& w, sf::Sprite bg, sf::Sprite w1, sf::Sprite w2);

struct arrPosition {
    ChessPiece* aux_arr[8][8];
};

class Game {
protected:
    sf::Texture t[15], r[3], chb;
    sf::Sprite fig[12], back_button, rewind_button, rotate_button, res_info[3];

public:
    Game() {}
    virtual void gameReset() = 0;
    virtual void gameInit() = 0;
    virtual void gameLoop(sf::RenderWindow&, sf::RectangleShape&, sf::Sprite bg) = 0;
    virtual bool moveJudge(const int, const int, const int, const int) = 0;
};

class ChessGame : public Game
{
public:
    ChessPiece* game_pos[8][8];
    std::vector<arrPosition> game_storage;
    std::string pos = "";

    ChessGame();
    char whoseMove;
    virtual void gameReset();
    virtual void gameInit();
    sf::Sprite drawPiece(ChessPiece* arr[8][8], int b_size, int i, int j, bool);
    virtual void gameLoop(sf::RenderWindow&, sf::RectangleShape&, sf::Sprite bg);
    void gameLoopOnline(sf::RenderWindow&, sf::RectangleShape&, sf::Sprite bg);
    bool isCheck(ChessPiece* arr[8][8]);
    bool isStalemate();
    bool isCheckmate();
    bool wasCastle = false;
    void moveMaker(int, int, int, int);
    virtual bool moveJudge(const int r1, const int c1, const int r2, const int c2);
    void storePos(ChessPiece* arr[8][8]);
    void previousPos();
    void makeCompMove(int, int, int, int);
    std::string moveString(int, int, int, int);
    void makeStringMove(std::string);

    friend void drawLoad(sf::RenderWindow& w, sf::Sprite bg, sf::Sprite w1, sf::Sprite w2);
    friend void sconnect(sf::TcpSocket& sock, sf::IpAddress ip, int p);
};

class FischerGame : public ChessGame
{
public:
    FischerGame();
    virtual void gameReset();
};

class CheckersGame : public Game
{
public:
    CheckersGame();
    bool is_capture;
    CheckersPiece* game_pos[8][8];
    char whoseMove;
    virtual void gameReset();
    virtual void gameInit();
    sf::Sprite drawPiece(CheckersPiece* arr[8][8], int b_size, int i, int j, bool);
    virtual void gameLoop(sf::RenderWindow&, sf::RectangleShape&, sf::Sprite bg);
    virtual bool moveJudge(const int r1, const int c1, const int r2, const int c2);
    void setIsCapture(int r = -1, int c = -1);
    void setPosition(int, int, int, int);
    bool isGameOver();
};

#endif