package com.team.hack.mipt.helper.mipthelper.helper;

import android.app.Service;
import android.bluetooth.BluetoothClass;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import com.team.hack.mipt.helper.mipthelper.helper.Values;

import java.io.*;
import java.util.Vector;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by semyon on 19.11.16.
 */
public class ServiceOfLab extends Service {

    public void onCreate() {
        super.onCreate();
    }

    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        MyRun myRun = new MyRun(intent);
        myRun.run();
        return super.onStartCommand(intent, flags, startId);
    }

    public IBinder onBind(Intent arg0) {
        return null;
    }

    class MyRun implements Runnable {

        private Intent mainIntent;

        MyRun(Intent intent) {
            mainIntent = intent;
        }

        public class database {
            public String name_lab = "";
            public Vector<CharSequence> link_stats = new Vector();
            public String btm;
        }

        public database Get_Data_Of_Labs(String name_lab) {
            database lab = new database();
            if (getSharedPreferences(Values.configSettings, 0).contains(name_lab)) {
                try {
                    InputStream inputStream = openFileInput(name_lab);

                    if (inputStream != null) {
                        InputStreamReader br = new InputStreamReader(inputStream);
                        BufferedReader reader = new BufferedReader(br);
                        String line;
                        StringBuilder builder = new StringBuilder();
                        String str;

                        str = reader.readLine();
                        lab.name_lab = str;
                        int i = 0;
                        while ((str = reader.readLine()) != null) {
                            lab.link_stats.addElement(str);

                        }
                        br.close();

                    }
                } catch (Throwable t) {
                    Toast.makeText(getApplicationContext(),
                            "Exception: " + t.toString(), Toast.LENGTH_LONG).show();
                }

            }
            return lab;
        }

        public void run() {

            Intent intent = new Intent(Values.listOfLabs);
            String name_lb = mainIntent.getStringExtra(Values.name_lab);
// начинаем выполнение задачи
            database Laba = Get_Data_Of_Labs(name_lb);

// сообщаем об окончании задачи
            CharSequence[] st = new CharSequence[Laba.link_stats.size()];
            Laba.link_stats.copyInto(st);
            intent.putExtra(Values.list_list, st);
            intent.putExtra(Values.link_bitmap, Laba.btm);
            intent.setAction(Values.showOneLab);
            sendBroadcast(intent);

        }

    }
}

