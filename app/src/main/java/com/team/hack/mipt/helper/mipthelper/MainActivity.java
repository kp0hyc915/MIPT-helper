package com.team.hack.mipt.helper.mipthelper;

import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;

import com.team.hack.mipt.helper.mipthelper.helper.Values;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        SharedPreferences sp = getSharedPreferences(Values.configSettings, 0);

        if (sp.getBoolean(Values.firstBootPath, true)) {
            startActivity(new Intent(this, FirstBootActivity.class));
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater mi = getMenuInflater();
        mi.inflate(R.menu.main_menu, menu);
        return true;
    }

    public void onPhysicsLabsClick(View v) {
        startActivity(new Intent(this, ListOfLabsActivity.class));
    }

    public void onClick(View v) {
        /**/
    }
}
