#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::cin;
using std::endl;

class Graph {

protected:
	size_t vertexCount;
	size_t edgeCount;
	bool is_directed_;

public:
	typedef size_t  Vertex;

	explicit Graph(size_t vertex_count, bool is_directed = true) :
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

	void DFS_Visit_has_cycle(const Graph& graph, const Vertex& v, std::vector<VertexMark>& color, bool& has_cycle)
	{
		color[v] = GRAY;

		for (const auto& u : graph.GetNeighbors(v))
		{
			if (color[u] == WHITE)
			{
				DFS_Visit_has_cycle(graph, u, color, has_cycle);

			}
			else if (color[u] == GRAY)
			{
				has_cycle = true;
			}
		}

		color[v] = BLACK;
	}

	void DFS_TopSort(const Graph& graph, const Vertex& v, std::vector<VertexMark>& color, std::vector<Vertex>& sorted)
	{
		color[v] = GRAY;

		for (const auto& u : graph.GetNeighbors(v))
		{
			if (color[u] == WHITE)
			{
				DFS_TopSort(graph, u, color, sorted);
			}
		}

		color[v] = BLACK;
		sorted.push_back(v);
	}

	bool IsCyclic(const Graph& graph)
	{
		vector<VertexMark> color(graph.GetVertexCount(), WHITE);
		bool has_cycle = false;

		for (Vertex v = 0; v < graph.GetVertexCount(); v++)
		{
			DFS_Visit_has_cycle(graph, v, color, has_cycle);
		}

		return has_cycle;
	}

	std::vector<Vertex> TopSort(const Graph& graph)
	{
		if (graph.isDirected() == false || IsCyclic(graph) == true)
		{
			return {};
		}

		std::vector<VertexMark> color(graph.GetVertexCount(), WHITE);
		std::vector<Vertex> sorted;

		for (Vertex v = 0; v < graph.GetVertexCount(); v++)
		{
			if (color[v] == WHITE)
			{
				DFS_TopSort(graph, v, color, sorted);
			}
		}

		return{ sorted.rbegin(), sorted.rend() };
	}
};

int main()
{
	size_t  vertexNumber, edgeNumber;
	cin >> vertexNumber >> edgeNumber;

	GraphAdjList graph(vertexNumber);
	for (size_t i = 0; i < edgeNumber; i++) {
		Graph::Vertex start, finish;
		cin >> start >> finish;
		graph.addEdge(start - 1, finish - 1);
	}

	std::vector<Graph::Vertex> topsort = GraphProcessing::TopSort(graph);

	if (topsort.empty())
	{
		cout << -1;
	}
	else
	{
		for (const auto& v : topsort)
		{
			cout << v + 1 << " ";
		}
	}

	return 0;
}