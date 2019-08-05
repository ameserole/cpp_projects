#include "cpp_requests.h"

CppRequests::CppRequests() {
	requestHeaders["User-Agent"] = "CppRequests-0.1";
	requestHeaders["Accept"] = "text/html";
}

RequestObj CppRequests::get(string url) {

	// https://stackoverflow.com/questions/5620235/cpp-regular-expression-to-validate-url
	regex url_regex (
    	R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
    	regex::extended
  	);

	smatch url_match_result;
	regex_match(url, url_match_result, url_regex);

	string host = url_match_result.str(4);
	string path = url_match_result.str(5);
	//cout << "Host: " << host << " Path: " << path << endl;

	requestHeaders["Host"] = host;

	cppClient client(host, 80);
	stringstream fullRequest;

	fullRequest << "GET " << path << " HTTP/1.1\n";
	for(auto const& header: requestHeaders) {
		fullRequest << header.first << ": " << header.second << "\n";
	}

	fullRequest << "\n";

	//cout << "Full Request:\n" << fullRequest.str() << endl;

	client.send(fullRequest.str());
	string response = client.recv();

	//cout << "Response:\n" << response << endl;
	
	RequestObj respObj;
	return respObj;
}