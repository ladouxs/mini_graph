// Created 2017/12/10
// Author: Sylvain Ladoux
//
//
// Licensed under the Apache License, Version 2.0
// (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.

#pragma once

#include <list>
#include <iterator>
#include <map>

namespace mini_graph
{
	using namespace std;

	template<class T>
	class graph_node_
	{
	public:
		graph_node_(T a_node) : node(a_node) {};
		T node;
		list<T> adj;
	};

	template < class T >
	bool operator== (const graph_node_<T> & lhs, const graph_node_<T> & rhs)
	{
		return lhs.node == rhs.node;
	}

	template < class T >
	bool operator< (const graph_node_<T> & lhs, const graph_node_<T> & rhs)
	{
		return lhs.node < rhs.node;
	}

	template<class T>
	graph_node_<T>& addEdge(graph_node_<T>& g, T const node, T const next)
	{
		g.adj.push_back(next);
		return g;
	};

	template<class T>
	void addEdge(typename list< graph_node_<T> >::iterator g, T const node, T const next)
	{
		if (find(g->adj.begin(), g->adj.end(), next) == g->adj.end())
		{
			g->adj.push_back(next);
		}
	};

	template<class T >
	class mgraph
	{
	public:
		list<graph_node_<T> > edges;
		size_t count() { return edges.size(); }
		bool empty() { return edges.empty(); }
	};


	// Add an edge in a directed graph and creates non-existing nodes passed as arguments.
	// Assume the edge is created only once (no dupplicated).
	template<class T>
	auto addEdge(mgraph<T> &g, T const node, T const next)
	{
		graph_node_<T> firstNode(node), nextNode(next);
		auto it = find(g.edges.begin(), g.edges.end(), firstNode);
		if (it != g.edges.end())
		{
			addEdge<T>(it, node, next);
		}
		else
		{
			g.edges.emplace_back(addEdge<T>(firstNode, node, next));
		}
		if (find(g.edges.begin(), g.edges.end(), nextNode) == g.edges.end())
		{
			g.edges.push_back(nextNode);
		}
	};

	template<class T>
	void removeEdge(mgraph<T> &g, T const node, T const next)
	{
		graph_node_<T> tmp(node);
		auto it = find(g.edges.begin(), g.edges.end(), tmp);
		if (it != g.edges.end())
		{
			it->adj.remove(next);
		}
	};

	// Find a path using Bread Search First (bfs).
	template<class T, class OutputIt >
	bool find_path_bsf(mgraph<T> const& g, T const start, T const target, OutputIt nodes)
	{
		list< graph_node_<T> > file, seen;
		map<T, T> fathers;
		auto startNode = find(g.edges.begin(), g.edges.end(), graph_node_<T>(start));
		if (startNode != g.edges.end())
		{
			file.push_back(*startNode);
			seen.push_back(*startNode);
		}
		while (!file.empty())
		{
			graph_node_<T> nextNode = file.front();
			file.pop_front();

			// TODO : add visitor here (proceed with node)

			for (auto& next : nextNode.adj)
			{
				if (find(seen.begin(), seen.end(), graph_node_<T>(next)) == seen.end())
				{
					auto child = find(g.edges.begin(), g.edges.end(), graph_node_<T>(next));
					file.push_back(*child);
					seen.push_back(*child);
					fathers[child->node] = nextNode.node;
				}
			}
		}

		if (find(seen.begin(), seen.end(), graph_node_<T>(target)) != seen.end())
		{
			// Extract shortest path
			list<T> shortestPath;
			shortestPath.push_back(target);
			auto e = target;
			do
			{
				auto father = fathers[e];
				shortestPath.push_front(father);
				e = father;
			} while (e != start);

			for (auto& e : shortestPath)
			{
				*nodes = e;
				nodes++;
			}
			return true;
		}
		return false;
	};

} /// namespace mini_graph