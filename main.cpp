#include <QApplication>
#include <QWidget>
#include <QIcon>
#include "main.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Set the application icon
    app.setWindowIcon(QIcon(":/resources/clip.ico"));

    ClipTrack window;
    window.show();

    return app.exec();
}