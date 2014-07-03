package itec.android.DASH.DASHEncoder;

import java.io.File;
import java.io.IOException;
import java.net.URLDecoder;
import java.util.Locale;

import org.apache.http.HttpEntity;
import org.apache.http.HttpEntityEnclosingRequest;
import org.apache.http.HttpException;
import org.apache.http.HttpRequest;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.MethodNotSupportedException;
import org.apache.http.entity.FileEntity;
import org.apache.http.entity.StringEntity;
import org.apache.http.protocol.HttpContext;
import org.apache.http.protocol.HttpRequestHandler;
import org.apache.http.util.EntityUtils;

public class HttpFileHandler implements HttpRequestHandler {

	private final String docRoot;

	public HttpFileHandler(final String docRoot) {
		super();
		this.docRoot = docRoot;
	}

	@SuppressWarnings("deprecation")
	public void handle(final HttpRequest request, final HttpResponse response, final HttpContext context) throws HttpException, IOException {

		String method = request.getRequestLine().getMethod().toUpperCase(Locale.ENGLISH);
		if (!method.equals("GET") && !method.equals("HEAD") && !method.equals("POST")) {
			throw new MethodNotSupportedException(method + " method not supported");
		}
		String target = request.getRequestLine().getUri();

		if (request instanceof HttpEntityEnclosingRequest) {
			HttpEntity entity = ((HttpEntityEnclosingRequest) request).getEntity();
			byte[] entityContent = EntityUtils.toByteArray(entity);
			System.out.println("Incoming entity content (bytes): " + entityContent.length);
		}

		final File file = new File(this.docRoot, URLDecoder.decode(target, "UTF-8"));
		
		if (!file.exists()) {
			
			response.setStatusCode(HttpStatus.SC_NOT_FOUND);
			StringEntity entity = new StringEntity("<html><body><h1>File" + file.getPath() + " not found</h1></body></html>", "UTF-8");
			entity.setContentType("text/html");
			response.setEntity(entity);
			System.out.println("File " + file.getPath() + " not found");

		} else if (!file.canRead() || file.isDirectory()) {

			response.setStatusCode(HttpStatus.SC_FORBIDDEN);
			StringEntity entity = new StringEntity("<html><body><h1>Access denied</h1></body></html>", "UTF-8");
			entity.setContentType("text/html");
			response.setEntity(entity);
			System.out.println("Cannot read file " + file.getPath());

		} else {

			response.setStatusCode(HttpStatus.SC_OK);
			FileEntity body = new FileEntity(file, "text/xml");

			response.setEntity(body);
			System.out.println("Serving file " + file.getPath());
		}
	}

}