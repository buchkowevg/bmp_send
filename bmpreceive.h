#ifndef BMPRECEIVE_H
#define BMPRECEIVE_H

#include <QDialog>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QFileDialog>

namespace Ui {
class BmpReceive;
}

class BmpReceive : public QDialog
{
    Q_OBJECT

public:
    explicit BmpReceive(QWidget *parent = nullptr);
    ~BmpReceive();

private slots:

    void on_btn_save_clicked();

private:
    void readPendingDatagrams();
    void datagramParser(const QNetworkDatagram &datagram);

    QImage image;
    QUdpSocket *socket;
    Ui::BmpReceive *ui;
};

#endif // BMPRECEIVE_H
