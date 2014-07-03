

package itec.android.DASH.DASHEncoder;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.text.Html;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;


public class DASHEncoder extends Activity implements OnClickListener{

	static final public String TAG = "DASHEncoder";

	private StreamingLoop mediaRecorderLoop;
	private SharedPreferences settings;
	private SurfaceView camera;
	private SurfaceHolder holder;
	private int resX, resY, fps, br, segmentSize;
	private PowerManager.WakeLock wl;
	private String uploadServer;
	private boolean enabledPreview = false;

	RecorderWithPreview mPreview;
	private Camera mCamera;
	int numberOfCameras;
	int cameraCurrentlyLocked;
	Context baseContext;
	FrameLayout fmain;
	// The first rear facing camera
	int defaultCameraId;

	// Our httpd
	final String wwwRoot = "/sdcard/dash";
	DASHServer dashServer;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		// load the JNI things 
		NativeInterface.LoadLibraries();

		// see if there are already some default values, if not tell the user that he/she should change them...

		updateSettings();

		// tell the user the standard settings, he has set at his last visit
		Toast toast = Toast.makeText(getApplicationContext(), "Frame width: " + resX + " height: " + resY + " fps: " + fps + " bitrate: "+ br + " segment size: " + segmentSize+ " server: " + uploadServer, Toast.LENGTH_LONG);
		toast.show();

		// Hide the window title.
		mediaRecorderLoop = new StreamingLoop("itec.camloop");
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

		mediaRecorderLoop.InitLoop();

		// init our httpd
		initHTTPD(8080, wwwRoot);

		deleteFiles(wwwRoot + "/live");
		mkdir(wwwRoot + "/live");    
		
		
	    fmain = new FrameLayout(this);
		LinearLayout ll = new LinearLayout(this);
		
		Button settingsButton = new Button(this);
		Button startButton = new Button(this);
		//test.setWidth(150);
		//test.setHeight(100);
		settingsButton.setText("Settings");
		startButton.setText("Start");
		ll.addView(settingsButton);
		ll.addView(startButton);
		settingsButton.setOnClickListener(this);
		startButton.setOnClickListener(this);
		fmain.addView(ll);
		ImageView myImage = new ImageView(this);
		myImage.setImageResource(R.drawable.iteclogo);
		fmain.addView(myImage, 0);
		setContentView(fmain);
		
	}

	public void onClick(View v) {
		Button b = (Button) v;
		if(b.getText().equals("Settings")) openMyMenu();
		else if(b.getText().equals("Start")){
			startEncoding();
			b.setText("Stop");
			return;
		}else if(b.getText().equals("Stop"))
		{
			stopEncoding();
			b.setText("Start");
		}
		
		
	}
	
	public void onOptiosMenuClosed(Menu menu)
	{
		openOptionsMenu();
	}
	
	
	public static void deleteFiles(String path) {

		File file = new File(path);

		if (file.exists()) {
			String deleteCmd = "rm -r " + path;
			Runtime runtime = Runtime.getRuntime();
			try {
				runtime.exec(deleteCmd);
			} catch (IOException e) { }
		}
	}
	private void initHTTPD(int port, String wwwroot)
	{
		initResources();
		try {
			dashServer = new DASHServer(port, wwwroot);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void mkdir(String directory)
	{
		String [] mkdirp = {"mkdir", directory};
		String [] chmod = {"chmod", "0777", directory};
		Process ps;
		try {
			ps = Runtime.getRuntime().exec(mkdirp);
			/*try {*/
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(ps.getInputStream()));
			//ps.waitFor();
			String line;
			while ((line = bufferedReader.readLine()) != null){

				Log.d("MDE_PS", line);
			}

			/*} catch (InterruptedException e) {
        	 	e.printStackTrace();
         	}*/
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}

	private void copyResourceFile(int rid, String targetFile) throws IOException {
		InputStream fin = ((Context)this).getResources().openRawResource(rid);
		FileOutputStream fos = new FileOutputStream(targetFile);  

		int     length;
		byte[] buffer = new byte[1024*32]; 
		while( (length = fin.read(buffer)) != -1){
			fos.write(buffer,0,length);
		}
		fin.close();
		fos.close();
	}

	private void initResources()
	{

		mkdir(wwwRoot);
		mkdir(wwwRoot+"/dash-js");
		try{

			copyResourceFile(R.raw.dashtest, wwwRoot + "/dashtest.html"  );
			copyResourceFile(R.raw.adaptationlogic, wwwRoot + "/dash-js/adaptationlogic.js"  );
			copyResourceFile(R.raw.mediasourcebuffer, wwwRoot + "/dash-js/mediaSourceBuffer.js"  );
			copyResourceFile(R.raw.bandwidth, wwwRoot + "/dash-js/bandwidth.js"  );
			copyResourceFile(R.raw.basebuffer, wwwRoot + "/dash-js/basebuffer.js"  );
			copyResourceFile(R.raw.buffer, wwwRoot + "/dash-js/buffer.js"  );
			copyResourceFile(R.raw.dash, wwwRoot + "/dash-js/dash.js"  );
			copyResourceFile(R.raw.dashplayervars, wwwRoot + "/dash-js/dashPlayerVars.js"  );
			copyResourceFile(R.raw.dashttp, wwwRoot + "/dash-js/DASHttp.js"  );
			copyResourceFile(R.raw.eventhandlers, wwwRoot + "/dash-js/eventHandlers.js"  );
			copyResourceFile(R.raw.fplot, wwwRoot + "/dash-js/fplot.js"  );
			copyResourceFile(R.raw.mediasourceapiadaptation, wwwRoot + "/dash-js/mediaSourceAPIAdaptation.js"  );
			copyResourceFile(R.raw.mpdparser, wwwRoot + "/dash-js/mpdParser.js"  );
			copyResourceFile(R.raw.rate_measurement, wwwRoot + "/dash-js/rate_measurement.js"  );
			copyResourceFile(R.raw.timebuffer, wwwRoot + "/dash-js/timeBuffer.js"  );

		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}

	public boolean updateSettings()
	{
		Pattern p;
		Matcher m;
		settings = getSharedPreferences("mobileDASHEncoder", 0);

		String resWH = settings.getString("pref_resolution", null);
		boolean allSettingsSet = true;
		if(resWH != null)
		{
			p = Pattern.compile("(\\d+)x(\\d+)");
			m = p.matcher((String) resWH); 
			m.find();
			resX = Integer.parseInt(m.group(1));
			resY = Integer.parseInt(m.group(2));
		}else{
			Toast toast = Toast.makeText(getApplicationContext(), "Please set the resolution under settings.", Toast.LENGTH_LONG);
			toast.show();
			resX = resY = 0;
			allSettingsSet = false;
		}

		String fpsS = settings.getString("pref_fps", null);

		if(fpsS != null)
		{
			p = Pattern.compile("(\\d+)[^\\d]+");
			m = p.matcher((String) fpsS);
			m.find();
			fps = Integer.parseInt(m.group(1));


		}else{
			Toast toast = Toast.makeText(getApplicationContext(), "Please set the frames per second under settings.", Toast.LENGTH_LONG);
			toast.show();
			fps = 0;
			allSettingsSet = false;
		}

		String bitrate = settings.getString("pref_bitrate", null);

		if(bitrate != null)
		{
			p = Pattern.compile("(\\d+)[^\\d]+");
			m = p.matcher((String) bitrate);
			br = (m.find() ? Integer.parseInt(m.group(1)) : 0);


		}else{
			Toast toast = Toast.makeText(getApplicationContext(), "Please set the bitrate under settings.", Toast.LENGTH_LONG);
			toast.show();
			br = 0;
			allSettingsSet = false;
		}

		String segSize = settings.getString("pref_segmentSize", null);

		if(segSize != null)
		{
			p = Pattern.compile("(\\d+)[^\\d]+");
			m = p.matcher((String) segSize);
			segmentSize = (m.find() ? Integer.parseInt(m.group(1)): 0);
		}else{
			Toast toast = Toast.makeText(getApplicationContext(), "Please set the segment size under settings.", Toast.LENGTH_LONG);
			toast.show();
			segmentSize = 0;
			allSettingsSet = false;
		}

		// uploadServer = settings.getString("pref_server", "none");
		uploadServer = "http://localhost";
		return allSettingsSet;
	}

	@Override
	protected void onResume() {
		super.onResume();
		DASHEncoderLog.debug("Resuming mde...");
	}

	@Override
	protected void onPause() {
		super.onPause();
		// wl.release();
	}


	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.dash_menu, menu);
		return true;
	}

	
	private void startEncoding()
	{
		if(updateSettings())
		{
			if(!enabledPreview)
			{
				enabledPreview = true;
				mPreview = new RecorderWithPreview(this.getBaseContext(), resX, resY, fps, br, 200, "/sdcard/setup.mp4");
				mPreview.setupCamera();
				fmain.removeViewAt(0);
				
				fmain.addView(mPreview, 0);
			

				baseContext = this.getBaseContext();

				// set the content view to the preview ...
				final Handler handler = new Handler();
				handler.postDelayed(new Runnable() {
					public void run() {
						mPreview.setupRecorder();
						mPreview.start();

						final Handler handler = new Handler();
						handler.postDelayed(new Runnable() {
							public void run() {
								mPreview.stop();
								fmain.removeView(mPreview);
								
								mPreview = new RecorderWithPreview(baseContext, resX, resY, fps, br, 0, mediaRecorderLoop.getSenderFileDescriptor());
								mPreview.setupCamera();
								
								//setContentView(mPreview);
								fmain.addView(mPreview,0);
								
								enabledPreview = true;
								final Handler handler = new Handler();
								handler.postDelayed(new Runnable() {
									public void run() {
										mkdir("/sdcard/dash/live");
										NativeInterface.NativeStartTranscoding("/sdcard/setup.mp4", "/sdcard/dash/live/live", mediaRecorderLoop.getReceiverFileDescriptor(), br, fps, segmentSize);
										mPreview.setupRecorder();
										mPreview.start();
									}                   			  
								}, 500);
							}
						}, 1000);
					}
				}, 200);
				// prepare everything for recording 
			}        		
		}
	}
	
	private void openMyMenu()
	{
		Intent intent = new Intent(this, itec.android.DASH.DASHEncoder.PreferenceActivity.class);
		startActivityForResult(intent,0);
	}
	
	private void stopEncoding()
	{
		if(mPreview != null){

			mPreview.stop();
			//NativeInterface.NativeStopTranscoding();
			mediaRecorderLoop.ReleaseLoop();
		}
		else
		{
			Toast msg = Toast.makeText(getApplicationContext(), "Recorder not running...", Toast.LENGTH_SHORT);
			msg.show();
		}
	}
	
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.btLiveSettings:

			openMyMenu();

			return true;

		case R.id.btLiveStart:
			// do the check if every option is set correctly
			startEncoding();
			return true;

		case R.id.btLiveStop:

			stopEncoding();
			return true;
		default:
			return super.onOptionsItemSelected(item);
		}
	}

	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		// User has changed quality
		if (resultCode==1000) {

			updateSettings();
			Toast toast = Toast.makeText(getApplicationContext(), "Frame width: " + resX + " height: " + resY + " fps: " + fps + " bitrate: "+ br + " segment size: " + segmentSize+ " server: " + uploadServer, Toast.LENGTH_LONG);
			toast.show();
		}

	}

	public void onBackPressed(){
		if(mPreview != null){
			mPreview.stop();
			//NativeInterface.NativeStopTranscoding();
			mediaRecorderLoop.ReleaseLoop();
		}
		else
		{
			Toast msg = Toast.makeText(getApplicationContext(), "Recorder not running...", Toast.LENGTH_SHORT);
			msg.show();
		} 
	
		super.onBackPressed();
	}

	public void log(String s) {
		// do real logging...
		Log.println(Log.DEBUG, "DEBUG", s);
	}


	

}