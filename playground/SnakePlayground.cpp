//
// Created by Hana on 7. 1. 2024.
//

#include "SnakePlayground.h"
#include "ui_SnakePlayground.h"
#include "../snake/Snake.h"
#include <QSettings>

SnakePlayground::SnakePlayground(QWidget *parent) : QMainWindow(parent), ui(new Ui::SnakePlayground) {
    ui->setupUi(this);

    auto *snake = new Snake(this);
    setCentralWidget(snake);
}

SnakePlayground::~SnakePlayground() {
    delete ui;
}

void SnakePlayground::initSize() {
    this->setMinimumSize(playgroundSize);
    this->setMaximumSize(playgroundSize);
}
