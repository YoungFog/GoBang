#include "Main_UI./Gobang.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // PreLoading
    Gobang w;
    w.show();
    w.BlurEnter();  // UI Enter with blur 0->1
    return a.exec();
}
