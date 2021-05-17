#include "bmpreceive.h"
#include "ui_bmpreceive.h"

BmpReceive::BmpReceive(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BmpReceive)
{
    socket = new QUdpSocket(this);
    connect(socket, &QUdpSocket::readyRead, this, &BmpReceive::readPendingDatagrams);
    socket->bind(QHostAddress::LocalHost, 7755);
    socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1250000); //увеличиваем баффер иначе теряем много пакетов из-за его переполнения
    ui->setupUi(this);
}

BmpReceive::~BmpReceive()
{
    delete socket;
    delete ui;
}

void BmpReceive::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        datagramParser(socket->receiveDatagram(socket->pendingDatagramSize()));
    }
}

void BmpReceive::datagramParser(const QNetworkDatagram &datagram) {
    static QByteArray barr;
    if (datagram.data().size() == 1)
    {
        if (datagram.data().at(0) == 0x00)
        {
            barr.clear();
        }
        else if(datagram.data().at(0) == 0x01)
        {
            image = QImage::fromData(barr,"BMP");
            if (!image.isNull())
            {
                QImage formattedImage = image.scaledToWidth(ui->lbl_image->width());
                ui->lbl_image->setPixmap(QPixmap::fromImage(formattedImage));
                ui->lbl_error->setText(QString());
                ui->btn_save->setEnabled(true);

            }
            else
            {
                ui->lbl_image->clear();
                ui->btn_save->setEnabled(false);
                ui->lbl_error->setText(QString("Что-то то пошло не так поробуйте еще раз"));
            }
            socket->flush();

        }
        else {
            barr.append(datagram.data());
        }
    }
    else {
        barr.append(datagram.data());
    }
}

void BmpReceive::on_btn_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as"),"./images", tr("Images (*.bmp)"));
    if (QString::compare(filename, QString()) != 0)
    {
        image.save(filename, "BMP");
    }

}

