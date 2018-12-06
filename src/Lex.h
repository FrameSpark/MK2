#ifndef LAB_2_LEX_H
#define LAB_2_LEX_H

#include <string>
#include <iostream>
#include <queue>
#include "G.h"

using namespace std;

class Lex
{
	queue<G> tokens;
	string debugout;  // DEBUG ONLY. string of G

	// таблицы для семантического анализатора
	queue<string> idTable; // соотв ID
	queue<string> strTable; // соотв HEX

	G getToken(string item);

public:
	Lex() : tokens(), idTable(), strTable() { debugout = ""; }

	// ввод всех слов строго через пробел
	bool analyze(string line);

	queue<G> getTokens() { return tokens; }
	string getStringTokens() { return debugout; }

	queue<string> getIdTable() { return idTable; }
	queue<string> getStrTable() { return strTable; }
};

bool checkFloat(string item, int n)
{
    for(int i=0; i<n; i++)
    {
        if(!isdigit(item[i]) && item[i] != '-' && item[i] !='+' && item[i] !='E' && item[i] !='e' && item[i] !='.')
            return false;
    }
    return true;
}

G Lex::getToken(string item) {

	if (item == "while")
		return WHILE;
	if (item == "done")
		return DONE;
    if (checkFloat(item,item.length()))
        return flt;
	if (item == "id")
		return ID;
	if (item == ">")
		return GT;
	if (item == "<")
		return LT;
	if (item == "=")
		return EQ;
	if (item == ";")
		return COLON;
	if (item == "(")
		return L_B;
	if (item == ")")
		return R_B;

	if (isalpha(item[0])) // если 1-й символ - буква
	{
	    if (isdigit(item[0]) )
            return ERR;
		for (int i = 1; i < item.size(); ++i)
			if (!isalpha(item[i]) && !isdigit(item[i]))
				return ERR; // если встретили неподходящий символ в идентификаторе => ошибка
		return ID;
	}

	// читаем строку
	if (item[0] == '"')
	{
		for (int i = 1; i < item.size(); ++i)
			if (!isalpha(item[i]) && !isdigit(item[i]))
				return ERR; // если встретили неподходящий символ в идентификаторе => ошибка
		return flt;
	}

	if (item == ":=")
		return ASGN;

	return ERR;
}

bool Lex::analyze(string line) {
	int k = 0;

	while (line[k] != '\0') // разбор строки на слова
	{
		string item = "";
		while (line[k] != ' ' && line[k] != '\0')
		{
			item += line[k];
			k++;
		}
		if (line[k] != '\0')
			k++;
		if (getToken(item) == ERR) {
			cout << endl << "\nError in: " << item << endl;
			return false; // встретили недопустимый символ
		}
		tokens.push(getToken(item)); // накапливаем токены

		// записываем данные ID и flt в таблицы
		if (getToken(item) == ID)
			idTable.push(item);
		if (getToken(item) == flt)
			strTable.push(item);

		// DEBUG only
		debugout += GToString(getToken(item));
		debugout += ' ';
	}
	tokens.push(END);
	debugout += GToString(END);
	return true;
}


#endif //LAB_2_LEX_H