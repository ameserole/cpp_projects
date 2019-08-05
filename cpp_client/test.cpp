#include "cpp_client.h"
#include <iostream>

int main() {
	cppClient client("127.0.0.1", 9000);

	client.send("GET /raw/jeMpNv9A HTTP/1.0\n\n");
	std::cout << client.recv() << std::endl;
}