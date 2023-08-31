#include "webserver.hpp"
#include <fstream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <ctime>

#define DEFAULT_HIBERNATION 1
#define DEFAULT_REVEIL 7

		
WebServer::WebServer() : WebServerHttp() {
	startTime = DEFAULT_HIBERNATION;
	endTime   = DEFAULT_REVEIL;
	status    = 2;
}

std::string nbrToStr(int nbr) {
	if (nbr < 10) {
		return "0" + std::to_string(nbr);
	} else {
		return std::to_string(nbr);
	}
}

std::string WebServer::getContent(std::string &url, std::string &type, char *buffer, int bufferSize, const Parameters &params) {
	std::string content;

	if (url == "/") {
		content = open_file("index.html");
	} else if (url == "/gettime") {
		if (status == 2) {
			content = nbrToStr(startTime) + nbrToStr(endTime);
		} else {
			content = "-1-1";
		}
	} else if (url == "/process.html") {
		if (params.find("hibernation") == params.end() || params.find("reveil") == params.end()) {
			content = open_file("incorrect.html");
		} else {
			int tmp_startTime, tmp_endTime;

			for (const auto &key : params) {
				if (key.first == "hibernation") {
					tmp_startTime = std::stoi(key.second.substr(0, 2));
				} else if (key.first == "reveil") {
					tmp_endTime = std::stoi(key.second.substr(0, 2));
				}
			}

			std::cout << tmp_startTime << " " << tmp_endTime << "\n";

			if (tmp_startTime < 12 && tmp_startTime >= tmp_endTime
			 || tmp_startTime > 12 && tmp_startTime <= tmp_endTime) {
				content = open_file("incorrect.html");
			} else {
				startTime = tmp_startTime;
				endTime   = tmp_endTime;
				content = open_file("index.html");
			}
		}
	} else {
		content = open_file(url.substr(1));
	}

	return content;
}

void WebServer::start(bool activeAsync) {
	int new_socket;
	is_alive = true;

	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	int res = bind(server_fd, (sockaddr*)&addr, sizeof(addr));

	if (res < 0) {
		std::cerr << "bind failed: " << res << std::endl;
		return;
	}


	res = listen(server_fd, -1);
	int addrlen = sizeof(addr);

	if (res < 0) {
		std::cerr << "listen error" << std::endl;
		return;
	}

	std::vector<std::future<void>> tasks;
	while (is_alive && (new_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addrlen))) {
		if (new_socket > 0) {
			struct in_addr ipAddr = addr.sin_addr;
			char str[INET_ADDRSTRLEN];

			const std::string ipClient = inet_ntoa(addr.sin_addr);

			try {
				new_http_request(new_socket, ipClient);
			} catch (const std::exception& e) {
				std::cerr << "Error during answer process: " << e.what() << std::endl;
			}
		}

		std::time_t currentTime = std::time(nullptr);
		std::tm* timeinfo = std::localtime(&currentTime);

		if (status == 2) {
			if (timeinfo->tm_hour == startTime) {
				status = 1;
				if (startTime > endTime) {
					startTime -= 24;
				}
				std::string cseconds = std::to_string((endTime - startTime)*3600);
				execlp("/usr/sbin/rtcwake", "-m", "disk", "-s", cseconds.c_str(), nullptr);
			}
		} else if (timeinfo->tm_hour == 10) {
			status = 2;
			startTime = DEFAULT_HIBERNATION;
			endTime   = DEFAULT_REVEIL;
		}
	}
}
