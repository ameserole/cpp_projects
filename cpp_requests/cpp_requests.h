#include "../cpp_client/cpp_client.h"
#include <map>
#include <regex>
#include <sstream>

using namespace std;

class RequestObj {
private:
	string text;
	string status;
	string url;
	string json;
	string raw;
	map<string,string> responseCookies;
	map<string,string> responseHeaders;
	string history;

	friend class CppRequests;

public:
	//RequestObj();
	//~RequestObj();

	string getText() { return text; }
	string getStatus() { return status; }
	string getUrl() { return url; }
	string getJson() { return json; }
	string getRaw() { return raw; }
	string getHistory() { return history; }
	map<string, string> getCookies() { return responseCookies; }
	map<string,string> getHeaders() { return responseHeaders; }
};

class CppRequests {
private:
	map<string,string> requestHeaders;
	map<string,string> requestCookies;

	map<string, string> parseUrl(string url);
	RequestObj parseResponse(string response);
	string createRequest(string requestLine, map<string,string> requestHeaders, map<string,string> requestCookies);

public:
	CppRequests();
	//~CppRequests();

	RequestObj get(string url);
	RequestObj post(string url, map<string,string> data);
	RequestObj post(string url, string data);
	RequestObj head(string url);
	RequestObj put(string url, string data);
	RequestObj options(string url);

	void setHeader(map<string,string> header);
	void setCookie(map<string,string> cookie);

};
