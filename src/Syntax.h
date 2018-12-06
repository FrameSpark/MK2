#ifndef LAB_2_SYNTAX_H
#define LAB_2_SYNTAX_H

#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include "G.h"
#include "GTrie.h"

using namespace std;

/*
  LL анализатор, сверху-вниз (предсказывающий разбор).

  Стек -  набор правил (в начале какое-то начальное состояние)
  Вход -  набор токенов, 2 таблицы для ID и HEX
	   Таблица ID, HEX  (в виде списка). В синтаксическом если встречаем
	   очередной ID или HEX, то лезем в эти таблицы соотв.
  Выход - Дерево в идеале в виде структуры, выводить можно как получится (ex winapi).
	   Можно и строкой, но в рассчете на последнюю лабу.

  Таблица анализатора - составляется с помощью функций first/follow.


 */
class Syntax
{
    stack<G> stack;              // "Магазин"
    vector<G> table[9][12];     // таблица анализатора с правилами в каждой ячейке набор.

    queue<G> out;               // выходная строка
    GTrie *trie;                // дерево разбора

    void manualTable(); // задание таблицы вручную
    void autoTable(vector<G> rules); // автоматическое создание таблицы по набору правил.
    void displayTable(); // вывод таблицы на экран
    void displayQueue(queue<G> Q); // вывод очереди

public:
    Syntax();

    bool analyze(queue<G> tokens);

    void displayOut() { displayQueue(out); } // вывод на экран строки
    void displayTree() { trie->print(); }

    queue<G> getOut() { return out; }   // возвращает строку
    GTrie *getTree() { return trie; }    // возвращает дерево
    void error(string msg) { cout << "\nError in: " << msg << endl; }
};



Syntax::Syntax() : stack(), table(), out()
{
    manualTable();
    trie = new GTrie();
}

void Syntax::manualTable() // создаем таблицу анализатора по шаблону
{
    // позиции для изменения
    int pos[15][2] = {
            {0, 0},
            {1, 1},
            {1, 2},
            {2, 1}, {2, 7}, {2, 11},
            {3, 8},
            {4, 9},
            {5, 2},
            {6, 2},
            {7, 2}, {7, 3},
            {8,	4}, {8,	5}, {8,	6}
    };

    int sizes[16] = { 2   , 1 , 1,    2     ,2		 ,   1  ,      5	      ,	  2  ,			4       , 1 , 1 ,  1  , 1 , 1 , 1 };
    G values[27] = { WHILE, C,  S2, B, DONE, S2, COLON, S, END, L_B, ID, F, I, R, R_B, B, ID, ASGN, I1, S1, ID, ID, flt, GT, LT, EQ };

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 12; ++j)
            table[i][j].push_back(ERR);  // пустые ячейки заполнены ERR

    int iter = 0, j;
    for (int i = 0; i < 14; ++i) {
        table[pos[i][0]][pos[i][1]].pop_back(); // освобождаем место (от ERR) под изменения
        for (j = 0; j < sizes[i]; ++j)
            table[pos[i][0]][pos[i][1]].push_back(values[iter + j]); // записываем в таблицу
        iter += j;
    }
}

void Syntax::autoTable(vector<G> rules)  // автоматическое создание таблицы по набору правил
{
    /*
     *  Есть идея по поводу задания таблиц: смотрим по первому элементу в правиле –
        1) если терминал, то ставим в соотв с таким же столбцом,
        2) если нетерм, то идем в это правило и смотрим там первый элемент
        3) повторяем 1-2)
        Когда в правиле несколько подправил, то выставляем на всех уровнях выше по каждому такому же правилу (с применением п 1, 2)
        Все выставленные правила убираются из очереди
     */


}

void Syntax::displayTable()
{
    for (int i = 0; i < 9; ++i) // DEBUG вывод таблицы анализатора
    {
        for (int j = 0; j < 12; ++j)
            cout << GToString(table[i][j].front()) << "   \t"; // вывод по одному(!) символу в ячейке

        cout << endl;
    }
}

void Syntax::displayQueue(queue<G> Q) // вывод очереди в консоль
{
    while (!Q.empty())
    {
        cout << GToString(Q.front()) << " ";
        Q.pop();
    }
    cout << endl;
}

bool Syntax::analyze(queue<G> tokens) {
    //displayQueue(tokens);

    stack.push(END); // эквивалентно символу $
    stack.push(S);   // начальное состояние
    out.push(S);
    trie->insert(out);

    do // основной цикл
    {
        G x = stack.top();
        int tr;

        if (isTerminal(x))  // если в верхушке стека был терминал
            if (x == tokens.front())
            {
                stack.pop(); // удаляем x из стека
                tokens.pop(); // удаляем из входной строки токен
            }
            else {
                error(GToString(tokens.front()));
                return false;
            }
        else
        {
            // если нетерминал
            // [нетерм из стека] [входной токен (всегда term по идее)].
            tr = table[nTermIndex(x)][tokens.front()].front();
            if (tr != ERR) // Элемент (первый) не является ошибкой
            {
                stack.pop(); // удаляем x из стека
                // поместить Yk,Yk-1,...Y1 в магазин (Y1 на верхушку);
                for (int i = table[nTermIndex(x)][tokens.front()].size() - 1; i >= 0; i--) // просматриваем вектор поэлементно (с конца)
                    stack.push(table[nTermIndex(x)][tokens.front()][i]); // выкладываем в стек правило наоборот

                string temp = "";
                // вывести правило X->Y1Y2...Yk;
                trie->insert(table[nTermIndex(x)][tokens.front()]); // добавляем правило в дерево
                for (G i : table[nTermIndex(x)][tokens.front()]) // добавляем правило в строку
                    out.push(i);

            }
            else { // если в таблице попали на ERR
                error(GToString(tokens.front()));
                return false;
            }
        }
    } while (stack.top() != END); // пока не закончился стек

    return tokens.front() == END; // если строка закончилась - true. иначе - false
}

#endif //LAB_2_SYNTAX_H