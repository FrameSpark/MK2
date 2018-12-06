#include <iostream>
#include <fstream>
#include "Lex.h"
#include "Syntax.h"


int main()
{
    bool exit = false;
    fstream fin;
    string filename = "input.txt";
    while (true) {
        cout << "\n\nAnalyze file " << filename << "? [Yes - 1, No - 0]: ";
        cin >> exit;
        if (!exit)
            return 0;
        cout << endl;

        string input;
        Lex lex;
        Syntax syn;

        fin.open("C:\\Users\\Mrsla\\Desktop\\CM2-master\\input.txt", ios::in);
        cout << "Input text: " << endl;

        while (fin.is_open() && !fin.eof()) {
            string temp;
            fin >> temp;
            input += temp + " ";
        }
        cout << input << endl;

        if (lex.analyze(input))// если лексический анализ прошел успешно
            if (syn.analyze(lex.getTokens()))  // если синтаксический анализ прошел успешно
            {
                cout << endl;
                cout << "Analyze complete!\n";
                cout << "Line: \n";
                syn.displayOut();
                cout << "\nTree: \n";
                syn.displayTree();
/*
                  cout << "\nSTR table: ";
                    queue<string> table = lex.getStrTable();
                    while (!table.empty()) {
                        cout << table.front() << " ";
                        table.pop();
                    }

                    cout << "\nID table: ";
                    table = lex.getIdTable();
                    while (!table.empty()) {
                        cout << table.front() << " ";
                        table.pop();
                    } */
            }
            else
                cout << "Syntax error!\n";
        else
            cout << "Lexical error!\n";
        fin.close();
    }
}