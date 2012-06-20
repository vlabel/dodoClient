#include "network.h"
#include <QDateTime>

void Network::run()
{
    exec();
}

void Network::send(const Pack_t &packet)
{
    QTcpSocket soc;
    soc.connectToHost("192.168.0.66",3456, QTcpSocket::WriteOnly);
    soc.waitForConnected();
    if (soc.state() == QTcpSocket::ConnectedState) {
        soc.write(packet.toJson().toUtf8().data());
        soc.waitForBytesWritten();
    }
}

QString Pack_t::toJson(void) const
{
    QString json("[\"%0\", \"%1\", %2, %3,
                 "%4, %5, %6, %7, \"%8\","
                 "\"%9\", \"%10\", \"%11\","
                 "\"%12\", \"%13\", \"%14\"]");

    return json.arg(imei).arg(name).arg(gsm).arg(QDateTime::currentDateTime().toTime_t()())
            .arg(cdma).arg(wcdma).arg(altitude).arg(longitude).arg(azimuth)
            .arg(xMag).arg(yMag).arg(zMag)
            .arg(xAc).arg(yAc).arg(zAc);
}
