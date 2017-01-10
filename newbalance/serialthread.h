#ifndef SERIALTHREAD
#define SERIALTHREAD


#include <QThread>
#include <QMutex>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>

class SerialThread : public QThread
{
    Q_OBJECT

public:
    SerialThread(QObject *parent = 0);
    ~SerialThread();

    void startSlave ( );
    void stopSlave ( );
    void run ( );
    void stop ( );
    char* zijian(void);

    int tmp_checkSerialPort(const QString &serialPortName);

    qint16 step;         //step = 0，标定；step = 1，测量

    QString PortName;
    qint16 speedValue;
    qint8 speedThreadValue;//打错字了 是速度阈值
    quint8 samplingPeriodValue;
//    测试相关参数
    qreal f;
    quint16 testCounter;
    qreal data11Pre, data12Pre, data21Pre, data22Pre;
    qreal data11, data12, data21, data22;
    qreal gain1, gain2;

signals:
    void changeStartButton ( );
    void changeCalibrationButton ( );
    void errorSerialOpen (int errorCode);
    void refreshSpeedDisplay (int fre);
    void dataOk (qreal d11, qreal d12, qreal d21, qreal d22, qreal f);

    //void serialPortOpenSuccess();

private:
    void readSerial ( );
    void startSerial ( );
    void startMeasurement ( );
    void writeSerialPort(const QByteArray &writeData);
    int  openSerialPort (const QString &serialPortName);
    void closeSerialPort ( );
    int  initSerialPort ( );

    qreal charToFloat (char d[4], qreal input, qreal gain);
    QMutex mutex;
    bool quit;
    QSerialPort m_serialPort;
    QByteArray m_writeData;
    qint32      m_serialPortActivated;
    QTextStream m_standardOutput;

};

 // SERIALTHREAD
#endif
