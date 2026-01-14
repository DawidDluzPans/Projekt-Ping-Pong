#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h> // dla _kbhit() i _getch()
#include <windows.h>
#include "Deklaracje.h"
using namespace std;

GameObject::GameObject(int x, int y, int w, int h)
    : x(x), y(y), width(w), height(h) {}

//tu proste jedna linijka i baza
int GameObject::getX() const { return x; }
int GameObject::getY() const { return y; }
int GameObject::getHeight() const { return height; }


//implementacja paddle
Paddle::Paddle(int x, int y, int w, int h, int speed) :GameObject(x, y, w, h), speed(speed) {}

void Paddle::moveUp()
{
    y -= speed;
}

void Paddle::moveDown()
{
    y += speed;
}

void Paddle::update()
{
    // sterowanie z klasy Game
}

int Paddle::getCenterY() const
{
    return y + height / 2;
}

void Paddle::draw()
{
    cout << "Paddle at (" << x << ", " << y << ")" << endl;
}


Ball::Ball(int x, int y, int size, int dx, int dy)
    : GameObject(x, y, size, size), dx(dx), dy(dy) {}

void Ball::update()
{
    x += dx;
    y += dy;
}

void Ball::invertX()
{
    dx = -dx;
}

void Ball::invertY()
{
    dy = -dy;
}

void Ball::draw()
{
    cout << "Ball at (" << x << ", " << y << ")" << endl;
}


Game::Game()
    : screenWidth(80),
    screenHeight(25),
    trwaGra(true),
    leftPaddle(1, 10, 1, 5, 1),
    rightPaddle(79, 10, 1, 5, 1),
    ball(40, 12, 1, -1, 1), // 1 runda pilka leci w prawo w dol
    scoreLeft(0),
    scoreRight(0)
{}

void Game::run() {
    while (trwaGra) {
        getInput();
        updateGame();
        checkCollisions();
        render();
    }
}

void Game::updateGame() {
    leftPaddle.update();
    rightPaddle.update();
    ball.update();
}

//bez tego pojawia siê pointer tekstowy i strasznie irytuje przy dzia³aniu gry
void clearScreen() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = { 0, 0 };
    SetConsoleCursorPosition(hOut, topLeft);
}

void Game::render() {
    clearScreen();

    // Tworzymy ekran
    char ekran[25][80]; // screenHeight x screenWidth
    for (int i = 0; i < screenHeight; i++)
        for (int j = 0; j < screenWidth; j++)
            ekran[i][j] = ' '; // t³o

    //ograniczenie okna gry
    for (int j = 0; j < screenWidth; j++) {
        ekran[0][j] = '-';                     // sufit
        ekran[screenHeight - 1][j] = '-';      // pod³oga
    }

    //rysowanie lewej paletki
    for (int i = 0; i < leftPaddle.getHeight(); i++) {
        int y = leftPaddle.getY() + i;
        int x = leftPaddle.getX();
        if (y > 0 && y < screenHeight - 1 && x >= 0 && x < screenWidth)
            ekran[y][x] = '|';
    }

    //rysowanie prawej paletki
    for (int i = 0; i < rightPaddle.getHeight(); i++) {
        int y = rightPaddle.getY() + i;
        int x = rightPaddle.getX();
        if (y > 0 && y < screenHeight - 1 && x >= 0 && x < screenWidth)
            ekran[y][x] = '|';
    }

    //i rysowanie pilki  
    int ballX = ball.getX();
    int ballY = ball.getY();
    if (ballY > 0 && ballY < screenHeight - 1 && ballX >= 0 && ballX < screenWidth)
        ekran[ballY][ballX] = 'O';

    //wysw scoreboardu
    cout<<"Sterowanie: Lewa paletka W gora, S dol; Prawa paletka O gora, L dol" << endl;
    cout << "Wynik: Left: " << scoreLeft << "  Right: " << scoreRight << endl << endl;

    //wyswietlenie calego ekranu
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++)
            cout << ekran[i][j];
        cout << endl;
    }
}

void Game::checkCollisions() {

    //Kolizja pi³ki góra dó³
    //góra okna
    if (ball.getY() <= 0)
    {
        ball.invertY();
        ball.update();
    }
    //dó³ okna
    if (ball.getY() + ball.getHeight() >= screenHeight)
    {
        ball.invertY();
        ball.update();
    }

    //Kolizja paletek góra dó³ - nie mog¹ wyjœæ przecie¿ za okno!
    //lewa paletka
    //góra
    if (leftPaddle.getY() <= 0)
    {
        leftPaddle.moveDown(); //cofniêcie ruchu
    }
    if (leftPaddle.getY() + leftPaddle.getHeight() > screenHeight - 1) // -1 bo nagle z 5 kreskowej paletki robi³a siê 4, mimo moveup po tym
    {
        leftPaddle.moveUp(); //cofniêcie ruchu
    }

    //prawa paletka
    if (rightPaddle.getY() <= 0)
    {
        rightPaddle.moveDown(); //cofniêcie ruchu
    }
    if (rightPaddle.getY() + rightPaddle.getHeight() > screenHeight - 1) //analogicznie jak wy¿ej
    {
        rightPaddle.moveUp(); //cofniêcie ruchu
    }


    // kolizje paletek z pi³k¹
    // 
    // lewa paletka
    if (ball.getX() <= leftPaddle.getX() &&
        ball.getX() >= leftPaddle.getX() &&
        ball.getY() >= leftPaddle.getY() &&
        ball.getY() <= leftPaddle.getY() + leftPaddle.getHeight()) {
        ball.invertX();
    }

    // prawa paletka
    if (ball.getX()  >= rightPaddle.getX() &&
        ball.getX()  <= rightPaddle.getX() &&
        ball.getY() + ball.getHeight() >= rightPaddle.getY() &&
        ball.getY() <= rightPaddle.getY() + rightPaddle.getHeight()) {
        ball.invertX();
    }

    //no i brak kolizji - czyli punktowanko
    // pi³ka minê³a lew¹ paletkê
    if (ball.getX() < 0) {
        // punkt dla prawego gracza
        scoreRight++;
        ball = Ball(screenWidth / 2, screenHeight / 2, 1, 1, 1); // reset
    }

    // pi³ka minê³a praw¹ paletkê
    if (ball.getX() > screenWidth) {
        // punkt dla lewego gracza
        scoreLeft++;
        ball = Ball(screenWidth / 2, screenHeight / 2, 1, -1, 1); // reset
    }



}

void Game::getInput() {
    if (_kbhit()) {       // jeœli wciœniêto klawisz
        char key = _getch(); //pobiera char i nie trzeba wtedy entera wciskac

        // lewa paletka
        if (key == 'w') leftPaddle.moveUp();
        if (key == 's') leftPaddle.moveDown();

        // prawa paletka
        if (key == 'o') rightPaddle.moveUp();
        if (key == 'l') rightPaddle.moveDown();

        // zakoñczenie gry
        if (key == 'q') trwaGra = false;
    }
}

void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE; // ukrycie kursora
    SetConsoleCursorInfo(hOut, &cursorInfo);
}