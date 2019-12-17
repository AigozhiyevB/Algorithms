#include <iostream>
#include <algorithm>
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

namespace GraphProcessing {
	typedef size_t Vertex;
	enum VertexMark { WHITE, GRAY, BLACK };
	const Vertex NO_VERTEX = -1;
	const size_t INF = -1;

	void dfsVisit(Vertex& v, const Graph& g, std::vector<VertexMark>& color, std::vector<bool>& is_cutpoint,
		size_t timer, std::vector<size_t>& tin, std::vector<size_t>& fup,
		Vertex prev = NO_VERTEX) 
	{
		color[v] = GRAY;
		tin[v] = fup[v] = timer;
		++timer;
		size_t precestor = 0;
		for (Vertex& u : g.GetNeighbors(v))
		{
			if (prev == u)
			{
				continue;
			}
			if (color[u] == GRAY)
			{
				fup[v] = std::min(tin[u], fup[v]);
			}
			if (color[u] == WHITE) 
			{
				dfsVisit(u, g, color, is_cutpoint, timer, tin, fup, v);
				fup[v] = std::min(fup[v], fup[u]);
				if (fup[u] >= tin[v] && prev != NO_VERTEX) 
				{
					is_cutpoint[v] = true;
				}
				precestor++;
			}
		}
		if (prev == NO_VERTEX && precestor > 1)
		{
			is_cutpoint[v] = true;
		}
	}

	std::vector<Vertex> findCutpoints(const Graph& g)
	{
		std::vector<Vertex> cutpoints(0);
		std::vector<VertexMark> color(g.GetVertexCount(), WHITE);
		std::vector<bool> is_cutpoint(g.GetVertexCount(), false);
		std::vector<size_t> tin(g.GetVertexCount(), INF);
		std::vector<size_t> fup(g.GetVertexCount(), INF);
		size_t timer = 0;

		for (Vertex v = 0; v < g.GetVertexCount(); v++)
		{
			if (color[v] == WHITE)
			{
				dfsVisit(v, g, color, is_cutpoint, timer, tin, fup);
			}
		}
		for (size_t i = 0; i < g.GetVertexCount(); i++)
		{
			if (is_cutpoint[i]) 
			{
				cutpoints.push_back(i);
			}
		}

		return cutpoints;
	}
}

int main() {
	size_t vertex_count, edges_count;
	std::cin >> vertex_count >> edges_count;
	GraphAdjList graph(vertex_count);

	for (size_t i = 0; i < edges_count; i++) 
	{
		Graph::Vertex from, to;
		std::cin >> from >> to;
		graph.addEdge(from - 1, to - 1);
	}

	std::vector<Graph::Vertex> cutpoints = GraphProcessing::findCutpoints(graph);

	std::cout << cutpoints.size() << "\n";

	for (Graph::Vertex& cutpoint : cutpoints)
	{
		std::cout << cutpoint + 1 << "\n";
	}

	return 0;
}