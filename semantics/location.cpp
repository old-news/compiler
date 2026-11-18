#include "location.h"
#include <algorithm>

Location::Location() {
	path = {};
}

Location::Location(const vector<size_t>& v) {
	path = v;
}

bool Location::operator==(const Location& other) const {
	return getPath() == other.getPath();
}

bool Location::is_in(const Location& other) {
	const vector<size_t>& opath = other.getPath();
	if (path.size() <= opath.size()) {
		return false;
	}
	for (size_t i = 0; i < opath.size(); i++) {
		if (path[i] != opath[i]) {
			return false;
		}
	}
	return true;
}

vector<size_t> Location::getPath() const {
	return path;
}
