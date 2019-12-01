#include Graph.h

using namespace tkv

Node::Node(){}

Edge::Edge(Node* firstNode, Node* lastNode){
	this->first = firstNode;
	this->last = lastNode;
}

Edge::SetFirst(Node* n){
	this->first = n;
}

Edge::SetLast(Node* n){
	this->last = n;
}

Graph::Graph(){

}
