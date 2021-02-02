#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "QStacker/qstacker.h"
#include <map>
#include "fmt/core.h"

std::string truncate(std::string original, uint lenght);

std::string demangle(const char* name);

class BadRequestEx : public ExceptionV2 {
      public:
	BadRequestEx(const QString& _msg)
	    : ExceptionV2(_msg, 6) {
	}
	BadRequestEx(const std::string& _msg)
	    : ExceptionV2(_msg, 6) {
	}
};

template <typename Type>
bool swapper(const std::string& from, Type& to) {
	if constexpr (std::is_same<Type, double>::value) {
		try {
			to = stod(from);
			return true;
		} catch (...) {
			return false;
		}
	} else if constexpr (std::is_integral_v<Type>) {
		try {
			to = stoll(from);
			return true;
		} catch (...) {
			return false;
		}
	} else if constexpr (std::is_same_v<std::string, Type>) {
		to = from;
		return true;
	} else {
		//poor man static assert that will also print for which type it failed
		typedef typename Type::something_made_up X;

		X y;     //To avoid complain that X is defined but not used
		(void)y; //TO avoid complain that y is unused
	}
}

template <typename Type>
bool ifset(const std::multimap<std::string, std::string>& get, const std::string& key, Type& value) {
	auto iter = get.find(key);
	if (iter == get.end()) {
		return false;
	}
	if (swapper(iter->second, value)) {
		return true;
	}
	auto copy = truncate(iter->second, 32);
	auto n    = demangle(typeid(value).name());
	throw BadRequestEx(fmt::format("key: >>>{}<<< with value >>>{}<<< is not convertible to {}", key, copy, n));
}

template <typename Type>
void require(const std::multimap<std::string, std::string>& get, const std::string& key, Type& value) {
	if (!ifset(get, key, value)) {
		throw BadRequestEx(fmt::format("key: >>>{}<<< is missing in the request", key));
	}
}

template <typename Type>
Type require(const std::multimap<std::string, std::string>& get, const std::string& key) {
	Type t;
	require(get, key, t);
	return t;
}



#endif // UTILITY_H
