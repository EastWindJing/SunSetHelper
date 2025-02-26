#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QDate"
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_SunSetHelper = new SunSet();

    m_Timetable_BeginDate = QDate::currentDate();               //开始日期
    ui->dateEdit_BeginDate->setDate(m_Timetable_BeginDate);
    m_Timetable_EndDate = QDate::currentDate().addDays(5);    //结束日期
    ui->dateEdit_EndDate->setDate(m_Timetable_EndDate);
    m_TimeZone = 8;                                             //时区
    ui->spinBox_TimeZone->setValue(m_TimeZone);
    m_Latitude = 30.6041382;                                    //纬度
    ui->lineEdit_latitude->setText(QString::number(m_Latitude,'f',7));
    m_Longitude = 114.2653871;                                  //经度
    ui->lineEdit_longitude->setText(QString::number(m_Longitude,'f',7));
    m_SunZenithAngle = 96.00;                                   //日落的太阳天顶角
    ui->doubleSpinBox_SunZenithAngle->setValue(m_SunZenithAngle);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//开始生成时间表
void MainWindow::on_btn_GenerateTimetable_clicked()
{
    //step1: 更新经纬度、时区、日落天顶角、开始日期、结束日期等信息
    m_TimeZone = ui->spinBox_TimeZone->value();                     //时区
    m_Latitude = ui->lineEdit_latitude->text().toDouble();          //纬度
    m_Longitude = ui->lineEdit_longitude->text().toDouble();        //经度
    m_SunZenithAngle = ui->doubleSpinBox_SunZenithAngle->value();   //日落的太阳天顶角
    m_Timetable_BeginDate = ui->dateEdit_BeginDate->date();         //开始日期
    m_Timetable_EndDate = ui->dateEdit_EndDate->date();             //结束日期

    //step2: 新建时间表文件并打开
    QString TimetableFullPath = PathUtils::getRootPath()+"/Setting/Timetable_UTC_"+ m_Timetable_BeginDate.toString("yyyyMMdd")
                                + "_" + m_Timetable_EndDate.toString("yyyyMMdd") + ".txt";
    qDebug()<<"TimetableFullPath = "<< TimetableFullPath;
    QFile TimetableFile(TimetableFullPath);
    // 打开文件，使用只读模式和文本模式
    if (!TimetableFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString ErrorMessage = "无法打开文件" + TimetableFullPath + "\n错误信息：" + TimetableFile.errorString();
        qDebug() << ErrorMessage;
    }

    //step3: 如果打开文件成功，就进行写入
    QDate sunset_date = m_Timetable_BeginDate.addDays(-1);
    QDate sunrise_date = m_Timetable_BeginDate.addDays(0);
    m_SunSetHelper->setPosition(m_Latitude, m_Longitude, m_TimeZone);
    for(int addDAY = 0; addDAY <= m_Timetable_BeginDate.daysTo(m_Timetable_EndDate); addDAY++)
    {
        //(1)计算日落时间
        m_SunSetHelper->setCurrentDate(sunset_date.year(), sunset_date.month(), sunset_date.day());
        double sunset_MinuteNum = m_SunSetHelper->calcCustomSunset(m_SunZenithAngle);
        int sunset_hours = static_cast<int>(sunset_MinuteNum) / 60;     // 取分钟的整数部分，除以 60 得到小时
        int sunset_minutes = static_cast<int>(sunset_MinuteNum) % 60;   // 取除小时后的剩余分钟数
        int sunset_seconds = static_cast<int>((sunset_MinuteNum - static_cast<int>(sunset_MinuteNum)) * 60);  // 小数部分转换为秒
        QTime sunset_time(sunset_hours, sunset_minutes, sunset_seconds);
        QDateTime sunset_DateTime(sunset_date, sunset_time);        //用Date和Time拼接出QDateTime格式的日落时间

        //(2)计算日出时间
        m_SunSetHelper->setCurrentDate(sunrise_date.year(), sunrise_date.month(), sunrise_date.day());
        double sunrise_MinuteNum = m_SunSetHelper->calcCustomSunrise(m_SunZenithAngle);
        int sunrise_hours = static_cast<int>(sunrise_MinuteNum) / 60;     // 取分钟的整数部分，除以 60 得到小时
        int sunrise_minutes = static_cast<int>(sunrise_MinuteNum) % 60;   // 取除小时后的剩余分钟数
        int sunrise_seconds = static_cast<int>((sunrise_MinuteNum - static_cast<int>(sunrise_MinuteNum)) * 60);  // 小数部分转换为秒
        QTime sunrise_time(sunrise_hours, sunrise_minutes, sunrise_seconds);
        QDateTime sunrise_DateTime(sunrise_date, sunrise_time);

        //(3)根据是否启用UTC调整时间
        sunset_DateTime = sunset_DateTime.toUTC();      //
        sunrise_DateTime = sunrise_DateTime.toUTC();
        qDebug()<< "日落" + sunset_DateTime.toString("yyyy-MM-dd hh:mm:ss") + "，至日出" + sunrise_DateTime.toString("yyyy-MM-dd hh:mm:ss") ;
        //(4)日期后移
        sunset_date = sunset_date.addDays(1);
        sunrise_date = sunrise_date.addDays(1);
        //(4)写入文件
        QTextStream out(&TimetableFile);     //创建 QTextStream 用于写入文本
        out << sunset_DateTime.toString("yyyy MM dd hh mm ss") << "-" << sunrise_DateTime.toString("yyyy MM dd hh mm ss") << "\n";
    }

    TimetableFile.close();



    //double sunset_MinuteNum = m_SunSetHelper->calcCivilSunset();




}

