#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::cin;
using std::endl;

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
	virtual void addEdge(const Vertex& start, const Vertex& finish) {
		edgeCount++;
	}

	virtual std::vector<Vertex> GetNeighbors(Vertex v) const = 0;
};

class GraphAdjList : public Graph
{
private:
	std::vector<std::vector<Vertex>> adjList;

public:

	explicit GraphAdjList(size_t vertexCount) :
		Graph(vertexCount), adjList(vertexCount) {}

	GraphAdjList(size_t vertexCount, const std::vector<std::vector<Vertex>>& adjMatrix
		, bool is_directed = false) :
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

	void addEdge(const Vertex& start, const Vertex& finish) override
	{
		if (is_directed_ == 0)
		{
			Graph::addEdge(start, finish);
			adjList[start].push_back(finish);
			adjList[finish].push_back(start);
		}
		else
		{
			Graph::addEdge(start, finish);
			adjList[start].push_back(finish);
		}
	}
};

namespace GraphProcessing
{
	typedef size_t Vertex;
	enum VertexMark { WHITE, BLACK, GRAY };

	void DFS_Visit(const Graph& graph, Vertex v, std::vector<VertexMark>& color,
		std::vector<Vertex>& component)
	{
		color[v] = GRAY;
		for (const Vertex& u : graph.GetNeighbors(v))
		{
			if (color[u] == WHITE)
			{
				DFS_Visit(graph, u, color, component);
			}
		}
		component.push_back(v);
		color[v] = BLACK;
	}

	std::vector<std::vector<Vertex>> GetSCC(const Graph& graph)
	{
		std::vector<std::vector<Vertex>> SCC_Container;
		std::vector<VertexMark> color(graph.GetVertexCount(), WHITE);

		for (Vertex v = 0; v < graph.GetVertexCount(); v++)
		{
			if (color[v] == WHITE)
			{
				std::vector<Vertex> component(0);
				DFS_Visit(graph, v, color, component);
				SCC_Container.push_back(component);
			}
		}

		return SCC_Container;
	}

}

int main()
{
	size_t  vertexNumber, edgeNumber;
	cin >> vertexNumber;

	GraphAdjList graph(vertexNumber);
	for (size_t i = 0; i < vertexNumber; i++) {
		Graph::Vertex start;
		cin >> start;
		graph.addEdge(start - 1, i);
	}

	vector<vector<Graph::Vertex>> SCC = GraphProcessing::GetSCC(graph);
	cout << SCC.size() << endl;

	return 0;
}