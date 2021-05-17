#ifndef BMPSEND_H
#define BMPSEND_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMimeDatabase>
#include <QColor>
#include <QBuffer>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class BmpSend; }
QT_END_NAMESPACE

class BmpSend : public QMainWindow
{
    Q_OBJECT

public:
    BmpSend(QWidget *parent = nullptr);
    ~BmpSend();

private slots:

    void change_error_msg_text(const QString &value);

    void on_btn_browse_clicked();

    void on_btn_open_clicked();

    void on_btn_grayscale_clicked();

    void on_btn_send_clicked();

signals:
    void error_msg_changed(const QString &value);

private:
    void error_msg_handler(const QString &value);
    void display_image();

    Ui::BmpSend *ui;
    QImage image;
    QString error;
    QUdpSocket *socket;
};
#endif // BMPSEND_H
