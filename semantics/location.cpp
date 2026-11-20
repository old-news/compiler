#include "location.h"

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

void Location::push(size_t i) {
	path.push_back(i);
}

void Location::set_back(size_t i) {
	path[path.size() - 1] = i;
}

string Location::toString() {
	if (path.empty()) {
		return "<top level>";
	}
	string s = "";
	for (int i = 0; i < path.size(); i++) {
		s += to_string(path[i]);
		if (i+1 < path.size()) {
			s += ".";
		}
	}
	return s;
}
