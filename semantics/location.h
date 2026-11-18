#include <vector>
#include <string>

using namespace std;

class Location {
	private:
		vector<size_t> path;
	
	public:
		Location();
		Location(const vector<size_t>& p);

		bool operator==(const Location&) const;
		bool is_in(const Location&);
		vector<size_t> getPath() const;
};

template <>
struct std::hash<Location> {
	size_t operator()(const Location& l) const noexcept {
		vector<size_t> p = l.getPath();
		size_t sum = p.size();
		for (int i = 0; i < min((size_t) 4, p.size()); i++) {
			sum <<= 6;
			sum += p[i];
		}
		return sum;
	}
};
