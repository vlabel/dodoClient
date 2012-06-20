#ifndef NETWORK_H
#define NETWORK_H

#include <QThread>
#include <QTcpSocket>

struct Pack_t {
    QString imei;
    QString name; // user name
    int gsm; // strenght GSM
    int cdma; // strenght CDMA
    int wcdma; // strenght WCDMA
    double altitude;
    double longitude;
    qreal azimuth;
    qreal xMag; // Magnetic field X
    qreal yMag; // Magnetic field Y
    qreal zMag; // Magnetic field Z
    qreal xAc; // Accelerometer X
    qreal yAc; // Accelerometer Y
    qreal zAc; // Accelerometer Z
    QString toJson(void) const;
};

class Network : public QThread
{
    Q_OBJECT
public:
    void run();

public slots:
    void send(const Pack_t&);
};
#endif // NETWORK_H
