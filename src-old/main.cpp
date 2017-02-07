#include <QApplication>
#include <PonyGL.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PonyGL *pgl = new PonyGL;
    pgl->initializeGL();

    return a.exec();
}
