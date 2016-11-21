package com.team.hack.mipt.helper.mipthelper;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.team.hack.mipt.helper.mipthelper.helper.Values;

public class SpecialQsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_special_qs);
        LinearLayout ll = (LinearLayout) findViewById(R.id.special_qs_layout);
        CharSequence[] st = getIntent().getCharSequenceArrayExtra(Values.keyQuestions);
        for (int i = 0; i < st.length; ++i) {
            TextView tv = new TextView(this);
            tv.setText(st[i]);
            ll.addView(tv);
        }

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater mi = getMenuInflater();
        mi.inflate(R.menu.main_menu, menu);
        return true;
    }
}
