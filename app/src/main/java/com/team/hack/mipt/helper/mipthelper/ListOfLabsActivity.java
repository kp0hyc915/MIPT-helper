package com.team.hack.mipt.helper.mipthelper;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.team.hack.mipt.helper.mipthelper.helper.GetLabsFromSDService;
import com.team.hack.mipt.helper.mipthelper.helper.ServiceOfLab;
import com.team.hack.mipt.helper.mipthelper.helper.Values;

import java.util.Arrays;
import java.util.HashSet;

public class ListOfLabsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list_of_labs);
        /*startService(new Intent(this, GetLabsFromSDService.class));
        BroadcastReceiver br = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                this.abortBroadcast();
                LinearLayout ll = (LinearLayout) findViewById(R.id.show_labs_list_layout);
                CharSequence[] cs = intent.getCharSequenceArrayExtra(Values.listOfLabs);
                for (int i = 0; i < cs.length; ++i) {
                    RelativeLayout rl = new RelativeLayout(context);
                    LinearLayout.LayoutParams params2 = new LinearLayout.
                            LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
                    rl.setLayoutParams(params2);

                    TextView tv = new TextView(context);
                    RelativeLayout.LayoutParams params = new RelativeLayout.
                            LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
                    params.addRule(RelativeLayout.CENTER_IN_PARENT);
                    tv.setLayoutParams(params);
                    tv.setText(cs[i]);

                    rl.addView(tv);
                    rl.setTag(cs[i]);
                    rl.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            Intent intent = new Intent(view.getContext(), ServiceOfLab.class);
                            intent.putExtra(Values.name_lab, ((String)view.getTag()));
                            sendBroadcast(intent);
                            BroadcastReceiver br = new BroadcastReceiver() {
                                @Override
                                public void onReceive(Context context, Intent intent) {
                                    this.abortBroadcast();
                                    intent.setClass(context, ShowLaborotorActivity.class);
                                    startActivity(intent);
                                }
                            };
                            registerReceiver(br, new IntentFilter(Values.showOneLab));
                            startActivity(new Intent(view.getContext(), ShowLaborotorActivity.class));
                        }
                    });
                    ll.addView(rl);
                }
            }
        };
        registerReceiver(br, new IntentFilter(Values.listOfLabsFromSD));*/
        LinearLayout ll = (LinearLayout) findViewById(R.id.show_labs_list_layout);
        SharedPreferences sp = getSharedPreferences(Values.configSettings, 0);
        String cs = sp.getString("dowloadedLabs", "");
        String str = "";
        for (int i = 0; i < cs.length(); ++i) {
            if (cs.charAt(i) == '/') {
                RelativeLayout rl = new RelativeLayout(this);
                LinearLayout.LayoutParams params2 = new LinearLayout.
                        LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
                rl.setLayoutParams(params2);

                TextView tv = new TextView(this);
                RelativeLayout.LayoutParams params = new RelativeLayout.
                        LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
                params.addRule(RelativeLayout.CENTER_IN_PARENT);
                tv.setLayoutParams(params);
                tv.setText(str);
                tv.setTextSize(100);

                rl.addView(tv);
                rl.setTag(str);
                rl.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        Intent intent = new Intent(view.getContext(), ServiceOfLab.class);
                        intent.putExtra(Values.name_lab, ((String)view.getTag()));
                        sendBroadcast(intent);
                        BroadcastReceiver br = new BroadcastReceiver() {
                            @Override
                            public void onReceive(Context context, Intent intent) {
                                this.abortBroadcast();
                                intent.setClass(context, ShowLaborotorActivity.class);
                                startActivity(intent);
                            }
                        };
                        registerReceiver(br, new IntentFilter(Values.showOneLab));
                        startActivity(new Intent(view.getContext(), ShowLaborotorActivity.class));
                    }
                });
                ll.addView(rl);
                str = "";
            } else {
                str += cs.charAt(i);
            }
        }

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater mi = getMenuInflater();
        mi.inflate(R.menu.main_menu, menu);
        return true;
    }
}
