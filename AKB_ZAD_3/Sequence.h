#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Sequence
{
public:
	Sequence();
	~Sequence();
	string id;
	string nucleos;
	int start_pos;
	int end_pos;
	int index_inseq;
	int index_inall;

};

