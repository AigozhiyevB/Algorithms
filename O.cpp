#include <iostream>
#include <cstdio>
#include <vector>
#include <climits>

class DSU {
protected:
	std::vector<size_t> heap_size_;
	std::vector<size_t> delegate_;
	std::vector<size_t> rank_;

public:
	typedef size_t element;

	const size_t NO_DELEGATE = UINT_MAX;
	const size_t NO_RANK = UINT_MAX;

	explicit DSU(size_t elements_count) :
		delegate_(elements_count),
		rank_(elements_count), 
		heap_size_(elements_count) 
	{
		for (element i = 0; i < elements_count; i++)
		{
			makeSet(i);
		}
	}

	void makeSet(element a) {
		heap_size_[a] = 0;
		delegate_[a] = a;
		rank_[a] = 0;
	}

	element find(element x) {
		if (delegate_[x] == x) {
			return x;
		}
		delegate_[x] = find(delegate_[x]);
		return delegate_[x];
	}

	element unite(element a, element b, size_t weight) {
		a = find(a);
		b = find(b);
		if (a == b) 
		{
			heap_size_[a] += weight;
			return a;
		}
		if (rank_[a] < rank_[b]) 
		{
			heap_size_[b] += heap_size_[a] + weight;
			delegate_[a] = b;
			return b;
		}
		else {
			heap_size_[a] += heap_size_[b] + weight;
			delegate_[b] = a;
			if (rank_[a] == rank_[b]) 
			{
				rank_[a]++;
			}
			return a;
		}
	}

	size_t getHeapSize(element x) const 
	{
		return heap_size_[x];
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	size_t vertex_count, command_count;
	std::cin >> vertex_count >> command_count;

	DSU dsu(vertex_count);

	size_t command;

	for (size_t i = 0; i < command_count; i++) {
		std::cin >> command;
		if (command == 1) {
			DSU::element from, to, weight;
			std::cin >> from >> to >> weight;
			dsu.unite(from - 1, to - 1, weight);
		}

		if (command == 2) {
			DSU::element component_member;
			std::cin >> component_member;
			DSU::element lead = dsu.find(component_member - 1);
			std::cout << dsu.getHeapSize(lead) << "\n";
		}
	}

	return 0;
}