package itec.android.DASH.DASHEncoder;

import java.io.FileDescriptor;

import android.util.Log;

public class NativeInterface {

	
	public NativeInterface() {    
    }
      
	static private native int dumbStreamToFile(String fileName, FileDescriptor in);
	static public void NativeDumbStreamToFile(String fileName, FileDescriptor in) {
		dumbStreamToFile(fileName, in);
    }
	
    static private native int startNativeMobileDashEncoder(String setupFileName, String baseName, FileDescriptor in, int bitrate, int fps, int segmentLength);
    static public void NativeStartTranscoding(String setupFileName, String baseName, FileDescriptor in, int bitrate, int fps, int segmentLength) {
    	startNativeMobileDashEncoder(setupFileName, baseName, in, bitrate, fps, segmentLength);
    }
    
    static private native void stopNativeMobileDashEncoder();
    static public void NativeStopTranscoding() {
    	stopNativeMobileDashEncoder();
    }
	
	public static void LoadLibraries() {
        //Local library .so files before this activity created.
		System.loadLibrary("netutils");
        System.loadLibrary("mobileDASHEncoder");		
        System.loadLibrary("nativehook");		
    }
}
