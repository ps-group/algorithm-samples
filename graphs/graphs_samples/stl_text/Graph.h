#pragma once

#include <stdint.h>
#include <iostream>
#include <boost/variant.hpp>

struct Edge
{
	// Индекс 1-й вершины (начала ребра).
	size_t start = 0;
	// Индекс 2-й вершины (конца ребра).
	size_t end = 0;
	// Цена перехода (вес ребра).
	int64_t weight = 0;
};

struct Vertex
{
	// Стоимость кратчайшего пути из начальной вершины в вершину i.
	int64_t cost = 0;
	// Индекс предыдущей вершины по кратчайшему пути.
	size_t previous = 0;
};

class CGraph
{
public:
	// "Бесконечно" большая стоимость, используется как маркер для непройденных вершин.
	static const int64_t INFINITIVE_COST;

	CGraph();
	~CGraph();

	bool ReadText(std::istream & in);
	void RunBellmanFord(std::ostream & out);

private:
	class CNegativeLoopException : public std::exception
	{
	public:
		CNegativeLoopException(size_t vertexId);
		size_t m_vertexId;
	};

	void InitBellmanFordState();
	void PrintNegativeLoop(size_t vertexId, std::ostream & out);
	void PrintResults(std::ostream & out);
	bool UpdateBellmanFordState(bool isLastIteration);
	void GetShortestPath(size_t vertexId, std::vector<size_t> & shortestPath)const;

	std::vector<Edge> m_edges;
	std::vector<Vertex> m_verticies;
	size_t m_startId = 0;
};
