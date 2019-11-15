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
	enum {VOID = 100001};

	void DFS_Visit(const Graph& graph, Vertex v, std::vector<VertexMark>& color,
		std::vector<Vertex>& parent, bool& has_cycle, Vertex& cycle_end)
	{
		color[v] = GRAY;
		for (const Vertex& u : graph.GetNeighbors(v))
		{
			if (parent[u] == VOID)
			{
				parent[u] = v;
			}
			if (color[u] == WHITE)
			{
				DFS_Visit(graph, u, color, parent, has_cycle, cycle_end);
			}
			if (color[u] == GRAY)
			{
				parent[u] = v;
				cycle_end = v;
				has_cycle = true;
			}
		}
		color[v] = BLACK;
	}

	vector<Vertex> GetCycle(const Graph& graph)
	{
		vector<Vertex> parent(graph.GetVertexCount(), VOID);
		vector<Vertex> cycle(0);
		vector<VertexMark> color(graph.GetVertexCount(), WHITE);

		bool has_cycle = false;

		Vertex cycle_end = 0;

		for (Vertex v = 0; v < graph.GetVertexCount(); v++)
		{
			if (color[v] == WHITE)
			{
				DFS_Visit(graph, v, color, parent, has_cycle, cycle_end);
			}
		}

		Vertex cycle_member = cycle_end;
		if (has_cycle == true)
		{
			cycle.push_back(VOID);
			while (parent[cycle_member] != cycle_end)
			{
				cycle.push_back(cycle_member);
				cycle_member = parent[cycle_member];
			}
			cycle[0] = cycle_member;
		}

		return { cycle.rbegin(), cycle.rend() };
	}

}

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

	vector<Graph::Vertex> cycle = GraphProcessing::GetCycle(graph);

	if (cycle.empty())
	{
		cout << "NO";
	}
	else
	{
		cout << "YES\n";

		for (const auto& v : cycle)
		{
			cout << v + 1 << " ";
		}
	}
	return 0;
}