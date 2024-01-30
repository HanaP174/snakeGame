#include <QApplication>
#include "playground/SnakePlayground.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    SnakePlayground snakePlayground;
    snakePlayground.initSize();
    snakePlayground.show();
    return QApplication::exec();
}
