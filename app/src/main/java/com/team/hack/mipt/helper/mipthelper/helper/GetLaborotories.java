package com.team.hack.mipt.helper.mipthelper.helper;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import static android.R.attr.id;

/**
 * Created by semyon on 19.11.16.
 */

//TODO remake this class
public class GetLaborotories extends Service {

    String recivemessage = new String();

    byte[] requetServer = null;

    public void onCreate() {
        super.onCreate();
    }

    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        MyRun myRun = new MyRun(intent, this);
        myRun.run();
        return super.onStartCommand(intent, flags, startId);
    }

    public IBinder onBind(Intent arg0) {
        return null;
    }

    class MyRun implements Runnable {

        private Intent mainIntent;
        private Context mainContext;

        MyRun(Intent intent, Context context) {
            mainIntent = intent;
            mainContext = context;
        }
        public void Get_Data_Of_Labs_From_Server(String name_lab1) {
            SharedPreferences.Editor editor = getSharedPreferences(Values.configSettings, 0).edit();
            if(getSharedPreferences(Values.configSettings, 0).getInt("id", -1) == -1){
                editor.putInt("id", 1);
            }
            editor.putString(Integer.toString(getSharedPreferences(Values.configSettings, 0).getInt("id", 0)), name_lab1);
            editor.putInt("id",getSharedPreferences(Values.configSettings, 0).getInt("id", 0) + 1);
            editor.apply();
            try {
                InputStream intputStream1 = openFileInput("recivedata");
                InputStreamReader rsw = new InputStreamReader(intputStream1);
                BufferedReader reader = new BufferedReader(rsw);
                String str;
                str = reader.readLine();
                int i = 0, j = 0;
                int size_of_file = Integer.parseInt(str.substring(0,str.indexOf('/')));
//Fuck this CODE!!!!!!!!!!!
                str = str.substring(str.indexOf('/'), str.length()-1);
                int size_of_picture = Integer.parseInt(str.substring(0,str.indexOf('/')));
                str = str.substring(str.indexOf('/'), str.length()-1);
                String Code_Picture = str.substring(0,str.indexOf('/'));
                str = str.substring(str.indexOf('/'), str.length()-1);
                String[] list_of_answerandquestion = str.split("/");
                OutputStream outputStream1;
                try{
                    outputStream1 = openFileOutput(name_lab1, 0);
                    OutputStreamWriter osw = new OutputStreamWriter(outputStream1);
                    osw.write(Integer.toString(size_of_file)+"\n");
                    osw.write(Integer.toString(size_of_picture)+"\n");
                    osw.write(Code_Picture + "\n");
                    osw.write(list_of_answerandquestion[0]);
                    int k = 0;
                    while (k < list_of_answerandquestion.length){
                        osw.write(list_of_answerandquestion[k]+"\n");
                        k++;
                    }
                    osw.close();
                } catch (Throwable t){

                }
            } catch (IOException e) {
                e.printStackTrace();
            }

        }


        public void run() {
            int sem = mainIntent.getIntExtra(Values.semesterNumber, -1);

            if(sem == -1){
                String str1[] = mainIntent.getStringArrayExtra("name_lab_load");
                int j = 0;
                while (j < str1.length) {
                    String sd = str1[j];
                    str1[j] = GetLabByName(str1[j]);


                    recivemessage = null;
                    HandlerRequest handlerRequest = new HandlerRequest(str1[j]);
                    handlerRequest.execute();
                    Get_Data_Of_Labs_From_Server(sd);
                    j++;
                }
            }else{
                String str = GetLabList(sem);
                recivemessage = null;
                HandlerRequest handlerRequest = new HandlerRequest(str);
                handlerRequest.execute();


                BroadcastReceiver br2 = new BroadcastReceiver() {
                    @Override
                    public void onReceive(Context
                                                  context, Intent intent) {
                        this.abortBroadcast();
                        CharSequence[] list_of_labs_from_server, list_of_labs;
                        list_of_labs_from_server = intent.getCharSequenceArrayExtra("qwerty");
                        list_of_labs = new CharSequence[list_of_labs_from_server.length];
                        int i = 0;
                        while (i < list_of_labs_from_server.length) {
                            if (!getSharedPreferences(Values.configSettings, 0).contains(list_of_labs_from_server[i].toString())) {
                                list_of_labs[i] = list_of_labs_from_server[i];
                            }
                            i++;
                        }
                        Intent intent2 = new Intent();
                        intent2.setAction(Values.intentGetLabs);
                        intent2.putExtra(Values.listOfLabs, list_of_labs);
                        sendBroadcast(intent2);
                    }
                };
                registerReceiver(br2, new IntentFilter("kp0hyc"));
            }

// сообщаем об окончании задачи
        }

        private String GetLabList(int NSemestr) {
            String sndMSG = "";
            sndMSG += "1/";
            sndMSG += String.valueOf(NSemestr);
            return sndMSG;
        }

        private String GetLabByName(String name_lab){
            String sndMSG = "2";
            sndMSG += '/';
            sndMSG += name_lab;
            return sndMSG;
        }
// доброе утро, город, меня зовут Вова, мой ник packman
        class HandlerRequest extends AsyncTask<Void, Integer, Void> {
            public String SERVERIP = "192.168.43.34"; //your computer IP address
            public int Port = 2233;
            Socket socket = null;
            String nSem = new String();
            String rcvMSG = new String();

            public HandlerRequest(String view) {
                nSem = view;
            }

            @Override
            protected Void doInBackground(Void... voids) {

                try {
                    socket = new Socket(SERVERIP, Port);



                    OutputStream outputStream = socket.getOutputStream();
                    InputStream inputStream = socket.getInputStream();

                    if (socket.isConnected()) {
                        outputStream.write(nSem.getBytes());
                    }

                    byte[] bufin = new byte[1024];

                    int bytesRead;
                    int lengthDatarecive = 0;
                    boolean check = false;
                    int dataLength = 0;
                    OutputStream outputStream1;
                    try{
                        outputStream1 = openFileOutput("recivedata", 2);
                        OutputStreamWriter osw = new OutputStreamWriter(outputStream1);

                        while ((bytesRead = inputStream.read(bufin,0,256)) != -1) {
                            if ((bytesRead != 256)&&(!check)){
                                String str = new String(bufin);

                                String[] stringarr = str.split("/");


                                socket.close();
                                Intent intent2 = new Intent();
                                intent2.putExtra("qwerty", stringarr);
                                intent2.setAction("kp0hyc");
                                sendBroadcast(intent2);

                                stopSelf();
                                return null;
                            }
                            else{
                                String out = new String(bufin);
                                check = true;
                                osw.append(out);

                                if (bytesRead != 256){

                                    String[] stringarr = {"REVIVEDINFILE"};


                                    osw.close();
                                    socket.close();
                                    Intent intent2 = new Intent();
                                    intent2.putExtra("qwerty", stringarr);
                                    intent2.setAction("kp0hyc");
                                    sendBroadcast(intent2);

                                    stopSelf();
                                    return null;
                                }
                            }
                        }
                    } catch (Throwable t){

                    }
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                } finally {
                    if (socket != null) {
                        try {
                            socket.close();
                        } catch (IOException e) {
                        }
                    }
                }
                return null;
            }
        }
    }
}