#include "stdafx.h"
#include "BoostGraph.h"

#undef max

// "Бесконечно" большая стоимость, используется как маркер для непройденных вершин.
const int64_t CBoostGraph::INFINITIVE_COST = std::numeric_limits<int64_t>::max();


CBoostGraph::CNegativeLoopException::CNegativeLoopException(size_t vertexId)
	: std::exception("negative loop")
	, m_vertexId(vertexId)
{
}

CBoostGraph::CBoostGraph()
{
}


CBoostGraph::~CBoostGraph()
{
}

bool CBoostGraph::ReadText(std::istream & in)
{
	size_t edgeCount = 0;
	size_t vertexCount = 0;
	in >> vertexCount >> edgeCount >> m_startId;
	// индекс уменьшаем на единицу, т.к. индексация вершин начинается с 0.
	--m_startId;

	for (size_t i = 0; i < vertexCount; ++i)
	{
		// add_vertex() возвращает индекс вершины (size_t).
		boost::add_vertex(m_graph);
	}

	for (size_t i = 0; i < edgeCount; ++i)
	{
		Edge edge;
		size_t start = 0;
		size_t end = 0;
		if (!(in >> start >> end >> edge.weight))
		{
			return false;
		}
		// индексы уменьшаем на единицу, т.к. индексация вершин начинается с 0.
		--start;
		--end;

		// add_edge(...) возвращает пару {индексатор ребра, bool}.
		auto result = boost::add_edge(start, end, m_graph);
		m_graph[result.first] = edge;
	}

	return true;
}

bool CBoostGraph::ReadGraphviz(std::istream & in)
{
	// Используем свойство вершины 'root' для обозначения начальной вершины.
	// http://www.graphviz.org/doc/info/attrs.html#d:root
	const char C_IS_START[] = "root";
	const char C_WEIGHT[] = "weight";
	try
	{
		boost::dynamic_properties dp(boost::ignore_other_properties);
		dp.property(C_IS_START, boost::get(&Vertex::isStart, m_graph));
		dp.property(C_WEIGHT, boost::get(&Edge::weight, m_graph));

		boost::read_graphviz(in, m_graph, dp);
	}
	catch (std::exception const& ex)
	{
		std::cerr << "ReadGraphviz failed: " << ex.what() << std::endl;
		return false;
	}

	// Ищем начальную вершину.
	auto vertices = m_graph.vertex_set();
	auto it = std::find_if(vertices.begin(), vertices.end(), [this](size_t vi) {
		return m_graph[vi].isStart;
	});
	if (it == vertices.end())
	{
		std::cerr << "Exactly 1 vertex with [" << C_IS_START << "=true] required." << std::endl;
		return false;
	}
	m_startId = *it;

	return true;
}

void CBoostGraph::RunBellmanFord(std::ostream & out)
{
	try
	{
		InitBellmanFordState();
		auto vertices = m_graph.vertex_set();
		for (size_t vertexId : vertices)
		{
			const bool isLastIteration = (vertexId + 1 == size_t(vertices.size()));
			// Обновляем состояние проходом по всем рёбрам.
			// Оптимизация алгоритма: если состояние не изменилось, прерываем цикл.
			if (!UpdateBellmanFordState(isLastIteration))
			{
				break;
			}
		}
		PrintResults(out);
	}
	catch (CNegativeLoopException const& ex)
	{
		// Найден цикл отрицательной стоимости, печатаем его.
		PrintNegativeLoop(ex.m_vertexId, out);
	}
}

void CBoostGraph::InitBellmanFordState()
{
	/* Обход через итераторы - каждый итератор при разыменовании даёт индекс вершины.
	vertex_iterator_t vi;
	vertex_iterator_t viEnd;
	for (std::tie(vi, viEnd) = boost::vertices(m_graph); vi != viEnd; ++vi)
	{
		m_graph[*vi].cost = INFINITIVE_COST;
	}
	*/

	// Более современный (C++ 2011) вариант обхода вершин:
	for (size_t vertexId : m_graph.vertex_set())
	{
		m_graph[vertexId].cost = INFINITIVE_COST;
	}
	// Инициализируем начальную вершину.
	{
		Vertex &start = m_graph[m_startId];
		start.cost = 0;
		start.previous = m_startId;
	}
}

void CBoostGraph::PrintNegativeLoop(size_t vertexId, std::ostream & out)
{
	// Обходим цикл, пока не вернёмся к vertexId.
	std::vector<size_t> negativeLoop;
	negativeLoop.push_back(vertexId);
	for (;;)
	{
		size_t previous = m_graph[negativeLoop.back()].previous;
		negativeLoop.push_back(previous);
		if (previous == vertexId)
		{
			break;
		}
	}
	// Меняем порядок на обратный, чтобы распечатать цикл с начала.
	std::reverse(negativeLoop.begin(), negativeLoop.end());

	out << "No" << std::endl;
	out << negativeLoop.size();
	for (size_t loopVertexId : negativeLoop)
	{
		out << ' ' << (loopVertexId + 1);
	}
	out << std::endl;
}

void CBoostGraph::PrintResults(std::ostream & out)
{
	std::vector<size_t> shortestPath;
	for (size_t vertexId : m_graph.vertex_set())
	{
		Vertex const& vertex = m_graph[vertexId];
		if (vertex.cost == INFINITIVE_COST)
		{
			out << "No" << std::endl;
		}
		else
		{
			out << vertex.cost << ' ';
			GetShortestPath(vertexId, shortestPath);
			out << shortestPath.size();
			for (size_t pathVertexId : shortestPath)
			{
				out << ' ' << (pathVertexId + 1);
			}
			out << std::endl;
		}
	}
}

bool CBoostGraph::UpdateBellmanFordState(bool isLastIteration)
{
	/* Обход через итераторы - каждый итератор при разыменовании даёт индекс ребра.
	edge_iterator_t ei;
	edge_iterator_t eiEnd;
	for (std::tie(ei, eiEnd) = boost::edges(m_graph); ei != eiEnd; ++ei)
	{
		// do something.
	}
	*/

	/* Более современный (C++ 2011) вариант обхода вершин:
	edge_iterator_t ei;
	edge_iterator_t eiEnd;
	std::tie(ei, eiEnd) = boost::edges(m_graph);
	auto range = boost::make_iterator_range(ei, eiEnd);
	*/

	// Временные переменные не нужны.
	// Пара, возвращённая одной функцией, будет развёрнута в другой.
	auto range = boost::make_iterator_range(boost::edges(m_graph));
	bool hasChanges = false;
	for (auto edge : range)
	{
		int64_t startWeight = m_graph[edge.m_source].cost;
		int64_t endWeight = m_graph[edge.m_target].cost;
		int64_t edgeWeight = m_graph[edge].weight;
		if ((startWeight != INFINITIVE_COST) && (endWeight > startWeight + edgeWeight))
		{
			if (isLastIteration)
			{
				// На последней итерации стоимость для какой-то вершины сократилась.
				// Значит, есть бесконечный цикл.
				throw CNegativeLoopException(edge.m_target);
			}
			else
			{
				m_graph[edge.m_target].cost = startWeight + edgeWeight;
				m_graph[edge.m_target].previous = edge.m_source;
				hasChanges = true;
			}
		}
	}

	return hasChanges;
}

void CBoostGraph::GetShortestPath(size_t vertexId, std::vector<size_t>& shortestPath) const
{
	shortestPath.clear();
	for (size_t i = vertexId; i != m_startId; i = m_graph[shortestPath.back()].previous)
	{
		shortestPath.push_back(i);
	}
	std::reverse(shortestPath.begin(), shortestPath.end());
}
