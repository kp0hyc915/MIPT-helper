package com.team.hack.mipt.helper.mipthelper;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import com.team.hack.mipt.helper.mipthelper.helper.GetLaborotories;
import com.team.hack.mipt.helper.mipthelper.helper.Values;

public class FirstBootActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first_boot);
    }

    public void onContinueClick(View v) {
        String name = ((EditText) findViewById(R.id.name_edit_text)).getText().toString();
        String group = ((EditText) findViewById(R.id.group_edit_text)).getText().toString();
        String semester = ((EditText) findViewById(R.id.sem_edit_text)).getText().toString();
        int iSem;
        if (!name.equals("") && checkGroup(group) && ((iSem = checkSemester(semester)) != 0)) {
            SharedPreferences.Editor spe = getSharedPreferences(Values.aboutUserPath, 0).edit();
            spe.putString(Values.name, name);
            spe.putString(Values.group, group);
            spe.putInt(Values.semester, iSem);
            spe.apply();

            Intent intent = new Intent(this, GetLaborotories.class);
            intent.setAction(Values.get_labs_for_semester);
            startService(intent);

            BroadcastReceiver br = new GetterLabsForSemester();
            registerReceiver(br, new IntentFilter(Values.get_labs_for_semester));
        } else {
            //TODO here we should make design
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage(R.string.dialog_ask_to_complete_gap)
                    .setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.dismiss();
                        }
                    });
            builder.create().show();
        }
    }
//TODO make the checking of group
    private boolean checkGroup(String group) {
        return true;
    }

    private int checkSemester(String sem) {
        try {
            int i =  Integer.parseInt(sem);
            if (i >= 0 && i <= 6)
                return i;
            else
                return 0;
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    class GetterLabsForSemester extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            unregisterReceiver(this);
            Intent sh_intent = new Intent(context, FirstLoadActivity.class);
            sh_intent.putExtra(Values.listOfLabs, intent.getCharSequenceArrayExtra(Values.listOfLabs));
            startActivity(sh_intent);
        }
    }
}
