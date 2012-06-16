#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGeoPositionInfoSource *geo = QGeoPositionInfoSource::createDefaultSource(this);
    if (geo) {
        connect(geo, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
        geo->startUpdates();
    }
    connect(&c, SIGNAL(readingChanged()), this, SLOT(compass()));
    connect(&a, SIGNAL(readingChanged()), this, SLOT(accelerometer()));
    connect(&m, SIGNAL(readingChanged()), this, SLOT(magnetometer()));
    connect(&n, SIGNAL(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode,int)),
            this, SLOT(strength(QSystemNetworkInfo::NetworkMode,int)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(send()));
    qRegisterMetaType<Pack_t>("Pack_t");
    connect(this, SIGNAL(send(const Pack_t&)), &net, SLOT(send(const Pack_t&)));
//    ui->mcc->setText(n.homeMobileCountryCode());
//    ui->mnc->setText(n.homeMobileNetworkCode());

    packet.imei = d.imei();
    packet.gsm = n.networkSignalStrength(QSystemNetworkInfo::GsmMode);
    packet.cdma = n.networkSignalStrength(QSystemNetworkInfo::CdmaMode);
    packet.wcdma = n.networkSignalStrength(QSystemNetworkInfo::WcdmaMode);
    ui->gsm->setNum(packet.gsm);
    ui->cdma->setNum(packet.cdma);
    ui->wcdma->setNum(packet.wcdma);
    net.start();
    c.start();
    m.start();
    a.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::compass(void)
{
    packet.azimuth = c.reading()->azimuth();
    ui->compass->setNum(packet.azimuth);
}

void MainWindow::accelerometer(void)
{
    packet.xAc = a.reading()->x();
    packet.yAc = a.reading()->y();
    packet.zAc = a.reading()->z();
    ui->xAc->setNum(packet.xAc);
    ui->yAc->setNum(packet.yAc);
    ui->zAc->setNum(packet.zAc);
}

void MainWindow::magnetometer(void)
{
    packet.xMag = m.reading()->x();
    packet.yMag = m.reading()->y();
    packet.zMag = m.reading()->z();
    ui->xMag->setNum(packet.xMag);
    ui->yMag->setNum(packet.yMag);
    ui->zMag->setNum(packet.zMag);
}

void MainWindow::strength(QSystemNetworkInfo::NetworkMode mode, int level)
{
    level = 4;
    switch (mode) {
    case QtMobility::QSystemNetworkInfo::GsmMode: {
        packet.gsm = level;
        ui->gsm->setNum(packet.gsm);
    } break;
    case QtMobility::QSystemNetworkInfo::CdmaMode: {
        packet.cdma = level;
        ui->cdma->setNum(packet.cdma);
    } break;
    case QtMobility::QSystemNetworkInfo::WcdmaMode: {
        packet.wcdma = level;
        ui->gsm->setNum(packet.wcdma);
    } break;
    default: {}
    }
}

void MainWindow::on_bClose_clicked()
{
    close();
}

void MainWindow::send()
{
    emit send(packet);
}

void MainWindow::positionUpdated(QGeoPositionInfo info)
{
    packet.altitude = info.coordinate().altitude();
    packet.longitude = info.coordinate().longitude();
    ui->altitude->setNum(packet.altitude);
    ui->longitude->setNum(packet.longitude);
}


void MainWindow::on_pushButton_clicked()
{
    packet.name = ui->login->text();
    timer.start(500);
}
