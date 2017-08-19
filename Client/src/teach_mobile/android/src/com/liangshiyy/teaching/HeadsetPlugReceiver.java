package com.liangshiyy.teaching;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.widget.Toast;

public class HeadsetPlugReceiver extends BroadcastReceiver {

	private static final String TAG = "HeadsetPlugReceiver";

	@Override
	public void onReceive(Context context, Intent intent) {
		if(intent.hasExtra("state")){
			AudioManager audioManager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
			if(intent.getIntExtra("state", 0) == 0){
				audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
				audioManager.setSpeakerphoneOn(true);
				Toast.makeText(context, "没有检测到耳机", Toast.LENGTH_LONG).show();
			}
			else if (intent.getIntExtra("state", 0) == 1){
				audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
				audioManager.setSpeakerphoneOn(false);
				Toast.makeText(context, "检测到耳机", Toast.LENGTH_LONG).show();
			}
		  }
		
	}

}