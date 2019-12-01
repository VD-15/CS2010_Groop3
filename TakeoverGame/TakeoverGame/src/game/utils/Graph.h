#pragma once

#include "../../engine/utils/Vector.h"
#include <vector>

namespace tkv {
	struct Node {
		Node();
		Vector3 location;
	};

	struct Edge {
		Edge(Node* firstNode, Node* lastNode);
		SetFirst(Node* n);
		SetLast(Node* n);
	private:
		Node* first;
		Node* last;
	};

	struct Graph {
		Graph();
		std::vector<Edge> edges;
	};
}
