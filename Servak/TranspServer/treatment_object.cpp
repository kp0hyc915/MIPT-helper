#include <treatment_object.h>
#include <QFile>
#include <QDir>
#include <fstream>
#include <QRegExp>
#include <iostream>
#include <QStringList>
#include <stdlib.h>
#include <cmath>
#include <string.h>

#define M_PI 3.14159265358979323846

Treat_objects::Treat_objects(){
    pobj= NULL;
    pWAYPOINT = NULL;
    len = 0;
    lenWP = 0;
    current = 0;
    currentWP =0;
    need_time = 0;
    curr_time = time(NULL);
    timer = new QTimer(this);
    timer->setInterval(20);
    timer->start();
    uselesstime.start();
    time_curr = &uselesstime;
    //start_time = uselesstime.currentTime();
    QDir a;
    pathtofile = a.currentPath();
    delete_data();
    connect(timer,SIGNAL(timeout()),this,SLOT(processingPosObject()));
}

void Treat_objects::delete_data(){
    QString currpath = pathtofile;
    currpath.append("/data/");
    QDir a(pathtofile);
    a.mkdir("data");
    a.cd("data");
    QStringList alldata = a.entryList();
    for (int i = 0; i < alldata.size(); ++i){
        a.remove(alldata[i]);
    }
    a.cdUp();
    return;
}

void Treat_objects::add_object(object newObject){
    qDebug()<<newObject.id;
    if (strcmp(newObject.id,"") ==0){
        if (strcmp(newObject.type,"pause") == 0){
            timer->stop();
            return;
        }
        if (strcmp(newObject.type,"play") == 0){
            timer->start();
            return;
        }
        if (strcmp(newObject.type,"playspeed") == 0){
            timer->setInterval(20/pow(2,newObject.speed));
            return;
        }
        if (strcmp(newObject.type,"setTime") == 0){
            set_time(newObject);
            return;
        }
    }
    for (int i = 0; i< current; ++i){
        if (strcmp(pobj[i].id, newObject.id) == 0){
            set_newobjectdata(newObject,i);
            return;
        }
    }
    newObject.timelinecheck = time_curr->elapsed()/1000;
    if (strcmp(newObject.type,"WP") == 0){
        add_WP(newObject);
        return;
    }
    if (strcmp(newObject.type,"RWP") == 0){
        for (int i = 0; i< currentWP; ++i){
            if (strcmp(newObject.id,pWAYPOINT[i].id) == 0){
                strcpy(pWAYPOINT[i].RID,newObject.RID);
                pWAYPOINT[i].speed = newObject.speed;
                pWAYPOINT[i].follow = newObject.follow;
                pWAYPOINT[i].y = newObject.y;
                pWAYPOINT[i].duration = newObject.duration;
                save_data();
                return;
            }
        }
        return;
    }
    add_OBJ(newObject);
    return;
}

void Treat_objects::set_newobjectdata(object newObject, int i){
    object newWP;// = {"",0,0,0,0,0,0,0,"","","","",0,0,0,0,0,0,0,0,0,0,0};
    char rid [16];
    int idlen;
    strcpy(rid,"");
    for (int k = 0; k < currentWP; ++k){
        if (strcmp(pobj[i].RID,pWAYPOINT[k].RID) == 0){
            if (strcmp(rid,pWAYPOINT[k].id) < 0){
                strcpy(rid,pWAYPOINT[k].id);
                idlen = strlen(pWAYPOINT[k].id);
            }
        }
    }
    if (strcmp(rid,"") == 0){
        strcpy(pobj[i].RID,"R");
        strcat(pobj[i].RID,pobj[i].id);
        strcpy(rid,pobj[i].RID);
        strcat(rid,"1");
        strcpy(newWP.id,rid);
    }
    else{
        char *pnum = rid + idlen-1;
        int num = atof(pnum);
        num++;
        strncpy(newWP.id,rid,idlen-1);
        strcat(newWP.id,QString::number(num).toLatin1().constData());
    }
    newWP.speed = pobj[i].speed;
    newWP.latitude = pobj[i].latitude;
    newWP.longitude = pobj[i].longitude;
    newWP.y = pobj[i].y;
    strcpy(newWP.RID,pobj[i].RID);
    newWP.fromScenario = false;
    add_WP(newWP);
    pobj[i].y = newObject.y;
    pobj[i].speed = newObject.speed;
    save_data();
    return;
}

void Treat_objects::set_time(object newObject){
    need_time = newObject.timelinecheck;
    QString need_file;
    curr_time = uselesstime.elapsed()/1000;
    if (need_time > 0){
        need_time += uselesstime.elapsed()/1000;
        uselesstime = uselesstime.addSecs(86400-newObject.timelinecheck);
    }
    else{
        if ((uselesstime.elapsed()/1000 + need_time) < 0){
            uselesstime.restart();
            need_time = 0;
        }
        else{
            need_time += uselesstime.elapsed()/1000;
            uselesstime = uselesstime.addSecs(86400 - newObject.timelinecheck);
        }
    }
    curr_time = uselesstime.elapsed()/1000 - curr_time;
    if (newObject.timelinecheck<0){
        need_file = find_out(need_time);
        get_data_from_file(need_file);
        calc_data(true);
        return;
    }
    calc_data(false);
    return;
}

void Treat_objects::add_OBJ(object newObject){
    object *newArr = pobj;
    if (newObject.timetobirthday == -1){
        newObject.timetobirthday = time_curr->elapsed()/1000;
    }
    for (int i = 0; i <len; ++i){
        newArr[i] = pobj[i];
    }
    int uselesslen = current;
    if (current == len){
        if (len == 0)
            len = 1000;
        len *= 2;
        pobj = new object [len];
        for (int i = 0; i < uselesslen; ++i){
            pobj[i] = newArr[i];
        }
        pobj[uselesslen] = newObject;
        if (newArr != NULL)
            delete [] newArr;
        current++;
        save_data();
        return;
    }
    pobj = new object [len];
    for (int i = 0; i < uselesslen; ++i){
        pobj[i] = newArr[i];
    }
    pobj[uselesslen] = newObject;
    if (newArr != NULL)
        delete [] newArr;
    current++;
    save_data();
    return;
}

void Treat_objects::add_WP(object newObject){
    object *newArrWP = pWAYPOINT;
    for (int i = 0; i <lenWP; ++i){
        newArrWP[i] = pWAYPOINT[i];
    }
    int uselessWP = currentWP;
    if (currentWP == lenWP){
        if (lenWP == 0)
            lenWP = 1000;
        lenWP *= 2;
        pWAYPOINT = new object [lenWP];
        for (int i = 0; i < uselessWP; ++i){
            pWAYPOINT[i] = newArrWP[i];
        }
        pWAYPOINT[uselessWP] = newObject;
        if (newArrWP != NULL)
            delete [] newArrWP;
        currentWP++;
        save_data();
        return;
    }
    pWAYPOINT = new object [lenWP];
    for (int i = 0; i < uselessWP; ++i){
        pWAYPOINT[i] = newArrWP[i];
    }
    pWAYPOINT[uselessWP] = newObject;
    if (newArrWP != NULL)
        delete [] newArrWP;
    currentWP++;
    save_data();
    return;
}

int* Treat_objects::retCurr(){
    return &current;
}

object* Treat_objects::retBArr(){
    return pobj;
}

void Treat_objects::Death(object &a, object &b){
    strcpy(a.type,"empty");
    strcpy(a.WPID,"");
    strcpy(a.RID,"");
    a.speed=0;
    a.direction_long=0;
    a.direction_lat=0;
    a.fof=0;
    strcpy(b.type,"fight");
    strcpy(b.WPID,"");
    strcpy(b.RID,"");
    b.speed=0;
    b.direction_long=0;
    b.direction_lat=0;
    b.fof=0;
    return;
}

void Treat_objects::processingObject(){
    for (int i = 0; i < current; ++i){
        for (int j = i; j < current; ++j){
            if ((pobj[i].fof*pobj[j].fof) == -1){
                if(strcmp(pobj[i].id,"BB1") == 0){
                    if (strcmp(pobj[j].id,"A2") == 0){
                        qDebug()<<fabs ( (pobj[i].latitude - pobj[j].latitude) * 111134.86)<<(pobj[i].longitude - pobj[j].longitude) * cos((pobj[i].latitude * M_PI / 180))*400000/3.5;
                    }
                if(strcmp(pobj[j].id,"BB1") == 0){
                    if (strcmp(pobj[i],"A2") == 0){
                        qDebug()<<fabs ( (pobj[i].latitude - pobj[j].latitude) * 111134.86)<<(pobj[i].longitude - pobj[j].longitude) * cos((pobj[i].latitude * M_PI / 180))*400000/3.5;
                    }
                 if (fabs ( (pobj[i].latitude - pobj[j].latitude) * 111134.86) < 150 ){
                     if ( fabs ( (pobj[i].longitude - pobj[j].longitude) * cos((pobj[i].latitude * M_PI / 180))*400000/3.5) < 150) {
                         if (strcmp(pobj[j].type, "Rocket")==0) {
                             if ((strcmp(pobj[i].type, "Helicopter")==0)||(strcmp(pobj[i].type, "LowFlightTarget")==0)||
                                     (strcmp(pobj[i].type,"Plane")==0) || (strcmp(pobj[i].type, "Undefined")==0)||
                                     (strcmp(pobj[i].type, "Ship")==0)){
                                 Death(pobj[j],pobj[i]);
                             }
                         }
                         if (strcmp(pobj[i].type, "Rocket")==0) {
                             if ((strcmp(pobj[j].type, "Helicopter")==0)||(strcmp(pobj[j].type, "LowFlightTarget")==0)||
                                     (strcmp(pobj[j].type,"Plane")==0) || (strcmp(pobj[j].type, "Undefined")==0)||
                                     (strcmp(pobj[j].type, "Ship")==0)){
                                 Death(pobj[i],pobj[j]);
                             }
                         }

                         if (strcmp(pobj[i].type, "Torpedo")==0){
                             if (strcmp(pobj[j].type, "Ship")==0){
                                 Death(pobj[i],pobj[j]);
                             }
                         }
                         if (strcmp(pobj[j].type, "Torpedo")==0){
                             if (strcmp(pobj[i].type, "Ship")==0){
                                 Death(pobj[j],pobj[i]);
                             }
                         }
                     }
                 }
            }
        }
    }
}

void Treat_objects::processingPosObject(){
    curr_time = time_curr->elapsed()/(1000);
    for (int i = 0; i < current; ++i){
        pobj[i].timelinecheck = curr_time;
        pobj[i].isdraw = true;


        if (strcmp(pobj[i].WPID,"---") == 0){  //просчет координат объекта который добавлен пользователем
            if(fabs(pobj[i].longitude) > 180){
                if (pobj[i].longitude > 180){
                    pobj[i].longitude -=360;
                }
                else{
                    pobj[i].longitude +=360;
                }
            }
            pobj[i].longitude += sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].latitude * M_PI / 180)))/50*pobj[i].speed;
            pobj[i].latitude += cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;

            continue;
        }

        if (pobj[i].TimeOfBirthday > 0){ // если объект должен появится через какое то время то уменьшаем счетчик времени до появления
            pobj[i].isdraw = false;
            pobj[i].TimeOfBirthday--;
            continue;
        }


        if (pobj[i].duration_check){ // если объект двигается в каком то направлении то просчет движения по направлению
            pobj[i].duration--;
            if (pobj[i].duration<=0){
                pobj[i].duration_check = false;
                char last[16];
                strcpy(last,pobj[i].WPID);
                for (int j = 0; j< currentWP; ++j){
                    if (strcmp(pobj[i].RID,pWAYPOINT[j].RID) == 0){
                        if (strcmp(pobj[i].WPID,pWAYPOINT[j].id) < 0){
                            pobj[i].direction_lat = pWAYPOINT[j].latitude;
                            pobj[i].direction_long = pWAYPOINT[j].longitude;
                            strcpy(pobj[i].WPID,pWAYPOINT[j].id);
                            pobj[i].speed = pWAYPOINT[j].speed;
                            pobj[i].y = pWAYPOINT[j].y;
                            pobj[i].dx = calc_dist(pobj[i].direction_lat,pobj[i].direction_long,pobj[i].latitude,pobj[i].longitude);
                            break;
                        }

                    }
                }
                if (strcmp(pobj[i].WPID,last) == 0){
                    pobj[i].direction_lat = pobj[i].latitude;
                    pobj[i].direction_long = pobj[i].longitude;
                    strcpy(pobj[i].WPID,"--");
                    if (strcmp(pobj[i].type, "Rocket") == 0){
                        strcpy(pobj[i].type, "empty");
                    }
                    pobj[i].speed = 0;
                    break;
                }
            }
            pobj[i].latitude += cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;
            pobj[i].longitude += sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].latitude * M_PI / 180)))/50*pobj[i].speed;
            if(fabs(pobj[i].longitude) > 180){
                if (pobj[i].longitude > 180){
                    pobj[i].longitude -=360;
                }
                else{
                    pobj[i].longitude +=360;
                }
            }
            continue;
        }


        if ((strcmp(pobj[i].GRID, "") > 0)&&(strcmp(pobj[i].GRID,"OK") != 0)){  //задание координат у группового объекта
            for (int k = 0; k < current; ++k){
                if (strcmp(pobj[k].id, pobj[i].GRID) == 0){
                    pobj[i].follow = false;
                    for (int l = 0; l < currentWP; ++l){
                        if (strcmp(pobj[i].RID,pWAYPOINT[l].RID) == 0){
                            if (pWAYPOINT[l].follow)
                                pobj[i].follow = true;
                            pobj[i].y = pWAYPOINT[l].y;
                        }
                    }
                    if (!pobj[i].follow)
                        strcpy(pobj[i].GRID,"OK");

                    if (pobj[i].follow)
                        pobj[i].y = pobj[k].y;
                    pobj[i].latitude = pobj[k].latitude + pobj[i].Offset_y /180.0*M_PI/111134.0;
                    pobj[i].longitude = pobj[k].longitude +
                            pobj[i].Offset_x/(400000/3.5*cos((pobj[k].latitude * M_PI / 180)));
                    pobj[i].isdraw = true;
                    break;
                }

            }
            continue;
        }

        if (strcmp(pobj[i].WPID,"") == 0){ //для объектов из сценария выбор направления маршрута
            for (int j = 0; j< currentWP; ++j){
                if (strcmp(pobj[i].RID,pWAYPOINT[j].RID) == 0){
                    strcpy(pobj[i].WPID,pWAYPOINT[j].id);
                    if (pWAYPOINT[j].duration>0){
                        pobj[i].speed = pWAYPOINT[j].speed;
                        pobj[i].duration = pWAYPOINT[j].duration;
                        if (pWAYPOINT[j].y == pWAYPOINT[j].y)
                            pobj[i].y = pWAYPOINT[j].y;
                        if (pobj[i].y != pobj[i].y)
                            pobj[i].y = 0;
                        pobj[i].duration_check = true;
                        pobj[i].latitude =pobj[i].latitude +  cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;
                        pobj[i].longitude =pobj[i].longitude+ sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].latitude * M_PI / 180)))/50*pobj[i].speed;
                        break;
                    }
                    if (pWAYPOINT[j].latitude == pWAYPOINT[j].latitude)
                        pobj[i].direction_lat = pWAYPOINT[j].latitude;
                    if (pWAYPOINT[j].longitude == pWAYPOINT[j].longitude)
                        pobj[i].direction_long = pWAYPOINT[j].longitude;
                    pobj[i].speed = pWAYPOINT[j].speed;
                    pobj[i].dx = calc_dist(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
                    pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
                    break;
                }
            }
        }


        if ((pobj[i].duration_check)||(pobj[i].follow))
            continue;

        pobj[i].latitude += cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;
        pobj[i].longitude += sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].latitude * M_PI / 180)))/50*pobj[i].speed;


        // дохождение объекта из сценария до обпределенной точки
        if (pobj[i].dx < calc_dist(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat)){
            char last[16];
            strcpy(last,pobj[i].WPID);
            for (int j = 0; j< currentWP; ++j){
                if (strcmp(pobj[i].RID,pWAYPOINT[j].RID) == 0){
                    if (strcmp(pobj[i].WPID,pWAYPOINT[j].id) < 0){
                        pobj[i].direction_lat = pWAYPOINT[j].latitude;
                        pobj[i].direction_long = pWAYPOINT[j].longitude;
                        strcpy(pobj[i].WPID,pWAYPOINT[j].id);
                        pobj[i].speed = pWAYPOINT[j].speed;
                        pobj[i].dx = calc_dist(pobj[i].direction_lat,pobj[i].direction_long,pobj[i].latitude,pobj[i].longitude);
                        break;
                    }

                }
            }
            if (strcmp(pobj[i].WPID,last) == 0){
                pobj[i].direction_lat = pobj[i].latitude;
                pobj[i].direction_long = pobj[i].longitude;
                strcpy(pobj[i].WPID,"--");
                pobj[i].speed = 0;
                break;
            }
            pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
        }
        else{
            pobj[i].dx = calc_dist(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
            pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
        }
        /*if(fabs(pobj[i].latitude) > 90){
                pobj[i].y +=180;
                pobj[i].longitude +=180;
        }*/
        if(fabs(pobj[i].longitude) > 180){
            if (pobj[i].longitude > 180){
                pobj[i].longitude -=360;
                pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
            }
            else{
                pobj[i].longitude +=360;
                pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
            }
        }
    }
    processingObject();
    emit valueChanged();
}

void Treat_objects::processingPosObject_withoutsenddata(int time){
    for (int i = 0; i < current; ++i){
        pobj[i].timelinecheck = time;
        pobj[i].isdraw = true;

        if (strcmp(pobj[i].WPID,"---") == 0){  //просчет координат объекта который добавлен пользователем
            pobj[i].longitude += sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].latitude * M_PI / 180)))/50*pobj[i].speed;
            pobj[i].latitude += cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;
            if(fabs(pobj[i].longitude) > 180){
                if (pobj[i].longitude > 180){
                    pobj[i].longitude -=360;
                }
                else{
                    pobj[i].longitude +=360;
                }
            }
            continue;
        }

        if (pobj[i].TimeOfBirthday > 0){ // если объект должен появится через какое то время то уменьшаем счетчик времени до появления
            pobj[i].isdraw = false;
            pobj[i].TimeOfBirthday--;
            continue;
        }


        if (pobj[i].duration_check){ // если объект двигается в каком то направлении то просчет движения по направлению
            pobj[i].duration--;
            if (pobj[i].duration<=0){
                pobj[i].duration_check = false;
                char last[16];
                strcpy(last,pobj[i].WPID);
                for (int j = 0; j< currentWP; ++j){
                    if (strcmp(pobj[i].RID,pWAYPOINT[j].RID) == 0){
                        if (strcmp(pobj[i].WPID,pWAYPOINT[j].id) < 0){
                            pobj[i].direction_lat = pWAYPOINT[j].latitude;
                            pobj[i].direction_long = pWAYPOINT[j].longitude;
                            strcpy(pobj[i].WPID,pWAYPOINT[j].id);
                            pobj[i].speed = pWAYPOINT[j].speed;
                            pobj[i].y = pWAYPOINT[j].y;
                            pobj[i].dx = calc_dist(pobj[i].direction_lat,pobj[i].direction_long,pobj[i].latitude,pobj[i].longitude);
                            break;
                        }

                    }
                }
                if (strcmp(pobj[i].WPID,last) == 0){
                    pobj[i].direction_lat = pobj[i].latitude;
                    pobj[i].direction_long = pobj[i].longitude;
                    strcpy(pobj[i].WPID,"--");
                    if (strcmp(pobj[i].type, "Rocket") == 0){
                        strcpy(pobj[i].type, "empty");
                    }
                    pobj[i].speed = 0;
                    break;
                }
            }
            pobj[i].latitude += cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;
            pobj[i].longitude += sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].longitude * M_PI / 180)))/50*pobj[i].speed;
            if(fabs(pobj[i].longitude) > 180){
                if (pobj[i].longitude > 180){
                    pobj[i].longitude -=360;
                }
                else{
                    pobj[i].longitude +=360;
                }
            }
            continue;
        }


        if ((strcmp(pobj[i].GRID, "") > 0)&&(strcmp(pobj[i].GRID,"OK") != 0)){  //задание координат у группового объекта
            for (int k = 0; k < current; ++k){
                if (strcmp(pobj[k].id, pobj[i].GRID) == 0){
                    for (int l = 0; l < currentWP; ++l){
                        if (strcmp(pobj[i].RID,pWAYPOINT[l].RID) == 0)
                            if (pWAYPOINT[l].follow)
                                pobj[i].follow = true;
                    }
                    if (!pobj[i].follow)
                        strcpy(pobj[i].GRID,"OK");
                    if (pobj[i].follow)
                        pobj[i].y = pobj[k].y;
                    pobj[i].latitude = pobj[k].latitude + pobj[i].Offset_y /180.0*M_PI/111134.0;
                    pobj[i].longitude = pobj[k].longitude +
                            pobj[i].Offset_x/(400000/3.5*cos((pobj[k].longitude * M_PI / 180)));
                    pobj[i].isdraw = true;
                    break;
                }

            }
            continue;
        }

        if (strcmp(pobj[i].WPID,"") == 0){ //для объектов из сценария выбор направления маршрута
            for (int j = 0; j< currentWP; ++j){
                if (strcmp(pobj[i].RID,pWAYPOINT[j].RID) == 0){
                    strcpy(pobj[i].WPID,pWAYPOINT[j].id);
                    if (pWAYPOINT[j].duration>0){
                        pobj[i].speed = pWAYPOINT[j].speed;
                        pobj[i].duration = pWAYPOINT[j].duration;
                        pobj[i].y = pWAYPOINT[j].y;
                        pobj[i].duration_check = true;
                        pobj[i].latitude += cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;
                        pobj[i].longitude += sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].latitude * M_PI / 180)))/50*pobj[i].speed;
                        break;
                    }
                    pobj[i].direction_lat = pWAYPOINT[j].latitude;
                    pobj[i].direction_long = pWAYPOINT[j].longitude;
                    pobj[i].speed = pWAYPOINT[j].speed;
                    pobj[i].dx = calc_dist(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
                    pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
                    break;
                }
            }
        }


        if ((pobj[i].duration_check)||(pobj[i].follow))
            continue;

        pobj[i].latitude += cos((pobj[i].y)/180.0*M_PI)/111134.0*pobj[i].speed/50.0;
        pobj[i].longitude += sin(((pobj[i].y))/180*M_PI)/(400000/3.5*cos((pobj[i].longitude * M_PI / 180)))/50*pobj[i].speed;


        // дохождение объекта из сценария до обпределенной точки
        if (pobj[i].dx < calc_dist(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat)){
            char last[16];
            strcpy(last,pobj[i].WPID);
            for (int j = 0; j< currentWP; ++j){
                if (strcmp(pobj[i].RID,pWAYPOINT[j].RID) == 0){
                    if (strcmp(pobj[i].WPID,pWAYPOINT[j].id) < 0){
                        pobj[i].direction_lat = pWAYPOINT[j].latitude;
                        pobj[i].direction_long = pWAYPOINT[j].longitude;
                        strcpy(pobj[i].WPID,pWAYPOINT[j].id);
                        pobj[i].speed = pWAYPOINT[j].speed;
                        pobj[i].dx = calc_dist(pobj[i].direction_lat,pobj[i].direction_long,pobj[i].latitude,pobj[i].longitude);
                        break;
                    }

                }
            }
            if (strcmp(pobj[i].WPID,last) == 0){
                pobj[i].direction_lat = pobj[i].latitude;
                pobj[i].direction_long = pobj[i].longitude;
                strcpy(pobj[i].WPID,"--");
                pobj[i].speed = 0;
                break;
            }
            pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
        }
        else{
            pobj[i].dx = calc_dist(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
            pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
        }
        /*if(fabs(pobj[i].latitude) > 90){
                pobj[i].y +=180;
                pobj[i].longitude +=180;
        }*/
        if(fabs(pobj[i].longitude) > 180){
            if (pobj[i].longitude > 180){
                pobj[i].longitude -=360;
                pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
            }
            else{
                pobj[i].longitude +=360;
                pobj[i].y = calc_fi(pobj[i].longitude,pobj[i].latitude,pobj[i].direction_long,pobj[i].direction_lat);
            }
        }
    }
    processingObject();
}

double Treat_objects::calc_dist(double x1, double y1, double x2, double y2){
    double directly = sqrt((x1-x2)*(x1-x2) +(y1-y2)*(y1-y2));
    double linedate = sqrt((360-fabs(x1)-fabs(x2))*(360-fabs(x1)-fabs(x2)) +(y1-y2)*(y1-y2));
    if (directly<linedate)
        return directly;
    return linedate;
}

double Treat_objects::calc_fi(double x1, double y1, double x2, double y2){
    double fi = 0;
    double directly = sqrt((x1-x2)*(x1-x2) +(y1-y2)*(y1-y2));
    double linedate = sqrt((360-fabs(x1)-fabs(x2))*(360-fabs(x1)-fabs(x2)) +(y1-y2)*(y1-y2));
    if (fabs(x1 - x2) < epsilon ){
        if (y1<y2)
            return 0;
        return 180;
    }
    if (directly<=linedate){
        if (y1<y2){
            fi = atan((x2-x1)/(y2-y1)) * 180 / M_PI;
            return fi;
        }
        if (x1<x2){
            fi = -atan((x2-x1)/(y2-y1)) * 180 / M_PI+90;
            return fi;
        }
        fi = -atan((x2-x1)/(y2-y1)) * 180 / M_PI-90;
        return fi;
    }
    else{
        fi = atan(360-fabs(x1)-fabs(x2)/(y2-y1))*180/M_PI;
        if (x1>0){
            return fi;
        }
        else{
            return -fi;
        }
    }
    return 0;
}

int Treat_objects::save_data(){
    QString currpath = pathtofile;
    currpath.append("/data/");
    QDir a(pathtofile);
    a.mkdir("data");
    a.cd("data");
    int newcurrtime = time_curr->elapsed()/1000;
    currpath.append(QString::number(newcurrtime,10));
    QFile ofs(currpath);
    if ( ! ofs.open(QIODevice::ReadWrite) ){
        qDebug() << "Can't open first file!";
        return 1;
    }
    for (int i = 0 ; i < current ; i++){
        QString towrite;
        towrite.append(pobj[i].id);
        towrite.append("@");
        towrite.append(QString::number(pobj[i].Offset_x));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].Offset_y));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].Offset_z));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].altitude));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].latitude));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].longitude));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].fof));
        towrite.append("@");
        towrite.append(pobj[i].type);
        towrite.append("@");
        towrite.append(pobj[i].WPID);
        towrite.append("@");
        towrite.append(pobj[i].RID);
        towrite.append("@");
        towrite.append(pobj[i].GRID);
        towrite.append("@");
        towrite.append(QString::number(pobj[i].speed));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].y));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].direction_long));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].direction_lat));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].dx));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].TimeOfBirthday));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].timelinecheck));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].duration));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].duration_check));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].isdraw));
        towrite.append("@");
        towrite.append(QString::number(pobj[i].follow));
        towrite.append("@\n");
        ofs.write(towrite.toLatin1().constData(),towrite.size());
    }
    ofs.write("@!@\n");
    for (int i = 0 ; i < currentWP ; i++){
        QString towrite;
        towrite.append(pWAYPOINT[i].id);
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].Offset_x));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].Offset_y));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].Offset_z));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].altitude));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].latitude));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].longitude));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].fof));
        towrite.append("@");
        towrite.append(pWAYPOINT[i].type);
        towrite.append("@");
        towrite.append(pWAYPOINT[i].WPID);
        towrite.append("@");
        towrite.append(pWAYPOINT[i].RID);
        towrite.append("@");
        towrite.append(pWAYPOINT[i].GRID);
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].speed));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].y));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].direction_long));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].direction_lat));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].dx));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].TimeOfBirthday));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].timelinecheck));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].duration));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].duration_check));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].isdraw));
        towrite.append("@");
        towrite.append(QString::number(pWAYPOINT[i].follow));
        towrite.append("@\n");
        ofs.write(towrite.toLatin1().constData(),towrite.size());
    }
    ofs.close();
    a.cdUp();
    return 0;
}

QString Treat_objects::find_out(int want_time)                                                                       // находит название файла с нужными данными (время записи файла меньше введённого времени)
{
    QString Dir_path = pathtofile;
    Dir_path.append("/data/");
    QDir Dir1(Dir_path); // ВАЖНО!!! Название УЖЕ СУЩЕСТВУЮЩЕГО!!! каталога пока что вводится вручную в коде
    QStringList fonts = Dir1.entryList(); // создали список qstring - ов, состоящий из имён всех файлов внутри каталога
    int delta = want_time - 1;
    int iskomij_num = -1;
    int i = fonts.size() - 1 ;
    int min_of_MAXs = 2147483647;
    while( i >= 0){
        if ( atof ( fonts.at( i ).toLocal8Bit().constData() ) <= want_time ){
            if ( want_time - atof ( fonts.at( i ).toLocal8Bit().constData() ) < delta ){
                delta = want_time - atof ( fonts.at( i ).toLocal8Bit().constData() ) ;
                iskomij_num = i;
            }
        }
        else{
            if ( atof ( fonts.at( i ).toLocal8Bit().constData() ) < min_of_MAXs ){
                QString s = QString::number( min_of_MAXs );
                QFile( Dir_path + s ).remove();
                min_of_MAXs = atof ( fonts.at( i ).toLocal8Bit().constData() ) ;
            }
            else{
                QFile( Dir_path + fonts.at( i ) ).remove();
            }
        }
        i--;
    }
    if ( iskomij_num == -1)
        if ( min_of_MAXs == 2147483647 ){
            qDebug() << "ERROR : dannie ne najdeny!!!";
            return 0;
        }
        else{
            QString s = QString::number( min_of_MAXs );
            return s;
        }
    QString c = fonts.at( iskomij_num );

    return c;
}

void Treat_objects::get_data_from_file(QString filename){
       bool t;
       QString pathtoneedfile = pathtofile;


       pathtoneedfile.append("/data/");
       pathtoneedfile.append(filename);
       QFile fin (pathtoneedfile);
       if (!fin.open(QIODevice::ReadOnly)){
           qDebug()<<"cant open file" << pathtoneedfile;
           return;
       }
       int i = 0;
       while (!fin.atEnd()){
           if (len <= i){
               if (len == 0){
                   len = 1000;
                   pobj = new object [len];
               }
               else{
                   object *temp = pobj;
                   pobj = new object [len*2];
                   for (int k = 0; k < i; ++k){
                       pobj[k] = temp[k];
                   }
                   len *=2;
                   delete [] temp;
               }
           }


           QString line = fin.readLine();
           if (line[1] == '!')break;
           QRegExp rx("@");
           QStringList line_elem = line.split(rx);
           strcpy(pobj[i].id,line_elem[0].toLatin1().constData());
           pobj[i].Offset_x = line_elem[1].toDouble(&t);
           pobj[i].Offset_y = line_elem[2].toDouble(&t);
           pobj[i].Offset_z = line_elem[3].toDouble(&t);
           pobj[i].altitude = line_elem[4].toDouble(&t);
           pobj[i].latitude = line_elem[5].toDouble(&t);
           pobj[i].longitude = line_elem[6].toDouble(&t);
           pobj[i].fof = line_elem[7].toInt(&t);
           strcpy(pobj[i].type,line_elem[8].toLatin1().constData());
           strcpy(pobj[i].WPID,line_elem[9].toLatin1().constData());
           strcpy(pobj[i].RID,line_elem[10].toLatin1().constData());
           strcpy(pobj[i].GRID,line_elem[11].toLatin1().constData());
           pobj[i].speed = line_elem[12].toDouble(&t);
           pobj[i].y = line_elem[13].toDouble(&t);
           pobj[i].direction_long = line_elem [14].toDouble(&t);
           pobj[i].direction_lat = line_elem [15].toDouble(&t);
           pobj[i].dx = line_elem[16].toDouble(&t);
           pobj[i].TimeOfBirthday = line_elem[17].toDouble(&t);
           pobj[i].timelinecheck = line_elem[18].toUInt(&t);
           pobj[i].duration = line_elem[19].toInt(&t);
           pobj[i].duration_check = line_elem[20].toInt(&t);
           pobj[i].isdraw = line_elem[21].toInt(&t);
           pobj[i].follow = line_elem[22].toInt(&t);
           i++;
       }
       current = i;
       i = 0;
       while (!fin.atEnd()){
           if (lenWP <= i){
               if (lenWP == 0){
                   lenWP = 1000;
                   pWAYPOINT = new object [lenWP];
               }
               else{
                   object *temp = pWAYPOINT;
                   pWAYPOINT = new object [lenWP*2];
                   for (int k = 0; k < i; ++k){
                       pWAYPOINT[k] = temp[k];
                   }
                   lenWP *=2;
                   delete [] temp;
               }
           }


           QString line = fin.readLine();
           if (line[1] == '!')break;
           QRegExp rx("@");
           QStringList line_elem = line.split(rx);
           strcpy(pWAYPOINT[i].id,line_elem[0].toLatin1().constData());
           pWAYPOINT[i].Offset_x = line_elem[1].toDouble(&t);
           pWAYPOINT[i].Offset_y = line_elem[2].toDouble(&t);
           pWAYPOINT[i].Offset_z = line_elem[3].toDouble(&t);
           pWAYPOINT[i].altitude = line_elem[4].toDouble(&t);
           pWAYPOINT[i].latitude = line_elem[5].toDouble(&t);
           pWAYPOINT[i].longitude = line_elem[6].toDouble(&t);
           pWAYPOINT[i].fof = line_elem[7].toInt(&t);
           strcpy(pWAYPOINT[i].type,line_elem[8].toLatin1().constData());
           strcpy(pWAYPOINT[i].WPID,line_elem[9].toLatin1().constData());
           strcpy(pWAYPOINT[i].RID,line_elem[10].toLatin1().constData());
           strcpy(pWAYPOINT[i].GRID,line_elem[11].toLatin1().constData());
           pWAYPOINT[i].speed = line_elem[12].toDouble(&t);
           pWAYPOINT[i].y = line_elem[13].toDouble(&t);
           pWAYPOINT[i].direction_long = line_elem [14].toDouble(&t);
           pWAYPOINT[i].direction_lat = line_elem [15].toDouble(&t);
           pWAYPOINT[i].dx = line_elem[16].toDouble(&t);
           pWAYPOINT[i].TimeOfBirthday = line_elem[17].toDouble(&t);
           pWAYPOINT[i].timelinecheck = line_elem[18].toUInt(&t);
           pWAYPOINT[i].duration = line_elem[19].toInt(&t);
           pWAYPOINT[i].duration_check = line_elem[20].toInt(&t);
           pWAYPOINT[i].isdraw = line_elem[21].toInt(&t);
           pWAYPOINT[i].follow = line_elem[22].toInt(&t);

           i++;
       }
       currentWP = i;

}

void Treat_objects::calc_data(bool checker){
    if (checker){
        int time = (need_time - pobj[0].timelinecheck) *50;
        while(time>0){
            processingPosObject_withoutsenddata(need_time);
            time --;
        }
        return;
    }
    int time = (curr_time) *50;
    while(time>0){
        processingPosObject_withoutsenddata(need_time);
        time --;
    }
}

