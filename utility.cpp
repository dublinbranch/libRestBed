#include "utility.h"
#include <cxxabi.h>
#include <restbed>

using namespace std;
using namespace restbed;

std::string truncate(std::string original, uint lenght) {
	if (original.size() > lenght) {
		original.resize(lenght);
	}
	return original;
}

std::string demangle(const char* name) {
	int                                    status = -4; // some arbitrary value to eliminate the compiler warning
	std::unique_ptr<char, void (*)(void*)> res{
	    abi::__cxa_demangle(name, NULL, NULL, &status),
	    std::free};
	return (status == 0) ? res.get() : name;
}

std::string getPost(const std::shared_ptr<Session>& session) {
	auto& request = session->get_request();
	
	//What is this ? Why not a simple get_post ???

	auto content_length = request->get_header("Content-Length", 0);

	string nonsensicalComplexity;

	session->fetch(content_length, [request, &nonsensicalComplexity](const shared_ptr<Session> session, const Bytes& body) {
		fprintf(stdout, "Inner debug %.*s\n", (int)body.size(), body.data());

		(void)session;
		if (body.empty()) {
			return;
		}
		//Is not even working whitout explicit char* cast! why, just why!!!
		nonsensicalComplexity.append((char*)body.data(), body.size());
		session->close( OK, body, { { "Content-Length", ::to_string( body.size( ) ) }, { "Content-Type", "text/plain" } } );
	});
	return nonsensicalComplexity;
}
