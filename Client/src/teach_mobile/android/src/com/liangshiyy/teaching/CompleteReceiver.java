package com.liangshiyy.teaching;

import java.io.File;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;

class CompleteReceiver extends BroadcastReceiver {
		//public static HashMap<long, String> downloadSavePath = new HashMap<long, String>();
		public String save_path = "";
		@Override
		public void onReceive(Context context, Intent intent) {
			// get complete download id
			//long completeDownloadId = intent.getLongExtra(DownloadManager.EXTRA_DOWNLOAD_ID, -1);
			System.out.println("download OK: "+ save_path);
			downComplete(save_path);
			// to do here
		}
		
		private void downComplete(String filePath){
			System.out.println("filePath : "+filePath);
			File _file =  new File(filePath);
			Intent intent = new Intent();
			System.out.println("安装apk ："+_file.getName()+" : "+_file.length()+"--"+_file.getPath()+"--"+_file.canRead()+"--"+_file.exists());
			intent.setAction("android.intent.action.VIEW");//向用户显示数据
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);//以新压入栈
			intent.addCategory("android.intent.category.DEFAULT");
			//intent.setType("application/vnd.android.package-archive");
			//intent.setData(Uri.fromFile(file));
			Uri abc = Uri.fromFile(_file);
			intent.setDataAndType(abc, "application/vnd.android.package-archive");
			
			//startActivity(intent);
		}
	};