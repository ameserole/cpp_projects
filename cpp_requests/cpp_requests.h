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

	string getText();
	string getStatus();
	string getUrl();
	string getJson();
	string getRaw();
	string getCookies();
	string getHistory();
	string getHeaders();
};

class CppRequests {
private:
	map<string,string> requestHeaders;
	map<string,string> requestCookies;

public:
	CppRequests();
	//~CppRequests();

	RequestObj get(string url);
	RequestObj post(string url, string data);
	RequestObj head(string url);
	RequestObj put(string url, string data);
	RequestObj options(string url);

	void setHeader(map<string,string> header);
	void setCookie(map<string,string> cookie);

};
