#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CarDatabase carDatabase;
    carDatabase.show();

    return app.exec();
}
