#pragma once
#include "Sequence.h"
class Vertex :
	public Sequence
{
public:
	int vert_id;
	vector<int> neighbours;
	bool operator< (const Vertex &other) const {
		return neighbours.size() > other.neighbours.size();
	}
	Vertex();
	~Vertex();
};

