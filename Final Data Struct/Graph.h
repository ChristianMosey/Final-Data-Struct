#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <thread>
#include "Course.h"
using namespace std;

template<class T>
class Graph {

public:
	struct Vertex {
		Graph* graph;
		vector<pair<Vertex*, float>> adjList;
		unsigned int indegree = 0;
		bool known = false;
		T data;

		Vertex(T _data, Graph* _graph) : data(_data), graph(_graph) {}
		void connectTo(T _data, float weight);
	};

	unordered_map<T, Vertex*> graph;
	vector<T> topsort();
	vector<T> topsortBF();
	float getWeight(T from, T to);
	vector<pair<Vertex*, float>> getAdjacent(T vertex);
	int getVertexCount();
	Vertex& operator[](const T vertData);
};

template<class T>
vector<T> Graph<T>::topsort() {
	vector<T> toporder;
	queue<Vertex*> q;
	int counter = 0;
	q.empty();
	for (auto v : graph)
		if (v.second->indegree == 0)
			q.push(v.second);
	while (q.size() != 0) {
		Vertex* v = q.front();
		q.pop();
		toporder.push_back(v->data); // Assign next number
		for (auto w : v->adjList)
			if (--(*w.first).indegree == 0)
				q.push(w.first);
	}
	return toporder;
}

template<class T>
vector<T> Graph<T>::topsortBF() {
	vector<T> toporder;
	for (int counter = 0; counter < getVertexCount(); counter++) {
		Vertex* q = nullptr;
		for (auto v : graph) {
			if (v.second->known) continue;
			if (v.second->indegree == 0) {
				q = v.second;
				v.second->known = true;
				break;
			}
		}
		if (q == nullptr) continue;
		toporder.push_back(q->data);
		for (auto w : q->adjList) (*w.first).indegree--;
	}
	return toporder;
}

template<class T>
void Graph<T>::Vertex::connectTo(T _data, float weight) {
	Vertex& v = graph->operator[](_data);
	v.indegree++;
	adjList.push_back(make_pair(&v, weight));
}

template<class T>
float Graph<T>::getWeight(T from, T to) {
	if (graph.find(from) == graph.end())
		return 0.0f;
	for (auto itr : graph[from]->adjList) {
		if (itr.first->data == to) return itr.second;
	}
}

template<class T>
vector<pair<typename Graph<T>::Vertex*, float>> Graph<T>::getAdjacent(T vertex) {
	if (graph.find(vertex) == graph.end()) return vector<pair<Vertex*, float>>();
	return graph[vertex]->adjList;
}

template<class T>
int Graph<T>::getVertexCount() {
	return graph.size();
}

template<class T>
typename Graph<T>::Vertex& Graph<T>::operator[](const T vertData) {
	if (graph.find(vertData) == graph.end()) {
		graph[vertData] = new Vertex(vertData, this);
	}
	else {
		graph[vertData]->data = vertData;
	}
	return *graph.at(vertData);
}
