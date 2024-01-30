//
// Created by Hana on 7. 1. 2024.
//

#ifndef SNAKEGAME_SNAKEPLAYGROUND_H
#define SNAKEGAME_SNAKEPLAYGROUND_H

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui {class SnakePlayground;}
QT_END_NAMESPACE
class SnakePlayground : public QMainWindow {
    Q_OBJECT

public:
    explicit SnakePlayground(QWidget *parent = nullptr);
    ~SnakePlayground() override;

    void initSize();

private:
    Ui::SnakePlayground *ui;

    // size of the playground, based on that the maximum parts of snake is count
    QSize playgroundSize = QSize(1000, 600);
};


#endif //SNAKEGAME_SNAKEPLAYGROUND_H
