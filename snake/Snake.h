//
// Created by Hana on 7. 1. 2024.
//

#ifndef SNAKEGAME_SNAKE_H
#define SNAKEGAME_SNAKE_H


#include <QWidget>
#include <random>

class Snake : public QWidget {
    Q_OBJECT

public:
    explicit Snake(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *timerEvent) override;

private:
    enum GameObjectType {
        Apple,
        Bomb,
        Head,
        Body,
        Wall
    };

    enum Direction {
        Up,
        Down,
        Left,
        Right
    };

    struct SnakePart {
        int x;
        int y;
    };

    struct GameObject {
        int x = -1;
        int y = -1;
    };

    static const int PLAYGROUND_WIDTH = 1000;
    static const int PLAYGROUND_HEIGHT = 600;
    static const int BODY_PART_SIZE = 20;
    static const int APPLE_SIZE = 20;
    static const int BOMB_SIZE = 20;
    static const int WALL_SIZE = 20;
    static const int START_BODY_PARTS_COUNT = 5;
    static const int DELAY = 150;
    static const int BOMB_TIMEOUT = 30;
    static const int APPLE_TIMEOUT = 30;

    QImage head;
    QImage bodyPart;
    QImage apple;
    QImage bomb;
    QImage wall;

    int snakeLength = START_BODY_PARTS_COUNT;
    QList<SnakePart> snake;
    Direction snakeCurrentDirection = Right;
    Direction snakePreviousDirection = Right;

    bool gameOver = false;

    int bombTimer = 0;
    int appleTimer = 0;
    int bombsCounter = 1;

    GameObject appleObject;
    std::vector<GameObject> walls;
    std::vector<GameObject> bombs;

    void loadGameImages();
    void setImageSize(GameObjectType type);
    void startGame();
    void initSnake();
    void paintSnake(QPainter &painter);
    void paintApple(QPainter &painter);
    void paintBomb(QPainter &painter);
    void paintWalls(QPainter &painter);
    void move();
    void putApple();
    void eatApple();
    void addSnakePart();
    void checkCollision();
    void putBombs();
    void generateGameObject(GameObjectType type);
    static bool gameObjectExists(GameObject &gameObject);
    void generateApple(std::mt19937 &gen);
    void generateBomb(std::mt19937 &gen);
    void endGame(QPainter &qp);
    void checkBomb();
    void generateWalls();
    bool regenerateBombPosition(int x, int y);
    bool regenerateApplePosition(int x, int y);
    void checkApple();
    void generateBombs();
    void rotateSnakeHead();
    void rotateSnakeHeadRight();
    void rotateSnakeHeadLeft();
    void rotateSnakeHeadUp();
    void rotateSnakeHeadDown();
};


#endif //SNAKEGAME_SNAKE_H
