#include "cpp_requests.h"

CppRequests::CppRequests() {
	requestHeaders["User-Agent"] = "CppRequests-0.1";
	requestHeaders["Accept"] = "*/*";
}

map<string, string> CppRequests::parseUrl(string url) {
	// https://stackoverflow.com/questions/5620235/cpp-regular-expression-to-validate-url
	regex url_regex (
    	R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
    	regex::extended
  	);

	smatch url_match_result;
	regex_match(url, url_match_result, url_regex);

	string host = url_match_result.str(4);
	string path = url_match_result.str(5);
	string port = "80";

	if(host.find(":") != string::npos) {
		port = host.substr(host.find(":") + 1, string::npos);
		host.erase(host.find(":"));
	}

	map<string, string> resp;
	resp["host"] = host;
	resp["path"] = path;
	resp["port"] = port;

	return resp;
}

RequestObj CppRequests::parseResponse(string response) {
	RequestObj parsedResponse;

	parsedResponse.raw = response;

	istringstream fullResponse(response);
    string line;  

    // Get Status only
    getline(fullResponse, line);
    parsedResponse.status = line.substr(line.find(" ") + 1, string::npos);

    getline(fullResponse, line);
    while (line.find(":") != string::npos) { 	
        parsedResponse.responseHeaders[line.substr(0, line.find(":"))] = line.substr(line.find(":")+2, string::npos);

        if(line.substr(0, 11) == "Set-Cookie:") {
        	string name = line.substr(12, (line.find("=") - 12));
        	string value = line.substr(line.find("=")+1, (line.find(";") - (line.find("=")+1)));
        	parsedResponse.responseCookies[name] = value;
        }

        getline(fullResponse, line);
    }

    getline(fullResponse, line);
    parsedResponse.text = "";
    while(getline(fullResponse, line)) {
    	parsedResponse.text += line + "\n";
    }

    return parsedResponse;
}

string CppRequests::createRequest(string requestLine, map<string,string> requestHeaders, map<string,string> requestCookies) {
	stringstream fullRequest;

	fullRequest << requestLine << "\r\n";
	for(auto const& header: requestHeaders) {
		fullRequest << header.first << ": " << header.second << "\r\n";
	}

	fullRequest << "\r\n";

	return fullRequest.str();
}

RequestObj CppRequests::get(string url) {
	map<string, string> urlInfo = parseUrl(url);
	string host = urlInfo["host"];
	string path = urlInfo["path"];
	string port = urlInfo["port"];

	if(path == "")
		path = "/";

	requestHeaders["Host"] = host;

	cppClient client(host, stoi(port));

	string requestLine = "GET " + path + " HTTP/1.1";
	client.send(createRequest(requestLine, requestHeaders, requestCookies));
	string response = client.recv();
	
	RequestObj parsedResponse = parseResponse(response);
	parsedResponse.url = url;

	while(parsedResponse.status.find("301") != string::npos)
		parsedResponse = get(parsedResponse.responseHeaders["Location"]);

	return parsedResponse;
}

RequestObj CppRequests::post(string url, map<string,string> data) {
	map<string, string> urlInfo = parseUrl(url);
	string host = urlInfo["host"];
	string path = urlInfo["path"];
	string port = urlInfo["port"];

	if(path == "")
		path = "/";

	string encodedData = "";
	for(auto var: data) {
		encodedData += var.first + "=" + var.second + "&";
	}
	encodedData.pop_back();

	requestHeaders["Host"] = host;
	requestHeaders["Content"] = "application/x-www-form-urlencoded";
	requestHeaders["Content-Length"] = encodedData.length();

	cppClient client(host, stoi(port));
	string requestLine = "POST " + path + " HTTP/1.1";
	string fullRequest = createRequest(requestLine, requestHeaders, requestCookies);
	fullRequest += encodedData + "\r\n";
	cout << "Full Request: \n" << fullRequest << endl;
	client.send(fullRequest);
	string response = client.recv();
	
	RequestObj parsedResponse = parseResponse(response);
	parsedResponse.url = url;

	while(parsedResponse.status.find("301") != string::npos)
		parsedResponse = get(parsedResponse.responseHeaders["Location"]);

	return parsedResponse;
}

RequestObj CppRequests::post(string url, string data) {
	map<string, string> urlInfo = parseUrl(url);
	string host = urlInfo["host"];
	string path = urlInfo["path"];
	string port = urlInfo["port"];

	if(path == "")
		path = "/";

	requestHeaders["Host"] = host;
	requestHeaders["Content"] = "application/x-www-form-urlencoded";
	requestHeaders["Content-Length"] = to_string(data.length());

	cppClient client(host, stoi(port));
	string requestLine = "POST " + path + " HTTP/1.1";
	string fullRequest = createRequest(requestLine, requestHeaders, requestCookies);
	fullRequest += data + "\r\n";
	cout << "Full Request: \n" << fullRequest << endl;

	client.send(fullRequest);
	string response = client.recv();
	
	RequestObj parsedResponse = parseResponse(response);
	parsedResponse.url = url;

	while(parsedResponse.status.find("301") != string::npos)
		parsedResponse = get(parsedResponse.responseHeaders["Location"]);

	return parsedResponse;
}

RequestObj CppRequests::head(string url) {
	map<string, string> urlInfo = parseUrl(url);
	string host = urlInfo["host"];
	string path = urlInfo["path"];
	string port = urlInfo["port"];

	if(path == "")
		path = "/";

	requestHeaders["Host"] = host;
	cppClient client(host, stoi(port));
	stringstream fullRequest;

	string requestLine =  "HEAD " + path + " HTTP/1.1";
	client.send(createRequest(requestLine, requestHeaders, requestCookies));
	string response = client.recv();
	
	RequestObj parsedResponse = parseResponse(response);
	parsedResponse.url = url;

	while(parsedResponse.status.find("301") != string::npos)
		parsedResponse = get(parsedResponse.responseHeaders["Location"]);

	return parsedResponse;
}

RequestObj CppRequests::options(string url) {
		map<string, string> urlInfo = parseUrl(url);
	string host = urlInfo["host"];
	string path = urlInfo["path"];
	string port = urlInfo["port"];

	if(path == "")
		path = "/";

	requestHeaders["Host"] = host;
	cppClient client(host, stoi(port));
	stringstream fullRequest;

	string requestLine =  "OPTIONS " + path + " HTTP/1.1";
	client.send(createRequest(requestLine, requestHeaders, requestCookies));
	string response = client.recv();
	
	RequestObj parsedResponse = parseResponse(response);
	parsedResponse.url = url;

	while(parsedResponse.status.find("301") != string::npos)
		parsedResponse = get(parsedResponse.responseHeaders["Location"]);

	return parsedResponse;
}
