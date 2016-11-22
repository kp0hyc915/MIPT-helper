#include "handlerrequest.h"

HandlerRequest::HandlerRequest(){


}

QString HandlerRequest::getLabsBySemestr(int n){
    return QString("111/222/333");
}

void HandlerRequest::handle (QString inputstring){

    switch (inputstring[0].toLatin1()){
        case '1':{
                QString semnum;
                inputstring.remove(0,2);
                QByteArray a = getLabsBySemestr(1).toLatin1();
                emit dataReadyToSends(a);
        }
    }

    std::cout<<inputstring.toStdString()<<"\n";
}
