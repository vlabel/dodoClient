#ifndef NETWORK_H
#define NETWORK_H

#include <QThread>
#include <QTcpSocket>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>

struct Pack_t {
    QString imei; //+
    QString name; //+
    int gsm; //+
    int cdma; //+
    int wcdma; //+
    double altitude; //+
    double longitude; //+
    qreal azimuth;
    qreal xMag;
    qreal yMag;
    qreal zMag;
    qreal xAc;
    qreal yAc;
    qreal zAc;
};
Q_DECLARE_METATYPE(Pack_t)

class Network : public QThread
{
    Q_OBJECT
public:
    void run();

private:
    static QScriptValue packageToScriptValue(QScriptEngine *engine, const Pack_t&);
    static void packageFromScriptValue(const QScriptValue &obj, Pack_t&);
    QString PackToQString(const Pack_t &);
signals:
    
public slots:
    void send(const Pack_t&);
};
#endif // NETWORK_H
