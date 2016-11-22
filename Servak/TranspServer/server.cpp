#include <QDir>
#include <QFile>

#include "server.h"


Server::Server(int nPort)
{

    m_pTcpServer = new QTcpServer(this);
    m_pClientSocket1 = new QTcpSocket;
    m_pClientSocket2 = new QTcpSocket;
    if(!m_pTcpServer->listen(QHostAddress::Any, nPort)){
        m_pTcpServer->close();
        return;
    }

    initData();
    connect(m_pTcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

Server::~Server(){
}

void Server::slotNewConnection()
{
    std::cout<<"new connection \n";
    if(m_pClientSocket1->state() == QTcpSocket::UnconnectedState){
        m_pClientSocket1 = m_pTcpServer->nextPendingConnection();
        connect(m_pClientSocket1, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    }
    else{
        m_pClientSocket2 = m_pTcpServer->nextPendingConnection();
        connect(m_pClientSocket2, SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void Server::slotReadClient()
{
    QTcpSocket *m_pClientSocket = (QTcpSocket*)sender();
    input  = m_pClientSocket->readAll();
    std::cout<<"!"<<input.data()<<"!\n";
    newDatarecived();
    return;
}

void Server::SendToClient(QByteArray a){
    QTcpSocket *m_pClientSocket = (QTcpSocket*)sender();
    m_pClientSocket->write(a);
    std::cout<<a.data()<<" ";
    std::cout<<a.size()<<" write end\n";
    return;
}

void Server::newDatarecived(){

    QString get = QString(input);
    QString num;

    num += get[0].toLatin1();
    bool check = true;
    int i = -1;
    i = num.toInt(&check);
    get.remove(0,2);
    QString data = get;
    switch (i){
        case 1:{
            int nSem = data.toInt(&check);
            getLabList(nSem);
            break;
        }
        case 2:{
            getDataLaba(data);
            break;
        }
    }
}

void Server::getDataLaba(QString LName){
    for (int i = 0; i < 5; ++i){
        for(int j = 0; j < SDATA[i].labas.size(); ++j){
            if (SDATA[i].labas[j].LabaName.compare(LName) == 0){

               QFile img(SDATA[i].labas[j].PDFDATA);
               img.open(QIODevice::ReadWrite);
               if (!img.isOpen()){
                   std::cout<<"kek1";
                   return;
               }
               QByteArray bimg;
               bimg += img.size();
               bimg += "/";
               bimg += img.readAll();
               bimg += "/";
               QFile question(SDATA[i].labas[j].QuestionData);
               question.open(QIODevice::ReadWrite);
               if (!question.isOpen()){
                   std::cout<<"kek2";
                   return;
               }
               bimg +=question.readAll();
               QFile book(SDATA[i].labas[j].BookData);
               book.open(QIODevice::ReadWrite);
               if (!book.isOpen()){
                   std::cout<<"kek3";
                   return;
               }
               bimg +=book.readAll();
               QByteArray out;
               out.setNum(bimg.size());
               out +="/";
               out += bimg;
               SendToClient(out);
               return;
            }
        }
    }
    std::cout<<LName.toStdString()<<" ne azaza ";
}


void Server::initData(){
    for(int i = 0; i < 5; ++i)
        SDATA[i].nLaba = 0;
    QFile alldata("semestr.txt");

    alldata.open(QIODevice::ReadOnly);
    if (alldata.isOpen()){
        QByteArray semestrdata;
        semestrdata = alldata.readAll();
        alldata.close();
        QString alldatastring = QString(semestrdata);
        QStringList strlist = alldatastring.split("\n");
        QStringList lineData;
        QString line;
        bool checker;
        int nsem;
        laba newlaba;
        for(int i = 0; i < strlist.size(); i++){
            line = strlist[i];
            lineData = line.split('|');
            QString record = lineData[0];
            nsem = record.toInt(&checker);
            newlaba.LabaName = lineData[1];
            newlaba.PDFDATA = lineData[2];
            newlaba.QuestionData = lineData[3];
            newlaba.BookData = lineData[4];
            std::cout<<newlaba.LabaName.toStdString()<<" "<<newlaba.PDFDATA.toStdString()<<" "<<newlaba.QuestionData.toStdString()<<" "<<newlaba.BookData.toStdString()<<"\n";
            SDATA[nsem-1].labas.append(newlaba);
            SDATA[nsem-1].nLaba++;
        }

    }else{
        return;
    }
}

void Server::getLabList(int n){
    QString labanames;
    for(int i = 0; i < SDATA[n-1].labas.size(); ++i){
        labanames += SDATA[n-1].labas[i].LabaName;
        if(i != SDATA[n-1].labas.size()-1)
            labanames += '/';
    }
    SendToClient(labanames.toLatin1());
}
