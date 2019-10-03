#include <iostream>
#include <vector>

typedef size_t Vertex;

class Graph {
protected:
	size_t vertexСount;
	size_t edgeСount;
	bool is_directed_;

public:
	typedef size_t Vertex;
	Graph(size_t vertex_count, bool is_directed)
		: vertexСount(vertex_count),
		is_directed_(is_directed),
		edgeСount(0) {}

	virtual size_t GetVertexCount() const {
		return vertexСount;
	}

	virtual size_t GetEdgeCount() const {
		return edgeСount;
	}

	virtual bool IsDirected() const {
		return is_directed_;
	}

	virtual ~Graph(){}
};

class GraphAdjMatrix : public Graph {
private:
	std::vector<std::vector<Vertex>> adj_matrix_;

public:
	GraphAdjMatrix(const std::vector<std::vector<Vertex>>& graph, bool is_directed)
		: Graph(graph.size(), is_directed),
		adj_matrix_(graph) 
	{
		for (size_t i = 0; i < graph.size(); i++) {
			for (size_t j = 0; j < graph.size(); j++) {
				edgeСount += adj_matrix_[i][j];
			}
		}
		edgeСount = edgeСount / 2;
	}

	size_t GetEdgeCount() const {
		return edgeСount;
	}

	~GraphAdjMatrix() {};
};

int main() {
	int n;
	std::cin >> n;

	std::vector<std::vector<Vertex>> adj_matrix(n, std::vector<Vertex>(n));

	int input;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cin >> input;
			adj_matrix[i][j] = input;
		}
	}
	GraphAdjMatrix graph_adj_matrix = GraphAdjMatrix(adj_matrix, false);

	std::cout << graph_adj_matrix.GetEdgeCount();

//	system("pause");

	return 0;
}
