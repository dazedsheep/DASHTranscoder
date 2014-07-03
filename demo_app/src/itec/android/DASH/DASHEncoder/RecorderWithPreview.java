package itec.android.DASH.DASHEncoder;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.List;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Size;
import android.media.CamcorderProfile;
import android.media.MediaRecorder;
import android.media.MediaRecorder.OnInfoListener;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;


public class RecorderWithPreview extends SurfaceView implements SurfaceHolder.Callback {


	public static int DEFAULT_WIDTH = 640;
	public static int DEFAULT_HEIGHT = 480;
	public static int DEFAULT_FPS = 25;
	public static int DEFAULT_BPS = 1000;

	private final String TAG = "RecorderWithPreview";

	SurfaceView mSurfaceView;
	SurfaceHolder mHolder;
	Size mPreviewSize;
	List<Size> mSupportedPreviewSizes;
	Camera mCamera;
	int numberOfCameras;
	int defaultCameraId;


	private int width, height, fps, bps;
	private MediaRecorder mr,mrtest;
	private boolean recording = false;
	private File dir;
	private String filename;
	private FileDescriptor fd;
	private int duration;

	public RecorderWithPreview(Context context, int width, int height, int fps, int bitrate, int maxDuration, FileDescriptor fd) {
		super(context);

		this.fd = fd;
		this.filename = null;
		init(width,height,fps,bitrate,maxDuration);

	}

	public RecorderWithPreview(Context context, int width, int height, int fps, int bitrate, int maxDuration, String file) {
		super(context);

		this.filename = file;
		this.fd = null;
		init(width,height,fps,bitrate,maxDuration);
	}

	private void init(int width, int height, int fps, int bitrate, int maxDuration)
	{
		mHolder = getHolder();
		mHolder.addCallback(this);
		mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

		this.width = width;
		this.height = height;
		this.fps = fps;
		this.bps = bitrate;	
		this.duration = maxDuration;

	}

	public void setupCamera()
	{
		mCamera = Camera.open();

		/*mCamera.stopPreview();

		try {
			mCamera.setPreviewDisplay(mHolder);

		} catch (IOException e1) {
			e1.printStackTrace();
		}	*/
	}

	public void setupRecorder()
	{
		if (recording) return;

		this.mr = new MediaRecorder();

		mCamera.unlock();

		// 1. set camera to mediaRecorder
		mr.setCamera(mCamera);

		// 2. set our sources
		mr.setVideoSource(MediaRecorder.VideoSource.CAMERA);

		// 3. set the profile/codec and container

		mr.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);

		mr.setVideoEncoder(MediaRecorder.VideoEncoder.H264);

		mr.setVideoFrameRate(fps);

		mr.setVideoSize(width, height);

		mr.setVideoEncodingBitRate(bps*1024);

		if(duration > 0) mr.setMaxDuration(duration);
		else
			mr.setMaxDuration(-1);

		// 4. output file, in our case a file descriptor

		if(filename != null) mr.setOutputFile(filename);
		else
			mr.setOutputFile(fd);

		// 5. set the preview

		mr.setPreviewDisplay(mHolder.getSurface());

		try {
			mr.prepare();
		} catch (IOException e) {

			DASHEncoderLog.error(TAG, "An error occured at preparing the MediaRecorder, find below a stacktrace!");
			e.printStackTrace();
			return;
		}
	}

	public void surfaceCreated(SurfaceHolder holder) 
	{
	}

	public void start()
	{
		if(recording) return;

		try {
			mr.start(); 
			recording = true;
			Log.e(DASHEncoder.TAG,"Started recording...");
		} catch (RuntimeException e) {
			DASHEncoderLog.error(TAG, "Cannot start recording, find below a stacktrace!");
			e.printStackTrace();
		}
	}

	public void stop()
	{
		if(!recording) return;
		recording = false;

		try {

			mr.stop(); 
			mr.reset();
			mr.release();
			mCamera.stopPreview();
			//mCamera.lock();
			mCamera.release();
		}
		catch (IllegalStateException e) {
			DASHEncoderLog.error(TAG,"MediaRecorder could not be stopped! - Aborting!");
			e.printStackTrace();
			mr.reset();
			return;
		}		
	}



	public void surfaceDestroyed(SurfaceHolder holder) {

	}

	private Camera.Size getBestPreviewSize(int width, int height,
			Camera.Parameters parameters) {
		Camera.Size result=null;

		for (Camera.Size size : parameters.getSupportedPreviewSizes()) {
			if (size.width<=width && size.height<=height) {
				if (result==null) {
					result=size;
				}
				else {
					int resultArea=result.width*result.height;
					int newArea=size.width*size.height;

					if (newArea>resultArea) {
						result=size;
					}
				}
			}
		}

		return(result);
	}
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

		if (mCamera != null && mr == null) {
			Log.d("MDE", "surfaceChanged: width " + w + ", height " + h);
	

			try {
				mCamera.setPreviewDisplay(mHolder);
			} catch ( Exception ex) {
				ex.printStackTrace(); 
			}

			Camera.Parameters parameters= mCamera.getParameters();
			Camera.Size size= getBestPreviewSize(width, height,
					parameters);
			
			if (size!=null) {
				parameters.setPreviewSize(size.width, size.height);
				mCamera.setParameters(parameters);
			}

			mCamera.startPreview();
		}
	}	

	public void draw(Canvas canvas) {
		super.draw(canvas);
		Paint p= new Paint(Color.RED);
		Log.d(TAG,"draw");
		canvas.drawText("PREVIEW", canvas.getWidth()/2, canvas.getHeight()/2, p );
	}
}
