package com.team.hack.mipt.helper.mipthelper;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Html;
import android.text.method.LinkMovementMethod;
import android.view.Menu;
import android.view.MenuInflater;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.team.hack.mipt.helper.mipthelper.helper.Values;

public class LiteratureActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_literature);
        LinearLayout ll = (LinearLayout) findViewById(R.id.links_layout);
        CharSequence[] st = getIntent().getCharSequenceArrayExtra(Values.keyQuestions);
        for (int i = 0; i < st.length; ++i) {
            TextView textView =(TextView)findViewById(R.id.textView);
            textView.setClickable(true);
            textView.setMovementMethod(LinkMovementMethod.getInstance());
            String text = "<a href='" + st[i] + "'> " + st[i] + " </a>";
            textView.setText(Html.fromHtml(text));
            ll.addView(textView);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater mi = getMenuInflater();
        mi.inflate(R.menu.main_menu, menu);
        return true;
    }
}
