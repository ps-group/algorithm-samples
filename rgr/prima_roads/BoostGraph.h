#pragma once
#include "stdafx.h"

struct Edge
{
    // Цена асфальтирования (вес ребра).
    int64_t price = 0;
    bool accepted = false;
};

struct Vertex
{
    bool accepted = false;
};

class CBoostGraph
{
public:
    using StepHandler = std::function<void(const std::string &dotCode)>;

	CBoostGraph();
	~CBoostGraph();

    void SetStepHandler(const StepHandler & handler);

    bool ReadText(std::istream & in);
    void RunPrima(std::ostream & outs);

private:
    using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;
    using edge_iterator = boost::graph_traits<graph_t>::edge_iterator;
    using vertex_iterator = boost::graph_traits<graph_t>::vertex_iterator;
    using edge_descriptor = boost::graph_traits<graph_t>::edge_descriptor;

    std::vector<edge_descriptor> FindMinimalTree();
    void SerializeCurrentStep() const;
    void PrintResults(std::vector<edge_descriptor> && minimalTree, std::ostream & out);

    graph_t m_graph;
    StepHandler m_stepHandler;
};

