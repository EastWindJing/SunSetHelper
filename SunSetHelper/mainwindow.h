#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sunset.h"
#include "QDateTime"
#include "QFile"
#include "PathUtils.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_btn_GenerateTimetable_clicked();

private:
    Ui::MainWindow *ui;
    SunSet* m_SunSetHelper;

    int m_TimeZone = 8;             //设备所在的时区，东为正，西为负
    double m_Longitude = 125.0;     //东经为正，西经为负
    double m_Latitude = 45.0;       //北纬为正，南纬为负
    QDate m_Timetable_BeginDate = QDate::currentDate();             //开始观测的日期
    QDate m_Timetable_EndDate =  QDate::currentDate().addDays(366); //结束观测的日期
    double m_SunZenithAngle = 96;   //太阳天顶角大于此角度时认为日落

};
#endif // MAINWINDOW_H
