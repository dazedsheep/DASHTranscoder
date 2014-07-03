package itec.android.DASH.DASHEncoder;


import java.util.List;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.hardware.Camera;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.Preference.OnPreferenceClickListener;
import android.preference.PreferenceFragment;
import android.util.Log;

@SuppressLint("ValidFragment")
@TargetApi(11)
public class Preferences extends PreferenceFragment {

	private List<Camera.Size> supportedWH;
	private String [] resolutions;    
	private String [] fps;
	private String [] bitrate;
	private Context ctx;
	
	@SuppressLint("ValidFragment")
	public Preferences(Context ctx)
	{
		this.ctx = ctx;
	}
	
	public void onCreate(Bundle savedInstanceState)
	{		
		super.onCreate(savedInstanceState);
		addPreferencesFromResource(R.xml.dash_preferences);
		
		ListPreference lpref = (ListPreference)findPreference("pref_resolution");
		List<int[]> supportedFps;
		Camera cam = Camera.open();
	        
	    supportedWH = cam.getParameters().getSupportedVideoSizes();
	    supportedFps = cam.getParameters().getSupportedPreviewFpsRange();
	    // fill our options ..
	    if( supportedWH != null) 
	    {
	    	resolutions = new String[supportedWH.size()];
	        
	      	for(int i=0;i<supportedWH.size(); i++) resolutions[i] = supportedWH.get(i).width + "x" + supportedWH.get(i).height;
	    }else{
	    	
	    	// default resolutions if something goes wrong...
	    	resolutions = new String[] {
	        		"640x480",
	        		"320x240",
	        		"176x144"  		
	        };    	
	    	
	    }
	     
	     lpref.setEntries(resolutions);
	     lpref.setEntryValues(resolutions);
	     lpref.setOnPreferenceChangeListener(new myPrefChangeListener(ctx));
	
		 // frame rate
		 
	     lpref = (ListPreference)findPreference("pref_fps");
	     
	     if( supportedFps != null)
	     {
	    	 if(supportedFps.size() > 1)
	    	 {
	    		 int fps_fixed = 0;
		    	 // the paramters form the camera are returned in ranges min-max (if min=max => fixed frame rate) and we only want fixed frame rates!
	    		 for(int i = 0; i < supportedFps.size(); i++)
		    	 {
		    		 if(supportedFps.get(i)[0] == supportedFps.get(i)[1]) fps_fixed++;
		    	 }
	    		 
	    		 fps_fixed = supportedFps.size();
	    		 
	    		 fps = new String[fps_fixed];
		    	 
		    	 for(int i = 0; i < fps_fixed; i++)
		    	 {
		    		// if(supportedFps.get(i)[0] == supportedFps.get(i)[1])
		    			 fps[i] = Float.toString((float)(supportedFps.get(i)[cam.getParameters().PREVIEW_FPS_MAX_INDEX] / 1000)) + " fps";
		    	 }
	    	 }else{
	    		 fps = new String[] {
	 		     		"20 fps",
	 		    		"15 fps",
	 		    		"10 fps",
	 		    		"4 fps"
	 		     };
	    	 }
	    	 
	     }else{
		     fps = new String[] {
		     		"20 fps",
		    		"15 fps",
		    		"10 fps",
		    		"4 fps"
		     };
	     }
	     cam.release();
	     lpref.setEntries(fps);
	     lpref.setEntryValues(fps);
	     lpref.setOnPreferenceChangeListener(new myPrefChangeListener(ctx));
		// now the bitrate
	     
	     lpref = (ListPreference)findPreference("pref_bitrate");
	     
	     bitrate = new String[] {
	     		"Default",
	     		"8000 kb/s",
	     		"6000 kb/s",
	     		"4000 kb/s",
	     		"3000 kb/s",
	     		"2000 kb/s",
	    		"1000 kb/s",
	    		"500 kb/s",
	    		"100 kb/s",
	    		"50 kb/s",
	    		"10 kb/s"
	    };
	     
	    lpref.setEntries(bitrate);
	    lpref.setEntryValues(bitrate);
	    lpref.setOnPreferenceChangeListener(new myPrefChangeListener(ctx));
	    
	    lpref = (ListPreference)findPreference("pref_segmentSize");
	     
	     bitrate = new String[] {
	    		"2 seconds",
	    		"4 seconds",
	    		"6 seconds",
	    		"8 seconds",
	    		"10 seconds"
	    };
	     
	    lpref.setEntries(bitrate);
	    lpref.setEntryValues(bitrate);
	    lpref.setOnPreferenceChangeListener(new myPrefChangeListener(ctx));
	
	    /*EditTextPreference tpref = (EditTextPreference)findPreference("pref_server");
	    
	    tpref.setOnPreferenceChangeListener(new myPrefChangeListener(ctx)); */
	}
	
	public class myPrefChangeListener implements OnPreferenceChangeListener{

		private Context ctx;
		
		public myPrefChangeListener(Context ctx)
		{
			this.ctx = ctx;
		}
		
		public boolean onPreferenceChange(Preference myPref, Object value) {
		
			SharedPreferences settings = ctx.getSharedPreferences("mobileDASHEncoder", 0);
			Editor editor = settings.edit();
			editor.putString(myPref.getKey(), (String) value);
			editor.commit();
			Log.d("prefs", "setting: "+ myPref.getKey() + " value: " + (String) value);
			return true;
		}
	}
	
	
}
