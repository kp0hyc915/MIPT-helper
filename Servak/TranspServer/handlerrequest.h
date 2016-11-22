#ifndef HANDLERREQUEST_H
#define HANDLERREQUEST_H
#include <iostream>
#include <QObject>
#include <QString>
#include <QByteArray>

class HandlerRequest: public QObject
{
    Q_OBJECT
public:
    HandlerRequest();
    QString getLabsBySemestr(int n);
    void handle (QString);

signals:
    void dataReadyToSends(QByteArray);
};



#endif // HANDLERREQUEST_H

