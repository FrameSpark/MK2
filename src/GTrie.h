#ifndef LAB_2_TRIE_H
#define LAB_2_TRIE_H

#include <iostream>
#include <string>
#include <queue>
#include "G.h"

using namespace std;

/*
 * Trie-дерево из enum G
 * Вставка осуществляется в в первый попавшийся НЕтерминал с child == NULL
 * Листья выделяются символом #, они всегда будут терминалами
 */
class GTrie
{
	struct Node //узел trie дерева
	{
		G item;
		Node *other; //указатель на брата
		Node *child; //указатель на потомка
	};
	Node *root;

	Node *createEmptyNode();                // создание путого узла
	void insert(Node *&N, queue<G> &que);   // основная функция вставки
	void print(Node *N, int level);         // основная функция вывода в консоль

public:
	GTrie() { root = nullptr; }

	// перегрузка вставки
	void insert(vector<G> que);
	void insert(queue<G> que);

	void print() { print(root, 0); } // вывод дерева "на бок"
};

GTrie::Node *GTrie::createEmptyNode() //создание пустого узла
{
	Node *node;
	node = new Node;

	node->item = ERR;
	//обнуляем указатели
	node->other = nullptr;
	node->child = nullptr;
	return node; //возвращаем указатель на созданный узел
}

void GTrie::insert(GTrie::Node *&N, queue<G> &que) // вставка
{
	// обход всего дерева рекурсивно
	// если очередь пуста, то возвращаемся
	for (Node *node = N; node != nullptr; node = node->other) //проход по списку соседних элементов
	{
		if (que.empty()) // если вектор пуст, то возвращаемся
			return;

		if (node->child == nullptr && node->item != ERR && !isTerminal(node->item)) // если нашли место для вставки и встретили НЕтерминал
		{
			// вставляем данные из очереди в node->child
			node->child = createEmptyNode();
			node->child->item = que.front();
			que.pop();

			Node *temp = node->child;
			while (!que.empty())
			{
				temp->other = createEmptyNode();  // создаем узел
				temp->other->item = que.front();  // записываем значение из очереди
				que.pop();              // удаление элемента
				temp = temp->other; // двигаемся вправо
			}
			return;
		}

		if (node->child != nullptr) // двигаемся к потомку
			insert(node->child, que);
	}
}

void GTrie::print(GTrie::Node *N, int level) //функция вывода дерева по уровням
{
	//проход по списку братьев
	for (Node *node = N; node != nullptr; node = node->other)
	{
		//добавляем сдвиг строки на уровень
		for (int i = 0; i < level; i++)
			cout << "  ";
		cout << "--";

		//если item - терминал, то выводим последний item и знак листа
		if (isTerminal(node->item))
			cout << GToString(node->item) << " #" << endl; //вывод как дерево
		else //если только item, то выводим его
			cout << GToString(node->item) << endl;

		//двигаемся к потомку, если указатель на него не нулевой
		if (node->child != nullptr)
			print(node->child, level + 1);
	}
}

void GTrie::insert(vector<G> que) // преобразование из вектора в queue
{
	queue<G> temp;
	for (G i : que)
		temp.push(i); // преобразование к очереди
	insert(temp);
}

void GTrie::insert(queue<G> que) // вставка в дерево
{
	if (root == nullptr) // создание пустого дерева
	{
		root = createEmptyNode();
		root->item = que.front();
		que.pop();
	}

	insert(root, que);
}


#endif //LAB_2_TRIE_H