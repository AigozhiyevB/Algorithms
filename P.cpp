#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::cout;
using std::cin;
using std::endl;

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


class Graph
{
protected:
	size_t vertexCount;
	size_t edgeCount;
	bool is_directed_;

public:
	typedef size_t  Vertex;

	explicit Graph(size_t vertex_count, bool is_directed = false) :
		vertexCount(vertex_count),
		edgeCount(0),
		is_directed_(is_directed) {}

	size_t GetVertexCount() const {
		return vertexCount;
	}
	size_t GetEdgeCount() const {
		return edgeCount;
	}
	bool isDirected() const {
		return is_directed_;
	}
	virtual void addEdge(const Vertex& start, const Vertex& finish, size_t weight) {
		edgeCount++;
	}

	virtual std::vector<Vertex> GetNeighbors(Vertex v) const = 0;

	virtual Graph& Transpose() const = 0;
};

class GraphAdjList : public Graph
{
private:
	std::vector<std::vector<Vertex>> adjList;

public:

	explicit GraphAdjList(size_t vertexCount) :
		Graph(vertexCount), adjList(vertexCount) {}

	GraphAdjList(size_t vertexCount, const std::vector<std::vector<Vertex>>& adjMatrix
		, bool is_directed = true) :
		Graph(vertexCount, is_directed)
	{
		adjList = std::vector<std::vector<Vertex>>(vertexCount);
		for (size_t i = 0; i < vertexCount; i++)
		{
			for (size_t j = 0; j < vertexCount; j++)
			{
				if (adjMatrix[i][j] == 1)
				{
					adjList[i].push_back(j);
					edgeCount++;
				}
			}
		}
	}

	std::vector<Vertex> GetNeighbors(Vertex v) const override
	{
		return adjList[v];
	}

	void addEdge(Vertex from, Vertex to, size_t weight) override {
		if (from >= vertexCount || to >= vertexCount) {
			throw std::runtime_error("Invalid vertex");
		}
		if (!is_directed_) {
			adjList[from].push_back( Edge(from, to, weight) );
			adjList[to].push_back({ to, from, weight });
		}
		else {
			adjList[from].push_back({ from, to,  weight });
		}
	}
};

class Edge {
private:
	Graph::Vertex to_;
	Graph::Vertex from_;
	size_t weight_;

public:
	const Graph::Vertex NO_VERTEX = UINT_MAX;

	Edge(const Edge& edge) :
		weight_(edge.weight_),
		from_(edge.from_),
		to_(edge.to_)
	{}
	Edge() : from_(NO_VERTEX), to_(NO_VERTEX), weight_(0) {}
	Edge(Graph::Vertex from, Graph::Vertex to, size_t weight) :
		from_(from), to_(to), weight_(weight) {}

	Graph::Vertex getTo() const
	{
		return to_;
	}
	Graph::Vertex  getFrom() const 
	{
		return from_;
	}
	size_t getWeight() const 
	{
		return weight_;
	}

	Edge& operator=(const Edge& edge) {
		this->weight_ = edge.weight_;
		this->to_ = edge.to_;
		this->from_ = edge.from_;
		return *this;
	};

	bool operator<(const Edge& a)
	{
		return weight_ < a.getWeight();
	}
};

class Cmp {
public:
	bool operator()(const Edge& lhs, const Edge& rhs) 
	{
		return lhs.getWeight() < rhs.getWeight();
	}
};


namespace GraphProcessing
{
	typedef size_t Vertex;
	enum VertexMark { WHITE, GRAY, BLACK };
	const Vertex NO_VERTEX = -1;
	const size_t INF = -1;


	size_t KruskalReturnsWeight(const Graph& g, const std::vector<Edge>& edges, DSU vertices) {
		size_t total_weight = 0;
		for (size_t i = 0; i < edges.size(); ++i) {
			Vertex from = edges[i].getFrom();
			Vertex to = edges[i].getTo();
			size_t weight = edges[i].getWeight();
			if (vertices.find(from) != vertices.find(to)) {
				total_weight += weight;
				vertices.unite(from, to);
			}
		}
		return total_weight;

	}

}

int main() {
	size_t vertex_count, edges_count;
	std::cin >> vertex_count >> edges_count;
	std::vector<Edge> edges(0);
	GraphAdjList g(vertex_count);
	DSU vertices(vertex_count);

	for (size_t i = 0; i < edges_count; ++i) {
		size_t from, to, weight;
		std::cin >> from >> to >> weight;
		g.addEdge(--from, --to, weight);
		edges.push_back({ from, to, weight });
	}

	std::sort(edges.begin(), edges.end(), Cmp());

	size_t weight_of_min_spanning_tree = GraphProcessing::KruskalReturnsWeight(g, edges, vertices);
	std::cout << weight_of_min_spanning_tree;


	return 0;
}