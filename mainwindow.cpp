#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace{
    bool SourceOpen = true;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);

    setWindowTitle("BolomTerminal");
    setWindowState(Qt::WindowMaximized);

    UDP_Command_Term = new UdpClient();
    UDP_CommandThread = new QThread();
    UDP_Command_Term->moveToThread(UDP_CommandThread);
    UDP_CommandThread->start();

    My_FrameUpdater = new FrameUpdater(&frame);
    My_FrameUpdaterThread = new QThread();
    My_FrameUpdater->moveToThread(My_FrameUpdaterThread);
    My_FrameUpdaterThread->start();

    ptimer_MW = new QTimer();
    ptimer_MW->setInterval(40);
    ptimer_MW->start();

//    connect(this, SIGNAL(onSendUDP_PacketToAirUnit(QByteArray)), UDP_Command_Term, SLOT(SendUDP_Packet(QByteArray)));
//    connect(UDP_Command_Term, SIGNAL(send_buffer(QByteArray)), this, SLOT(UDPReady(QByteArray)));
    connect(My_FrameUpdater, SIGNAL(onSourceisAvailable()), this, SLOT(SourceIsAvailable()));
    connect(ptimer_MW, SIGNAL(timeout()), this, SLOT(Timer_MW()));
    connect(ui->horizontalSlider_Threshold, SIGNAL(valueChanged(int)), ui->label_Threshold, SLOT(setNum(int)));
    connect(this, SIGNAL(OpenSource()), My_FrameUpdater, SLOT(OpenSource()));

    emit OpenSource();
}


//void MainWindow::UDPReady(QByteArray){

//}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_checkBox_SwitchCenter_clicked(bool checked)
//{
//    QByteArray ba;
//    ba.resize(4);
//    ba[0] = 0xcc;
//    ba[1] = 0xcc;
//    checked == true ? ba[2] = 0x01 : ba[2] = 0x00;
//    emit onSendUDP_PacketToAirUnit(ba);
//}


void MainWindow::Timer_MW(){
    SourceIsAvailableCounter++;

    static cv::Mat fon(ui->graphicsView->height(), ui->graphicsView->width(), CV_8UC3, cv::Scalar(255, 255, 255));
    static int counter = 0; //если прошло 30 тиков то цвет меняется
    static cv::Scalar color = cv::Scalar(255,0,0);

    if (SourceIsAvailableCounter > 30)
    {
        SourceOpen = false;

        if(counter % 30 == 0){
            color == cv::Scalar(255,0,0) ? color = cv::Scalar(0,0,255) : color = cv::Scalar(255,0,0);
            double scale = 0.1;
            double fontScale = cv::min(ui->graphicsView->width(),ui->graphicsView->height())/(25/scale);
            cv::String text = "no connection";
            cv::Size textsize = cv::getTextSize(text, cv::FONT_HERSHEY_DUPLEX, fontScale, 2, 0);
            cv::putText(fon,
                        text,
                        cv::Point(ui->graphicsView->width() / 2 - (textsize.width / 2), ui->graphicsView->height() / 2 + (textsize.height / 2)),
                        cv::FONT_HERSHEY_DUPLEX,
                        fontScale,
                        color,
                        2);
        }
        frame = fon;
        counter++;
    }

    if(!frame.empty())
    {
        if(SourceOpen){
            cv::Mat gray_image;
            cv::cvtColor(frame, gray_image, cv::COLOR_BGR2GRAY);

            if(ui->checkBox_Cut->isChecked()){
                cv::threshold(frame, frame, ui->horizontalSlider_Threshold->value(), 255, cv::THRESH_BINARY);

                if(ui->checkBox_GetCenter->isChecked()){
                    using namespace cv;
                    Mat canny_output;
                    std::vector<std::vector<Point> > contours;
                    std::vector<Vec4i> hierarchy;

                    // detect edges using canny
                    // вообще делает почти то же самое, что и трешолд, но сложнее и связнее. Поэтому протестить
//                    Canny( gray_image, canny_output, 50, 150, 3 );

                    // find contours
                    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

                    // get the moments
                    std::vector<Moments> mu(contours.size());
                    for( int i = 0; i<contours.size(); i++ )
                    { mu[i] = moments( contours[i], false ); }

                    // get the centroid of figures.
                    std::vector<Point> mc(contours.size());
                    for( int i = 0; i<contours.size(); i++)
                    { mc[i] = Point( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); } // вместо флоат взял просто поинт, проверить
                    // правильно ли усекает

                    for(auto& p : mc){
                        cv::circle(frame, p, 3, cv::Scalar(0,0,255));
                        static int size_of_line = 10;
                        cv::line(frame, {p.x - size_of_line/2, p.y}, {p.x + size_of_line/2, p.y}, cv::Scalar(0,0,255), 2);
                        cv::line(frame, {p.x - size_of_line/2, p.y}, {p.x + size_of_line/2, p.y}, cv::Scalar(0,0,255), 2);
                        cv::line(frame, {p.x, p.y - size_of_line/2}, {p.x, p.y + size_of_line/2}, cv::Scalar(0,0,255), 2);
                        }
                    }
                }
            }
        QImage qimg(frame.data,
                    frame.cols,
                    frame.rows,
                    frame.step,
                    QImage::Format_RGB888);
        pixmap.setPixmap( QPixmap::fromImage(qimg) );
        ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
    }
    qApp->processEvents();
}


void MainWindow::SourceIsAvailable()
{
    if(!SourceOpen) SourceOpen = true;
    SourceIsAvailableCounter = 0;
}

