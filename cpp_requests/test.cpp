#include "cpp_requests.h"

int main() {
	CppRequests r = CppRequests();
	map<string,string> test;
	test["test"] = "data";
	
	auto response = r.post("http://ptsv2.com/t/asdf/edit", "AuthUsername=asdf&AuthPassword=asdf&ResponseCode=200&ResponseBody=asdf&ResponseDelay=0");
	cout << response.getRaw() << endl;

}