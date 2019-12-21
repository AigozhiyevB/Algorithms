#include <iostream>
#include <vector>

class Graph {
protected:
	size_t vertexCount;
	size_t edgeCount;
	bool is_directed_;

public:
	typedef size_t Vertex;
	explicit Graph(size_t vertex_count, bool is_directed)
		: vertexCount(vertex_count),
		is_directed_(is_directed),
		edgeCount(0) {}

	size_t GetVertexCount() const {
		return vertexCount;
	}

	size_t GetEdgeCount() const {
		return edgeCount;
	}

	bool GetDirection() const {
		return is_directed_;
	}

	virtual size_t GetVertexDeg(const Vertex& vertex) const = 0;

	virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;
};

class GraphAdjList : public Graph {
private:
	std::vector<std::vector<Vertex>> adj_list_;

public:

	explicit GraphAdjList(size_t vertex_count, bool is_directed)
		: Graph(vertex_count, is_directed),
		adj_list_(vertex_count + 1) {}

	void AddEdge(const Vertex& start, const Vertex& finish) override {
		adj_list_[start].push_back(finish);
		if (!is_directed_) {
			adj_list_[finish].push_back(start);
		}
		edgeCount++;
	}

	size_t GetVertexDeg(const Vertex& vertex) const override {
		return adj_list_[vertex].size();
	}
};

int main() {
	size_t n, m;
	std::cin >> n >> m;

	GraphAdjList g = GraphAdjList(n, false);

	for (int i = 0; i < m; i++) {
		Graph::Vertex start, finish;
		std::cin >> start >> finish;
		g.AddEdge(start, finish);
	}

	for (int i = 1; i < n + 1; i++) {
		std::cout << g.GetVertexDeg(i) << " ";
	}

//	system("pause");

	return 0;
}
