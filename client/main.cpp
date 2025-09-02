#include <QApplication>
#include <QWidget>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QWidget w;
    w.setWindowTitle("test2 client placeholder");
    w.resize(400, 300);
    w.show();
    return app.exec();
}
