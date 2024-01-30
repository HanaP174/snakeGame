//
// Created by Hana on 7. 1. 2024.
//

#include "Snake.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTime>
#include <random>

Snake::Snake(QWidget *parent) : QWidget(parent) {

    loadGameImages();
    setFocus();
    startGame();
}

void Snake::loadGameImages() {
    if (head.load(":/resources/head.png")) {
        setImageSize(Head);
    }
    if (apple.load(":/resources/apple.png")) {
        setImageSize(Apple);
    }
    if (bodyPart.load(":/resources/body.png")) {
        setImageSize(Body);
    }
    if (bomb.load(":/resources/bomb.png")) {
        setImageSize(Bomb);
    }
    if (wall.load(":/resources/wall.png")) {
        setImageSize(Wall);
    }
}

void Snake::setImageSize(Snake::GameObjectType type) {
    switch (type) {
        case Apple:
            apple = apple.scaled(APPLE_SIZE, APPLE_SIZE);
            break;
        case Bomb:
            bomb = bomb.scaled(BOMB_SIZE, BOMB_SIZE);
            break;
        case Head:
            head = head.scaled(BODY_PART_SIZE, BODY_PART_SIZE).transformed(QTransform().rotate(270));
            break;
        case Body:
            bodyPart = bodyPart.scaled(BODY_PART_SIZE, BODY_PART_SIZE);
            break;
        case Wall:
            wall = wall.scaled(WALL_SIZE, WALL_SIZE);
    }
}

void Snake::startGame() {
    generateWalls();
    initSnake();
    putApple();
    putBombs();
    startTimer(DELAY);
}

void Snake::initSnake() {
    snakeLength = START_BODY_PARTS_COUNT;
    for (int i = 0; i < snakeLength; i++) {
        snake << SnakePart{100 - (i * BODY_PART_SIZE), WALL_SIZE};
    }
}

void Snake::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!gameOver) {
        paintWalls(painter);
        paintSnake(painter);
        paintApple(painter);
        paintBomb(painter);
    } else {
        endGame(painter);
    }
    QWidget::paintEvent(event);
}

void Snake::paintSnake(QPainter &painter) {
    painter.drawImage(snake[0].x, snake[0].y, head);
    for (int i = 1; i < snakeLength; i++) {
        painter.drawImage(snake[i].x, snake[i].y, bodyPart);
    }
}

void Snake::paintApple(QPainter &painter) {
    painter.drawImage(appleObject.x, appleObject.y, apple);
}

void Snake::paintBomb(QPainter &painter) {
    for (const auto &bombObject : bombs) {
        painter.drawImage(bombObject.x, bombObject.y, bomb);
    }
}

void Snake::paintWalls(QPainter &painter) {
    for (const auto &wallObject : walls) {
        painter.drawImage(wallObject.x, wallObject.y, wall);
    }
}

void Snake::timerEvent(QTimerEvent *timerEvent) {
    if (!gameOver) {
        eatApple();
        checkCollision();
        move();
        checkBomb();
        checkApple();
    }
    repaint();
}

void Snake::keyPressEvent(QKeyEvent *event) {
    snakePreviousDirection = snakeCurrentDirection;
    switch (event->key()) {
        case Qt::Key_Up:
            snakeCurrentDirection = Up;
            rotateSnakeHead();
            break;
        case Qt::Key_Right:
            snakeCurrentDirection = Right;
            rotateSnakeHead();
            break;
        case Qt::Key_Down:
            snakeCurrentDirection = Down;
            rotateSnakeHead();
            break;
        case Qt::Key_Left:
            snakeCurrentDirection = Left;
            rotateSnakeHead();
            break;
        default:
            snakeCurrentDirection = Right;
            rotateSnakeHead();
            break;
    }

    QWidget::keyPressEvent(event);
}

void Snake::move() {
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    switch (snakeCurrentDirection) {
        case Up:
            snake[0].y -= BODY_PART_SIZE;
            break;
        case Right:
            snake[0].x += BODY_PART_SIZE;
            break;
        case Down:
            snake[0].y += BODY_PART_SIZE;
            break;
        case Left:
            snake[0].x -= BODY_PART_SIZE;
            break;
    }
}

void Snake::putApple() {
    appleObject = GameObject();
    appleTimer = 0;
    generateGameObject(Apple);
}

void Snake::eatApple() {

    if ((snake.first().x == appleObject.x) && (snake.first().y == appleObject.y)) {
        addSnakePart();
        putApple();
        bombsCounter++;
        putBombs();
    }
}

void Snake::addSnakePart() {
    snakeLength++;
    snake.append(SnakePart{snake.last().x + BODY_PART_SIZE, snake.last().y + BODY_PART_SIZE});
}

void Snake::checkCollision() {
    for (int i = snakeLength - 1; i > 0; i--) {
        if ((snake.first().x == snake[i].x) && (snake.first().y == snake[i].y)) {
            gameOver = true;
            return;
        }
    }

    for (const auto &bombObject : bombs) {
        if ((snake.first().x == bombObject.x) && (snake.first().y == bombObject.y)) {
            gameOver = true;
            return;
        }
    }

    for (const auto &wallObject : walls) {
        if ((snake.first().x == wallObject.x) && (snake.first().y == wallObject.y)) {
            gameOver = true;
            return;
        }
    }
}

void Snake::putBombs() {
    bombTimer = 0;
    generateBombs();
}

void Snake::generateGameObject(GameObjectType type) {
    QTime time = QTime::currentTime();
    // sets the seed for the random number generator based on the current milliseconds
    std::mt19937 gen(time.msec());

    if (type == Apple) {
        generateApple(gen);
    }

    if (type == Bomb) {
        generateBomb(gen);
    }
}

bool Snake::gameObjectExists(GameObject &gameObject) {
    return gameObject.x != -1 && gameObject.y != -1;
}

void Snake::generateApple(std::mt19937 &gen) {
    std::uniform_int_distribution distX(0, PLAYGROUND_WIDTH / WALL_SIZE - 1);
    std::uniform_int_distribution distY(0, PLAYGROUND_HEIGHT / WALL_SIZE - 1);
    int x, y;
    do {
        x = distX(gen) * APPLE_SIZE;
        y = distY(gen) * APPLE_SIZE;
    }
    while (regenerateApplePosition(x, y));

    appleObject.x = x;
    appleObject.y = y ;
}

void Snake::generateBomb(std::mt19937 &gen) {
    for (auto &bombObject : bombs) {
        std::uniform_int_distribution distX(0, PLAYGROUND_WIDTH / WALL_SIZE - 1);
        std::uniform_int_distribution distY(0, PLAYGROUND_HEIGHT / WALL_SIZE - 1);
        int x, y;
        do {
            x = distX(gen) * BOMB_SIZE;
            y = distY(gen) * BOMB_SIZE;
        }
        while (regenerateBombPosition(x, y));

        bombObject.x = x;
        bombObject.y = y;
    }
}

void Snake::endGame(QPainter &qp) {
    QString message = "Game over";
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(message);

    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, message);
}

void Snake::checkBomb() {
    bombTimer++;
    if (bombTimer >= BOMB_TIMEOUT) {
        putBombs();
        bombTimer = 0;
    }
}

void Snake::generateWalls() {
    for (int i = 0; i < PLAYGROUND_WIDTH; i += WALL_SIZE) {
        walls.push_back({i, 0});
        walls.push_back({i, PLAYGROUND_HEIGHT - 2 * WALL_SIZE});
    }

    for (int i = WALL_SIZE; i < PLAYGROUND_HEIGHT - 1; i += WALL_SIZE) {
        walls.push_back({0, i});
        walls.push_back({PLAYGROUND_WIDTH - WALL_SIZE, i});
    }
}

bool Snake::regenerateBombPosition(int x, int y) {
    bool regenerate = gameObjectExists(appleObject) && appleObject.x == x && appleObject.y == y;

    for (const auto &wallObject : walls) {
        if (wallObject.x == x && wallObject.y == y) {
            return true;
        }
    }
    return regenerate;
}

bool Snake::regenerateApplePosition(int x, int y) {
    bool regenerate = false;
    for (auto &bombObject : bombs) {
        regenerate = regenerate || bombObject.x == x && bombObject.y == y;
    }

    for (const auto &wallObject : walls) {
        if ((wallObject.x + WALL_SIZE > x) && (x + APPLE_SIZE > wallObject.x)
            && (wallObject.y + WALL_SIZE > y) && (y + APPLE_SIZE > wallObject.y)) {
            return true;
        }
    }
    return regenerate;
}

void Snake::checkApple() {
    appleTimer++;
    if (appleTimer >= APPLE_TIMEOUT) {
        generateGameObject(Apple);
        appleTimer = 0;
    }
}

void Snake::generateBombs() {
    if (bombs.size() < bombsCounter) {
        bombs.emplace_back(GameObject());
    }
    generateGameObject(Bomb);
}

void Snake::rotateSnakeHead() {
    if (snakeCurrentDirection == Right) {
        rotateSnakeHeadRight();
    } else if (snakeCurrentDirection == Left) {
        rotateSnakeHeadLeft();
    } else if (snakeCurrentDirection == Down) {
        rotateSnakeHeadDown();
    } else {
        rotateSnakeHeadUp();
    }
}

void Snake::rotateSnakeHeadRight() {
    switch (snakePreviousDirection) {
        case Right:
            break;
        case Up:
            head = head.transformed(QTransform().rotate(90));
            break;
        case Down:
            head = head.transformed(QTransform().rotate(270));
            break;
        case Left:
            head = head.transformed(QTransform().rotate(180));
            break;
    }
}

void Snake::rotateSnakeHeadLeft() {
    switch (snakePreviousDirection) {
        case Right:
            head = head.transformed(QTransform().rotate(180));
            break;
        case Up:
            head = head.transformed(QTransform().rotate(270));
            break;
        case Down:
            head = head.transformed(QTransform().rotate(90));
            break;
        case Left:
            break;
    }
}

void Snake::rotateSnakeHeadUp() {
    switch (snakePreviousDirection) {
        case Right:
            head = head.transformed(QTransform().rotate(270));
            break;
        case Up:
            break;
        case Down:
            head = head.transformed(QTransform().rotate(180));
            break;
        case Left:
            head = head.transformed(QTransform().rotate(90));
            break;
    }
}

void Snake::rotateSnakeHeadDown() {
    switch (snakePreviousDirection) {
        case Right:
            head = head.transformed(QTransform().rotate(90));
            break;
        case Up:
            head = head.transformed(QTransform().rotate(180));
            break;
        case Down:
            break;
        case Left:
            head = head.transformed(QTransform().rotate(270));
            break;
    }
}
