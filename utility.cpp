#include "utility.h"
#include <cxxabi.h>

std::string truncate(std::string original, uint lenght) {
	if (original.size() > lenght) {
		original.resize(lenght);
	}
	return original;
}

std::string demangle(const char *name) {
	int                                    status = -4; // some arbitrary value to eliminate the compiler warning
	std::unique_ptr<char, void (*)(void*)> res{
		abi::__cxa_demangle(name, NULL, NULL, &status),
				std::free};
	return (status == 0) ? res.get() : name;
}
