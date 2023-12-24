#pragma once
#include <iostream>
#include <format>


namespace Error {
	inline bool hadError = false;

	inline void report(int line, const std::string& where, const std::string& message) {
		std::cerr << std::format("[Line {}] Error {}: {}\n", line, where, message);
		hadError = true;
	}

	inline void error(int line, const std::string& message) {
		report(line, "", message);
	}
}





