#include <iostream>
#include <vector>
#include <queue>
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

	typedef Graph::Vertex Vertex;

	void DFS(GraphAdjList &g, const Vertex& vertex, std::vector<VertexMark>& color) {
		color[vertex] = GREY;
		for (auto &u : g.GetAllNeighbors(vertex)) {
			if (color[u] == WHITE) {
				DFS(g, u, color);
			}
		}
	}

	size_t CountComponentVertix(GraphAdjList &g, const Vertex& vertex){
		std::vector<VertexMark> colors(g.GetVertexCount() + 1, WHITE);
		DFS(g, vertex, colors);
		size_t componentVertexCount = 0;
		for (Vertex v = 1; v < g.GetVertexCount() + 1; v++) {
			if (colors[v] == GREY) {
				componentVertexCount++;
			}
		}
		return componentVertexCount;
	}

	void BFS(GraphAdjList &g, std::vector<Vertex>& prev, const Vertex& start, const Vertex& finish
	, std::vector<int> dist) {

		std::queue<Vertex> q;

		dist[start] = 0;
		q.push(start);

		while (q.size() != 0) {
			Vertex vertex = q.front();
			q.pop();
			for (auto u : g.GetAllNeighbors(vertex)) {
				if (dist[u] == -1) {
					dist[u] = dist[vertex] + 1;
					q.push(u);
					prev[u] = vertex;
				}
			}
		}
	}


	std::vector<Vertex> FindMinPathVertices(GraphAdjList &g, const Vertex& start, const Vertex& finish) {
		std::vector<Vertex> prev(g.GetVertexCount()+ 1, 0);

		std::vector<int> dist(g.GetVertexCount() + 1, -1);

		BFS(g, prev, start, finish, dist);
		std::vector<Vertex> path;
		if (dist[finish] != -1) {
			Vertex tmp = finish;
			while (tmp != 0) {
				path.push_back(tmp);
				tmp = prev[tmp];
			}
		}

		std::reverse(prev.begin(), prev.end());

		return prev;
	}
};




int main() {
	size_t n;
	Graph::Vertex start, finish;
	std::cin >> n;

	GraphAdjList g = GraphAdjList(n + 1, false);

	for (int i = 1; i < n + 1; i++) {
		for (int j = 1; j < n + 1; j++) {
			size_t input;
			std::cin >> input;
			if (input == 1) {
				g.AddEdge(i, j);
			}
		}
	}

	std::cin >> start >> finish;

	auto path = GraphProcessing::FindMinPathVertices(g, start, finish);

	if (path.empty()) {
		std::cout << -1;
	}
	else {
		size_t size = path.size() - 1;
		std::cout << size << std::endl;
		if (size != 0) {
			for (const auto &i : path) {
				std::cout << i << ' ';
			}
		}
	}

//	system("pause");

	return 0;
}


