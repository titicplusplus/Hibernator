// webserver.hpp
#pragma once

#include <TWebServer/WebServerHttp.hpp>

class WebServer : public WebServerHttp {
	int startTime;
	int endTime;

	uint8_t status; // 0 disable, 1 during process, 2 ready
	public:
		WebServer();
		virtual std::string getContent(std::string &url, std::string &type, char *buffer, int bufferSize, 
				const Parameters &params);
		virtual void start(bool activeAsync);
};
