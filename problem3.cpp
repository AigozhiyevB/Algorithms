#include <iostream>
#include <vector>

class Graph {
protected:
	size_t vertexCount;
	size_t edgeCount;
	bool is_directed_;

public:
	typedef size_t Vertex;

	explicit Graph(size_t vertex_count, bool is_directed = false)
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

	virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;

};

class GraphAdjList : public Graph {
private:
	std::vector<std::vector<Vertex>> adjList;

public:

	enum VertexMark { WHITE, GREY, BLACK };

	void DFS(const Vertex& vertex, std::vector<VertexMark>& used) const {
		used[vertex] = GREY;
		for (auto &u : adjList[vertex]) {
			if (used[u] == WHITE) {
				DFS(u, used);
			}
		}
	}

	explicit GraphAdjList(size_t vertex_count, bool is_directed)
		: Graph(vertex_count, is_directed),
		adjList(vertex_count + 1) {}

	void AddEdge(const Vertex& start, const Vertex& finish) override {
		adjList[start].push_back(finish);
		if (!is_directed_) {
			adjList[finish].push_back(start);
		}
		edgeCount++;
	}

	size_t CountComponentVertices(const Vertex& vertex) const {
		std::vector<VertexMark> colors(vertexCount + 1, WHITE);
		DFS(vertex, colors);
		size_t componentVertexCount = 0;
		for (Vertex v = 1; v < vertexCount + 1; v++) {
			if (colors[v]) {
				componentVertexCount++;
			}
		}
		return componentVertexCount;
	}
};

int main() {
	size_t n;
	Graph::Vertex vertex;
	std::cin >> n >> vertex;

	GraphAdjList g = GraphAdjList(n, false);
	size_t input;
	for (int i = 1; i < n + 1; ++i) {
		for (int j = 1; j < n + 1; ++j) {
			std::cin >> input;
			if (input == 1) {
				g.AddEdge(i, j);
			}
		}
	}

	std::cout << g.CountComponentVertices(vertex);

//	system("pause");

	return 0;
}