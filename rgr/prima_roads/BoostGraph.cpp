#include "stdafx.h"
#include "BoostGraph.h"


CBoostGraph::CBoostGraph()
{
}


CBoostGraph::~CBoostGraph()
{
}

// Устанавливает функцию-обработчик шага алгоритма.
// Функция принимает строку - сериализованное в dot состояние графа.
void CBoostGraph::SetStepHandler(const CBoostGraph::StepHandler &handler)
{
    m_stepHandler = handler;
}

bool CBoostGraph::ReadText(std::istream & in)
{
	size_t edgeCount = 0;
	size_t vertexCount = 0;
    if (!(in >> vertexCount >> edgeCount))
    {
        return false;
    }

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
        if (!(in >> start >> end >> edge.price))
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

void CBoostGraph::RunPrima(std::ostream & out)
{
    PrintResults(FindMinimalTree(), out);
}

std::vector<CBoostGraph::edge_descriptor> CBoostGraph::FindMinimalTree()
{
    SerializeCurrentStep();

    auto vertices = m_graph.vertex_set();
    std::unordered_set<size_t> remainingIds(vertices.begin(), vertices.end());

    // Определим вспомогательную λ-функцию, отмечающую вершину как пройденную.
    auto markVertex = [&](size_t vertexId) {
        remainingIds.erase(vertexId);
        m_graph[vertexId].accepted = true;
    };
    markVertex(vertices.front());

    SerializeCurrentStep();

    std::vector<edge_descriptor> minimalTree;
    std::vector<edge_descriptor> candidates;

    while (!remainingIds.empty())
    {
        candidates.clear();
        // Пара, возвращённая одной функцией, будет развёрнута в другой.
        for (const auto &edge : boost::make_iterator_range(boost::edges(m_graph)))
        {
            // Выбираем в массив кандидатов рёбра, ровно один конец которых уже был посещён.
            if (remainingIds.count(edge.m_source) != remainingIds.count(edge.m_target))
            {
                candidates.push_back(edge);
            }
        }
        // Находим лучшее ребро из тех, что можно добавить.
        auto compareFn = [this](const edge_descriptor &a, const edge_descriptor &b) {
            return m_graph[a].price < m_graph[b].price;
        };
        edge_descriptor bestEdge = *std::min_element(candidates.begin(), candidates.end(), compareFn);

        // Добавляем ребро в минимальное дерево и помечаем вершины как пройденные
        m_graph[bestEdge].accepted = true;
        minimalTree.push_back(bestEdge);
        markVertex(bestEdge.m_source);
        markVertex(bestEdge.m_target);
        SerializeCurrentStep();
    }

    return minimalTree;
}

void CBoostGraph::SerializeCurrentStep() const
{
    if (!m_stepHandler)
    {
        return;
    }
    std::stringstream out;

    auto vertexWriter = [&](std::ostream& out, size_t vertexId) {
        const char *color = (m_graph[vertexId].accepted ? "FF0000" : "C0FFC0");
        out << "[style=\"filled\", fillcolor=\"#" << color << "\"]";
    };
    auto edgeWriter = [&](std::ostream& out, edge_descriptor edgeId) {
        unsigned width = (m_graph[edgeId].accepted ? 5 : 2);
        out << "[penwidth=" << width << "]";

    };

    boost::write_graphviz(out, m_graph, vertexWriter, edgeWriter);
    m_stepHandler(out.str());
}

void CBoostGraph::PrintResults(std::vector<edge_descriptor> &&minimalTree, std::ostream & out)
{
    std::vector<edge_descriptor> edges(minimalTree);
    int64_t cost = 0;

    for (edge_descriptor & edge : edges)
    {
        cost += m_graph[edge].price;
        if (edge.m_source > edge.m_target)
        {
            std::swap(edge.m_source, edge.m_target);
        }
    }
    std::sort(edges.begin(), edges.end(), [](const edge_descriptor &a, const edge_descriptor &b) {
        return a.m_source < b.m_source || (a.m_source == b.m_source && a.m_target < b.m_target);
    });

    out << cost << std::endl;
    for (edge_descriptor & edge : edges)
    {
        out << (edge.m_source + 1) << " " << (edge.m_target + 1) << std::endl;
    }
}
