#ifndef LAB_2_G_H
#define LAB_2_G_H

#include <string>

using namespace std;

/*
 * Терминалы/нетерминалы, по сути грамматика
 */
 // P.S. не предусмотрено использование EMPTY
enum G
{
	WHILE, DONE, ID, flt, GT, LT, EQ, COLON, L_B, R_B, ASGN, END, ERR, // терминалы
	S, S1, S2, C, R, B, I, I1, F    // нетерминалы
};

string GToString(G t) // преобразование элемента в строку
{
	switch (t)
	{
	case WHILE:
		return "WHILE";
	case DONE:
		return "DONE";
	case ID:
		return "ID";
	case flt:
		return "flt";
	case GT:
		return "GT";
	case LT:
		return "LT";
	case EQ:
		return "EQ";
	case L_B:
		return "L_B";
	case R_B:
		return "R_B";
	case COLON:
		return "COLON";
	case ASGN:
		return "ASGN";
	case END:
		return "END";
	case ERR:
		return "ERR";
	case S:
		return "S";
	case S1:
		return "S1";
	case S2:
		return "S2";
	case C:
		return "C";
	case R:
		return "R";
	case B:
		return "B";
	case I:
		return "I";
	case I1:
		return "I1";
	case F:
		return "F";
	}
}

// сдвиг для НЕтерминального, относительно терминальных состояний
int nTermIndex(G nterm) { return nterm - 13; }

bool isTerminal(G x) // проверка - является ли x терминалом
{
	return (
		x == WHILE || x == DONE || x == ID ||
		x == GT || x == LT || x == EQ ||
		x == flt || x == ASGN || x == END ||
		x == L_B || x == R_B || x == COLON
		);
}

#endif //LAB_2_G_H