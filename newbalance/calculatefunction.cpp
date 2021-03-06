#include "calculatefunction.h"

#include <QtMath>
#include <QDebug>

#define  Pi 3.14159265

QT_USE_NAMESPACE

CalculateFunction::CalculateFunction(QObject *parent):
    QObject(parent),
    m_standardOutput(stdout),
    a(35), b(82), c(39), r1(41), r2(41),
    compensate1A1(0), compensate1P1(0), compensate1A2(0), compensate1P2(0),
    compensate2A1(0), compensate2P1(0), compensate2A2(0), compensate2P2(0),
    compensate3A1(0), compensate3P1(0), compensate3A2(0), compensate3P2(0),
    addDelete1(0), addDelete2(0),
    ampAmend1No(0), phaseAmend1No(0), ampAmend2No(0), phaseAmend2No(0),
    //k11Amp(124.3336), k11Pha(180.6246), k12Amp(103.9310), k12Pha(180.5628),
    //k21Amp(120.3518), k21Pha(182.5430), k22Amp(105.7568), k22Pha(180.3261),
    file("para.txt")
{
}

CalculateFunction::~CalculateFunction()
{

}

void CalculateFunction::calRead()
{
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        m_standardOutput << QString::fromLocal8Bit("Can't open the file") << endl;

//    file.close();


    QFile f(".//test.txt");
    if (!f.exists()) {
        // react
        qDebug()<<" file doesn't exist!"<<endl;
    }
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
        return;
    }

    QTextStream txtInput(&f);

    k11 = txtInput.readLine().toDouble();
    k12 = txtInput.readLine().toDouble();
    k13 = txtInput.readLine().toDouble();
    k14 = txtInput.readLine().toDouble();
    k21 = txtInput.readLine().toDouble();
    k22 = txtInput.readLine().toDouble();
    k23 = txtInput.readLine().toDouble();
    k24 = txtInput.readLine().toDouble();

    f.close();

    k11Amp = k11;
    k11Pha = k12;
    k12Amp = k13;
    k12Pha = k14;
    k21Amp = k21;
    k21Pha = k22;
    k22Amp = k23;
    k22Pha = k24;
}

qreal CalculateFunction::calculatePhase (qreal x, qreal y)
{
    qreal p;
    if(x == 0)
    {
        if (y > 0)
            p = 90;
        else
            p = 270;
    }
    else
        p = qAtan(y/x)*180/Pi;
    if(x < 0)
        p = p + 180;
    else if ((x > 0)&&(y < 0))
        p = p + 360;
    return p;
}

void CalculateFunction::measureProcess (qreal d11, qreal d12, qreal d21, qreal d22, qreal f)
{
    qreal amp1, amp2;
    qreal phase1, phase2;
    amp1 = qSqrt(d11*d11+d12*d12);
    phase1 = calculatePhase(d11,d12);
    amp2 = qSqrt(d21*d21+d22*d22);
    phase2 = calculatePhase(d21,d22);
//    m_standardOutput << QString::fromLocal8Bit("d11=%1\n").arg(d11)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("d12=%1\n").arg(d12)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("d21=%1\n").arg(d21)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("d22=%1\n").arg(d22)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("amp1=%1\n").arg(amp1)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("amp2=%1\n").arg(amp2)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("phase1=%1\n").arg(phase1)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("phase2=%1\n").arg(phase2)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("f=%1\n").arg(f)<< endl;

    /***************************频率补偿*****************************/

    qreal w;							//角频率
    qreal kamp, kang;						//幅度、相位补偿值
    w=2*Pi*f;
//    m_standardOutput << QString::fromLocal8Bit("f=%1\n").arg(f)<< endl;
    kamp = 11.88*w/qSqrt(qPow((1-0.0788410062015*qPow(w,2)+0.0000123515183*qPow(w,4)),2)+qPow((2.23555*w-0.0013938279698*qPow(w,3)) ,2));
    kang = -90-calculatePhase((2.23555*w-0.0013938279698*qPow(w,3)),(1-0.0788410062015*qPow(w,2)+0.0000123515183*qPow(w,4)));
    ampAmend1 = amp1/kamp;
    phaseAmend1 = phase1+kang;
    phaseAmend1 = phaseAmend1-(int)(phaseAmend1/360)*360;
    if (phaseAmend1<0 )
        phaseAmend1 = phaseAmend1+360;
    ampAmend2 = amp2/kamp;
    phaseAmend2 = phase2+kang;
    phaseAmend2 = phaseAmend2-(int)(phaseAmend2/360)*360;
    if (phaseAmend2<0 )
        phaseAmend2 = phaseAmend2+360;
//    m_standardOutput << QString::fromLocal8Bit("kamp=%1\n").arg(kamp)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("kang=%1\n").arg(kang)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("ampAmend1=%1\n").arg(ampAmend1)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("ampAmend2=%1\n").arg(ampAmend2)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("phaseAmend1=%1\n").arg(phaseAmend1)<< endl;
//    m_standardOutput << QString::fromLocal8Bit("phaseAmend2=%1\n").arg(phaseAmend2)<< endl;
}

void CalculateFunction::calculateProcess (qreal f)
{
    qreal temp1X = ampAmend1*qCos(phaseAmend1*Pi/180);
    qreal temp1Y = ampAmend1*qSin(phaseAmend1*Pi/180);
    qreal temp2X = compensate1A1*qCos(compensate1P1*Pi/180);
    qreal temp2Y = compensate1A1*qSin(compensate1P1*Pi/180);
    qreal temp3X = compensate2A1*qCos(compensate2P1*Pi/180);
    qreal temp3Y = compensate2A1*qSin(compensate2P1*Pi/180);
    qreal temp4X = compensate3A1*qCos(compensate3P1*Pi/180);
    qreal temp4Y = compensate3A1*qSin(compensate3P1*Pi/180);
    qreal temp5X = temp1X - temp2X - temp3X - temp4X;
    qreal temp5Y = temp1Y - temp2Y - temp3Y - temp4Y;
    qreal amp_end11 = qSqrt(qPow(temp5X,2)+qPow(temp5Y,2));
    qreal phase1_end1 = calculatePhase(temp5Y,temp5X);
    temp1X = ampAmend2*qCos(phaseAmend2*Pi/180);
    temp1Y = ampAmend2*qSin(phaseAmend2*Pi/180);
    temp2X = compensate1A2*qCos(compensate1P2*Pi/180);
    temp2Y = compensate1A2*qSin(compensate1P2*Pi/180);
    temp3X = compensate2A2*qCos(compensate2P2*Pi/180);
    temp3Y = compensate2A2*qSin(compensate2P2*Pi/180);
    temp4X = compensate3A2*qCos(compensate3P2*Pi/180);
    temp4Y = compensate3A2*qSin(compensate3P2*Pi/180);
    temp5X = temp1X - temp2X - temp3X - temp4X;
    temp5Y = temp1Y - temp2Y - temp3Y - temp4Y;
    qreal ampEnd22 = qSqrt(qPow(temp5X,2) + qPow(temp5Y,2));
    qreal phase2End2 = calculatePhase(temp5Y,temp5X);
    qreal coeff1 = r1*(b+c)*qPow(f,3)/(a+b+c);
    qreal coeff2 = r2*c*qPow(f,3)/(a+b+c);
    qreal coeff3 = r1*a*qPow(f,3)/(a+b+c);
    qreal coeff4 = r2*(a+b)*qPow(f,3)/(a+b+c);

    temp1X = 1e-6*k21Amp*ampEnd22*coeff2*qCos((k21Pha+phase2End2+180)*Pi/180);
    temp1Y = 1e-6*k21Amp*ampEnd22*coeff2*qSin((k21Pha+phase2End2+180)*Pi/180);
    temp2X = 1e-6*k22Amp*amp_end11*coeff4*qCos((k22Pha+phase1_end1)*Pi/180);
    temp2Y = 1e-6*k22Amp*amp_end11*coeff4*qSin((k22Pha+phase1_end1)*Pi/180);
    temp3X = 1e-12*k11Amp*k22Amp*coeff4*coeff1*qCos((k11Pha+k22Pha)*Pi/180);
    temp3Y = 1e-12*k11Amp*k22Amp*coeff4*coeff1*qSin((k11Pha+k22Pha)*Pi/180);
    temp4X = 1e-12*k21Amp*k12Amp*coeff2*coeff3*qCos((k21Pha+k12Pha+180)*Pi/180);
    temp4Y = 1e-12*k21Amp*k12Amp*coeff2*coeff3*qSin((k21Pha+k12Pha+180)*Pi/180);

    qreal temp12Amp = qSqrt(qPow((temp1X+temp2X),2)+qPow((temp1Y+temp2Y),2));
    qreal temp12Pha = calculatePhase((temp1Y+temp2Y),(temp1X+temp2X));
    qreal temp34Amp = qSqrt(qPow((temp3X+temp4X),2)+qPow((temp3Y+temp4Y),2));
    qreal temp34Pha = calculatePhase((temp3Y+temp4Y),(temp3X+temp4X));
    ampEnd1 = temp12Amp/temp34Amp;
    phaseEnd1 = temp12Pha - temp34Pha + 180*addDelete1;
    while (phaseEnd1 < 0)
        phaseEnd1 = phaseEnd1+360;
    phaseEnd1 = phaseEnd1-(int)(phaseEnd1/360)*360;

    temp1X = 1e-6*k11Amp*ampEnd22*coeff1*qCos((k11Pha+phase2End2+180)*Pi/180);
    temp1Y = 1e-6*k11Amp*ampEnd22*coeff1*qSin((k11Pha+phase2End2+180)*Pi/180);
    temp2X = 1e-6*k12Amp*amp_end11*coeff3*qCos((k12Pha+phase1_end1)*Pi/180);
    temp2Y = 1e-6*k12Amp*amp_end11*coeff3*qSin((k12Pha+phase1_end1)*Pi/180);
    temp3X = 1e-12*k12Amp*k21Amp*coeff3*coeff2*qCos((k12Pha+k21Pha)*Pi/180);
    temp3Y = 1e-12*k12Amp*k21Amp*coeff3*coeff2*qSin((k12Pha+k21Pha)*Pi/180);
    temp4X = 1e-12*k11Amp*k22Amp*coeff1*coeff4*qCos((k11Pha+k22Pha+180)*Pi/180);
    temp4Y = 1e-12*k11Amp*k22Amp*coeff1*coeff4*qSin((k11Pha+k22Pha+180)*Pi/180);

    temp12Amp = qSqrt(qPow((temp1X+temp2X),2)+qPow((temp1Y+temp2Y),2));
    temp12Pha = calculatePhase((temp1Y+temp2Y),(temp1X+temp2X));
    temp34Amp = qSqrt(qPow((temp3X+temp4X),2)+qPow((temp3Y+temp4Y),2));
    temp34Pha = calculatePhase((temp3Y+temp4Y),(temp3X+temp4X));
        ampEnd2 = temp12Amp/temp34Amp;
        phaseEnd2 = temp12Pha - temp34Pha + 180*addDelete2;
        while (phaseEnd2 < 0)
            phaseEnd2 = phaseEnd2+360;
        phaseEnd2 = phaseEnd2-(int)(phaseEnd2/360)*360;
        printf("xishu--%f;%f;%f;%f;%f;%f;%f;%f\n", k11Amp,k11Pha,k12Amp,k12Pha,k21Amp,k21Pha,k22Amp,k22Pha);

        printf("____amp_end1=%f\n", ampEnd1);
        printf("____amp_end2=%f\n", ampEnd2);
        printf("____phase_end1=%f\n", phaseEnd1);
        printf("____phase_end2=%f\n", phaseEnd2);
}

void CalculateFunction::calNone()
{
    ampAmend1No = ampAmend1;
    ampAmend2No = ampAmend2;
    phaseAmend1No = phaseAmend1;
    phaseAmend2No = phaseAmend2;
}

void CalculateFunction::calLeft(qreal f)
{
    qreal amp1Amend1, phase1Amend1, amp2Amend2, phase2Amend2;
    qreal ampAmend1X, ampAmend1Y, ampAmend2X, ampAmend2Y;
    qreal ampAmend1NoX, ampAmend1NoY, ampAmend2NoX, ampAmend2NoY;
    qreal ampAmend1XX, ampAmend1YY, ampAmend2XX, ampAmend2YY;
    amp1Amend1 = ampAmend1;
    phase1Amend1 = phaseAmend1;
    amp2Amend2 = ampAmend2;
    phase2Amend2 = phaseAmend2;
    ampAmend1X = amp1Amend1*qCos((phase1Amend1)*Pi/180);
    ampAmend1Y = amp1Amend1*qSin((phase1Amend1)*Pi/180);

    ampAmend2X = amp2Amend2*qCos((phase2Amend2)*Pi/180);
    ampAmend2Y = amp2Amend2*qSin((phase2Amend2)*Pi/180);

    ampAmend1NoX = ampAmend1No*qCos((phaseAmend1No)*Pi/180);
    ampAmend1NoY = ampAmend1No*qSin((phaseAmend1No)*Pi/180);

    ampAmend2NoX = ampAmend2No*qCos((phaseAmend2No)*Pi/180);
    ampAmend2NoY = ampAmend2No*qSin((phaseAmend2No)*Pi/180);

    ampAmend1XX = ampAmend1X - ampAmend1NoX ;
    ampAmend1YY = ampAmend1Y - ampAmend1NoY ;

    ampAmend2XX = ampAmend2X - ampAmend2NoX ;
    ampAmend2YY = ampAmend2Y - ampAmend2NoY ;
    amp1Amend1 = qSqrt(qPow(ampAmend1XX,2) + qPow(ampAmend1YY,2));
    qreal ampAmend1Tan = calculatePhase(ampAmend1YY,ampAmend1XX);
    amp2Amend2 = qSqrt(qPow(ampAmend2XX,2) + qPow(ampAmend2YY,2));
    qreal ampAmend2Tan = calculatePhase(ampAmend2YY,ampAmend2XX);
    qreal coeff1 = r1*(b+c)*qPow(f,3)/(a+b+c);
    qreal coeff3 = r1*a*qPow(f,3)/(a+b+c);

    k11 = amp1Amend1*1e6/(testAmp1*coeff1);
    k12 = ampAmend1Tan - testPha1;
    k12 = k12-(qint32)(k12/360)*360;
    if (k12<0)
        k12 = k12+360;
    k13 = amp2Amend2*1e6/(testAmp1*coeff3);
    k14 = ampAmend2Tan - testPha1;
    k14 = k14-(qint32)(k14/360)*360;
    if (k14 < 0)
        k14 = k14 + 360;
}

void CalculateFunction::calRight(qreal f)
{
    qreal amp1Amend1, phase1Amend1, amp2Amend2, phase2Amend2;
    qreal ampAmend1X, ampAmend1Y, ampAmend2X, ampAmend2Y;
    qreal ampAmend1NoX, ampAmend1NoY, ampAmend2NoX, ampAmend2NoY;
    qreal ampAmend1XX, ampAmend1YY, ampAmend2XX, ampAmend2YY;
    amp1Amend1 = ampAmend1;
    phase1Amend1 = phaseAmend1;
    amp2Amend2 = ampAmend2;
    phase2Amend2 = phaseAmend2;
    ampAmend1X = amp1Amend1*qCos((phase1Amend1)*Pi/180);
    ampAmend1Y = amp1Amend1*qSin((phase1Amend1)*Pi/180);

    ampAmend2X = amp2Amend2*qCos((phase2Amend2)*Pi/180);
    ampAmend2Y = amp2Amend2*qSin((phase2Amend2)*Pi/180);

    ampAmend1NoX = ampAmend1No*qCos((phaseAmend1No)*Pi/180);
    ampAmend1NoY = ampAmend1No*qSin((phaseAmend1No)*Pi/180);

    ampAmend2NoX = ampAmend2No*qCos((phaseAmend2No)*Pi/180);
    ampAmend2NoY = ampAmend2No*qSin((phaseAmend2No)*Pi/180);

    ampAmend1XX = ampAmend1X - ampAmend1NoX ;
    ampAmend1YY = ampAmend1Y - ampAmend1NoY ;

    ampAmend2XX = ampAmend2X - ampAmend2NoX ;
    ampAmend2YY = ampAmend2Y - ampAmend2NoY ;

    amp1Amend1 = qSqrt(qPow(ampAmend1XX,2) + qPow(ampAmend1YY,2));
    qreal ampAmend1Tan = calculatePhase(ampAmend1YY,ampAmend1XX);
    amp2Amend2 = qSqrt(qPow(ampAmend2XX,2) + qPow(ampAmend2YY,2));
    qreal ampAmend2Tan = calculatePhase(ampAmend2YY,ampAmend2XX);
    qreal coeff2 = r2*c*qPow(f,3)/(a+b+c);
    qreal coeff4 = r2*(a+b)*qPow(f,3)/(a+b+c);

    k21 = amp1Amend1*1e6/(testAmp2*coeff2);
    k22 = ampAmend1Tan - testPha2;
    k22 = k22-(qint32)(k22/360)*360;
    if (k22<0)
        k22 = k22 + 360;
    k23 = amp2Amend2*1e6/(testAmp2*coeff4);
    k24 = ampAmend2Tan - testPha2;
    k24 = k24 - (qint32)(k24/360)*360;
    if (k24 < 0)
        k24 = k24 + 360;
}

void CalculateFunction::calSave()
{
    /*if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        m_standardOutput << QString::fromLocal8Bit("Can't open the file") << endl;
    QDataStream out(&file);
    out << "%1\n%2\n%3\n%4\n",k11,k12,k13,k14;
    out << "%1\n%2\n%3\n%4\n",k21,k22,k23,k24;
    file.close();*/

    QFile f(".//test.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
        return ;
    }

    QTextStream txtOutput(&f);


    QString s1(QString::number(k11));
    txtOutput << s1 << endl;
    s1=QString::number(k12);
    txtOutput << s1 << endl;
    s1=QString::number(k13);
    txtOutput << s1 << endl;
    s1=QString::number(k14);
    txtOutput << s1 << endl;
    s1=QString::number(k21);
    txtOutput << s1 << endl;
    s1=QString::number(k22);
    txtOutput << s1 << endl;
    s1=QString::number(k23);
    txtOutput << s1 << endl;
    s1=QString::number(k24);
    txtOutput << s1 << endl;


    txtOutput << s1 << endl;


    f.close();
}




