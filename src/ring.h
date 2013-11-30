#include <cstdint>
namespace emb {

template <typename T, int order>
class ring {
public:
	using size_type = uint32_t;
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;

	ring() : r(0), w(0)
	{}
	constexpr static size_type max_size() { return 1 << order; }
	void push_back(const T val) { buf[w++ & mask()] = val; }
	const T pop_front() { return buf[r++ & mask()]; }
	const size_type size() const { return w - r; }
	reference operator[](const size_type at) { return buf[(r+at) & mask()]; }
	const_reference operator[](const size_type at) const { return buf[(r+at) & mask()]; }
	bool empty() const { return r == w; }
private:
	T buf[max_size()];
	size_type r, w;

	constexpr size_type mask() { return max_size() - 1; }
};

};


