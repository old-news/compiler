square: int a -> int {
        return a * a;
}

map<T>: vector<T> arr, function<vector<T>> f -> vector<T> {
        vector<T> o;
        for (item, arr) {
                o.push_back(f(item));
        }
        return o;
}

map: (vector<T> arr, function<vector<T>> f) -> vector<T> {
        vector<T> o;
        for (item, arr)
                o.push_back(f(item));
        return o;
}

template <typename T>
vector<T> map(vector<T> arr, function<vector<T>> f) {
        vector<T> o;
        for (auto item : arr) {
                o.push_back(f(item));
        }
        return o;
}

pow: (int a, int b) -> int {
	return a ** b;
}

int pow(int a, int b) {
	return a ** b;
}

cube: int a -> int {
	return pow(a, 3);
}

vector<T> vector<T>::filter(function<bool(T)> f) {
	vector<T> o;
	for (item in this) {
		if (f(item)) o.push(item);
	}
	this = o;
	return this;
}

vector<T> vector<T>::filter(method<T, bool()> f) {
	return filter(item => item.f());
}

vector<T> vector<T>::map(callable<T(T)> f) {
	for (i, 0, this.size()) {
		this[i] = f(this[i]);
	}
	return this;
}

vector<T> vector<T>::map(method<T, (T)> f) {
	return map(item => item.f());
}

int int::cube() {
	return this ** 3;
}

bool int::even() {
	return this % 2 == 0;
}

bool int::odd() {
	return this % 2 == 1;
}

vector<int> v = [1, 2, 3, 4];
v.filter(int::even).map(item => item.sqad(2));

alias callable<T(X..)> = function<T(X..)>;
alias callable<Y, T(X..)> = method<Y, T(X..)>;

T callable<T(X..)>::() {
	return antialias(X..);
}

T callable<Y, T(X..)>::() {
	return antialias.(X..);
}

int int::square() {
	return this * this;
}

auto thing = int::square;

int doubleDo(int i, callable<int(int)> f) {
	return f(f(i));
}

multiclass callable<T(X[:])> {
	alias function<T(X[:])>:
		T (X[:]) {
			antialias(X..);
		}

	alias method<X[0], T(X[1:])>:
		T (X[1:]) {
			Y::antialias(X[1:]);
		}
}

alias callable<T(X[:])> = function<T(X[:])> || method<X[0], T(X[1:])>

T call(function<T(X[:])> f, X[:] args) {
	return f(args);
}

T call(method<X[0], T(X[1:])> m, X[0] object, X[1:] args) {
	return object.m(args);
}

int a = 2;
a = doubleDo(a, thing);
print(a);

// outputs 16 == a.thing().thing();

int int::sqad(int a) {
	return this * this + a;
}

int sqad(int a, int b) {
	return a * a + b;
}

generictype Number {
	float, double,
	Integer
}

generictype Integer {
	short, int,
	i8, i16, i24, i32, i48, i64, i128,
}

generictype uint {
	ui8, ui16, ui24, ui32, ui48, ui64, ui128
}

interface Iterator<typename T> {
	begin() -> @T
	end() -> @T
	next() -> @T

	copyto(it: Iterator<T>) {
		v: @T = next()
		to: @T = it.next()
		while (v != end()) {
			if (to == it.end()) {
				exception(this, "Failed copy of Iterator to Iterator")
			}
			to@ = v@
			v = next()
			to = it.next()
		}
	}
}

interface Iterable<typename T> {
	iter() -> Iterator<T>
}

interface Collection<typename T, onetype<@T, uint> K> impl Iterable<T> {
	size() -> uint
	push(T)
	pop(K)
	begin() -> @T

	copyto(newloc: Iterable<T>) {
		iter().copyto(newloc.iter())
	}

	operator::dealloc() {
		for (item: @T in this) {
			dealloc item@
		}
	}
}

interface List<typename T> impl Collection<T> {
	pop(uint)
	empty() -> bool
}

class contiguous_iterator<typename T> impl Iterator {
	// Iterates over a contiguous block of memory
	private:
		start, finish: @T
		index: uint
	public:
		iterator(s, e: @T) {
			start = s
			finish = e
			index = 0
		}

		size() -> uint {
			return end() - begin()
		}

		begin() -> @T {
			return start
		}

		end() -> @T {
			return finish
		}

		next() -> @T {
			index++
			return begin() + index - 1
		}
}

class vector<typename T> impl Collection<T> {
	private:
		loc: @T
		len, allocated_len: ui48

		grow() {
			grow(4)
		}

		grow(newsize: uint) {
			new_alloc_len: int = max(size() * 1.5, newsize)
			newloc: @T = alloc(heap, sizeof(T) * new_alloc_len)
			copyto(newloc)
			allocated_len = new_alloc_len
			dealloc loc
			loc = newloc
		}
	public:
		vector() {
			this(0)
		}

		vector(start_space: uint) {
			start_space = start_space.min(2)
			loc = alloc(heap, sizeof(T) * start_space)
			len = 0
			allocated_len = start_space
		}

		vector(col: Collection<T>) {
			this(col.size())
			concat(col)
		}

		vector(it: Iterable<T>) {
			this()
			concat(it)
		}

		push(item: T) start_space = {
			if (len >= allocated_len) {
				grow()
			}
			loc[size()] = item
			len++
		}

		pop(index: Integer) {
			if (index < 0 || index >= size())
				exception(this, "Attempted pop at %d, which is not in bounds of vector with length %d".format(index, size()))
			if (index == size() - 1) {
				return pop()
			}
			iterator(begin() + index + 1, end()).copyto(begin() + index)
		}

		pop() {
			if (0 == size())
				exception(this, "Attempted pop when empty")
			len--
		}

		concat(col: Collection<T>) {
			newlen := size() + col.size()
			if (newlen > allocated_len) {
				grow(newlen)
			}
			col.copyto(loc + size())
			len = newlen
		}

		concat(it: Iterable<T>) {
			for (item: @T in it) {
				push(item@)
			}
		}

		iter() -> Iterator<T> {
			return contiguous_iterator(loc, loc + len)
		}

		size() -> size_t {
			return len;
		}

		at(i: Integer) -> T {
			if (i < 0 || i >= size())
				exception(this, "Attempted lookup at %, which is out of bounds of vector with length %".format(i, size()))
			return this[i]
		}

		operator::[] (i: Integer) -> T {
			return loc[i]
		}

		operator::= (Collection<T> c) {
			if (c.begin() == begin()) return
			dealloc this
			this(c)
		}

		operator::dealloc() {
			Collection::dealloc(this)
			dealloc loc
		}

		empty() -> bool {
			return 0 == size()
		}
}

class string extends vector<char> {
	public:
		format<arglist(N, args)>(args) {
			if (empty()) return this
			string out;
			for (i := 0 to size()) {
				c: char = this[i]
				if ('%' != c) {
					out.push(c)
				} else if (i + 1 == size() || '%' == this[i + 1]) {
					out.push(args[N].tostring())
					n++
				} else {
					out.push('%')
					i++
				}
			}
		}
}

a: int[4] = [1, 2, 3, 4]
v: vector<int> = a
for (item in v)
	println(item)

struct string {
	start: char@
	size: uint48
	alloc_size: uint48
}

at(string str, uint48 index): char {
	if (index > str.size) {
		return 0;
	}
	return (str.size + index)@;
}
