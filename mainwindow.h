#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/cvdef.h"


#include "udpclient.h"
#include "frameupdater.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMutex*, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FrameUpdater *My_FrameUpdater;
    UdpClient *UDP_Command_Term;
    QThread *UDP_CommandThread, *My_FrameUpdaterThread;
    cv::Mat frame;
    QTimer* ptimer_MW;
    int SourceIsAvailableCounter = 0;
    QGraphicsPixmapItem pixmap;
    QMutex* m;

signals:
    void onSendUDP_PacketToAirUnit(QByteArray);
    void OpenSource();
    void EnableReadSummImage();

public slots:
//    void UDPReady(QByteArray);
    void SourceIsAvailable();

private slots:
    void Timer_MW();
    void on_checkBox_Summ_clicked(bool checked);
};
#endif // MAINWINDOW_H
