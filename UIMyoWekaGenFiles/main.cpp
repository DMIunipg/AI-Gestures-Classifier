#include "GestureForm.h"
#include "GenMyoWindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    //init qt
    QApplication a(argc, argv);

#if 0
    //load style sheet
    QFile qss(":/qdarkstyle/style.qss");
    if (qss.exists())
    {
        qss.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&qss);
        a.setStyleSheet(ts.readAll());
    }
#endif

    //init window
    GenMyoWindow w;
    w.setWindowFlags( Qt::Window |
                      Qt::WindowTitleHint |
                      Qt::WindowStaysOnTopHint |
                      Qt::WindowCloseButtonHint |
                      Qt::CustomizeWindowHint );

    w.show();
    //start
    return a.exec();
}
