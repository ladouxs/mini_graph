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


	// Ajoute un arc
	// Si un des noeuds n'existe pas, il est crée.
	// Cette méthode gère les doublons : un arc n'est inséré qu'une fois.
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


	// Find a path bread search first
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
			//cout << nextNode.node;

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
			// extract shortest path
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