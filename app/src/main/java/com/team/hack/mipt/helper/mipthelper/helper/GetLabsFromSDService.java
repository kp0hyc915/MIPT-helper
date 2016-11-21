package com.team.hack.mipt.helper.mipthelper.helper;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class GetLabsFromSDService extends Service {
    public GetLabsFromSDService() {

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Intent local_intent = new Intent();
        local_intent.setAction(Values.listOfLabsFromSD);
        CharSequence[] cs = new CharSequence[5];
        cs[0] = "hello";
        cs[1] = "fucking";
        cs[2] = "world";
        cs[3] = "how";
        cs[4] = "are";
        local_intent.putExtra(Values.listOfLabs, cs);
        sendBroadcast(local_intent);
        stopSelf();
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
