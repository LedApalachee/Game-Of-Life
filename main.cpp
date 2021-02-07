#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

const int fieldSizeX = 40;
const int fieldSizeY = 40;
bool field[fieldSizeX][fieldSizeY];

int CursorPosX = fieldSizeX/2;
int CursorPosY = fieldSizeY/2;

bool getCell(int posX, int posY)
{
    if (posX < 0 && posY >= 0 && posY < fieldSizeY) return getCell(fieldSizeX+posX, posY);
    if (posX >= 0 && posX < fieldSizeX && posY < 0) return getCell(posX, fieldSizeY+posY);
    if (posX >= fieldSizeX && posY >= 0 && posY < fieldSizeY) return getCell(posX-fieldSizeX, posY);
    if (posX >= 0 && posX < fieldSizeX && posY >= fieldSizeY) return getCell(posX, posY-fieldSizeY);
    if (posX < 0 && posY < 0) return getCell(fieldSizeX+posX, fieldSizeY+posY);
    if (posX >= fieldSizeX && posY >= fieldSizeY) return getCell(posX-fieldSizeX, posY-fieldSizeY);
    return field[posX][posY];
}

int getAround(int posX, int posY)
{
    int result = 0;
    for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y)
            if (getCell(posX+x, posY+y) && !(x == 0 && y == 0)) ++result;
    return result;
}

void Draw()
{
    std::string render = "";
    for (int i = 0; i < fieldSizeX + 2; ++i)
        render += '#';
    render += '\n';

    for (int i = fieldSizeY-1; i >= 0; --i)
    {
        render += '#';
        for(int j = 0; j < fieldSizeX; ++j)
        {
            if (j == CursorPosX && i == CursorPosY)
            {
                render += '?';
                continue;
            }
            render += (field[j][i] ? 'O' : ' ');
        }
        render += "#\n";
    }

    for (int i = 0; i < fieldSizeX + 2; ++i)
        render += '#';
    render += '\n';
    std::cout << render << "\ni - info.\n";
}

void Logic()
{
    bool nextgen[fieldSizeX][fieldSizeY];
    for (int x = 0; x < fieldSizeX; ++x)
        for (int y = 0; y < fieldSizeY; ++y)
            if ((!field[x][y] && getAround(x,y) == 3) || (field[x][y] && (getAround(x,y) == 3 || getAround(x,y) == 2))) 
                nextgen[x][y] = true;
            else nextgen[x][y] = false;
    for (int x = 0; x < fieldSizeX; ++x)
        for (int y = 0; y < fieldSizeY; ++y)
            field[x][y] = nextgen[x][y];
}

void clear()
{
    for (int i = 0; i < fieldSizeX; ++i)
        for (int j = 0; j < fieldSizeY; ++j)
            field[i][j] = false;
}

void showInfo()
{
    system("cls");
    std::cout << "wasd - move cursor\n" <<
                  "f - put alive cell\n" <<
                  "e - erase alive cell\n" <<
                  "c - clear field\n" <<
                  "p - pause/run\n" <<
                  "n - next turn (during the pause)\n" <<
                  "q - quit\n";
    getch();
    system("cls");
}

int main()
{
    system("cls");
    bool pause = true;
    while(true)
    {
        Draw();
        std::cout << CursorPosX << ' ' << CursorPosY << '\n';
        std::cout << (field[CursorPosX][CursorPosY] ? "alive" : "dead") << '\n';
        if (pause) std::cout << "\nPAUSED.\n";
        if (kbhit() || pause)
        {
            switch (getch())
            {
                case 'w':
                    if (CursorPosY < fieldSizeY-1) ++CursorPosY;
                    else CursorPosY = 0;
                    break;
                case 's':
                    if (CursorPosY > 0) --CursorPosY;
                    else CursorPosY = fieldSizeY-1;
                    break;
                case 'a':
                    if (CursorPosX > 0) --CursorPosX;
                    else CursorPosX = fieldSizeX-1;
                    break;
                case 'd':
                    if (CursorPosX < fieldSizeX-1) ++CursorPosX;
                    else CursorPosX = 0;
                    break;
                case 'f':
                    field[CursorPosX][CursorPosY] = true;
                    break;
                case 'p':
                    pause = !pause;
                    break;
                case 'c':
                    clear();
                    break;
                case 'e':
                    field[CursorPosX][CursorPosY] = false;
                    break;
                case 'n':
                    if (pause) Logic();
                    break;
                case 'i':
                    showInfo();
                    break;
                case 'q':
                    return 0;
            }
        }
        if (!pause) Logic();
        if (!pause) Sleep(75);
        system("cls");
    }

    return 0;
}