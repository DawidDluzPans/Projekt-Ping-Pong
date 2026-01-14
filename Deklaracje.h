#pragma once
using namespace std;

//DEKLARACJA

//klasa bazowa, z niej tworzone bêd¹ paletki i pi³ka
//
class GameObject {
protected:
    int x, y; //pozycja obiektu
    int width, height; //rozmiar obiektu

public:
    GameObject(int x, int y, int w, int h); //konstruktor potrzebuje pozycji i wymiarów obiektu
    virtual ~GameObject() = default;

    //metody wirtualna umo¿liwi jej override dla klas dziedzicz¹cych
    //rysowaæ mo¿na ka¿dy obiekt ale paletkê inaczej i pi³kê inaczej
    virtual void update() = 0;
    virtual void draw() = 0;

    //metody do pobierania aktualnej pozycji i rozmiaru, bo paletka ma tylko jedn¹ pozycjê i dodawany jest do niej jej rozmiar
    //co zmniejsza iloœæ zmiennych i umo¿liwia pracê na tej samej klasie co pi³ka
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
};

class Paddle : public GameObject {
private:
    int speed;
public:
    Paddle(int x, int y, int w, int h, int speed);

    void moveUp(); // logika metody to y -= speed
    void moveDown(); // y += speed

    int getCenterY() const; // metoda do znalezienia œrodka paletki

    //overridujemy z gameobject
    void update() override;
    void draw() override;
};

class Ball :public GameObject {
private:
    int dx, dy;
public:
    Ball(int x, int y, int size, int dx, int dy);
    void update() override;
    void draw() override;

    //metody  wywo³ywane przy wykryciu kolizji
    void invertX();
    void invertY();
};


/*  Klasa do panowania nad obiektami:

    sprawdza kolizje,
    rysuje pilke paletki,
*/
class Game {
private:
    //wymiary okna gry
    const int screenWidth;
    const int screenHeight;

    //odpowiada za aktywacje
    bool trwaGra;

    //punktacja
    int scoreLeft;
    int scoreRight;

    //tutaj tworzymy obiekty klas wszystkie obiekty,
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;

    void getInput(); //obs³uguje input z klawiatury
    
    void updateGame(); //sterowanie przebiegiem gry / a nó¿ widelec ¿eby nie by³o update jak w GameObject
    void checkCollisions(); //sprawdza kolizje,
    void render(); // rysuj wszystko, cala konsola

public:
    Game(); //konstruktor
    void run(); //odpalenie start

};

void hideCursor();