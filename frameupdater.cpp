#include "frameupdater.h"

FrameUpdater::FrameUpdater(QMutex* _m, cv::Mat* frame, QObject *parent) : QObject(parent), m(_m)
{
    pTimer_MW = new QTimer();
    pTimer_MW->setInterval(20);
    pTimer_MW->start();
    my_frame = frame;

    pipeline = "udpsrc port=31990 ! application/x-rtp, media=video, clock-rate=90000, encoding-name=H265, "
               "payload=96 ! rtph265depay ! decodebin ! videoconvert ! appsink";

    connect(pTimer_MW, SIGNAL(timeout()), this, SLOT(Timer_MW_timeout()));
}

void FrameUpdater::Timer_MW_timeout()
{
    if(source.isOpened())
    {
        emit onSourceisAvailable();

        if(!AccumImages){
            QMutexLocker locker(m);

            source.read(*my_frame);
            cv::cvtColor(*my_frame, *my_frame, cv::COLOR_BGR2RGB);
        }
        else{
            static int counter = 0;
            static cv::Mat result;
            cv::Mat temp;
            if(counter <= NumImForSumm){
                if(counter == 0){
                    result = cv::Mat::zeros(512, 640, CV_8UC3);
                }
                source.read(temp);
                temp /= NumImForSumm;
                result += temp;
                counter++;
            }
            else{
                *my_frame = result;
                emit EnableReadSummImage();
                counter = 0;
            }
        }
    }
}

void FrameUpdater::OpenSource()
{
    source.open(pipeline, cv::CAP_GSTREAMER);
}
