#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

class Piece{
	char color;

public:
	Piece(char);
	char whatColor() { return color; }
};

//Chess
class ChessPiece : public Piece {
public:
	ChessPiece(char color) : Piece(color) {}
	virtual bool correctMove(int, int, int, int, ChessPiece* arr[8][8]) = 0;
	virtual char pieceName() = 0;
	bool firstmove = true;
};

class Pawn : public ChessPiece {
public:
	Pawn(char color) : ChessPiece(color) {}
	virtual bool correctMove(int, int, int, int, ChessPiece* arr[8][8]);
	virtual char pieceName() { return 'P'; }
};

class Rook : public ChessPiece {
public:
	Rook(char color) : ChessPiece(color) {}
	virtual bool correctMove(int, int, int, int, ChessPiece* arr[8][8]);
	virtual char pieceName() { return 'R'; }
	bool firstmove = true;
};

class Knight : public ChessPiece {
public:
	Knight(char color) : ChessPiece(color) {}
	virtual bool correctMove(int, int, int, int, ChessPiece* arr[8][8]);
	virtual char pieceName() { return 'N'; }
};

class Bishop : public ChessPiece {
public:
	Bishop(char color) : ChessPiece(color) {}
	virtual bool correctMove(int, int, int, int, ChessPiece* arr[8][8]);
	virtual char pieceName() { return 'B'; }
};

class Queen : public ChessPiece {
public:
	Queen(char color) : ChessPiece(color) {}
	virtual bool correctMove(int, int, int, int, ChessPiece* arr[8][8]);
	virtual char pieceName() { return 'Q'; }
};

class King : public ChessPiece {
public:
	King(char color) : ChessPiece(color) {}
	virtual bool correctMove(int, int, int, int, ChessPiece* arr[8][8]);
	virtual char pieceName() { return 'K'; }
	bool firstmove = true;
};

//Checkers
class CheckersPiece : public Piece {
public:
	CheckersPiece(char color) : Piece(color) {}
	virtual bool correctMove(int, int, int, int, CheckersPiece* arr[8][8]) = 0;
	virtual char pieceName() = 0;
};

class Man : public CheckersPiece {
public:
	Man(char color) : CheckersPiece(color) {}
	virtual bool correctMove(int, int, int, int, CheckersPiece* arr[8][8]);
	virtual char pieceName() { return 'M'; }
};

class CheckerKing : public CheckersPiece {
public:
	CheckerKing(char color) : CheckersPiece(color) {}
	virtual bool correctMove(int, int, int, int, CheckersPiece* arr[8][8]);
	virtual char pieceName() { return 'K'; }
};

#endif