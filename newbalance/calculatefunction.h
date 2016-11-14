#ifndef CALCULAFUNCTION
#define CALCULAFUNCTION

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QDataStream>


class CalculateFunction : public QObject
{
    Q_OBJECT

public:
    CalculateFunction(QObject *parent = 0);
    ~CalculateFunction();
    qreal ampAmend1, phaseAmend1, ampAmend2, phaseAmend2;
    void calRead();
    void measureProcess (qreal d11, qreal d12, qreal d21, qreal d22, qreal f);
    void calculateProcess (qreal f);
    void calNone();
    void calLeft(qreal f);
    void calRight(qreal f);
    void calSave();

//    void calLeft ();

    qreal testPha1, testPha2;        //标定位置
    qreal testAmp1, testAmp2;        //标定质量

    QFile file;

    qreal k11Amp, k11Pha, k12Amp, k12Pha;
    qreal k21Amp, k21Pha, k22Amp, k22Pha;

    qreal ampEnd1, ampEnd2;
    qreal phaseEnd1, phaseEnd2;
    qreal a, b, c, r1, r2;

    qreal leftTolerance,rightTolerance;
    qreal leftDividedNum,rightDividedNum;
    qreal leftInitialAngle,rightInitialAngle;

    int supportMethod;
    int leftCorrectMethod;
    int rightCorrectMethod;
    int displayMode;
    int rotation;


private:
    qreal calculatePhase (qreal x, qreal y);
    QTextStream m_standardOutput;

    qreal compensate1A1, compensate1P1, compensate1A2, compensate1P2;
    qreal compensate2A1, compensate2P1, compensate2A2, compensate2P2;
    qreal compensate3A1, compensate3P1, compensate3A2, compensate3P2;
    quint8 addDelete1, addDelete2;
//    qreal k11Amp, k11Pha, k12Amp, k12Pha;
//    qreal k21Amp, k21Pha, k22Amp, k22Pha;

    qreal ampAmend1No, phaseAmend1No;							//平面1标定残余量
    qreal ampAmend2No, phaseAmend2No;							//平面2标定残余量
    qreal k11, k12, k13, k14, k21, k22, k23, k24;							//标定系数暂存
};

#endif // CALCULAFUNCTION

