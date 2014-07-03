package itec.android.DASH.DASHEncoder;

import android.util.Log;


/*
 * Logging class for mobile DASHEncoder
 * TODO: Should be extended to save logs and sens them to the app authors (for bug fixing...)
 */
public class DASHEncoderLog {

	
	static void error(String tag, String s)
	{
		Log.e(tag, s);	
	}
	
	static void debug(String tag, String s)
	{
		Log.d(tag, s);	
	}
	
	static void error(String s)
	{
		Log.e("DEFAULT", s);
	}
	
	static void debug(String s)
	{
		Log.d("DEFAULT", s);		
	}
}
