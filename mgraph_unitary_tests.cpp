#include "mgraph.hpp"
#include <gtest\gtest.h>

using namespace std;
using namespace mini_graph;

TEST(mini_graph, node_struct)
{
	struct Node {};
	Node node_1, node_2;
	graph_node_<Node> gn(node_1);
	addEdge(gn, node_1, node_2);
	EXPECT_FALSE(gn.adj.empty());
}

TEST(mini_graph, node_int)
{
	graph_node_<int> gn(0);
	addEdge<int>(gn, 0, 1);
	EXPECT_FALSE(gn.adj.empty());
}

TEST(mini_graph, three_stars)
{
	mgraph<int> g;
	vector<pair<int, int>> links = {
		{ 28, 36 },	{ 0, 2 },	{ 3, 34 },	{ 29, 21 },	{ 37, 35 },	{ 28, 32 },	{ 0, 10 },	{ 37, 2 },	{ 4, 5 },	{ 13, 14 },
		{ 34, 35 },	{ 27, 19 },	{ 28, 34 },	{ 30, 31 },	{ 18, 26 },	{ 0, 9 },	{ 7, 8 },	{ 18, 24 },	{ 18, 23 },	{ 0, 5 },
		{ 16, 17 },	{ 29, 30 },	{ 10, 11 },	{ 0, 12 },	{ 15, 16 },	{ 0, 11 },	{ 0, 17 },	{ 18, 22 },	{ 23, 24 },	{ 0, 7 },
		{ 35, 23 },	{ 22, 23 },	{ 1, 2 },	{ 0, 13 },	{ 18, 27 },	{ 25, 26 },	{ 32, 33 },	{ 28, 31 },	{ 24, 25 },	{ 28, 35 },
		{ 21, 22 },	{ 4, 33 },	{ 28, 29 },	{ 36, 22 },	{ 18, 25 },	{ 37, 23 },	{ 18, 21 },	{ 5, 6 },	{ 19, 20 },	{ 0, 14 },
		{ 35, 36 },	{ 9, 10 },	{ 0, 6 },	{ 20, 21 },	{ 0, 3 },	{ 33, 34 },	{ 14, 15 },	{ 28, 33 },	{ 11, 12 },	{ 12, 13 },
		{ 17, 1 },	{ 18, 19 },	{ 36, 29 },	{ 0, 4 },	{ 0, 15 },	{ 0, 1 },	{ 18, 20 },	{ 2, 3 },	{ 0, 16 },	{ 8, 9 },
		{ 0, 8 },	{ 26, 27 },	{ 28, 30 },	{ 3, 4 },	{ 31, 32 },	{ 6, 7 },	{ 37, 1 },	{ 37, 24 },	{ 35, 2 }
	};
	vector<int> targets = { 0, 18, 28 };
	const int start = 37, expected_path_size = 3;

	//
	// Build the undirected graph.
	//
	for (auto& e : links)
	{
		addEdge<int>(g, e.first, e.second);
		addEdge<int>(g, e.second, e.first);
	}
	EXPECT_FALSE(g.empty());

	//
	// Retrieves shortests paths using Bread First Search.
	// Note that in this particular test, all expected paths have three elements.
	//
	vector<int> v;
	back_insert_iterator<vector<int> > backiter(v);

	for (auto& e : targets)
	{
		bool lb_found = find_path_bsf<int, back_insert_iterator<vector<int>> >(g, start, e, backiter);
		EXPECT_TRUE(lb_found);
		EXPECT_EQ(expected_path_size, v.size());
		v.clear();
		lb_found = find_path_bsf<int, back_insert_iterator<vector<int>> >(g, e, start, backiter);
		EXPECT_TRUE(lb_found);
		EXPECT_EQ(expected_path_size, v.size());
		v.clear();
	}
}

TEST(mini_graph, unreachable_node)
{
	mgraph<int> g;
	addEdge<int>(g, 0, 1);
	vector<int> v;
	back_insert_iterator<vector<int> > backiter(v);
	bool lb_found = find_path_bsf<int, back_insert_iterator<vector<int>> >(g, 0, 42, backiter);
	EXPECT_FALSE(lb_found);
	v.clear();
	lb_found = find_path_bsf<int, back_insert_iterator<vector<int>> >(g, 42, 0, backiter);
	EXPECT_FALSE(lb_found);
	v.clear();
	lb_found = find_path_bsf<int, back_insert_iterator<vector<int>> >(g, 42, 1099, backiter);
	EXPECT_FALSE(lb_found);
}

