#include <QtNetwork/QNetworkAccessManager>
#include <cmath>
#include <QTimer>
#include <ctime>
#include <QTime>

struct object{
    char id[16];
    double Offset_x;
    double Offset_y;
    double Offset_z;
    double altitude;
    double latitude; // lat
    double longitude; // lng
    int fof; // friendly or enemy or neutral 1 - friend, 0 - neural, -1 - enemy
    char type[32]; // figure
    char WPID[16]; //id next point
    char RID[32]; // Road id
    char GRID[16];
    char groupid[16];
    double speed;
    double y; // fi
    double direction_long; // napr_lng
    double direction_lat; // napr lat
    double dx;
    double timetobirthday;
    double TimeOfBirthday; //time in which the object is created
    double timelinecheck; //the time elapsed since the start of the program
    double duration;
    bool duration_check;
    bool isdraw;
    bool follow;
    bool fromScenario;
};

class Treat_objects : public QObject
{
    Q_OBJECT

public:
    Treat_objects();
    int retLen();
    int* retCurr();
    object* retBArr();
    void add_object(object);
    void processingObject();
    double calc_dist(double,double,double,double);
    double calc_fi(double,double,double,double);
    int save_data();
    QString find_out(int );
    void get_data_from_file(QString);
    void calc_data(bool);
    void processingPosObject_withoutsenddata(int);
    void create_script(object,QString);
    void delete_data();

signals:
    void valueChanged();

private slots:
    void processingPosObject();

private:
    void add_WP(object);
    void add_OBJ(object);
    void set_time(object);
    void set_newobjectdata(object,int);
    double epsilon = 0.00001;
    object *pobj;
    object *pWAYPOINT;
    int len;
    int lenWP;
    int current;
    int currentWP;
    int curr_time;
    int need_time;
    int start_time;
    pthread_t id;
    QTimer *timer;
    QTime *time_curr;
    QTime uselesstime;
    QString pathtofile;
    void Death(object&, object&);
};

