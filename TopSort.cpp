#include <iostream>
#include <vector>
#include <algorithm>

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

	bool IsDirected() const {
		return is_directed_;
	}

	virtual void AddEdge(const Vertex &start, const Vertex &finish) = 0;

	virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;

};

class GraphAdjList : public Graph {
private:
	std::vector<std::vector<Vertex>> adj_list_;

public:
	explicit GraphAdjList(size_t vertex_count, bool is_directed)
		: Graph(vertex_count, is_directed),
		adj_list_(vertex_count + 1) {}

	void AddEdge(const Vertex &start, const Vertex &finish) override {
		adj_list_[start].push_back(finish);
		if (!is_directed_) {
			adj_list_[finish].push_back(start);
		}
		edgeCount++;
	}

	std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
		return adj_list_[vertex];
	}
};

namespace GraphProcessing {

	enum VertexMark { WHITE, GREY, BLACK };

	void DFS_TopSort(const Graph &graph, const Graph::Vertex &vertex, std::vector<VertexMark> &color, std::vector<Graph::Vertex> &topsort) {
		color[vertex] = GREY;
		for (auto &u : graph.GetAllNeighbors(vertex)) {
			if (!color[u]) {
				DFS_TopSort(graph, u, color, topsort);
			}
		}
		topsort.push_back(vertex);
	}

	bool Is_DFS_Cycling(const Graph &graph, const Graph::Vertex &vertex, std::vector<VertexMark> &color) {
		color[vertex] = GREY;
		for (auto &u : graph.GetAllNeighbors(vertex)) {
			if (color[u] == WHITE) {
				if (!Is_DFS_Cycling(graph, u, color)) {
					return false;
				}
			} else if (color[u] == GREY) {
				return false;
			}
		}
		color[vertex] = BLACK;
		return true;
	}

	bool IsCycling(const Graph &graph) {
		std::vector<VertexMark> visited(graph.GetVertexCount() + 1, WHITE);
		for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
			if (visited[vertex] == WHITE) {
				if (!Is_DFS_Cycling(graph, vertex, visited)) {
					return true;
				}
			}
		}
		return false;
	}

	std::vector<Graph::Vertex> TopSort(const Graph &graph) {
		if (graph.IsDirected() != 1 || IsCycling(graph)) {
			return {};
		}
		std::vector<VertexMark> color(graph.GetVertexCount() + 1, WHITE);

		std::vector<Graph::Vertex> topsort;

		for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; vertex++) {
			if (color[vertex] == WHITE) {
				DFS_TopSort(graph, vertex, color, topsort);
			}
		}
		
		std::reverse(topsort.begin(), topsort.end());

		return topsort;
	}
}

int main() {
	size_t n, m;
	std::cin >> n >> m;

	GraphAdjList g = GraphAdjList(n, true);

	for (int i = 0; i < m; i++) {
		size_t v1, v2;
		std::cin >> v1 >> v2;
		g.AddEdge(v1, v2);
	}

	auto topsort = GraphProcessing::TopSort(g);

	if (topsort.empty()) {
		std::cout << "No";
	}
	else {
		std::cout << "Yes" << "\n";
		for (auto vertex : topsort) {
			std::cout << vertex << " ";
		}
	}

//	system("pause");

	return 0;
}
