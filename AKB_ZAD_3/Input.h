#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


class Input
{
public:
	Input();
	~Input();
	string input_id;
	string whole_sequence;
	vector<int> quals;
	//vector<Input> instances;
	
};

