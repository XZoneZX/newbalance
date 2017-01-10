#include "balancingsys.h"
#include "ui_balancingsys.h"


#include <QMessageBox>
#include <QtWidgets>
#include <Qdir>
#include <QCoreApplication>
#define  PI 3.14159265


BalancingSys::BalancingSys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BalancingSys)
{
    ui->setupUi(this);
    initui_window0_cb_comport( );
    ui -> ui_window6_lb_speedDisplay -> setText("");
    ui -> ui_window6_lb_leftWeightDisplay -> setText("");
    ui -> ui_window6_lb_leftAngleDisplay -> setText("");
    ui -> ui_window6_lb_rightWeightDisplay -> setText("");
    ui -> ui_window6_lb_rightAngleDisplay -> setText("");
    ui -> speedInput -> setText(QString::fromLocal8Bit("%1"). arg(serialThread.speedValue));
    ui -> speedThreadInput -> setText(QString::fromLocal8Bit("%1"). arg(serialThread.speedThreadValue));
    ui -> samplingPeriodInput -> setText(QString::fromLocal8Bit("%1"). arg(serialThread.samplingPeriodValue));
    startButtonFlag = 0;
    calibrationFlag = 0;
    stepCalibration = 0;
    serialThread.step = 1;
    stepWeightSet = 0;
    connect (&serialThread, SIGNAL (changeStartButton ( )), this, SLOT (startButtonChange ( )));
    connect (&serialThread, SIGNAL (changeCalibrationButton()), this, SLOT (calibrationButtonChange()));
    connect (&serialThread, SIGNAL (errorSerialOpen (int)), this, SLOT (SerialOpenError(int)));
    connect (&serialThread, SIGNAL (refreshSpeedDisplay (int)), this, SLOT (speedDisplayRefresh (int)));
    connect (&serialThread, SIGNAL (dataOk (qreal, qreal, qreal, qreal, qreal)), this, SLOT (dataCalculate (qreal, qreal, qreal, qreal, qreal)));
    QDir::setCurrent ( QCoreApplication::applicationDirPath () );
    calculateFunction.calRead();
    ui -> ui_window9_ledit_k11AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k11Amp));
    ui -> ui_window9_ledit_k11PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k11Pha));
    ui -> ui_window9_ledit_k12AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k12Amp));
    ui -> ui_window9_ledit_k12PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k12Pha));
    ui -> ui_window9_ledit_k21AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k21Amp));
    ui -> ui_window9_ledit_k21PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k21Pha));
    ui -> ui_window9_ledit_k22AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k22Amp));
    ui -> ui_window9_ledit_k22PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k22Pha));
    ui->ui_a->setText(QString::number( calculateFunction.a));
    ui->ui_b->setText(QString::number( calculateFunction.b));
    ui->ui_c->setText(QString::number( calculateFunction.c));
    ui->ui_D->setText(QString::number( calculateFunction.r1*2));



    //自检串口
    int COMPortIndex = ui -> ui_window0_cb_comport -> currentIndex( );
    serialThread.PortName = (ui -> ui_window0_cb_comport -> itemText (COMPortIndex));
    serialThread.tmp_checkSerialPort(serialThread.PortName);

    //进行radiobutton分组
    supportMethodGroup = new QButtonGroup;
    supportMethodGroup->addButton(ui->ui_window2_rb_method1);
    supportMethodGroup->addButton(ui->ui_window2_rb_method2);
    supportMethodGroup->addButton(ui->ui_window2_rb_method3);
    supportMethodGroup->addButton(ui->ui_window2_rb_method4);
    supportMethodGroup->addButton(ui->ui_window2_rb_method5);
    supportMethodGroup->addButton(ui->ui_window2_rb_method6);
    ui->ui_window2_rb_method1->setChecked(true);

    leftCorrectMethodGroup = new QButtonGroup;
    leftCorrectMethodGroup->addButton(ui->ui_window30_rb_leftaddweight);
    leftCorrectMethodGroup->addButton(ui->ui_window30_rb_leftremoveweight);
    ui->ui_window30_rb_leftremoveweight->setChecked(true);

    rightCorrectMethodGroup = new QButtonGroup;
    rightCorrectMethodGroup->addButton(ui->ui_window30_rb_rightaddweight);
    rightCorrectMethodGroup->addButton(ui->ui_window30_rb_rightremoveweight);
    ui->ui_window30_rb_rightremoveweight->setChecked(true);

    displayModeGroup = new QButtonGroup;
    displayModeGroup->addButton(ui->ui_window30_rb_dynamic);
    displayModeGroup->addButton(ui->ui_window30_rb_static);
    displayModeGroup->addButton(ui->ui_window30_rb_couplestatic);
    displayModeGroup->addButton(ui->ui_window30_rb_staticcouple);
    ui->ui_window30_rb_dynamic->setChecked(true);

    rotationGroup =new QButtonGroup;
    rotationGroup->addButton(ui->ui_window30_rb_forward);
    rotationGroup->addButton(ui->ui_window30_rb_backward);
    ui->ui_window30_rb_forward->setChecked(true);

    //lineedit数字限制
    QValidator *validator=new QDoubleValidator(this);
    QObjectList list = children();
    QLineEdit *c;
    foreach (QObject *obj, list) {
        c = qobject_cast<QLineEdit*>(obj);
        if(c&&(QRegExp(".*ledit.*").exactMatch(c->objectName()))){
            c->setValidator(validator);
        }
    }

    //设置初始单位
     ui->ui_window70_cb_tip1->setCurrentIndex(1);


//读取样式文件
    QString qss;
    QFile qssFile(":/new/prefix1/qssfile");

    qssFile.open(QFile::ReadOnly);

    if(qssFile.isOpen())
    {
    qDebug()<<"qssopen"<<endl;
    qss = qssFile.readAll().data();
        qApp->setStyleSheet(qss);

        qssFile.close();

    }

}

BalancingSys::~BalancingSys()
{
    delete ui;
}

void BalancingSys::showallobjinwindow(int window)
{
    hideall();
    QObjectList list = children();
    QWidget *b;
    foreach (QObject *obj, list) {
        b = qobject_cast<QWidget*>(obj);
        if(b&&(QRegExp("ui_window"+QString::number(window)+"_.*").exactMatch(b->objectName()))){
            b->setHidden(false);
        }
    }
}

void BalancingSys::switchWindowTo(int window)
{
    switch (window)
    {
    /*显示窗口的控件*/
        case 0:showallobjinwindow(0);break;
        case 1:showallobjinwindow(1);break;
        case 2:showallobjinwindow(2);break;
        case 3:showallobjinwindow(3);break;
        case 4:
        case 5:showallobjinwindow(5);break;
        case 6:showallobjinwindow(6);break;
        case 7:showallobjinwindow(7);break;
        case 8:showallobjinwindow(8);break;
        case 9:showallobjinwindow(9);break;
        case 30:showallobjinwindow(30);break;
        case 70:showallobjinwindow(70);break;
        default:break;
    }
}
void BalancingSys::hideall()
{
    QObjectList list = children();
        QWidget *b;
        foreach (QObject *obj, list) {
            b = qobject_cast<QWidget*>(obj);
            if(b&&(!QRegExp("uilabel.*").exactMatch(b->objectName()))){
                b->setHidden(true);
            }
        }
}

void BalancingSys::initui_window0_cb_comport( )
{
    //
    // COM端口选择组合框参数设置:
    //
    static int const NUM_COMS = 16;
    ui -> ui_window0_cb_comport -> clear( );

    for ( int i = 1 ; i <= NUM_COMS ; ++i )
    {
        ui -> ui_window0_cb_comport -> addItem( QString( "COM%1" ).arg( i ) );
    }
}

void BalancingSys::dataDisplayFresh(qreal a1, qreal p1, qreal a2, qreal p2)
{
    ui -> ui_window6_lb_leftWeightDisplay -> setText (QString::fromLocal8Bit("%1"). arg(a1));
    ui -> ui_window6_lb_leftAngleDisplay -> setText (QString::fromLocal8Bit("%1"). arg(p1));
    ui -> ui_window6_lb_rightWeightDisplay -> setText (QString::fromLocal8Bit("%1"). arg(a2));
    ui -> ui_window6_lb_rightAngleDisplay -> setText (QString::fromLocal8Bit("%1"). arg(p2));

    drawPolarPoint(a1,p1,ui->ui_window6_gvlb_polar1);
    drawPolarPoint(a2,p2,ui->ui_window6_gvlb_polar2);


}

void BalancingSys::startButtonChange ( )
{
    if(!startButtonFlag)
    {
        ui -> ui_window6_btn_startButton -> setText( QString::fromLocal8Bit( "停止检测" ) );
        startButtonFlag = 1;
    }
    else
    {
        ui -> ui_window6_btn_startButton -> setText( QString::fromLocal8Bit( "开始检测" ) );
        startButtonFlag = 0;
    }
}

void BalancingSys::calibrationButtonChange ( )
{
    if(!calibrationFlag)
    {
        ui -> ui_window8_btn_calibrateButton -> setEnabled(false);
        if(stepCalibration == 0)
        {
            ui -> ui_window8_btn_calibrateButton -> setText(QString::fromLocal8Bit("下一步"));
            ui -> ui_window8_lb_calibrateLabel -> setText(QString::fromLocal8Bit("请不放试重开车;\n调整转速到%1r/min左右！").arg(serialThread.speedValue));
        }
        if(stepCalibration == 1)
        {
            ui -> ui_window8_btn_calibrateButton -> setText(QString::fromLocal8Bit("下一步"));
            ui -> ui_window8_lb_calibrateLabel -> setText(QString::fromLocal8Bit
                                            ("请在平面1的%1°加试重%2g;\n开车调整转速到%3r/min左右！").
                                            arg(calculateFunction.testPha1).arg(calculateFunction.testAmp1).arg(serialThread.speedValue));

        }
        if(stepCalibration == 2)
        {
            ui -> ui_window8_btn_calibrateButton -> setText(QString::fromLocal8Bit("标定"));
            ui -> ui_window8_lb_calibrateLabel -> setText(QString::fromLocal8Bit
                                            ("请在平面2的%1°加试重%2g;\n开车调整转速到%3r/min左右！").
                                            arg(calculateFunction.testPha2).arg(calculateFunction.testAmp2).arg(serialThread.speedValue));
        }
        calibrationFlag = 1;
    }
    else
    {
        if(stepCalibration == 0)
        {
            ui -> ui_window8_lb_calibrateLabel -> setText(QString::fromLocal8Bit("第一步完成，请停车！"));
            stepCalibration++;
        }
        else if(stepCalibration == 1)
        {
            ui -> ui_window8_lb_calibrateLabel -> setText(QString::fromLocal8Bit("第二步完成，请停车！"));
            stepCalibration++;
        }
        else if(stepCalibration == 2)
        {
            ui -> ui_window8_lb_calibrateLabel -> setText(QString::fromLocal8Bit("标定完成，请停车！"));
            stepCalibration = 0;
            calibrationFlag = 0;
            calculateFunction.calSave();
            calculateFunction.calRead();
            ui -> ui_window9_ledit_k11AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k11Amp));
            ui -> ui_window9_ledit_k11PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k11Pha));
            ui -> ui_window9_ledit_k12AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k12Amp));
            ui -> ui_window9_ledit_k12PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k12Pha));
            ui -> ui_window9_ledit_k21AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k21Amp));
            ui -> ui_window9_ledit_k21PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k21Pha));
            ui -> ui_window9_ledit_k22AmpDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k22Amp));
            ui -> ui_window9_ledit_k22PhaDisplay -> setText(QString::fromLocal8Bit("%1"). arg(calculateFunction.k22Pha));
            serialThread.step = 1;
            switchWindowTo(9);
        }
       // stepCalibration++;
        calibrationFlag = 0;
        ui -> ui_window8_btn_calibrateButton -> setEnabled(true);
    }
}

void BalancingSys::SerialOpenError (int errorCode)
{
    //
    // 串口错误提示信息表:
    //
    if(errorCode != 7)
    {
        static const QString COM_ERROR_HINT[ ] = {
            QString::fromLocal8Bit( "串口未被占用，但未连接机器" ) ,
            QString::fromLocal8Bit( "选定串口已被占用或者不存在" ) ,
            QString::fromLocal8Bit( "设置波特率失败" ) ,
            QString::fromLocal8Bit( "设置数据位失败" ) ,
            QString::fromLocal8Bit( "设置校验模式失败" ) ,
            QString::fromLocal8Bit( "设置停止位失败" ) ,
            QString::fromLocal8Bit( "设置流控制失败" )
        };
        /*ui->ui_window0_comcondition->setText(QString::fromLocal8Bit("串口通讯异常"));

        QMessageBox::warning( this ,
                                 QString::fromLocal8Bit( "无法打开选定串口" ) ,
                                 QString::fromLocal8Bit( "%1" ).arg( COM_ERROR_HINT[errorCode] )
                               );*/
        ui->ui_window0_comcondition->setText(QString::fromLocal8Bit("串口通讯异常\n")+QString::fromLocal8Bit( "%1" ).arg( COM_ERROR_HINT[errorCode]));
        ui->ui_window0_selftest->setDisabled(true);

    }
    else
    {
        ui->ui_window0_comcondition->setText(QString::fromLocal8Bit("串口通讯正常"));
        ui->ui_window0_selftest->setDisabled(false);
    }
}

void BalancingSys::speedDisplayRefresh (int fre)
{
    ui -> ui_window6_lb_speedDisplay -> setText(QString::fromLocal8Bit( "%1" ).arg(fre));
}

void BalancingSys::dataCalculate (qreal d11, qreal d12, qreal d21, qreal d22, qreal f)
{
    calculateFunction. measureProcess(d11, d12, d21, d22, f);
    if(serialThread.step == 0)
    {
        if(stepCalibration == 0)
        {
            calculateFunction.calNone();
        }
        else if(stepCalibration == 1)
        {
            calculateFunction.calLeft(f);
        }
        else if(stepCalibration == 2)
        {
            calculateFunction.calRight(f);
        }
    }
    else if(serialThread.step == 1)
    {
        calculateFunction. calculateProcess(f);
        printf("____amp_end1=%f\n", calculateFunction.ampEnd1);
        printf("____amp_end2=%f\n", calculateFunction.ampEnd2);
        printf("____phase_end1=%f\n",calculateFunction.phaseEnd1);
        printf("____phase_end2=%f\n", calculateFunction.phaseEnd2);
        dataDisplayFresh(calculateFunction.ampEnd1, calculateFunction.phaseEnd1, calculateFunction.ampEnd2, calculateFunction.phaseEnd2);
    }
}



void BalancingSys::on_ui_window6_btn_startButton_clicked()
{
    if (!startButtonFlag)
    {
        int COMPortIndex = ui -> ui_window0_cb_comport -> currentIndex( );
        serialThread.PortName = (ui -> ui_window0_cb_comport -> itemText (COMPortIndex));
        ui -> ui_window6_lb_speedDisplay -> setText("");
        ui -> ui_window6_lb_leftWeightDisplay -> setText("");
        ui -> ui_window6_lb_leftAngleDisplay -> setText("");
        ui -> ui_window6_lb_rightWeightDisplay -> setText("");
        ui -> ui_window6_lb_rightAngleDisplay -> setText("");
        serialThread.startSlave ( );
    }
    else
        serialThread.stopSlave ( );
}

void BalancingSys::on_ui_window8_btn_calibrateButton_clicked()
{
    if(!calibrationFlag)
    {
        serialThread.step = 0;
        int COMPortIndex = ui -> ui_window0_cb_comport -> currentIndex( );
        serialThread.PortName = (ui -> ui_window0_cb_comport -> itemText (COMPortIndex));
        ui -> ui_window6_lb_speedDisplay -> setText("");
        ui -> ui_window6_lb_leftWeightDisplay -> setText("");
        ui -> ui_window6_lb_leftAngleDisplay -> setText("");
        ui -> ui_window6_lb_rightWeightDisplay -> setText("");
        ui -> ui_window6_lb_rightAngleDisplay -> setText("");
        serialThread.startSlave ( );
    }
}

void BalancingSys::on_paramentOkBtn_clicked()
{
    QString str = ui -> speedInput -> text();
    serialThread.speedValue = str.toInt();
    str = ui -> speedThreadInput -> text();
    serialThread.speedThreadValue = str.toInt();
    str = ui -> samplingPeriodInput -> text();
    serialThread.samplingPeriodValue = str.toInt();
    str = ui -> calibrateWeightInput1 -> text();
    calculateFunction.testAmp1 = str.toFloat();
    str = ui -> calibrateAngleInput1 -> text();
    calculateFunction.testPha1 = str.toFloat();
    str = ui -> calibrateWeightInput1 -> text();
    calculateFunction.testAmp2 = str.toFloat();
    str = ui -> calibrateAngleInput2 -> text();
    calculateFunction.testPha2 = str.toFloat();
}



void BalancingSys::on_ui_parameterok_clicked()
{


    calculateFunction.a= ui -> ui_a -> text().toInt();
    calculateFunction.b= ui -> ui_b -> text().toInt();
    calculateFunction.c= ui -> ui_c -> text().toInt();
    calculateFunction.r1= ui -> ui_D -> text().toFloat()/2;
    calculateFunction.r2= ui -> ui_D -> text().toFloat()/2;



}

void BalancingSys::on_ui_window1_btn_newzhuanzifile_clicked()
{
    switchWindowTo(2);
}

void BalancingSys::saveTheParameter()
{
    QStringList nameFilters;
    nameFilters<< "Text files (*.txt)";
    QFileDialog dialog(this, tr("save File"),QDir::currentPath());
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilters(nameFilters);
    dialog.setDefaultSuffix("txt");
    dialog.setFileMode(QFileDialog::AnyFile);

  //  QImage image(scene2->width(), scene2->height(),QImage::Format_ARGB32);
  //  QPainter painter(&image);
  //  painter.setRenderHint(QPainter::Antialiasing);
   // scene2->render(&painter);

  while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles()[0])) {}
}

bool BalancingSys::saveFile(const QString &filename)
{
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream txtOutput(&f);
    QString s1(QString::number(calculateFunction.a));
    txtOutput << s1 << endl;
    s1=QString::number(calculateFunction.b);
    txtOutput << s1 << endl;
    s1=QString::number(calculateFunction.c);
    txtOutput << s1 << endl;
    s1=QString::number(calculateFunction.r1);
    txtOutput << s1 << endl;
    s1=QString::number(calculateFunction.r2);
    txtOutput << s1 << endl;
    s1=QString::number(serialThread.speedValue);
    txtOutput << s1 << endl;
    s1=QString::number(serialThread.speedThreadValue);
    txtOutput << s1 << endl;
    s1=QString::number(serialThread.samplingPeriodValue);
    txtOutput << s1 << endl;



    f.close();


    return true;
}

void BalancingSys::on_ui_window1_btn_loadzhuanzifile_clicked()
{
        QStringList nameFilters;
        nameFilters<< "Text files (*.txt)";
        QFileDialog dialog(this, tr("Open File"),QDir::currentPath() );
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setNameFilters(nameFilters);

        while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) { }
}

bool BalancingSys::loadFile(const QString &filename)
{
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream txtInput(&f);

    calculateFunction.a = txtInput.readLine().toDouble();
    calculateFunction.b = txtInput.readLine().toDouble();
    calculateFunction.c = txtInput.readLine().toDouble();
    calculateFunction.r1 = txtInput.readLine().toDouble();
    calculateFunction.r2 = txtInput.readLine().toDouble();
    serialThread.speedValue = txtInput.readLine().toInt();
    serialThread.speedThreadValue = txtInput.readLine().toInt();
    serialThread.samplingPeriodValue = txtInput.readLine().toInt();


    f.close();
    switchWindowTo(6);
    return true;
}

void BalancingSys::on_uilabel_window1_main_clicked()
{
    switchWindowTo(1);
}

void BalancingSys::on_uilabel_window5_allparameter_clicked()
{
    switchWindowTo(5);
}

void BalancingSys::on_uilabel_window6_polar_clicked()
{
    switchWindowTo(6);
    drawPolarPoint(0,0,ui->ui_window6_gvlb_polar1);
    drawPolarPoint(0,0,ui->ui_window6_gvlb_polar2);
}

void BalancingSys::on_uilabel_window7_maincalibrate_clicked()
{
    switchWindowTo(7);
}

void BalancingSys::on_uilabel_window0_welcome_clicked()
{
    switchWindowTo(0);
}

void BalancingSys::on_ui_window0_cb_comport_currentIndexChanged(const QString &arg1)
{

    serialThread.PortName = arg1;
    serialThread.tmp_checkSerialPort(serialThread.PortName);

}

void BalancingSys::drawPolarPoint(qreal a, qreal p,QLabel *polar)
{
    int width = polar->width();
    int height = polar->height();
    qreal maxa = 4;
    int pointsize = 6;

    QPixmap pix(width,height); //作为绘图设备
    QPainter painter(&pix); //创建一直画笔
    painter.setPen(Qt::black);
    painter.drawArc(0.1*width,0.1*height,0.8*width,0.8*height,0*16,360*16);
    painter.drawArc(0.2*width,0.2*height,0.6*width,0.6*height,0*16,360*16);
    painter.drawArc(0.3*width,0.3*height,0.4*width,0.4*height,0*16,360*16);
    painter.drawArc(0.4*width,0.4*height,0.2*width,0.2*height,0*16,360*16);
    painter.drawLine(0,height/2,width,height/2);
    painter.drawLine(width/2,0,width/2,height);
    painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
    static const QPointF points[4] = {QPointF(0.95*width,0.5*height),QPointF(0.95*width,0.51*height),QPointF(width,0.5*height),QPointF(0.95*width,0.49*height)};
    painter.drawPolygon(points,4);
    static const QPointF points1[4] = {QPointF(0.5*width,0.05*height),QPointF(0.51*width,0.05*height),QPointF(0.5*width,0),QPointF(0.49*width,0.05*height)};
    painter.drawPolygon(points1,4);
    painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
    painter.drawEllipse(0.5*width+cos(p/180*PI)*a/maxa*width/2-pointsize/2,0.5*height-sin(p/180*PI)*a/maxa*height/2-pointsize/2,pointsize,pointsize);

    polar->setPixmap(pix);
}



void BalancingSys::on_ui_window0_next_clicked()
{
    switchWindowTo(1);
}

void BalancingSys::on_uilabel_window2_supportMethod_clicked()
{
    switchWindowTo(2);
}

void BalancingSys::on_uilabel_window3_unitMethod_clicked()
{
    switchWindowTo(3);
}

void BalancingSys::on_ui_window2_btn_next_clicked()
{
    if(ui->ui_window2_rb_method1->isChecked())calculateFunction.supportMethod = 1;
    else if(ui->ui_window2_rb_method2->isChecked())calculateFunction.supportMethod = 2;
    else if(ui->ui_window2_rb_method3->isChecked())calculateFunction.supportMethod = 3;
    else if(ui->ui_window2_rb_method4->isChecked())calculateFunction.supportMethod = 4;
    else if(ui->ui_window2_rb_method5->isChecked())calculateFunction.supportMethod = 5;
    else calculateFunction.supportMethod = 6;
    switchWindowTo(3);
}

void BalancingSys::on_ui_window3_btn_next_clicked()
{
    switch(ui->ui_window3_cb_a->currentIndex())
    {
    case 1:calculateFunction.a = ui->ui_window3_ledit_a->text().toDouble()*10.0;break;
    case 2:calculateFunction.a = ui->ui_window3_ledit_a->text().toDouble()*1000.0;break;
    default:calculateFunction.a = ui->ui_window3_ledit_a->text().toDouble();
    }
    serialThread.speedValue = ui->ui_window3_ledit_presetspeed->text().toDouble();
    serialThread.speedThreadValue = ui->ui_window3_ledit_speedthreshold->text().toDouble();
    serialThread.samplingPeriodValue = ui->ui_window3_ledit_continuousnum->text().toDouble();

    calculateFunction.leftTolerance = ui->ui_window3_ledit_lefttolerance->text().toDouble();
    calculateFunction.leftDividedNum = ui->ui_window3_ledit_leftdividednum->text().toDouble();
    switch(ui->ui_window3_cb_leftinitialangle->currentIndex())
    {
    case 1:calculateFunction.leftInitialAngle = ui->ui_window3_ledit_leftinitialangle->text().toDouble()*1000;break;
    case 2:calculateFunction.leftInitialAngle = ui->ui_window3_ledit_leftinitialangle->text().toDouble()*1000000;break;
    default:calculateFunction.leftInitialAngle = ui->ui_window3_ledit_leftinitialangle->text().toDouble();
    }
    calculateFunction.rightTolerance = ui->ui_window3_ledit_righttolerance->text().toDouble();
    calculateFunction.rightDividedNum = ui->ui_window3_ledit_rightdividednum->text().toDouble();
    switch(ui->ui_window3_cb_rightinitialangle->currentIndex())
    {
    case 1:calculateFunction.rightInitialAngle = ui->ui_window3_ledit_rightinitialangle->text().toDouble()*1000;break;
    case 2:calculateFunction.rightInitialAngle = ui->ui_window3_ledit_rightinitialangle->text().toDouble()*1000000;break;
    default:calculateFunction.rightInitialAngle = ui->ui_window3_ledit_rightinitialangle->text().toDouble();
    }


    switchWindowTo(30);
}

void BalancingSys::on_uilabel_window8_calibratetip_clicked()
{
    switchWindowTo(8);
}

void BalancingSys::on_uilabel_window70_testmass_clicked()
{
    switchWindowTo(70);
}

void BalancingSys::on_ui_window1_btn_exit_clicked()
{
    exit(0);
}

void BalancingSys::on_ui_window2_btn_back_clicked()
{
    switchWindowTo(1);
}

void BalancingSys::on_ui_window3_btn_previous_clicked()
{
    switchWindowTo(2);
}

void BalancingSys::on_ui_window30_btn_previous_clicked()
{
    switchWindowTo(3);
}

void BalancingSys::on_ui_window30_btn_next_clicked()
{

    if(ui->ui_window30_rb_leftremoveweight->isChecked())calculateFunction.leftCorrectMethod = 1;
    else calculateFunction.leftCorrectMethod = 2;

    if(ui->ui_window30_rb_rightremoveweight->isChecked())calculateFunction.rightCorrectMethod = 1;
    else calculateFunction.rightCorrectMethod = 2;

    if(ui->ui_window30_rb_dynamic->isChecked())calculateFunction.displayMode = 1;
    else if(ui->ui_window30_rb_staticcouple->isChecked())calculateFunction.displayMode = 2;
    else if(ui->ui_window30_rb_couplestatic->isChecked())calculateFunction.displayMode = 3;
    else calculateFunction.displayMode = 4;

    if(ui->ui_window30_rb_forward->isChecked())calculateFunction.rotation = 1;
    else calculateFunction.rotation = 2;


    //更新参数汇总显示
    QPixmap pic;
    pic.load(QString::fromLocal8Bit(":/new/prefix1/pic/0%1.jpg").arg(calculateFunction.supportMethod));
    ui->ui_window5_lbpic_support->setPixmap(pic);
    QString unit = ui->ui_window3_cb_a->currentText();
    ui->ui_window5_lb_abcr->setText(QString::fromLocal8Bit("A=%1%6    B=%2%6   C=%3%6   R1=%4%6   R2=%5%6")
                                    .arg(QString::number(calculateFunction.a),QString::number(calculateFunction.b),QString::number(calculateFunction.c)
                                         ,QString::number(calculateFunction.r1),QString::number(calculateFunction.r2),unit));
    QString unit1 = ui->ui_window3_cb_lefttolerance->currentText();
    QString unit2 = ui->ui_window3_cb_leftinitialangle->currentText();
    QString strLine;
    if(calculateFunction.leftCorrectMethod == 1)
        strLine = QString::fromLocal8Bit("校正方式：去  重 公差：");
    else
        strLine = QString::fromLocal8Bit("校正方式：加  重 公差：");
    strLine.append("%1%2" + QString::fromLocal8Bit(" 等分点数：")+"%3" + QString::fromLocal8Bit( "起始角度：")+"%4%5").arg(QString::number(calculateFunction.leftTolerance),unit1
                                                     ,QString::number(calculateFunction.leftDividedNum),QString::number(calculateFunction.leftInitialAngle),unit2);
    ui->ui_window5_lb_leftparameter->setText(strLine);

    unit1 = ui->ui_window3_cb_righttolerance->currentText();
    unit2 = ui->ui_window3_cb_rightinitialangle->currentText();
    if(calculateFunction.rightCorrectMethod == 1)
        strLine = QString::fromLocal8Bit("校正方式：去  重 公差：");
    else
        strLine = QString::fromLocal8Bit("校正方式：加  重 公差：");
    strLine.append("%1%2" + QString::fromLocal8Bit(" 等分点数：")+"%3" + QString::fromLocal8Bit( "起始角度：")+"%4%5").arg(QString::number(calculateFunction.rightTolerance),unit1
                                                     ,QString::number(calculateFunction.rightDividedNum),QString::number(calculateFunction.rightInitialAngle),unit2);
    ui->ui_window5_lb_rightparameter->setText(strLine);

    QString strDisplayMethod,strRotation;
    switch(calculateFunction.displayMode)
    {
    case 1:strDisplayMethod = QString::fromLocal8Bit("动平衡");break;
    case 2:strDisplayMethod = QString::fromLocal8Bit("静/偶平衡");break;
    case 3:strDisplayMethod = QString::fromLocal8Bit("偶/静平衡");break;
    case 4:strDisplayMethod = QString::fromLocal8Bit("静平衡");break;
    default:strDisplayMethod = QString::fromLocal8Bit("动平衡");
    }
    if(calculateFunction.rotation == 1)
        strRotation = QString::fromLocal8Bit("正转");
    else
        strRotation = QString::fromLocal8Bit("反转");
    strLine = QString::fromLocal8Bit("显示模式：")+strDisplayMethod+QString::fromLocal8Bit("预设转速：")
            +"%1"+QString::fromLocal8Bit(" 转向")+strRotation+QString::fromLocal8Bit("定位：无  补偿：无").arg(serialThread.speedValue);
    ui->ui_window5_lb_displaymode->setText(strLine);
    switchWindowTo(5);
}

void BalancingSys::on_ui_window5_btn_cancel_clicked()
{
    switchWindowTo(1);
}

void BalancingSys::on_ui_window5_btn_previous_clicked()
{
    switchWindowTo(30);
}

void BalancingSys::on_ui_window5_btn_save_clicked()
{
    saveTheParameter();
    switchWindowTo(6);
}

void BalancingSys::on_ui_window8_btn_next_clicked()
{
    on_ui_window8_btn_calibrateButton_clicked();
}

void BalancingSys::on_ui_window6_btn_calibrate_clicked()
{
    switchWindowTo(7);
}

void BalancingSys::on_ui_window7_btn_calibration_clicked()
{
    switchWindowTo(70);
}

void BalancingSys::on_ui_window70_btn_ok_clicked()
{
    QString str ;
    if(stepWeightSet==0)
    {
        str = ui -> ui_window70_ledit_mass1 -> text();
        calculateFunction.testAmp1 = str.toFloat();
        str = ui -> ui_window70_ledit_mass2 -> text();
        calculateFunction.testPha1 = str.toFloat();

        ui->ui_window70_lb_tip1->setText(QString::fromLocal8Bit("请输入：平面2试重质量"));
        ui->ui_window70_lb_tip2->setText(QString::fromLocal8Bit("请输入：平面2相位"));
        ui->ui_window70_ledit_mass1->setText("");
        ui->ui_window70_ledit_mass2->setText("");

        ui->ui_window70_btn_ok->setText(QString::fromLocal8Bit("标定"));

        stepWeightSet = 1;
    }
    else
    {

        str = ui -> ui_window70_ledit_mass1 -> text();
        calculateFunction.testAmp2 = str.toFloat();
        str = ui -> ui_window70_ledit_mass2 -> text();
        calculateFunction.testPha2 = str.toFloat();

        ui->ui_window70_lb_tip1->setText(QString::fromLocal8Bit("请输入：平面1试重质量"));
        ui->ui_window70_lb_tip2->setText(QString::fromLocal8Bit("请输入：平面1相位"));
        ui->ui_window70_ledit_mass1->setText("");
        ui->ui_window70_ledit_mass2->setText("");

        ui->ui_window70_btn_ok->setText(QString::fromLocal8Bit("下一步"));

        stepWeightSet = 0;

        switchWindowTo(8);
    }
}

void BalancingSys::on_ui_window7_btn_calibrationparameter_clicked()
{
    switchWindowTo(9);
}

void BalancingSys::on_ui_window6_btn_parameterorgraphic_clicked()
{
    calculateFunction.calSave();
}

void BalancingSys::on_ui_window0_selftest_clicked()
{
    QByteArray condition=serialThread.zijian();
    QString conditionDisplay;
    conditionDisplay=QString("zijianbuff= %1;%2;%3\n").arg(quint8(condition[0])).arg(quint8(condition[1])).arg(quint8(condition[2]));
    if ((quint8(condition[0]) == 0x01)&&(quint8(condition[1]) == 0x01))
    {
        if((condition[2]&0x01) == 0x01)
        {
            conditionDisplay+= QString::fromLocal8Bit("测量通道1正常\n");
        }
        else
        {
            conditionDisplay+= QString::fromLocal8Bit("测量通道1异常\n");
        }
        if((condition[2]&0x02) == 0x02)
        {
            conditionDisplay+= QString::fromLocal8Bit("测量通道2正常\n");
        }
        else
        {
            conditionDisplay+= QString::fromLocal8Bit("测量通道2异常\n");
        }
        if((condition[2]&0x04) == 0x04)
        {
            conditionDisplay+= QString::fromLocal8Bit("参考通道正常\n");
        }
        else
        {
            conditionDisplay+= QString::fromLocal8Bit("参考通道异常\n");
        }
    }
    ui->ui_window0_comcondition->setText((conditionDisplay));

}

void BalancingSys::on_ui_window6_btn_measure_clicked()
{

}

void BalancingSys::on_ui_window6_btn_back_clicked()
{
    switchWindowTo(1);
}

void BalancingSys::on_ui_window8_btn_back_clicked()
{
    switchWindowTo(6);
}
