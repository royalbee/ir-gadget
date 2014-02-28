#include <ring.h>
#include <cassert>
#include <iostream>

int main()
{
	using namespace emb;
	using namespace std;

	ring<int, 10> r;

	assert(r.empty());
	for (int i = 0 ; i < r.max_size(); i++)
		r.push_back(i);
	cout << "size:" << r.size() << "max_size:" << r.max_size() << endl;
	assert(r.size() == r.max_size());
	for (int i = 0 ; i < r.max_size(); i++)
		assert(i == r.pop_front());
	assert(r.empty());

	for (int i = 0 ; i < r.max_size(); i++)
		r.push_back(i);
	cout << "size:" << r.size() << "max_size:" << r.max_size() << endl;
	assert(r.size() == r.max_size());

	//read access
	for (int i = 0 ; i < r.size(); i++)
		assert(i == r[i]);
	//modify
	for (int i = 0 ; i < r.size(); i++)
		r[i] *= i;
	//const access
	const decltype(r) *c = &r;
	for (int i = 0 ; i < c->size(); i++)
		assert(i*i == (*c)[i]);

	//iterator access
	{
		int i = 0;
		for (auto p = r.begin(), end = r.end(); p != end; p++)
		{
			assert(i*i == *p);
			i++;
		}
		cout << "iterator count:" << i << endl;
	}

	//range loop
	{
		int i = 0;
		for (auto v : r)
		{
			assert(i*i == v);
			i++;
		}
		cout << "range count:" << i << endl;
	}
	assert(!c->empty());
}
