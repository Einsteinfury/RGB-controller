#include "arduinoserial.h"
#include "ui_arduinoserial.h"

ArduinoSerial::ArduinoSerial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ArduinoSerial),
    m_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    m_serial->setPortName("/dev/ttyACM0");
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(m_serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));



    //m_serial->write("Hello \n");
    if (m_serial->open(QIODevice::ReadWrite)) {
        m_serial->write("0");
        qDebug() << "Written" <<endl;
        //m_serial->close();
    } else {
        qDebug() << "Error" << endl;
        //m_serial->close();
    }
}

ArduinoSerial::~ArduinoSerial()
{
    delete ui;
}

void ArduinoSerial::openSerialPort()
{

}

void ArduinoSerial::serialReceived(){
    QByteArray bytearray;
    bytearray=m_serial->readAll();
    QString input = QString(bytearray);
    qDebug() << input <<endl;

    switch (count) {
        case 0:
            ui->verticalSlider->setValue(input.toInt());
            count++;
            break;
        case 1:
            ui->verticalSlider_2->setValue(input.toInt());
            count++;
            break;
        case 2:
            ui->verticalSlider_3->setValue(input.toInt());
            count = 0;
            break;
        default:
            count = 0;
        break;
    }
}

void ArduinoSerial::setTimer(){
    qDebug() << QTime::currentTime();
    m_serial->write("6");
}

void ArduinoSerial::on_pushButton_clicked()
{
    m_serial->write("5");
    m_serial->write("20");
}

void ArduinoSerial::on_pushButton_2_clicked()
{
    m_serial->write("4");
    m_serial->write("10");
}

void ArduinoSerial::on_pushButton_3_clicked()
{
    m_serial->write("3");
     m_serial->write("40");
}

void ArduinoSerial::on_pushButton_4_clicked()
{
    m_serial->write("6");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setTimer()));
}

void ArduinoSerial::on_verticalSlider_sliderReleased()
{
    QByteArray val;
    val.setNum(ui->verticalSlider->value());
    m_serial->write("7");
    m_serial->write(val);
    qDebug() << val;
}

void ArduinoSerial::on_verticalSlider_2_sliderReleased()
{
    QByteArray val;
    val.setNum(ui->verticalSlider_2->value());
    m_serial->write("8");
    m_serial->write(val);
    qDebug() << val;
}

void ArduinoSerial::on_verticalSlider_3_sliderReleased()
{
    QByteArray val;
    val.setNum(ui->verticalSlider_3->value());
    m_serial->write("9");
    m_serial->write(val);
    qDebug() << val;
}

void ArduinoSerial::on_verticalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
}

void ArduinoSerial::on_verticalSlider_2_valueChanged(int value)
{
    ui->lineEdit_2->setText(QString::number(value));
}

void ArduinoSerial::on_verticalSlider_3_valueChanged(int value)
{
    ui->lineEdit_3->setText(QString::number(value));
}

void ArduinoSerial::on_dial_sliderReleased()
{
    QByteArray val;
    val.setNum(ui->dial->value());
    m_serial->write("b");
    m_serial->write(val);
}

void ArduinoSerial::on_dial_valueChanged(int value)
{
    ui->lineEdit_4->setText(QString::number(value));
}

void ArduinoSerial::on_dial_2_sliderReleased()
{
    QByteArray val;
    val.setNum(ui->dial_2->value());
    m_serial->write("d");
    m_serial->write(val);
}

void ArduinoSerial::on_dial_2_valueChanged(int value)
{
    ui->lineEdit_5->setText(QString::number(value));
}

void ArduinoSerial::on_dial_3_valueChanged(int value)
{
    countdown = value * 1000;
}

void ArduinoSerial::on_checkBox_toggled(bool checked)
{
    if (checked){
        timer->start(countdown);
    }

    else {
        timer->stop();
    }
}

void ArduinoSerial::on_pushButton_5_clicked()
{
    m_serial->write("s");
}
