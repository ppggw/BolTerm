#include "frameupdater.h"

FrameUpdater::FrameUpdater(cv::Mat* frame, QObject *parent) : QObject(parent)
{
    pTimer_MW = new QTimer();
    pTimer_MW->setInterval(5);
    pTimer_MW->start();
    my_frame = frame;

    pipeline = "udpsrc port=31990 ! application/x-rtp, media=video, clock-rate=90000, encoding-name=H265, "
               "payload=96 ! rtph265depay ! decodebin ! videoconvert ! appsink";

    connect(pTimer_MW, SIGNAL(timeout()), this, SLOT(Timer_MW_timeout()));
}

void FrameUpdater::Timer_MW_timeout()
{
//        temp_frame = imread("D:/Work/4.jpg", cv::IMREAD_COLOR);
    if(source.isOpened())
    {
        qDebug() << "F";
        source.read(temp_frame);
        m_mutex.lock();
        *my_frame = temp_frame;
        cv::cvtColor(*my_frame, *my_frame, cv::COLOR_BGR2RGB);
        cv::resize(*my_frame, *my_frame, cv::Size(1920, 1080));
        m_mutex.unlock();
        emit onSourceisAvailable();
    }
}

void FrameUpdater::OpenSource()
{
    source.open(pipeline, cv::CAP_GSTREAMER);
}
