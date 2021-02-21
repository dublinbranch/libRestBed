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

void error_handler(const int statusCode, const exception& exception, const shared_ptr<Session>& session) {
	fprintf(stdout, "error: %s\n", exception.what());
	// Place breakpoint here

	if (session && session->is_open()) {
		session->close(statusCode, exception.what(), {{"Server", "Restbed"}});
	}
}
