#include "cpp_requests.h"

int main() {
	CppRequests r = CppRequests();

	r.get("http://google.com/");
}