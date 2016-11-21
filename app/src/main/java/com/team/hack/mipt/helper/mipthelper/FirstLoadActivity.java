package com.team.hack.mipt.helper.mipthelper;

import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.team.hack.mipt.helper.mipthelper.helper.GetLaborotories;
import com.team.hack.mipt.helper.mipthelper.helper.Values;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

public class FirstLoadActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first_load);

        CharSequence[] labs = getIntent().getExtras().getCharSequenceArray(Values.listOfLabs);
        LinearLayout layout = (LinearLayout) findViewById(R.id.first_load_layout);
        for(int i = 0; i < labs.length; ++i) {
            ImageView iv = new ImageView(this);
            iv.setImageBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.ok));
            iv.setId(R.id.image_id);
            RelativeLayout.LayoutParams params = new RelativeLayout.
                    LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
            params.addRule(RelativeLayout.CENTER_VERTICAL);
            iv.setLayoutParams(params);

            TextView tv = new TextView(this);
            tv.setId(R.id.local_text);
            tv.setText(labs[i]);
            params = new RelativeLayout.
                    LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
            params.addRule(RelativeLayout.CENTER_VERTICAL);
            params.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
            params.addRule(RelativeLayout.RIGHT_OF, R.id.image_id);
            params.leftMargin = 10;
            tv.setLayoutParams(params);

            RelativeLayout rl = new RelativeLayout(this);
            LinearLayout.LayoutParams paramsRL = new LinearLayout.
                    LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
            paramsRL.topMargin = 10;
            paramsRL.bottomMargin = 10;
            rl.setLayoutParams(paramsRL);
            rl.addView(iv);
            rl.addView(tv);

            rl.setTag(R.id.dop_id, iv);
            rl.setTag(true);
            rl.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (view.getTag().equals(false)) {
                        ImageView iv = (ImageView) view.findViewById(R.id.image_id);
                        iv.setImageBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.ok));
                        view.setTag(true);
                    } else {
                        ImageView iv = (ImageView) view.findViewById(R.id.image_id);
                        iv.setImageBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.red_circle));
                        view.setTag(false);
                    }
                }
            });

            layout.addView(rl);
        }

    }

    public void onContinueButton(View view) {
        /*Intent intent = new Intent(this, DownloadLabs.class);

        startService(intent);

        BroadcastReceiver br = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {

            }
        };
        registerReceiver(br, new IntentFilter(Values.actionDownloadProgress));*/
        LinearLayout layout = (LinearLayout) findViewById(R.id.first_load_layout);
        Vector<String> vector = new Vector();
        for (int i = 0; i < layout.getChildCount(); ++i) {
            RelativeLayout v = (RelativeLayout) layout.getChildAt(i);
            if (v.getTag().equals(true)) {
                TextView tv = (TextView) v.getChildAt(1);
                vector.addElement(tv.getText().toString());
            }
        }
        String[] str = new String[vector.size()];
        vector.copyInto(str);
        Intent intent = new Intent(this, GetLaborotories.class);
        intent.putExtra("name_lab_load", str);
        //startService(intent);
        SharedPreferences.Editor spe = getSharedPreferences(Values.configSettings, 0).edit();
        spe.putBoolean(Values.firstBootPath, false);
        String str2 = "";
        for (int i = 0; i < str.length; ++i) {
            str2 += (str[i] + "/");
        }
        spe.putString("dowloadedLabs", str2);
        spe.apply();
        startActivity(new Intent(this, MainActivity.class));

        /*Intent intent = new Intent(this, ServiceOfLab.class);
        intent.putExtra(Values.name_lab, "name");
        startService(intent);

        BroadcastReceiver br = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                Toast t = Toast.makeText(context, intent.getStringExtra(Values.link_bitmap), Toast.LENGTH_SHORT);
                t.show();
            }
        };
        registerReceiver(br, new IntentFilter(Values.showOneLab));*/
    }
}

