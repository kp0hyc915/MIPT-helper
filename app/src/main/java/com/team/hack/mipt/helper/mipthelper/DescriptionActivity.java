package com.team.hack.mipt.helper.mipthelper;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.TransitionDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import static com.team.hack.mipt.helper.mipthelper.R.*;

public class DescriptionActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layout.activity_description);
        ImageView im = (ImageView) findViewById(id.description_image);
        im.setImageResource(drawable.p_4_8_3);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater mi = getMenuInflater();
        mi.inflate(R.menu.main_menu, menu);
        return true;
    }
    protected void onDraw(Canvas canvas){
        canvas.drawBitmap(BitmapFactory.decodeResource(getResources(), drawable.p_4_8_3),10,15,null);
    }
}
