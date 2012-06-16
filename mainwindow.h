#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QCompass>
#include <QAccelerometer>
#include <QMagnetometer>
#include <QSystemNetworkInfo>
#include <QSystemDeviceInfo>
#include <QTimer>
#include <QGeoPositionInfoSource>
#include "network.h"

using namespace QtMobility;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

private:
    Ui::MainWindow *ui;
    QCompass c;
    QAccelerometer a;
    QMagnetometer m;
    QSystemNetworkInfo n;
    QSystemDeviceInfo d;
    QGeoPositionInfo *geo;
    QTimer timer;

    Pack_t packet;
    Network net;

signals:
    void send(const Pack_t&);

private slots:
    void compass(void);
    void accelerometer(void);
    void magnetometer(void);
    void strength(QSystemNetworkInfo::NetworkMode,int);
    void on_bClose_clicked();
    void send(void);
    void positionUpdated(QGeoPositionInfo);
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
