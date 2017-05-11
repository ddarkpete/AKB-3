// AKB_ZAD_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//FUNKCJE



vector<Input> load_instance()
{
	
	vector<Input> inputs;
	fstream inp_file;
	fstream inp_file2;
	string open_file; 
	string open_fasta;
	string open_qual;
	string line;
	string line2;
	string seq_line = "";
	int x = 0;
	
	cout << "Prosze podac nazwe pliku: ";
	cin >> open_file;
	open_fasta = open_file + ".fasta";
	open_qual = open_file + ".qual";
	inp_file.open(open_fasta);
	inp_file2.open(open_qual);
	Input inp1;
	if (inp_file.good())
	{
		cout << endl;
		cout << "Plik otwarty poprawnie" << endl;
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
					cout << inp1.input_id << " "<< line << endl;
				}
				else
				{
					cout << line << endl;
					seq_line += line;
					inp1.whole_sequence = seq_line;
					inputs.push_back(inp1);
					seq_line = "";
				}

			}


		}

	}
	else
	{
		cout << "Problem z otwieraniem pliku .fasta " << endl;
		system("pause");
	}
	if (inp_file2.good())
	{


		while (getline(inp_file2, line2))
		{
			if (line2[0] == '>')
			{

				if (line2 != inputs[x].input_id)
				{
					cout << line2 << "Plik  .fasta  nie pasuje do pliku .qual " << endl;
					cout << inputs[x].input_id << endl;;
					cout << " " << x;
					system("pause");
				}
			}
			else if (line2.size() > 0)
			{
				string num = "";
				int i = 0;
				
				while (i < line2.size() )
				{
					if (line2[i] == ' ')
					{
						i++;
						continue;
					}
					else
					{
						num = line2[i];
						num += line2[i + 1];
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
		cout << "Problem z otwieraniem pliku  .qual " << endl;
	}
	

	return inputs;

}
vector<Sequence> load_seqs(vector<Input> instances, int len , int minimal , int deletions)
{
	vector<Sequence> seqs;
	Input temp;
	int allindex=0;
	for (std::vector<int>::size_type it = 0; it != instances.size(); it++) 
	{
		temp = instances[it];
		int sequence_index = 0;
		string sequence = temp.whole_sequence;
		vector<int> scores = temp.quals;
		for (int i = 0; i <= sequence.size() - len  ; i++)
		{	
			Sequence loaded;
			string loaded_seq = "";
			int len_help = 0;
			int too_low = 0;
			int iter = i;
			int start_pos;
			int end_pos;
			while (len_help < len)
			{
				if (scores[iter] >= minimal)
				{
					if (loaded_seq == "")
					{
						start_pos = iter;
						loaded_seq += sequence[iter];
						len_help++;
						iter++;
					}
					else
					{
						loaded_seq += sequence[iter];
						len_help++;
						end_pos = i;
						iter++;
					}
					
				}
				else // jak score za ma³y
				{
					too_low++;
					len_help++;
					iter++;
					
				}
				
			}
			if (too_low < deletions)//dopuszczalna liczba delecji
			{
				loaded.index_inseq = sequence_index;
				sequence_index++;
				loaded.index_inall = allindex;
				allindex++;
				loaded.id = temp.input_id;
				loaded.nucleos = loaded_seq;
				loaded.start_pos = start_pos;
				loaded.end_pos = end_pos;
				seqs.push_back(loaded);
				
					/*cout << loaded.id << endl;
					cout << sequence_index << endl;
					cout << loaded_seq << endl;*/
				
			}

		

		}
		
	}
return seqs;
}



vector<Vertex> load_graph(vector<Sequence> Seqs)
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
		new_vert.index_inall = Seqs[it].index_inall;
		new_vert.index_inseq = Seqs[it].index_inseq;
		new_vert.vert_id = id;
		id++;
		Graph.push_back(new_vert);
	}
	for (std::vector<int>::size_type it = 0; it != Graph.size(); it++)// tworzenie krawedzi w grafie
	{
		for (std::vector<int>::size_type itt = 0; itt != Graph.size(); itt++)
		{
			if (itt != it)
			{
				if ((Graph[it].nucleos.find(Graph[itt].nucleos) != std::string::npos) && Graph[it].id != Graph[itt].id)//jesli podciag zawiera sie w drugim i s¹ z innych sek
				{
					

					if (!(find(Graph[it].neighbours.begin(), Graph[it].neighbours.end(), Graph[itt].vert_id) != Graph[it].neighbours.end()) &&// jesli nie ma jeszcze takiego sasiedztwa
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



vector<Vertex> find_clique(vector<Vertex> vertexes_set)
{
	vector<Vertex> Clique;
	for (int i = 0; i < vertexes_set.size(); i++)
	{
		sort(vertexes_set.begin(), vertexes_set.end());
		if (i == 0)
		{
			Clique.push_back(vertexes_set[i]);// dodaje se pierwszy z posortowanej listy po najwiekszym stopniu verta
		}
		else
		{
			int analyzed_id = vertexes_set[i].vert_id;
			int check_counter = 0;
			for (int j = 0; j < Clique.size(); j++)
			{
				if (std::find(Clique[j].neighbours.begin(), Clique[j].neighbours.end(), analyzed_id) != Clique[j].neighbours.end())
				{//jeœli jest s¹siadem jednego z cz³onków kliki
					check_counter++;
				}
				if (check_counter == Clique.size())//jak znajduje sie w s¹siedztwie wszystkich aktualnych cz³onków kliki
				{
					Clique.push_back(vertexes_set[i]);
					break;
				}
			}


		}

	}
	return Clique;
}

void motif_searcher(vector<Vertex> graph_sorted, vector<Vertex> graph , vector<Input> inputs)
{
	string act_mot;
	int nres = 0;
	int maxres = 6;
	int windowsize = 4;//substr_len / 2;

	vector<string> results;

	while (nres < maxres)//ilu motywow szukac
	{
		vector<Vertex> StartClique = find_clique(graph_sorted);
		vector<Vertex> StartCliqueR = StartClique;
		map<string, int > main_occurance;
		for (int v = 0; v < StartClique.size(); v++)
		{
			if (main_occurance.count(StartClique[v].nucleos) > 0)
			{
				main_occurance[StartClique[v].nucleos]++;
			}
			else
			{
				main_occurance[StartClique[v].nucleos];//incjalizacja
				main_occurance[StartClique[v].nucleos]++;
			}
		}
		int main_currentMax = 0;
		string main_arg_max = "";

		for (auto it = main_occurance.cbegin(); it != main_occurance.cend(); ++it)
		{
			if (it->second > main_currentMax) {
				main_arg_max = it->first;
				main_currentMax = it->second;
			}
		}
		act_mot = main_arg_max;
		//cout << main_arg_max << endl;


		int past_len = act_mot.length();
		vector<Vertex> TempClique;
		bool left_side = true;
		bool right_side = true;


		//ROZSZERZANIE W LEWO

		while (left_side)
		{
			vector<Vertex> left;
			for (int i = 0; i < StartClique.size(); i++)
			{
				if (StartClique[i].index_inall - windowsize >= 0 && StartClique[i].id == graph[StartClique[i].index_inall - windowsize].id)
				{
					int left_ind = StartClique[i].index_inall - windowsize;
					left.push_back(graph[left_ind]);

				}
			}
			if (left.size() > 2)//jeœli jest sens szukaæ kliki
			{
				TempClique = find_clique(left);
				if (TempClique.size() > 2)
				{
					StartClique = TempClique;
					map<string, int > occurance;
					for (int v = 0; v < TempClique.size(); v++)
					{
						if (occurance.count(TempClique[v].nucleos) > 0)
						{
							occurance[TempClique[v].nucleos]++;
						}
						else
						{
							occurance[TempClique[v].nucleos];//incjalizacja
							occurance[TempClique[v].nucleos]++;
						}
					}
					int currentMax = 0;
					string arg_max = "";
					for (auto it = occurance.cbegin(); it != occurance.cend(); ++it)
					{
						if (it->second > currentMax) {
							arg_max = it->first;
							currentMax = it->second;
						}
					}
					for (int i = windowsize; i >= 0; i--)
					{
						if (i == 0)
						{
							act_mot = arg_max + act_mot;
							//cout << act_mot << endl;
						}

						string submotif_sub = arg_max.substr(i, arg_max.length());
						string motif_sub = act_mot.substr(0, arg_max.length() - i);
						if (submotif_sub == motif_sub)
						{
							act_mot = arg_max.substr(0, i) + act_mot;
							//cout << act_mot << endl;
							break;
						}

					}



				}
				else left_side = false;
			}
			else
			{
				left_side = false;
			}


		}


		TempClique.clear();
		StartClique = StartCliqueR;

		//ROZSZERZANIE W PRAWO

		while (right_side)
		{
			vector<Vertex> right;
			for (int i = 0; i < StartClique.size(); i++)
			{
				if (StartClique[i].index_inall + windowsize < graph.size() && StartClique[i].id == graph[StartClique[i].index_inall + windowsize].id)
				{
					int right_ind = StartClique[i].index_inall + windowsize;
					right.push_back(graph[right_ind]);

				}
			}
			if (right.size() > 2)//jeœli jest sens szukaæ kliki
			{
				TempClique = find_clique(right);//szukamy kliki
				if (TempClique.size() > 2)
				{
					StartClique = TempClique;
					map<string, int > rightoccurance;
					for (int v = 0; v < TempClique.size(); v++)
					{
						if (rightoccurance.count(TempClique[v].nucleos) > 0)
						{
							rightoccurance[TempClique[v].nucleos]++;
						}
						else
						{
							rightoccurance[TempClique[v].nucleos];//incjalizacja
							rightoccurance[TempClique[v].nucleos]++;
						}
					}
					int currentMax = 0;
					string arg_max = "";

					for (auto it = rightoccurance.cbegin(); it != rightoccurance.cend(); ++it)
					{
						if (it->second > currentMax) {
							arg_max = it->first;
							currentMax = it->second;
						}
					}

					for (int i = windowsize; i >= 0; i--)
					{
						if (i == 0)
						{
							act_mot = act_mot + arg_max.substr(i, arg_max.length());
						}

						int test = arg_max.length();

						string submotif_sub = arg_max.substr(0, arg_max.length() - i);
						string motif_sub = act_mot.substr(act_mot.length() - (arg_max.length() - i), act_mot.length());

						if (submotif_sub == motif_sub)
						{
							act_mot = act_mot + arg_max.substr(arg_max.length() - i);
							//cout << act_mot << endl;
							break;
						}

					}

					right.clear();

				}
				else right_side = false;
			}
			else
			{
				right_side = false;
			}


		}
		if (!(std::find(results.begin(), results.end(), act_mot) != results.end()))
		{
			results.push_back(act_mot);
		}

		graph_sorted.erase(graph_sorted.begin());
		nres++;


	}

	compare comp;

	sort(results.begin(), results.end(), comp);

	for (int i = 0; i < results.size(); i++)
	{
		cout << results[i] << endl;

	}
	//cout << results[i] << endl;
	cout << endl;
	int best_c = 1;
	if (results.size() > 1 && results[0].length() == results[1].length())
	{
		for (int i = 1; i < results.size(); i++)
		{
			if (results[i].length() == results[0].length()) best_c++;
		}
	}


	cout << "Sekwencje z delcjami zawierajace motyw(y):" << endl;
	cout << endl;

	for (int j = 0; j < best_c; j++)
	{
		cout << endl;
		cout << results[j] << endl;
		cout << endl;
		cout << results[j].length() << endl;
		cout << endl;

		for (int i = 0; i < inputs.size(); i++)
		{
			cout << inputs[i].input_id << endl;
			if (inputs[i].sequence_with_del.find(results[j]) != string::npos)
			{
				string seq_part_1 = inputs[i].sequence_with_del.substr(0, inputs[i].sequence_with_del.find(results[j]));
				string seq_part_2 = inputs[i].sequence_with_del.substr(inputs[i].sequence_with_del.find(results[j]) + results[j].length(), inputs[i].sequence_with_del.length());
				cout << seq_part_1;
				SetConsoleTextAttribute(hConsole, 2);
				cout << results[j];
				SetConsoleTextAttribute(hConsole, 7);
				cout << seq_part_2;
				cout << endl;
			}
			else cout << inputs[i].sequence_with_del << endl;
		}
		cout << endl;
	}
}

struct compare {
	bool operator()(const std::string& first, const std::string& second) {
		return first.size() > second.size();
	}
};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


int main()
{
	vector<Input> inputs;
	vector<Sequence> graph_sequences;
	vector<Vertex> graph;
	vector<Vertex> graph_sorted;
	

	int substr_len;
	int minimal_score;
	int deletions;
	


	cout << "Prosze podac minimalny qual: ";
	while (true)
	{
		cin >> minimal_score;
		if (minimal_score < 1 )
		{
			cout << endl;
			cout << "Prosze podac qual wiekszy niz 0" << endl;
			cin >> minimal_score;
		}
		else break;
	}
	
	cout << endl;
	cout << "Prosze podac dlugosc podciagow (4-7): ";
	while (true)
	{
		cin >> substr_len;
		if (substr_len < 4 || substr_len > 7)
		{
			cout << endl;
			cout << "Prosze podac prawidlowa dlugosc podciagow(4-7)" << endl;
			cin >> substr_len;
		}
		else break;
	}
	cout << endl;
	
	cout << "Prosze podac liczbe dopuszczonych delecji w podciagu( conajmniej 1/2 dlugosci podciagu): ";
	while (true)
	{
		cout << endl;
		cin >> deletions;
		if (deletions > substr_len/2)
		{
			cout << "Prosze podac prawidlowa dlugosc delecji" << endl;
			cin >> deletions;
		}
		else break;
	}


	inputs = load_instance();

	//TWORZENIE SEKWENCJI SKROCONYCH O DELECJE

	for (int it = 0; it < inputs.size(); it++)
	{
		inputs[it].sequence_with_del = "";
		for (int c = 0; c < inputs[it].whole_sequence.size(); c++)
		{
			if (inputs[it].quals[c] >= minimal_score)
			{
				inputs[it].sequence_with_del = inputs[it].sequence_with_del + inputs[it].whole_sequence[c];
			}
		}
	}

	


	graph_sequences = load_seqs(inputs, substr_len,  minimal_score , deletions);
	graph = load_graph(graph_sequences);
	graph_sorted = graph;
	sort(graph_sorted.begin(), graph_sorted.end());
	motif_searcher(graph_sorted, graph, inputs);


	
	
	

	system("pause");
	return 0;
}


