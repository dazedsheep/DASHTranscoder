package itec.android.DASH.DASHEncoder;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.ServerSocket;
import java.net.Socket;

import org.apache.http.ConnectionClosedException;
import org.apache.http.HttpException;
import org.apache.http.HttpResponseInterceptor;
import org.apache.http.HttpServerConnection;
import org.apache.http.impl.DefaultConnectionReuseStrategy;
import org.apache.http.impl.DefaultHttpResponseFactory;
import org.apache.http.impl.DefaultHttpServerConnection;
import org.apache.http.params.CoreConnectionPNames;
import org.apache.http.params.CoreProtocolPNames;
import org.apache.http.params.HttpParams;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;
import org.apache.http.protocol.HttpProcessor;
import org.apache.http.protocol.HttpRequestHandler;
import org.apache.http.protocol.HttpRequestHandlerRegistry;
import org.apache.http.protocol.HttpService;
import org.apache.http.protocol.BasicHttpProcessor;
import org.apache.http.protocol.ResponseConnControl;
import org.apache.http.protocol.ResponseContent;
import org.apache.http.protocol.ResponseDate;
import org.apache.http.protocol.ResponseServer;

public class HTTPServer extends Thread {

	private final ServerSocket serverSocket;

	private final HttpParams httpParams;

	private final HttpService httpService;

	private final HttpRequestHandlerRegistry registry;
	
	public static int DEFAULT_PORT = 8080;
	
	private int port;

	public HTTPServer(int port) throws IOException {
		this.port = port;
		serverSocket = new ServerSocket(port);

		httpParams = new BasicHttpParams();
		httpParams
				.setIntParameter(CoreConnectionPNames.SO_TIMEOUT, 5000)
				.setIntParameter(CoreConnectionPNames.SOCKET_BUFFER_SIZE,
						8 * 1024)
				.setBooleanParameter(
						CoreConnectionPNames.STALE_CONNECTION_CHECK, false)
				.setBooleanParameter(CoreConnectionPNames.TCP_NODELAY, true)
				.setParameter(CoreProtocolPNames.ORIGIN_SERVER,
						"HttpComponents/1.1");

		// Set up the HTTP protocol processor
		BasicHttpProcessor httpProcessor = new BasicHttpProcessor();
		httpProcessor.addInterceptor(new ResponseDate());
		httpProcessor.addInterceptor(new ResponseServer());
		httpProcessor.addInterceptor(new ResponseContent());
		httpProcessor.addInterceptor(new ResponseConnControl());
	

		// Set up request handlers
		registry = new HttpRequestHandlerRegistry();

		// Set up the HTTP service
		httpService = new HttpService(httpProcessor,
				new DefaultConnectionReuseStrategy(),
				new DefaultHttpResponseFactory());
		
		httpService.setParams(httpParams);
		httpService.setHandlerResolver(registry);
		
	}

	public void addService(String urlPattern, HttpRequestHandler service) {
		registry.register(urlPattern, service);
	}

	@Override
	public void run() {
		System.out.println("Running http server on port " + port + ".");
		while (!Thread.interrupted()) {
			try {
				// Set up HTTP connection
				Socket socket = serverSocket.accept();
				DefaultHttpServerConnection connection = new DefaultHttpServerConnection();
				System.out.println("Incoming connection from "
						+ socket.getInetAddress());
				connection.bind(socket, httpParams);

				// Start worker thread
				Thread t = new WorkerThread(this.httpService, connection);
				t.setDaemon(true);
				t.start();
			} catch (InterruptedIOException ex) {
				break;
			} catch (IOException e) {
				System.err.println("I/O error initialising connection thread: "
						+ e.getMessage());
				break;
			}
		}
	}
	
    private static class WorkerThread extends Thread {

        private final HttpService httpService;
        private final HttpServerConnection connection;

        public WorkerThread(
                final HttpService httpService,
                final HttpServerConnection connection) {
            super();
            this.httpService = httpService;
            this.connection = connection;
        }

        @Override
        public void run() {
            HttpContext context = new BasicHttpContext(null);
            try {
                while (!Thread.interrupted() && this.connection.isOpen()) {
                    this.httpService.handleRequest(this.connection, context);
                }
            } catch (ConnectionClosedException ex) {
                System.err.println("Client closed connection");
            } catch (IOException ex) {
                System.err.println("I/O error: " + ex.getMessage());
            } catch (HttpException ex) {
                System.err.println("Unrecoverable HTTP protocol violation: " + ex.getMessage());
            } finally {
                try {
                    this.connection.shutdown();
                } catch (IOException ignore) {}
            }
        }
    }
    
    public static HTTPServer startHTTPServer(int port, String wwwRoot)
    {
    	try {
    		HTTPServer serviceProvider = new HTTPServer(port);
    		
    		HttpFileHandler fileHandler = new HttpFileHandler(wwwRoot);
    		serviceProvider.addService("*", fileHandler);    		    		
    		serviceProvider.start();
    		return serviceProvider;
    		
    	} catch (Exception e) {
    		e.printStackTrace();
   
    	}
    	return null;
    }
    
   
}
