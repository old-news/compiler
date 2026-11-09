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
	short, int,
	i8, i16, i24, i32, i48, i64, i128,
	float, double
}

interface Iterable<typename T> {
	begin() -> @T
	end() -> @T
}

interface Collection<typename T> impl Iterable {
	begin() -> @T
	end() -> @T
	size() -> Number
	push(T)
}

class vector<typename T> impl Collection {
	private:
		loc: @T
		len, allocated_len: i48

		grow() {
			grow(4)
		}

		grow(newsize: Number) {
			new_alloc_len: int = max(size * 1.5, newsize)
			newloc: @T = alloc(heap, sizeof(T) * new_alloc_len)
			this.copy(newloc)
			allocated_len = new_alloc_len
			dealloc loc
			loc = newloc
		}
	public:
		vector() {
			vector(0)
		}

		vector(start_space: size_t) {
			start_space.min(2)
			loc = alloc(heap, sizeof(T) * start_space)
			len = 0
			allocated_len = start_space
		}

		vector(col: Collection) {
			vector(col.size())
			concat(col);
		}

		vector(it: Iterable) {
			vector()
			concat(it);
		}

		push(item: T) {
			if (len >= allocated_len) {
				grow()
			}
			loc[size()] = item
			len++
		}

		pop(index: Number) {
			if (index >= size())
				fail()
			if (index == size() - 1) {
				return pop()
			}
			newloc: @T = alloc(heap, sizeof(T) * allocated_len)
			if (index > 0)
				copy(begin(), begin() + index - 1, newloc)
			copy(begin() + index + 1, end(), newloc + index);
			dealloc loc
			loc = newloc
		}

		pop() {
			if (0 == size())
				fail()
			len--
		}

		concat(col: Collection) {
			newlen: Number = size() + col.size()
			if (newlen > allocated_len) {
				grow(newlen)
			}
			col.copyto(begin() + size())
			len = newlen
		}

		begin() -> @T {
			return loc;
		}

		end() -> @T {
			return loc + len;
		}

		size() -> size_t {
			return len;
		}

		at(i: Number) -> T {
			if (i < 0 || i >= size())
				fail()
			return this[i]
		}

		operator::[] (i: Number) -> T {
			return loc[i]
		}
}

Collection<T>::copyto(newloc: @T) {
	for (t in this) {
		newloc@ = t
		newloc++
	}
}

a: int[4] = [1, 2, 3, 4]
v: vector<int> = a
for (item in v)
	println(item)
