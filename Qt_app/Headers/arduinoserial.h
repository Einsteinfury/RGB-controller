#ifndef ARDUINOSERIAL_H
#define ARDUINOSERIAL_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>

namespace Ui {
class ArduinoSerial;
}

class ArduinoSerial : public QMainWindow
{
    Q_OBJECT

public:
    explicit ArduinoSerial(QWidget *parent = nullptr);
    ~ArduinoSerial();

private slots:
    void openSerialPort();
    void serialReceived();
    void setTimer();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_verticalSlider_sliderReleased();

    void on_verticalSlider_2_sliderReleased();

    void on_verticalSlider_3_sliderReleased();

    void on_verticalSlider_valueChanged(int value);

    void on_verticalSlider_2_valueChanged(int value);

    void on_verticalSlider_3_valueChanged(int value);

    void on_dial_sliderReleased();

    void on_dial_valueChanged(int value);

    void on_dial_2_sliderReleased();

    void on_dial_2_valueChanged(int value);

    void on_dial_3_valueChanged(int value);

    void on_checkBox_toggled(bool checked);

    void on_pushButton_5_clicked();

private:
    Ui::ArduinoSerial *ui;
    QSerialPort * m_serial = nullptr;
    QTimer *timer;
    int count = 0;
    int countdown;
};

#endif // ARDUINOSERIAL_H
