#include <iostream>
#include <vector>
#include <queue>
#include <climits>

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

	void BFS(const Graph& g, const Vertex& v,
		std::vector<size_t>& distance, std::vector<Vertex>& parent)
	{
		std::vector<VertexMark> color(g.GetVertexCount(), WHITE);
		color[v] = GRAY;
		distance[v] = 0;

		std::queue<Vertex> q;
		q.push(v);
		while (!q.empty()) 
		{
			Vertex u = q.front();
			q.pop();
			for (Vertex w : g.GetNeighbors(u)) 
			{
				if (color[w] == WHITE) 
				{
					parent[w] = u;
					color[w] = GRAY;
					distance[w] = distance[u] + 1;
					q.push(w);
				}
			}
			color[u] = BLACK;
		}
	}

	std::vector<Vertex> findShortestWay(Vertex from, Vertex to, const Graph& g)
	{
		std::vector<size_t> distance(g.GetVertexCount(), UINT_MAX);
		std::vector<Vertex> parent(g.GetVertexCount(), UINT_MAX);

		BFS(g, from, distance, parent);

		std::vector<Vertex> inverted_path(0);

		if (distance[to] == UINT_MAX)
		{
			return inverted_path;
		}

		inverted_path.push_back(to);
		
		while (to != from) 
		{
			to = parent[to];
			inverted_path.push_back(to);
		}
		
		return { inverted_path.rbegin(), inverted_path.rend() };
	}

	std::vector<Vertex> GetNeighboursChess(Vertex v, size_t table_size)
	{
		std::vector<Vertex> neighbors(0);

		std::vector<int> dx = { -2, -1, 1, 2 };
		std::vector<int> dy = { -1, -2, 2, 1 };

		size_t v_x = v / table_size;
		size_t v_y = v % table_size;
		
		for (size_t i = 0; i < dx.size(); i++) 
		{
			if (v_x + dx[i] >= 0 &&
				v_x + dx[i] < table_size &&
				v_y + dy[i] >= 0 &&
				v_y + dy[i] < table_size) 
			{
				Vertex neighbor = (v_x + dx[i]) * table_size + v_y + dy[i];
				neighbors.push_back(neighbor);
			}

			if (v_x + dx[i] >= 0 &&
				v_x + dx[i] < table_size &&
				v_y + dy[3 - i] >= 0 &&
				v_y + dy[3 - i] < table_size) 
			{
				Vertex neighbor = (v_x + dx[i]) * table_size + v_y + dy[3 - i];
				neighbors.push_back(neighbor);
			}
		}

		return neighbors;
	}
}

int main()
{
	size_t table_size;
	cin >> table_size;

	GraphAdjList graph(table_size * table_size);

	for (Graph::Vertex v = 0; v < table_size * table_size; ++v) 
	{
		std::vector<size_t> neighbor = GraphProcessing::GetNeighboursChess(v, table_size);
		for (Graph::Vertex u : neighbor) 
		{
			graph.addEdge(v, u);
		}
	}

	size_t x1, y1, x2, y2;
	std::cin >> x1 >> y1 >> x2 >> y2;
	Graph::Vertex begin = (--x1) * table_size + (--y1);
	Graph::Vertex end = (--x2) * table_size + (--y2);

	vector<Graph::Vertex> way = GraphProcessing::findShortestWay(begin, end, graph);

	cout << way.size() - 1 << endl;

	for (Graph::Vertex& v : way) 
	{
		cout << (v / table_size) + 1 << " " << (v % table_size) + 1 << endl;
	}

	return 0;
}