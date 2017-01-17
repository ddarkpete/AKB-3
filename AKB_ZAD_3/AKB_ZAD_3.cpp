// AKB_ZAD_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//wyjebac z klasy input load , vector inputów dac wmainie funkcje load daj w mainie i chuj i czeœæ



vector<Input> load_instance()
{
	vector<Input> inputs;
	fstream inp_file;
	fstream inp_file2;
	string open_file = "fasta";
	string open_fasta;
	string open_qual;
	string line;
	string line2;
	string seq_line = "";
	int x = 0;
	cout << "Please type the instance name: ";
	open_fasta = open_file + ".fasta";
	open_qual = open_file + ".qual";
	inp_file.open(open_fasta);
	inp_file2.open(open_qual);
	Input inp1;
	if (inp_file.good())
	{
		cout << "File was opened correctly." << endl;
		while (getline(inp_file, line))
		{
			if (line[0] == '>')
			{
				inp1.input_id = line;
			}
			else
			{
				if (seq_line == "")
				{
					seq_line = line;
					cout << inp1.input_id << line << endl;
				}
				else
				{
					cout << line << endl;
					seq_line += line;
					inp1.whole_sequence = seq_line;
					//cout << inp1.input_id << inp1.whole_sequence << endl;
					inputs.push_back(inp1);
					seq_line = "";
				}

			}


		}

	}
	else
	{
		cout << "Error! Can't open .fasta file." << endl;
		system("pause");
	}
	cout << inputs.size() << "size" << endl;
	if (inp_file2.good())
	{


		while (getline(inp_file2, line2))
		{
			if (line2[0] == '>')
			{

				if (line2 != inputs[x].input_id)
				{
					cout << line2 << "Error! .fasta does not match .qual file." << endl;
					cout << inputs[x].input_id;
					cout << " " << x;
					system("pause");
				}
				//else if (x != 0) x++;
			}
			else if (line2.size() > 0)
			{
				string num = "";
				int i = 0;
				//cout << line2 << endl;

				while (i < line2.size() - 1)
				{
					//cout << "tu sie jebie" << endl;
					if (line2[i] == ' ')
					{
						i++;
						//cout << "Dupa" << endl;
						continue;
					}
					else
					{
						num = line2[i];
						num += line2[i + 1];
						//cout << num << " " << i << endl;
						inputs[x].quals.push_back(atoi(num.c_str()));
						num = "";
						i += 2;
					}
				}
			}
			else x++;

		}

	}
	else
	{
		cout << "Error with opening .qual file" << endl;
	}
	//cout << inputs.size() << endl;


	return inputs;

}
vector<Sequence> load_seqs(vector<Input> instances, int len , int minimal)//w mainie musi byæ coœ równa siê tej funkcji
{
	vector<Sequence> seqs;
	Input temp;
	for (std::vector<int>::size_type it = 0; it != instances.size(); it++) {
		temp = instances[it];
		string sequence = temp.whole_sequence;
		vector<int> scores = temp.quals;
		for (int i = 0; i <= sequence.size() - len ; i++)
		{	//TODO
			//jakiœ while ¿eby puszcza³ od 4-7 i ograniczenie ze wzgledu na score np jakas
			//zmienna pomocnicza ktora nie bedzie sie zwiekszac jak score bedzie za maly i czyli jeszcze jedna zmiennna
			//puscic do funkcji
			//i z fartem
			Sequence loaded;
			string loaded_seq = "";
			int len_help = 0;
			int too_low = 0;
			int iter = i;
			while (len_help < len)
			{
				if (scores[iter] >= minimal)
				{
					loaded_seq += sequence[iter];
					len_help++;
					iter++;
				}
				else // jak score za ma³y
				{
					too_low++;
					len_help++;
					iter++;
				}
				
			}
			if (too_low < len / 2)
			{
				loaded.id = temp.input_id;
				loaded.nucleos = loaded_seq;
				loaded.start_pos = i;
				loaded.end_pos = i + len - 1;
				seqs.push_back(loaded);
			}

		

		}
	}
	return seqs;
}

vector<Vertex> load_graph(vector<Sequence> Seqs)//to raczej bêdzie vector i przepisanie na jakiœ vector w mainie
{
	int id = 0;
	vector<Vertex> Graph;
	for (std::vector<int>::size_type it = 0; it != Seqs.size(); it++)
	{
		Vertex new_vert;
		new_vert.id = Seqs[it].id;
		new_vert.nucleos = Seqs[it].nucleos;
		new_vert.start_pos = Seqs[it].start_pos;
		new_vert.end_pos = Seqs[it].end_pos;
		new_vert.vert_id = id;
		id++;
		Graph.push_back(new_vert);
	}
	for (std::vector<int>::size_type it = 0; it != Graph.size(); it++) 
	{
		for (std::vector<int>::size_type itt = 0; itt != Graph.size(); itt++)
		{
			if (itt != it)
			{
				if (Graph[it].nucleos.find(Graph[itt].nucleos) != std::string::npos)
				{
					cout << "som takie same xD" << endl;

					if ( !(find(Graph[it].neighbours.begin(), Graph[it].neighbours.end(), Graph[itt].vert_id) != Graph[it].neighbours.end()) &&
						!(find(Graph[itt].neighbours.begin(), Graph[itt].neighbours.end(), Graph[it].vert_id) != Graph[itt].neighbours.end()))
					{
						Graph[itt].neighbours.push_back(Graph[it].vert_id);
						Graph[it].neighbours.push_back(Graph[itt].vert_id);

					}
				}
			}
		}
	}
	return Graph;

}

vector<Vertex> graphSorter(vector<Vertex> graph) 
{
	vector< Vertex > graph_copy = graph;
	sort(graph_copy.begin(), graph_copy.end());
	//reverse(graph_copy.begin(), graph_copy.end());
	return graph_copy;
}

vector<Vertex> find_clique(vector<Vertex> graph, Vertex Analyzed, vector<Vertex> Clique)
{
	for (int i = 0; i < Analyzed.neighbours.size(); i++)
	{

	}

}


int main()
{
	vector<Input> inputs;
	vector<Sequence> graph_sequences;
	vector<Vertex> graph;
	vector<Vertex> graph_sorted;
	int substr_len;
	int minimal_score;
	//Input inpp =  new Input;
	
	//cout << inputs.size()<<endl;
	/*for (int i = 0; i < inputs.size(); i++)
	{
		cout << inputs[i].input_id<<endl;
		cout << inputs[i].whole_sequence << endl;
	}*/
	cout << "Type minimal qual score: ";
	cin >> minimal_score;
	cout << endl;
	cout << "Type lenght of sequences substrings (4-7): ";
	while (true)
	{
		cin >> substr_len;
		if (substr_len < 4 || substr_len > 7)
		{
			cout << "Please type lenght of substrings(4-7)" << endl;
		}
		else break;
	}
	inputs = load_instance();
	graph_sequences = load_seqs(inputs, substr_len, 1 /*minimal_score*/);
	graph = load_graph(graph_sequences);
	graph_sorted = graphSorter(graph);
	for (int i = 0; i < graph_sorted.size(); i++)
	{

	}
	
	system("pause");
    return 0;
}
//wierzcholek z najwieksz¹ liczba sasiadow jako pocz¹tek kliki
//pierwszy s¹siad tez do kliki
/*pierwszy sasiad tego sasiada wyzej taki co jest rozny od obecnych w klice  ,
trzeba sprawdziæ czy on ma na liscie s¹siadów tych wierzcholkow co sa juz w klice*/

//jak ma to do kliki

//czy klike budowaæ na podstawie tej samej co graf ( string w stringu) ????

//warunek zakonczenia ? jak przejdziemy liste sasiadow tego pierwszego?
//to sie troche prosi o rekurencje ale nwm 

