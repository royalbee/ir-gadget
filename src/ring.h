#include <cstdint>
#include <iterator>
namespace emb {

template <typename T, int order>
class ring {
public:
	using size_type = uint32_t;
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = T*;
	using const_pointer = const T*;
	class iterator;
	using const_iterator = const iterator;

	ring() : r(0), w(0)
	{}
	constexpr static size_type max_size() { return 1 << order; }
	void push_back(const T val) { buf[w++ & mask()] = val; }
	const T pop_front() { return buf[r++ & mask()]; }
	const size_type size() const { return w - r; }
	reference operator[](const size_type at) { return buf[(r+at) & mask()]; }
	const_reference operator[](const size_type at) const { return buf[(r+at) & mask()]; }
	bool empty() const { return r == w; }
	iterator begin() { return iterator(*this, 0); }
	iterator end() { return iterator(*this, max_size()); }
	const_iterator cbegin() { return iterator(*this, 0); }
	const_iterator cend() { return iterator(*this, max_size()); }
private:
	T buf[max_size()];
	size_type r, w;

	constexpr size_type mask() { return max_size() - 1; }
};

template <typename T, int order>
class ring<T, order>::iterator :
	std::iterator<std::random_access_iterator_tag, value_type, size_type>
{
public:
	using parent_type = ring<T, order>;
	using self_type = typename parent_type::iterator;

	iterator(parent_type &parent, size_type index) : parent(parent), index(index)
	{ }
	self_type &operator++()
	{ index = actual_index()+1; return *this; }
	self_type operator++(int) // postincrement
	{ self_type old(*this); operator++(); return old; }
	self_type &operator--()
	{ index = --actual_index(); return *this; }
	self_type operator--(int) // postdecrement
	{ self_type old(*this); operator--(); return old; }
	const_reference operator*() const { return parent[index]; }
	const_pointer operator->() const { return &(parent[index]); }
	reference operator*() { return parent[index]; }
	pointer operator->()  { return &(parent[index]); }

	bool operator==(const self_type &other) const
	{ return &parent == &other.parent && actual_index() == other.actual_index(); }
	bool operator!=(const self_type &other) const
	{ return !(other == *this); }

protected:
	size_type	actual_index() const { return index == parent.max_size() ? parent.size() : index; }

private:
	parent_type &parent;
	size_type    index;
};


};
