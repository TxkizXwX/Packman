﻿#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <thread>
#include <conio.h>

#define FOREGROUND_YELLOW 14

using namespace std;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

eDirection dir;

void gotoXY(int x, int y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(output, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(output, &cursorInfo);
}

pair<int, int> sizeMap() {
    ifstream fin;
    fin.open("map.txt");
    char ch;
    int a = 0, b = 0;
    while (fin.get(ch)) {
        if (b < 1)
            a++;
        if (ch == '\n')
            b++;
    }
    fin.close();

    return make_pair(a, b);
}

void mapFill(char** map, const int width, const int height) {

    ifstream fin1;
    fin1.open("map.txt");
    char ch;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fin1.get(ch);
            map[i][j] = ch;
        }
    }
    fin1.close();
}

void mapShow(char** map, const int width, const int height) {
    //Color
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_BLUE);

    gotoXY(0, 0);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << map[i][j];
        }
    }
}

void PacmanShow(int x, int y) {
    // Color
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_YELLOW);

    gotoXY(x, y);
    cout << '0';
}

void Input() {
    if (_kbhit())
    {
        switch (_getch()) {
        case 's':
            dir = DOWN;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 'a':
            dir = LEFT;
            break;
        }
    }
}

void Logic(char** map, int& xPos, int& yPos, int& count){
    switch (dir) {
    case UP:
        if (map[yPos - 1][xPos] == '#') break;
        yPos--;
        break;
    case DOWN:
        if (map[yPos + 1][xPos] == '#') break;
        yPos++;
        break;
    case LEFT:
        if (map[yPos][xPos - 1] == '#') break;
        xPos--;
        break;
    case RIGHT:
        if (map[yPos][xPos + 1] == '#') break;
        xPos++;
        break;
    }
    if (map[yPos][xPos] == '*') {
        map[yPos][xPos] = ' ';
        count++;
    }
}

void countShow(int width, int height, int count) {
    // Color
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_RED);
    gotoXY(width, height + 100);
    cout << "Count: " << count;
}

int main()
{
    bool start = false;
    // Создание карты
    pair<int, int> size = sizeMap();
    int width = size.first;
    int height = size.second;
    char** map = new char* [height];
    for (int i = 0; i < height; i++)
    {
        map[i] = new char[width];
    }
    mapFill(map, width, height);
    int xPos = width / 2, yPos = height / 2;
    int count = 0;

    //Start Game Menu
    if (start != true) {
        while (true)
        {
            gotoXY(0, 0);
            cout << "Press Key to START GAME!";
            if (_kbhit()) {
                start = true;
                break;
            }
        }
    }
    // Game Started
    while (true)
    {
        system("cls");
        mapShow(map, width, height);
        countShow(width, height, count);
        PacmanShow(xPos, yPos);
        Input();
        Logic(map, xPos, yPos, count);
        this_thread::sleep_for(0.1s);
    }

    return 0;
}