#ifndef BALANCINGSYS_H
#define BALANCINGSYS_H

#include <QDialog>
#include <QtWidgets>
#include <QPixmap>
#include "serialthread.h"
#include "calculatefunction.h"

namespace Ui {
class BalancingSys;
}

class BalancingSys : public QDialog
{
    Q_OBJECT

public:
    explicit BalancingSys(QWidget *parent = 0);
    ~BalancingSys();
    SerialThread serialThread;



private slots:
    void on_ui_window6_btn_startButton_clicked ( );
    void startButtonChange ( );
    void calibrationButtonChange ( );
    void SerialOpenError (int errorCode);
    void speedDisplayRefresh (int fre);
    void dataCalculate (qreal d11, qreal d12, qreal d21, qreal d22, qreal f);
    void resizeEvent(QResizeEvent *);

    void on_paramentOkBtn_clicked();

    void on_ui_window8_btn_calibrateButton_clicked();

    void on_ui_parameterok_clicked();

    void on_ui_window1_btn_newzhuanzifile_clicked();

    void on_ui_window1_btn_loadzhuanzifile_clicked();

    void on_uilabel_window1_main_clicked();

    void on_uilabel_window5_allparameter_clicked();

    void on_uilabel_window6_polar_clicked();

    void on_uilabel_window7_maincalibrate_clicked();

    void on_uilabel_window0_welcome_clicked();

    void on_ui_window0_cb_comport_currentIndexChanged(const QString &arg1);

    void on_ui_window0_btn_next_clicked();

    void on_uilabel_window2_supportMethod_clicked();

    void on_uilabel_window3_unitMethod_clicked();

    void on_ui_window2_btn_next_clicked();

    void on_ui_window3_btn_next_clicked();

    void on_uilabel_window8_calibratetip_clicked();

    void on_uilabel_window70_testmass_clicked();

    void on_ui_window1_btn_exit_clicked();

    void on_ui_window2_btn_back_clicked();

    void on_ui_window3_btn_previous_clicked();

    void on_ui_window30_btn_previous_clicked();

    void on_ui_window30_btn_next_clicked();

    void on_ui_window5_btn_cancel_clicked();

    void on_ui_window5_btn_previous_clicked();

    void on_ui_window5_btn_save_clicked();

    void on_ui_window8_btn_next_clicked();

    void on_ui_window6_btn_calibrate_clicked();

    void on_ui_window7_btn_calibration_clicked();

    void on_ui_window70_btn_ok_clicked();

    void on_ui_window7_btn_calibrationparameter_clicked();

    void on_ui_window6_btn_parameterorgraphic_clicked();

    void on_ui_window0_btn_selftest_clicked();

    void on_ui_window6_btn_measure_clicked();

    void on_ui_window6_btn_back_clicked();

    void on_ui_window8_btn_back_clicked();

    void on_ui_window9_btn_back_clicked();

private:
    void initui_window0_cb_comport( );
    void dataDisplayFresh(qreal a1, qreal p1, qreal a2, qreal p2);
    void switchWindowTo(int window);
    void hideall();
    void showallobjinwindow(int window);
    bool loadFile(const QString &filename);
    bool saveFile(const QString &filename);
    void saveTheParameter();
    void drawPolarPoint(qreal a, qreal p,QLabel *polar);
    void showParameter();


    CalculateFunction calculateFunction;
    qint32 startButtonFlag;
    qint32 calibrationFlag;
    qint32 stepCalibration;   //0:无试重；1：左平面；2：右平面
    qint32 stepWeightSet;
    Ui::BalancingSys *ui;

    QButtonGroup *supportMethodGroup;
    QButtonGroup *leftCorrectMethodGroup;
    QButtonGroup *rightCorrectMethodGroup;
    QButtonGroup *displayModeGroup;
    QButtonGroup *rotationGroup;

    qint32 windowHeight,windowWidth,initialHeight,initialWidth;

    QPixmap pic1,pic2,pic3,pic4,pic5,pic6;

};

#endif // BALANCINGSYS_H
