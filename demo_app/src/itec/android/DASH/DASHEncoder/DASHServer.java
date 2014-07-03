package itec.android.DASH.DASHEncoder;

import java.io.File;
import java.io.IOException;

public class DASHServer{
	
	public static HTTPServer myServer;
	
	public DASHServer(int port, String wwwroot) throws IOException {
		//super(port, new File( wwwroot).getAbsoluteFile() );
		
		try{
			myServer = HTTPServer.startHTTPServer(port, wwwroot);
		}catch(Exception e)
		{
			System.out.println("DASHServer already running");
		}
	}
}
