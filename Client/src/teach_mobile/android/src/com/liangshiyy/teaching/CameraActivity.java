package com.liangshiyy.teaching;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import com.liangshiyy.teaching.R;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.CameraInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class CameraActivity extends Activity {
	private static String TAG = "liangshi_teach";
	private SurfaceView _surfaceView = null;
	private SurfaceHolder _surfaceHolder = null;
	private Button _takePictureButton = null;
	private Camera _camera = null;
	private String _connectFilePath;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Log.d(TAG, "CameraActivity onCreate");
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_camera);
		
		///data/data/org.qtproject.example.teach_mobile/cache
		File dir = getCacheDir();
		_connectFilePath = dir.toString() + "/connectImage.jpg";
		Log.d(TAG, "Java part image path:".concat(_connectFilePath));
		
		_takePictureButton = (Button)this.findViewById(R.id.button);
		_surfaceView = (SurfaceView)this.findViewById(R.id.surfaceview);
		_surfaceHolder = _surfaceView.getHolder();
		_surfaceHolder.addCallback(new SurfaceHolderCallback());
		_surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		_takePictureButton.setOnClickListener(new View.OnClickListener(){
			public void onClick(View v) {
				Log.d(TAG, "takePicture button pressed");
				_camera.takePicture(null, null, pictureCallback);
			}
		});
	}
	
	@Override
	public void onBackPressed(){
		_camera.stopPreview();
		_camera.unlock();
		_camera.release();
		File file = new File(_connectFilePath);
		file.delete();
		try{
			file.createNewFile();
		} catch (IOException e) {
		}
		finish();
	}
	
	public class SurfaceHolderCallback implements SurfaceHolder.Callback {

		public void surfaceChanged(SurfaceHolder holder, int format, int width,
				int height) {
			Log.d(TAG, "surface changed");
			Bitmap getpage;
			getpage = Bitmap.createBitmap(800, 380, Bitmap.Config.ARGB_8888);
			Canvas canvas = new Canvas(getpage);
			canvas.drawColor(Color.LTGRAY);//这里可以进行任何绘图步骤 
			canvas.save(Canvas.ALL_SAVE_FLAG);
			canvas.restore();
		}

		public void surfaceCreated(SurfaceHolder holder) {
			int backCameraID = -1;
			int frontCameraID = -1;			
			if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD){
				for(int i = 0; i<Camera.getNumberOfCameras();i++){
					CameraInfo info = new CameraInfo();
					Camera.getCameraInfo(i, info);
					if(info.facing == CameraInfo.CAMERA_FACING_BACK){
						backCameraID = i;
					}
					if(info.facing == CameraInfo.CAMERA_FACING_FRONT){
						frontCameraID = i;
					}
				}
			}
			//优先用后置
			if(backCameraID != -1)
			{
				_camera = Camera.open(backCameraID);
			}
			else if(frontCameraID != -1)
			{
				_camera = Camera.open(frontCameraID);	
			}
			
			if(_camera == null){
				_camera = Camera.open();
				Log.d(TAG, "back camera opened");
			}
			try {
				_camera.setPreviewDisplay(_surfaceHolder);
				Log.d(TAG, "set preview display succeed");
			}catch (IOException e){
				e.printStackTrace();
				Log.d(TAG, "set preview display failed");
			}
			
			_camera.setDisplayOrientation(90);
			try {
				_camera.reconnect();
			} catch (IOException e) {
				e.printStackTrace();
			}
			try {
				_camera.reconnect();
			} catch (IOException e) {
				e.printStackTrace();
			}
			_camera.startPreview();
			//_camera.autoFocus(afcb);
			_camera.autoFocus(null);
			Log.d(TAG, "start preview");
			
		}

		public void surfaceDestroyed(SurfaceHolder holder){
			if(_camera != null){
				_camera = null;
				Log.d(TAG, "camera released");
			}
		}
	}
	
	AutoFocusCallback afcb= new AutoFocusCallback(){
		@Override
		public void onAutoFocus(boolean success, Camera camera) {
			// TODO Auto-generated method stub
			if(success){
				//对焦成功才拍照
				camera.takePicture(null, null, pictureCallback);
			}
		}
 	};
	
	private Camera.PictureCallback pictureCallback = new Camera.PictureCallback(){
		public void onPictureTaken(byte[] data, Camera camera){
			Log.d(TAG, "image handling");
			_camera.stopPreview();
			_camera.unlock();
			_camera.release();
			Log.d(TAG, "stop preview");
			Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length);


			Matrix matrix = new Matrix();

			//这里确定windows上的标准分辨率
			int deskTopWidth = 1278;
			int deskTopheight = 732;
			
			int width = bitmap.getWidth();
			int height = bitmap.getHeight();
			
			float scaleWidth = ((float) deskTopWidth) / width;
			float scaleHeight = ((float) deskTopheight) / height; 
			
			matrix.postScale(scaleWidth, scaleHeight);
			//matrix.postRotate(90);
			
			Bitmap mobileBitmap = Bitmap.createBitmap(bitmap, 0, 0, width, height, matrix, true);
			
			Log.d(TAG, "Java part image path:".concat(_connectFilePath));
			String beforeRename = _connectFilePath + "1";
			File pictureFile = new File(beforeRename);
			try{
				FileOutputStream fileOutputStream = new FileOutputStream(pictureFile);
				mobileBitmap.compress(Bitmap.CompressFormat.JPEG, 100, fileOutputStream);
				try{
					fileOutputStream.close();
					Log.d(TAG, "fileOutputStream close succeed");
				}catch(IOException e){
					e.printStackTrace();
					Log.d(TAG, "fileOutputStream close failed");
				}
			}catch(FileNotFoundException e){
				e.printStackTrace();
				Log.d(TAG, "file not found");
			}
			File to = new File(_connectFilePath);
			boolean ok = pictureFile.renameTo(to);
			if(!ok)
				Log.d(TAG, "rename failed");
			finish();
		}
	};
	
	private void displayToast(String s){
		Toast.makeText(this, s, 1).show();
	}
	
	/*
	 public static Intent createChoosePhotoIntent() {
		Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI );
		intent.setType("image/*");
		return Intent.createChooser(intent, "Select Image");
	}
	*/
}