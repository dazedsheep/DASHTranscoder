package itec.android.DASH.DASHEncoder;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

public class PreferenceActivity extends Activity {
	
	 protected void onCreate(Bundle savedInstanceState){
	        super.onCreate(savedInstanceState);
	
	        getFragmentManager().beginTransaction().replace(android.R.id.content, new Preferences(this.getBaseContext())).commit();
	    	
	 }
	 
	 @Override
	 public void onBackPressed()
	 {
		setResult(1000);
		finish();
		 
	 }

}
