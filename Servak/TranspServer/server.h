#include <QTcpSocket>
#include <QTcpServer>
#include <QString>
#include <QByteArray>
#include <iostream>
#include <QFile>
#include <QStringList>

struct laba{
    QString LabaName;
    QString PDFDATA;
    QString QuestionData;
    QString BookData;
};

struct semestr{
    int nLaba;
    QVector <laba> labas;
};


class Server : public QObject
{
    Q_OBJECT

    int len;
    QByteArray input;
    QByteArray output;
    QTcpServer* m_pTcpServer;
    QTcpSocket* m_pClientSocket1, *m_pClientSocket2;

    semestr SDATA[5];
   // HandlerRequest handle;


public:
    /*object* get_data();*/
    Server(int nPort);
    ~Server ();
    void newDatarecived();
    void getLabList(int n);
    void getDataLaba(QString LName);
    void initData();

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void SendToClient(QByteArray a);

};


