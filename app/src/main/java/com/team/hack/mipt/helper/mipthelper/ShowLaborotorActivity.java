package com.team.hack.mipt.helper.mipthelper;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;

import com.team.hack.mipt.helper.mipthelper.helper.GetLaborotories;
import com.team.hack.mipt.helper.mipthelper.helper.ServiceOfLab;
import com.team.hack.mipt.helper.mipthelper.helper.Values;

public class ShowLaborotorActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_show_laborotor);

    }

    public void onDescriptionClick(View v) {
        Intent intent = new Intent(this, DescriptionActivity.class);
        intent.putExtra(Values.currentLaba, intent.getStringExtra(Values.picturePath));
        startActivity(intent);
    }

    public void onSpecialQsClick(View v) {
        Intent intent = new Intent(this, SpecialQsActivity.class);
        intent.putExtra(Values.currentLaba, intent.getStringExtra(Values.keyQuestions));
        startActivity(intent);
    }

    public void onLiteratureClick(View v) {
        Intent intent = new Intent(this, LiteratureActivity.class);
        intent.putExtra(Values.currentLaba, intent.getStringExtra(Values.links));
        startActivity(intent);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater mi = getMenuInflater();
        mi.inflate(R.menu.main_menu, menu);
        return true;
    }
}
