#include "Piece.h"

Piece::Piece(char clr){
	color = clr;
}

bool Pawn::correctMove(int r1, int c1, int r2, int c2, ChessPiece* arr[8][8]) {
	char clr = whatColor();
	unsigned int en_pass = r2-r1;
	if (arr[r2][c2] == 0) {
		if (r1 == r2) {
			if (clr == 'W') {
				if (c2 - c1 == 1 || (c2 - c1 == 2 && (c1 == 1 && arr[r2][c2 - 1] == 0)))
					return 1;
				else
					return 0;
			}
			else {
				if (c2 - c1 == -1 || (c2 - c1 == -2 && (c1 == 6 && arr[r2][c2 + 1] == 0)))
					return 1;
				else
					return 0;
			}
		}
		/*else if (en_pass == 1) {
			if (clr == 'W') {
				if (arr[r2][c2-1])
			}
		}*/
		else
			return 0;
	}
	else {
		if (arr[r2][c2]->whatColor() != clr) {
			if ((r2 - r1) == -1 || (r2 - r1) == 1) {
				if ((clr == 'W' && c2 - c1 == 1) || (clr == 'B' && c2 - c1 == -1))
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}
		else
			return 0;
	}
}

bool Rook::correctMove(int r1, int c1, int r2, int c2, ChessPiece* arr[8][8]) {
	char clr = whatColor();
	if (arr[r2][c2] == 0 || arr[r2][c2]->whatColor() != clr) {
		if (r1 == r2) {
			int less = c1; int more = c2;
			if (c1 > c2) { less = c2; more = c1; }

			for (int i = less + 1; i < more; i++) {
				if (arr[r1][i] != 0)
					return 0;
			}
			return 1;
		}
		else if (c1 == c2) {
			int less = r1; int more = r2;
			if (r1 > r2) { less = r2; more = r1; }

			for (int i = less + 1; i < more; i++) {
				if (arr[i][c1] != 0)
					return 0;
			}
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

bool Knight::correctMove(int r1, int c1, int r2, int c2, ChessPiece* arr[8][8]) {
	char clr = whatColor();
	if (arr[r2][c2] == 0 || arr[r2][c2]->whatColor() != clr) {
		if ((r1 == r2 + 1) || (r1 == r2 - 1))
		{
			if ((c1 == c2 + 2) || (c1 == c2 - 2))
				return 1;
		}
		else if ((r1 == r2 + 2) || (r1 == r2 - 2))
		{
			if ((c1 == c2 + 1) || (c1 == c2 - 1))
				return 1;
		}
		else
			return 0;
	}
	return 0;
}

bool Bishop::correctMove(int r1, int c1, int r2, int c2, ChessPiece* arr[8][8]) {
	char clr = whatColor();
	int less_k = r1, more_k = r2, less_w = c1, more_w = c2;
	if (r1 > r2) { less_k = r2; more_k = r1; }
	if (c1 > c2) { less_w = c2; more_w = c1; }

	if ((arr[r2][c2] == 0 || arr[r2][c2]->whatColor() != clr) && (more_k - less_k == more_w - less_w)) {
		if (c1 > c2 && r1 > r2) {
			for (int i = 1; i < c1 - c2; i++) {
				if (arr[r2 + i][c2 + i] != 0)
					return 0;
			}
			return 1;
		}
		else if (c1 > c2 && r1 < r2) {
			for (int i = 1; i < c1 - c2; i++) {
				if (arr[r1 + i][c1 - i] != 0)
					return 0;
			}
			return 1;
		}
		else if (c1 < c2 && r1 > r2) {
			for (int i = 1; i < c2 - c1; i++) {
				if (arr[r2 + i][c2 - i] != 0)
					return 0;
			}
			return 1;
		}

		else if (c1 < c2 && r1 < r2) {
			for (int i = 1; i < more_k - less_k; i++) {
				if (arr[r1 + i][c1 + i] != 0)
					return 0;
			}
			return 1;
		}
	}
	else
		return 0;
}

bool Queen::correctMove(int r1, int c1, int r2, int c2, ChessPiece* arr[8][8]) {
	char clr = whatColor();
	arr[r1][c1] = nullptr;
	arr[r1][c1] = new Rook(clr);
	if (arr[r1][c1]->correctMove(r1, c1, r2, c2, arr)) {
		arr[r1][c1] = new Queen(clr);
		return 1;
	}

	arr[r1][c1] = nullptr;
	arr[r1][c1] = new Bishop(clr);
	if (arr[r1][c1]->correctMove(r1, c1, r2, c2, arr)) {
		arr[r1][c1] = new Queen(clr);
		return 1;
	}

	else {
		arr[r1][c1] = new Queen(clr);
		return 0;
	}
}

bool King::correctMove(int r1, int c1, int r2, int c2, ChessPiece* arr[8][8]) {
	char clr = whatColor();
	int zmp1 = r1 - r2, zmp2 = c1 - c2;
	if (zmp1 < 0) { zmp1 = -zmp1; } if (zmp2 < 0) { zmp2 = -zmp2; }

	if ((arr[r2][c2] == 0 || arr[r2][c2]->whatColor() != clr) && (zmp1 <= 1 && zmp2 <= 1)) {
		return 1;
	}
	if (arr[r1][c1]->firstmove == true && zmp2 == 0 && (arr[r2][c2] != 0) && arr[r2][c2]->pieceName() == 'R' && arr[r2][c2]->whatColor() == clr) {
		if (arr[r2][c2]->firstmove == true) {
			if (r2 < r1) {
				int temp = r2;
				r2 = r1;
				r1 = temp;
			}
			if (r2 - r1 > 1) {
				for (int i = r1 + 1; i < r2; i++) {
					if (arr[i][c1] != 0)
						return 0;
				}
			}
			return 1;
		}
	}
		return 0;
}

bool Man::correctMove(int r1, int c1, int r2, int c2, CheckersPiece* arr[8][8]){
	if (r2 >= 0 && r2 <= 7 && c2 >= 0 && c2 <= 7) {

		char clr = whatColor();
		int clrAddend = -1;
		if (clr == 'B')
			clrAddend = 1;

		if (c2 - c1 != -clrAddend && c2 - c1 != 2 * (-clrAddend))
			return 0;
		int zmp = r2 - r1;
		if ((abs)(zmp) <= 2 && arr[r2][c2] == 0) {
			if (c2 - c1 == -clrAddend && (abs)(zmp) == 1)
				return 1;
			if (arr[r1 + 1 * (zmp / 2)][c2 + clrAddend] != 0 && arr[r1 + 1 * (zmp / 2)][c2 + clrAddend]->whatColor() != clr) {
				return 1;
			}
			return 0;
		}
	}
	return 0;

}

bool CheckerKing::correctMove(int r1, int c1, int r2, int c2, CheckersPiece* arr[8][8]){
	if (r2 >= 0 && r2 <= 7 && c2 >= 0 && c2 <= 7) {
		char clr = whatColor();
		int zmp = r2 - r1;
		if ((abs)(zmp) <= 2 && arr[r2][c2] == 0) {
			if ((abs)(c2 - c1) == 1  && (abs)(zmp) == 1)
				return 1;
			if (r2 < r1) {
				int temp = r2;
				r2 = r1; r1 = temp;
			}
			if (c2 < c1) {
				int temp = c2;
				c2 = c1; c1 = temp;
			}
			if (arr[r2-1][c2-1] != 0 && arr[r2-1][c2-1]->whatColor() != clr) {
				return 1;
			}
			return 0;
		}
	}
	return 0;
}
