#include "webserver.hpp"
#include <csignal>
#include <fstream>

// g++ main.cpp webserver.cpp -o main -lTWebServer -lstdc++fs

WebServer server;

void end_signal(int signal) {
	std::cout << "signal: " << signal << std::endl;
	server.stop(signal);
}


int main() {
	std::signal(SIGINT, end_signal);
	std::signal(SIGTERM, end_signal);

	// For an http web server
	server.config_server(9000, -1);
		
	server.start(false);

	return 0;
}
