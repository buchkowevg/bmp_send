#include "bmpsend.h"
#include "ui_bmpsend.h"

BmpSend::BmpSend(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BmpSend)
{
    ui->setupUi(this);
    socket = new QUdpSocket(this);
    connect(this, &BmpSend::error_msg_changed, this, &BmpSend::change_error_msg_text); //Здесь этот connect не особо нужен, но просто решил посмотерть как он работает
}

BmpSend::~BmpSend()
{
    delete socket;
    delete ui;
}

void BmpSend::error_msg_handler(const QString &value)
{
    if (QString::compare(error, value) != 0) {
        error = value;
        emit error_msg_changed(value);
    }
}

void BmpSend::change_error_msg_text(const QString &value)
{
    ui->lbl_error->setText(value);
}

void BmpSend::display_image()
{
    QImage formattedImage = image.scaledToWidth(ui->lbl_image->width());
    ui->lbl_image->setPixmap(QPixmap::fromImage(formattedImage));
}

void BmpSend::on_btn_browse_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "./images", tr("Images (*.bmp)"));
    if (QString::compare(filename, QString()) != 0)
    {
        ui->line_filepath->setText(filename);
        change_error_msg_text(QString());
    }
}


void BmpSend::on_btn_open_clicked()
{
    QString filepath(ui->line_filepath->text());
    QMimeDatabase db;
    QMimeType filetype = db.mimeTypeForFile(filepath, QMimeDatabase::MatchContent);
    if ( QString::compare(filetype.preferredSuffix(), QString("bmp")) == 0 and image.load(filepath))
    {
        display_image();
        ui->lbl_currfile->setText(filepath);
        ui->btn_grayscale->setEnabled(true);
        ui->btn_send->setEnabled(true);
    }
    else
    {
        change_error_msg_text(QString("Ошибка открытия файла"));
        return;
    }
    change_error_msg_text(QString());
}


void BmpSend::on_btn_grayscale_clicked()
{
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            int gray = qGray(image.pixel(i, j));
            image.setPixel(i, j, QColor(gray, gray, gray).rgb());
        }
    }
    display_image();
    ui->btn_grayscale->setEnabled(false);
    change_error_msg_text(QString());
}


void BmpSend::on_btn_send_clicked()
{
    QUdpSocket socket;
    QByteArray barr;
    QBuffer buffer(&barr);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "BMP");
    socket.writeDatagram(QByteArray("\x00",1),QHostAddress::LocalHost,7755);
    for (int i = 0; i < barr.size(); i+=512)
        socket.writeDatagram(barr.mid(i,512), QHostAddress::LocalHost,7755);
    socket.writeDatagram(QByteArray("\x01",1),QHostAddress::LocalHost,7755);
}

