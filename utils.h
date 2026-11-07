#pragma once

#include <cstddef>
#include <string>

struct Pos {
	size_t line, col;

	std::string toString() const {
		return "(" + std::to_string(line) + ", " + std::to_string(col) + ")";
	}
};
