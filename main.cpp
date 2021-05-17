#include "bmpsend.h"
#include "bmpreceive.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BmpReceive r;
    r.show();

    BmpSend w;
    w.show();


    return a.exec();
}
