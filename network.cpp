#include "network.h"
#include <QDateTime>

void Network::run()
{
    exec();
}

void Network::send(const Pack_t &packet)
{
    QScriptEngine engine;
    qScriptRegisterMetaType(&engine, Network::packageToScriptValue, Network::packageFromScriptValue);
    //QScriptValue dt = qScriptValueFromValue<Pack_t>(&engine,packet);
    //QScriptValue dt = engine.toScriptValue<Pack_t>(packet);
    QString data = PackToQString(packet);
    //QString data = dt.toString();
    qDebug(data.toUtf8().data());
    QTcpSocket soc;
    soc.connectToHost("192.168.0.66",3456, QTcpSocket::WriteOnly);
    soc.waitForConnected();
    if (soc.state() == QTcpSocket::ConnectedState) {
        soc.write(data.toUtf8().data());
        soc.waitForBytesWritten();
    }
    //soc.disconnectFromHost();
}

QString Network::PackToQString(const Pack_t &P)
{
    QString gsm = QString::number(P.gsm);
    QString cdma = QString::number(P.cdma);
    QString wcdma = QString::number(P.wcdma);
    QString alt = QString::number(P.altitude);
    QString lon = QString::number(P.longitude);
    QString res = QString("[\"")+
            P.imei+
            QString("\",\"") +
            P.name +
           QString("\",") +
             gsm   +
             QString(",") +
            QString::number(QDateTime::currentDateTime().toTime_t()) +
            QString(",") +
    cdma +
             QString(",") +
    wcdma +
             QString(",\"") +
    alt +
              QString("\",\"") +
    lon +
            QString("\",\"") +
    QString::number(P.azimuth) +
            QString("\",\"") +
    QString::number(P.xMag) +
            QString("\",\"") +
    QString::number(P.yMag) +
            QString("\",\"") +
    QString::number(P.zMag) +
            QString("\",\"") +
    QString::number(P.xAc) +
            QString("\",\"") +
    QString::number(P.yAc) +
            QString("\",\"") +
    QString::number(P.zAc) +
            QString("\"]");
    return res;
}

QScriptValue Network::packageToScriptValue(QScriptEngine *engine, const Pack_t &packet)
{
    QScriptValue obj = engine->newObject();
    obj.setProperty("0", packet.name);
    obj.setProperty("1", packet.imei);
    obj.setProperty(2, packet.gsm);
    obj.setProperty(3, QDateTime::currentDateTime().toTime_t());
    obj.setProperty(4, packet.cdma);
    obj.setProperty(5, packet.wcdma);
    obj.setProperty(6, packet.altitude);
    obj.setProperty(7, packet.longitude);
    return obj;
}

void Network::packageFromScriptValue(const QScriptValue &obj, Pack_t &packet)
{
    packet.name = obj.property(0).toString();
    packet.imei = obj.property(1).toString();
    packet.gsm = obj.property(2).toString().toInt();
    packet.cdma = obj.property(4).toString().toInt();
    packet.wcdma = obj.property(5).toString().toInt();
    packet.altitude = obj.property(6).toString().toDouble();
    packet.longitude = obj.property(7).toString().toDouble();
}
