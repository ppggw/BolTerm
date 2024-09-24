#ifndef FRAMEUPDATER_H
#define FRAMEUPDATER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMutex>

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


class FrameUpdater : public QObject
{
    Q_OBJECT

private:
    cv::Mat *my_frame;
    QTimer *pTimer_MW;
    cv::VideoCapture source;
    cv::String pipeline;
    QMutex* m;

public:
    explicit FrameUpdater(QMutex*, cv::Mat* frame, QObject *parent = nullptr);

    bool SourceIsAvailable;
    bool AccumImages = false;
    int NumImForSumm = 0;

public slots:
    void Timer_MW_timeout(void);
    void OpenSource(void);

signals:
    void onSourceisAvailable();
    void EnableReadSummImage();

};

#endif // FRAMEUPDATER_H
