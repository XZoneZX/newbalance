#include "serialthread.h"
#include "balancingsys.h"

#include <QTime>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

SerialThread::SerialThread(QObject *parent):
    QThread(parent),
    quit(false),
    speedValue (720), speedThreadValue (5), samplingPeriodValue (10),
    m_standardOutput(stdout)
{

}

SerialThread::~SerialThread()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
}

void SerialThread::startSlave (void)
{
    quit = false;
    if (!isRunning())
        start();
}

void SerialThread::run (void)
{
    m_standardOutput << QString::fromLocal8Bit("tread success\n")<< endl;
    startSerial ( );
    while(!quit)
    {
        readSerial ( );
    }
}

void SerialThread::stop (void)
{
    mutex.lock();
    quit = true;
    mutex.unlock();
}

void SerialThread::readSerial ( )
{
    qint32 temp_int;
    char buff_j[2],buff_d[4];
    qint16 fre;
    temp_int = 0;
    while(temp_int < 2)
    {
        if(m_serialPort.read(&buff_j[temp_int],1) == 1)
            temp_int++;
        else if(m_serialPort.waitForReadyRead(-1))
            ;
    }
//    m_standardOutput << QString::fromLocal8Bit("begin with_%1;%2\n").arg(quint8(buff_j[0]),0,16).arg(quint8(buff_j[1]),0,16)<< endl;
    if ((buff_j[0] == 0x21)&&(buff_j[1] == 0x01))
    {
        temp_int = 0;
        while(temp_int < 1)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        m_standardOutput << QString::fromLocal8Bit("2101")<< endl;
        testCounter = 1;
    }
    else if ((buff_j[0] == 0x22)&&(buff_j[1] == 0x00))
    {
//        检测结束
//        if (position == 1)
//            show_position = 1;
//        else
//            show_position = 0;
    }
    else if ((buff_j[0] == 0x60)&&(buff_j[1] == 0x02))
    {
        temp_int = 0;
        while(temp_int < 2)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        m_standardOutput << QString::fromLocal8Bit("0x6002_%1;%2\n").arg(quint8(buff_d[0])).arg(quint8(buff_d[1]))<< endl;
        temp_int = quint8(buff_d[0])*256+quint8(buff_d[1]);
        if (temp_int < 65535)
        {
            f = 10000.0/(qreal)temp_int;
            fre = (quint16)60*f;
        }
        else
        {
            f = 0;
            fre = 0;
        }
        m_standardOutput << QString::fromLocal8Bit("f_%1\n").arg(f)<< endl;
        testCounter = 1;
        refreshSpeedDisplay (fre);
        m_standardOutput << QString::fromLocal8Bit("fre_%1\n").arg(fre) << endl;
    }
    else if ((buff_j[0] == 0x40)&&(buff_j[1] == 0x12))
    {
        temp_int = 0;
        while(temp_int < 2)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        testCounter = quint8(buff_d[0])*256+quint8(buff_d[1]);
        testCounter = testCounter/32;

        if (testCounter == 1)
            data11Pre = 0, data12Pre = 0, data21Pre = 0, data22Pre = 0;
        else
            data11Pre = data11, data12Pre = data12, data21Pre = data21, data22Pre = data22;


        m_standardOutput << QString::fromLocal8Bit("testCounter_%1\n").arg(testCounter) << endl;

        temp_int = 0;
        while(temp_int < 2)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
//        if(testCounter == 1)
//        {
//            m_standardOutput << QString::fromLocal8Bit("dangwei_%1;%2\n").arg(quint8(buff_d[0])).arg(quint8(buff_d[1]))<< endl;
//        }
        m_standardOutput << QString::fromLocal8Bit("dangwei_%1;%2\n").arg(quint8(buff_d[0])).arg(quint8(buff_d[1]))<< endl;
        switch(buff_d[0])							//根据增益档位设增益值
        {
            case 1: gain1 = 0.125;
                break;
            case 2: gain1 = 0.5;  
                break;
            case 3: gain1 = 2;
                break;
            case 4: gain1 = 8;
                break;
            case 5: gain1 = 32;
                break;
            case 6: gain1 = 128;
                break;
            case 7: gain1 = 512;
                break;
            default:
                return;
        }
        switch(buff_d[1])
        {
            case 1: gain2 = 0.125;
                break;
            case 2: gain2 = 0.5;
                break;
            case 3: gain2 = 2;
                break;
            case 4: gain2 = 8;
                break;
            case 5: gain2 = 32;
                break;
            case 6: gain2 = 128;
                break;
            case 7: gain2 = 512;
                break;
            default:
                return;
        }
        temp_int = 0;
        while(temp_int < 4)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        data11 = charToFloat(buff_d, data11Pre, gain1);
        temp_int = 0;
        while(temp_int < 4)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        data12 = charToFloat(buff_d, data12Pre, gain1);
        temp_int = 0;
        while(temp_int < 4)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        data21 = charToFloat(buff_d, data21Pre, gain2);
        temp_int = 0;
        while(temp_int < 4)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        data22 = charToFloat(buff_d, data22Pre, gain2);
//        m_standardOutput << QString::fromLocal8Bit("d11=%1;d12=%2;d21=%3;d22=%4\n").arg(data11).arg(data12).arg(data21).arg(data22) << endl;
         m_standardOutput << QString::fromLocal8Bit("%1,%2,%3,%4;\n").arg(data11).arg(data12).arg(data21).arg(data22) << endl;
        /***************************ad数据*************************************/
        qint8 i = 0;
        qint16 temp;
        temp_int = 0;
        while (i < 32)
        {
            while(temp_int < 2)
            {
                if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                    temp_int++;
                else if(m_serialPort.waitForReadyRead(-1))
                    ;
            }
            temp = (quint8(buff_d[0])*256+quint8(buff_d[1]));

            temp_int = 0;
            i++;
//            m_standardOutput << QString::fromLocal8Bit("%1,\n").arg(temp) << endl;
        }
        /***************************ad数据*************************************/
//        if (testCounter % samplingPeriodValue == 0)
        if (testCounter % 8 == 0)
        {
//            test_stop = 1;
//            test_state = 0;
            dataOk (data11, data12, data21, data22, f);
            if(testCounter == samplingPeriodValue * 32)
            {
                testCounter = 1;
                stopSlave();
            }
        }
    }
    if ((buff_j[0] == 0x70)&&(buff_j[1] == 0x01))
    {
        m_standardOutput << QString::fromLocal8Bit("7001")<< endl;
        temp_int = 0;
        while(temp_int < 1)
        {
            if(m_serialPort.read(&buff_d[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(-1))
                ;
        }
        m_standardOutput << QString::fromLocal8Bit("0000")<< endl;
    }
}

void SerialThread::startSerial ( )
{
    quint8 errorCode;
    if ( !( errorCode = openSerialPort (PortName)))
    {
        errorSerialOpen (errorCode);
        m_standardOutput << QString::fromLocal8Bit("打开串口成功") << endl;
        if(step == 1)    //测量
        {
            changeStartButton ();
        }
        else if(step == 0)    //标定
        {
            changeCalibrationButton();
        }
        startMeasurement ( );
    }
    else
    {
//            ui -> startButton -> setChecked( false );
        errorSerialOpen (errorCode);
        stop ( );
    }
}

void SerialThread::stopSlave ()
{
    if ( m_serialPortActivated )
        closeSerialPort( );
    if(step == 1)
    {
        changeStartButton ( );
    }
    else if(step == 0)
    {
        changeCalibrationButton();
    }
    stop ();
}

void SerialThread::startMeasurement ( )
{
    QByteArray buff_s;
    buff_s.resize(2);
    buff_s[0] = 0x20;
    buff_s[1] = 0x00;
    writeSerialPort(buff_s);
    buff_s.resize(4);
    buff_s[0] = 0x10;
    buff_s[1] = 0x02;
    buff_s[2] = ((600000/speedValue)%65536)/256;
    buff_s[3] = (600000/speedValue)%256;
    m_standardOutput << QString::fromLocal8Bit("buf1=%1;buf2=%2;\n").arg(quint8(buff_s[2])).arg(quint8(buff_s[3])) << endl;
    writeSerialPort(buff_s);
    buff_s.resize(3);
    buff_s[0] = 0x11;
    buff_s[1] = 0x01;
    buff_s[2] = (600000/speedValue)*speedThreadValue/100;
    m_standardOutput << QString::fromLocal8Bit("buf1=%1;\n").arg(quint8(buff_s[2])) << endl;
    writeSerialPort(buff_s);
    buff_s.resize(4);
    buff_s[0] = 0x13;
    buff_s[1] = 0x02;
    buff_s[2] = (samplingPeriodValue*32)/256;
    buff_s[3] = (samplingPeriodValue*32)%256;
    m_standardOutput << QString::fromLocal8Bit("buf1=%1;buf2=%2;\n").arg(quint8(buff_s[2])).arg(quint8(buff_s[3])) << endl;
    writeSerialPort(buff_s);
}

void SerialThread::writeSerialPort(const QByteArray &writeData)
{
    m_writeData = writeData;

    qint64 bytesWritten = m_serialPort.write(writeData);

    if (bytesWritten == -1) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 写COM%1异常, 错误信息--%2").arg( m_serialPort.portName( ) ).arg( m_serialPort.errorString( ) ) << endl;
    } else if (bytesWritten != m_writeData.size( ) ) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 写COM%1异常, 错误信息--%2").arg( m_serialPort.portName( ) ).arg( m_serialPort.errorString( ) ) << endl;
    }
    m_standardOutput << QString::fromLocal8Bit("bytesWritten_%1;\n").arg(bytesWritten)<< endl;
}

int SerialThread::openSerialPort (const QString &serialPortName)
{
    // 设置串口名:
    m_serialPort.setPortName (serialPortName);
    // 打开串口:
    quint8 errorCode;
    if ( !( errorCode = initSerialPort( ) ) )
        m_serialPortActivated = 1;

    return errorCode;
}

int SerialThread::tmp_checkSerialPort(const QString &serialPortName)
{
    // 设置串口名:
    m_serialPort.setPortName (serialPortName);
    // 打开串口:
    quint8 errorCode;

    if ( !( errorCode = initSerialPort( ) ) )
    {
        qint32 temp_int,tmp_cantread=0;
        char buff_j[2];
        temp_int = 0;
        while(temp_int < 2&&tmp_cantread <3)
        {
            if(m_serialPort.read(&buff_j[temp_int],1) == 1)
                temp_int++;
            else if(m_serialPort.waitForReadyRead(700))
                ;
            tmp_cantread++;
        }
        if ((buff_j[0] == 0x70)&&(buff_j[1] == 0x01))
        {
            m_standardOutput << QString::fromLocal8Bit("7001")<< endl;
            temp_int = 0;
            errorCode = 7;
            m_standardOutput << QString::fromLocal8Bit("0000")<< endl;
        }
        closeSerialPort ( );
    }
    errorSerialOpen(errorCode);

    return errorCode;
}

void SerialThread::closeSerialPort ( )
{
    m_serialPort.close( );
    m_serialPortActivated = 0;
}

int SerialThread::initSerialPort ( )
{
    // 设置工作模式:
    if ( !m_serialPort.open( QIODevice::ReadWrite ) ) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 打开串口失败, 错误信息--%1").arg( m_serialPort.errorString( ) ) << endl;
        return 1;
    }
    // 设置波特率:
    quint32 serialPortBaudRate = QSerialPort::Baud115200;
    if (!m_serialPort.setBaudRate(serialPortBaudRate)) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 设置波特率失败, 错误信息--%1").arg( m_serialPort.errorString( ) ) << endl;
        return 2;
    }
    // 设置数据位长度:
    if (!m_serialPort.setDataBits(QSerialPort::Data8)) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 设置数据位失败, 错误信息--%1").arg( m_serialPort.errorString( ) ) << endl;
        return 3;
    }
    // 设置奇偶校验:
    if (!m_serialPort.setParity(QSerialPort::NoParity)) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 设置校验模式失败, 错误信息--%1").arg( m_serialPort.errorString( ) ) << endl;
        return 4;
    }
    // 设置停止位:
    if (!m_serialPort.setStopBits(QSerialPort::OneStop)) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 设置停止位失败, 错误信息--%1").arg( m_serialPort.errorString( ) ) << endl;
        return 5;
    }
    // 设置流控制:
    if (!m_serialPort.setFlowControl(QSerialPort::NoFlowControl)) {
        m_standardOutput << QString::fromLocal8Bit("[ 串口错误 ] 设置流控制失败, 错误信息--%1").arg( m_serialPort.errorString( ) ) << endl;
        return 6;
    }
    return 0;
}

qreal SerialThread::charToFloat(char d[4], qreal input, qreal gain)
{
    qreal temp;
    quint8 ud0, ud1, ud2, ud3;
    qreal dout;
    ud0 = quint8(d[0]);
    ud1 = quint8(d[1]);
    ud2 = quint8(d[2]);
    ud3 = quint8(d[3]);
//    m_standardOutput << QString::fromLocal8Bit("0x40_%1;%2;%3;%4\n").arg(ud0).arg(ud1).arg(ud2).arg(ud3) << endl;
    if (ud0 > 127)
    {
        temp = ud0*256+ud1-65536;
        if(ud2 > 127)
            temp = temp+(ud2*256+ud3-65536)/1000.0;
        else
            temp = temp+(ud2*256+ud3)/1000.0;
    }
    else
    {
        temp = ud0*256+ud1;
        if(ud2 > 127)
            temp = temp+(ud2*256+ud3-65536)/1000.0;
        else
            temp = temp+(ud2*256+ud3)/1000.0;
    }
//    m_standardOutput << QString::fromLocal8Bit("d_%1\n").arg(temp) << endl;
    dout = input*(testCounter-1)/testCounter+temp/testCounter/gain;
//    m_standardOutput << QString::fromLocal8Bit("dd_%1\n").arg(dout) << endl;
    return dout;
}
char* SerialThread::zijian()
{
    qDebug()<<"zijian1"<<endl;
    QByteArray buff_s;
    char buff_j[3];
    buff_s.resize(2);
    buff_s[0] = 0x00;
    buff_s[1] = 0x00;
    writeSerialPort(buff_s);

    qint32 temp_int;
    qint16 fre;
    temp_int = 0;
    while(temp_int < 3)
    {
        if(m_serialPort.read(&buff_j[temp_int],1) == 1)
            temp_int++;
        else if(m_serialPort.waitForReadyRead(-1))
            ;
    }
    qDebug()<<"zijian2"<<endl;
    return buff_j;
    /*
        fd_set readfd;
        struct timeval tv;
        int ret,temp_int;
        unsigned char buff_jt[3];
        tcflush(fd, TCIOFLUSH);
        buff_jt[0] = 0x00;
        buff_jt[1] = 0x00;
        write(fd, buff_jt, 2);
        while (read(fd,buff_jt,1))
            printf("seril read=%X\n",buff_jt[0]);
        FD_ZERO(&readfd);
        FD_SET(fd,&readfd);
        tv.tv_sec = 10;    								//s
        tv.tv_usec = 0;
        ret = select(fd+1,&readfd,NULL,NULL,&tv);
        printf("ret= %d \n", ret);
        if(ret == 1)
        {
            temp_int = 0;
            while(temp_int < 3)
            {
                if(read(fd,&buff_jt[temp_int],1) == 1)
                    temp_int++;
            }
            printf("zijianbuff= %X;%X;%X\n", buff_jt[0],buff_jt[1],buff_jt[2]);
            if ((buff_jt[0] == 0x01)&&(buff_jt[1] == 0x01))
            {
                if((buff_jt[2]&0x01) == 0x01)
                {
                    if (lang == 1)
                        message = "测量通道1正常";
                    else if (lang == 2)
                        message = "Channel 1:Normal";
                    gdk_color_parse ("green", &color);
                    gtk_widget_modify_fg(message3_label, GTK_STATE_NORMAL, &color);
                    gtk_label_set_text(GTK_LABEL(message3_label),message);
                }
                else
                {
                    if (lang == 1)
                        message = "测量通道1异常";
                    else if (lang == 2)
                        message = "channel 1:Error";
                    gdk_color_parse ("red", &color);
                    gtk_widget_modify_fg(message3_label, GTK_STATE_NORMAL, &color);
                    gtk_label_set_text(GTK_LABEL(message3_label),message);
                }
                if((buff_jt[2]&0x02) == 0x02)
                {
                    if (lang == 1)
                        message = "测量通道2正常";
                    else if (lang == 2)
                        message = "Channel 2:Normal";
                    gdk_color_parse ("green", &color);
                    gtk_widget_modify_fg(message4_label, GTK_STATE_NORMAL, &color);
                    gtk_label_set_text(GTK_LABEL(message4_label),message);
                }
                else
                {
                    if (lang == 1)
                        message = "测量通道2异常";
                    else if (lang == 2)
                        message = "Channel 2:Error";
                    gdk_color_parse ("red", &color);
                    gtk_widget_modify_fg(message4_label, GTK_STATE_NORMAL, &color);
                    gtk_label_set_text(GTK_LABEL(message4_label),message);
                }
                if((buff_jt[2]&0x04) == 0x04)
                {
                    if (lang == 1)
                        message = "参考通道正常";
                    else if (lang == 2)
                        message = "Reference signal: Normal";
                    gdk_color_parse ("green", &color);
                    gtk_widget_modify_fg(message2_label, GTK_STATE_NORMAL, &color);
                    gtk_label_set_text(GTK_LABEL(message2_label),message);
                }
                else
                {
                    if (lang == 1)
                        message = "参考通道异常";
                    else if (lang == 2)
                        message = "Reference signal:Error";
                    gdk_color_parse ("red", &color);
                    gtk_widget_modify_fg(message2_label, GTK_STATE_NORMAL, &color);
                    gtk_label_set_text(GTK_LABEL(message2_label),message);
                }
                if((buff_jt[2]&0x08) == 0x08)
                    gb_position = TRUE;
                else
                {
                    gb_position = FALSE;
                    position = 0;
                }
                if((buff_jt[2]&0x10) == 0x10)
                    demarcate_active = 1;				//激活接近开关标定系数
                else
                    demarcate_active = 2;				//激活光电头标定系数
                refer_change = 1;
            }
        }
        else
        {
            if (lang == 1)
                message = "自检超时!";
            else if (lang == 2)
                message = "Overtime!";
            gdk_color_parse ("red", &color);
            gtk_widget_modify_fg(message2_label, GTK_STATE_NORMAL, &color);
            gtk_label_set_text(GTK_LABEL(message2_label),message);
        }
        gtk_widget_set_sensitive(GTK_WIDGET(button[88]),gb_position);
        */

}
