// AKB_ZAD_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//TEST!!!!!!



vector<Input> load_instance()
{
	vector<Input> inputs;
	fstream inp_file;
	fstream inp_file2;
	string open_file = "002";
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
	//cout << inputs.size() << "size" << endl;
	if (inp_file2.good())
	{


		while (getline(inp_file2, line2))
		{
			if (line2[0] == '>')
			{

				if (line2 != inputs[x].input_id)
				{
					cout << line2 << "Error! .fasta does not match .qual file." << endl;
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
		cout << "Error with opening .qual file" << endl;
	}
	//cout << inputs.size() << endl;


	return inputs;

}
vector<Sequence> load_seqs(vector<Input> instances, int len , int minimal)//w mainie musi by� co� r�wna si� tej funkcji
{
	vector<Sequence> seqs;
	Input temp;
	int allindex=0;
	for (std::vector<int>::size_type it = 0; it != instances.size(); it++) {
		temp = instances[it];
		int sequence_index = 0;
		string sequence = temp.whole_sequence;
		vector<int> scores = temp.quals;
		for (int i = 0; i <= sequence.size() - len  ; i++)
		{	
			//cout << sequence.size() - len << endl;
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
						//cout << len_help << endl;

						len_help++;
						//end_pos;
						iter++;
					}
					//else if(ite)
					else
					{
						loaded_seq += sequence[iter];
						//cout << len_help << endl;

						len_help++;
						end_pos = i;
						iter++;
					}
					
				}
				else // jak score za ma�y
				{
					too_low++;
					//cout << len_help << endl;
					len_help++;
					iter++;
					
				}
				
			}
			if (too_low < len / 2)
			{
				loaded.index_inseq = sequence_index;
				sequence_index++;// czy to cos da? bo w sumie to mo�na sie przesuwa� po ca�osci o jeden ale chyba da bo jak tu bedzie 0 to nie bedziesz potem jechal w lewo np
				//potem mozna sprawdzac jesli chodzi o prawo ze czy nastepna to koniec listy calosci lub 0 po prawej czyli koniec sekwencji
				loaded.index_inall = allindex;
				allindex++;
				loaded.id = temp.input_id;
				loaded.nucleos = loaded_seq;
				loaded.start_pos = start_pos;
				loaded.end_pos = end_pos;
				seqs.push_back(loaded);
				//if (loaded_seq == "TTTTTTT")
				//{
					cout << loaded.id << endl;
					cout << sequence_index << endl;
					cout << loaded_seq << endl;
				//}
			}

		

		}
	}
return seqs;
}

vector<Vertex> load_graph(vector<Sequence> Seqs)//to raczej b�dzie vector i przepisanie na jaki� vector w mainie
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
				if ((Graph[it].nucleos.find(Graph[itt].nucleos) != std::string::npos) && Graph[it].id != Graph[itt].id)//jesli sekwencja zawiera sie w drugiej i s� z innych id
				{
					//cout << "som takie same xD" << endl;

					if (!(find(Graph[it].neighbours.begin(), Graph[it].neighbours.end(), Graph[itt].vert_id) != Graph[it].neighbours.end()) &&// jesli jest juz takie s�siedztwo
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

/*vector<Vertex> graphSorter(vector<Vertex> graph)
{
	vector< Vertex > graph_copy = graph;
	sort(graph_copy.begin(), graph_copy.end());
	//reverse(graph_copy.begin(), graph_copy.end());
	return graph_copy;
}*/

vector<Vertex> find_clique(vector<Vertex> vertexes_set)
{
	vector<Vertex> Clique;
	for (int i = 0; i < vertexes_set.size(); i++)//znajdujemy nawinksz� klike ,  potrzeba mocniejsz� instancje z wieksz� ilo�cia klik wtedy zaczynaj�c od innego wierzcho�ka mo�na dalej szukac
	{
		sort(vertexes_set.begin(), vertexes_set.end());
		if (i == 0)
		{
			Clique.push_back(vertexes_set[i]);// dodaje se pierwszy z posortowanej listy po najwiekszym stopniu verta
		}
		else//tu zagadka jaka� pomocnicza lista dodanych i tam patrzymy czy s�siady czy jak?
		{
			int analyzed_id = vertexes_set[i].vert_id;
			int check_counter = 0;
			for (int j = 0; j < Clique.size(); j++)
			{
				if (std::find(Clique[j].neighbours.begin(), Clique[j].neighbours.end(), analyzed_id) != Clique[j].neighbours.end())
				{//je�li jest s�siadem jednego z cz�onk�w kliki
					check_counter++;
				}
				if (check_counter == Clique.size())//jak znajduje sie w s�siedztwie wszystkich cz�onk�w kliki
				{
					Clique.push_back(vertexes_set[i]);
					break;
				}
			}


		}

	}
	return Clique;
}
//vector<

/*vector<Vertex> find_clique(vector<Vertex> graph, Vertex Analyzed, vector<Vertex> Clique)
{
	for (int i = 0; i < Analyzed.neighbours.size(); i++)
	{

	}

}*/


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
	graph_sorted = graph;
	sort(graph_sorted.begin(), graph_sorted.end());

	//***************testowo do przysz�ej funkcji********************************** i nalezaloby w sumie przetestowac 
	/*for (int i = 0; i < graph.size(); i++)
	{
		cout << graph[i].id << endl;
		for (int j = 0; j < graph[i].neighbours.size(); j++)
		{
			cout << graph[i].neighbours[j] << "* "<< endl;
			cout << "******" << graph[graph[i].neighbours[j]].id << endl;

		}
		cout << endl;

	}*/

	//vector<Vertex> LargestClique ;
	//for (int i = 0; i < graph_sorted.size(); i++)//znajdujemy nawinksz� klike ,  potrzeba mocniejsz� instancje z wieksz� ilo�cia klik wtedy zaczynaj�c od innego wierzcho�ka mo�na dalej szukac
	//{
	//	if (i == 0)
	//	{
	//		LargestClique.push_back(graph_sorted[i]);// dodaje se pierwszy z posortowanej listy po najwiekszym stopniu verta
	//	}
	//	else//tu zagadka jaka� pomocnicza lista dodanych i tam patrzymy czy s�siady czy jak?
	//	{
	//		int analyzed_id = graph_sorted[i].vert_id;
	//		int check_counter = 0;
	//		for (int j = 0; j < LargestClique.size(); j++)
	//		{
	//			if (std::find(LargestClique[j].neighbours.begin(), LargestClique[j].neighbours.end(), analyzed_id) != LargestClique[j].neighbours.end()) 
	//			{//je�li jest s�siadem jednego z cz�onk�w kliki
	//				check_counter++;
	//			}
	//			if (check_counter == LargestClique.size())//jak znajduje sie w s�siedztwie wszystkich cz�onk�w kliki
	//			{
	//				LargestClique.push_back(graph_sorted[i]);
	//				break;
	//			}
	//		}


	//	}

	//}
	/*for (int i = 0; i < LargestClique.size(); i++)
	{
		cout << LargestClique[i].vert_id << " ";
	}
	cout << endl;
	cout << "Startowy motyw:" << endl;
	cout << LargestClique[0].nucleos << endl;*/
	string act_mot;
	int nres = 0;
	int maxres = 4;
	int windowsize = 1;//substr_len / 2;
	while (nres < maxres)//ile motywowow chcemy miec
	{
		vector<Vertex> StartClique = find_clique(graph);
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
		/*auto most = max_element(occurance.begin(), occurance.end(),
		[](const pair<string, int>& p1, const pair<int, int>& p2) {
		return p1.second < p2.second; });*/
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
		cout << main_arg_max << endl;
		int past_len = act_mot.length();
		vector<Vertex> TempClique;
		bool left_side = true;
		bool right_side = true;
		while (left_side)// ti warunek zakonczenia rozszerzenia w lewo czyli dla wszystkich jak ju�nie mo�na
		{
			cout << "rozszerzanie w lewo" << endl;
			vector<Vertex> left;
			for (int i = 0; i < StartClique.size(); i++)
			{
				if (StartClique[i].index_inall - windowsize >= 0 && StartClique[i].id == graph[StartClique[i].index_inall - windowsize].id)
				{
					int left_ind = StartClique[i].index_inall - windowsize;
					left.push_back(graph[left_ind]);

				}
			}
				if (left.size() > 2)//je�li jest sens szuka� kliki
				{
					TempClique = find_clique(left);//szukamy kliki , nie trzeba czyscic
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
						/*auto most = max_element(occurance.begin(), occurance.end(),
						[](const pair<string, int>& p1, const pair<int, int>& p2) {
						return p1.second < p2.second; });*/
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
								cout << act_mot << endl;
							}

							string submotif_sub = arg_max.substr(i,arg_max.length());
							string motif_sub = act_mot.substr(0,arg_max.length() - i);
							if (submotif_sub == motif_sub)
							{
								act_mot = arg_max.substr(0,i) + act_mot;
								cout << act_mot << endl;
								break;
							}

						}
						cout << arg_max << endl;

						//jak sparsowa� se now� czesc motywu z motywem?
						// tutaj for por�wnuj�cy aktuany motyw(ale tylko rozmiar okna) z dodawanym  podci�giem
						// to samo z prawej strony
						//jak sie nic nie zgadza to dodaj ca�y podciag



					}
					else left_side = false;
				}
				else
				{
					left_side = false;
				}

			
		}
		TempClique.clear();//czyscimy ze smieci z lewa
		StartClique = StartCliqueR;
		while (right_side)// tu warunek zakonczenia rozszerzenia w prawo czyli albo za ma�a klika z tych z prawej albo konce sekwencji tych z prawej czyli e albo next w ca�ym grafie jest zerem
			//albo next wiekszy niz rozmiar vectora grafu
		{
			cout << "rozszerzanie w prawo" << endl;
			vector<Vertex> right;
			for (int i = 0; i < StartClique.size(); i++)
			{
				if (StartClique[i].index_inall + windowsize < graph.size() && StartClique[i].id == graph[ StartClique[i].index_inall + windowsize ].id )
				{
					int right_ind = StartClique[i].index_inall + windowsize;// plus jeden czy plus wincyj?
					right.push_back(graph[right_ind]);

				}
			}
				if (right.size() > 2)//je�li jest sens szuka� kliki
				{
					//cout << "jest sens" << endl;
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
						/*auto most = max_element(occurance.begin(), occurance.end(),
						[](const pair<string, int>& p1, const pair<int, int>& p2) {
						return p1.second < p2.second; });*/
						int currentMax = 0;
						string arg_max = "";
						for (auto it = rightoccurance.cbegin(); it != rightoccurance.cend(); ++it)
						{
							if (it->second > currentMax) {
								arg_max = it->first;
								currentMax = it->second;
							}
						}

						for (int i = windowsize; i >= 0 ; i--)
						{
							if (i == 0)
							{
								act_mot = act_mot + arg_max.substr(i,arg_max.length()) ;
								cout << act_mot <<"ca�osc"<< endl;
							}
							//string submotif_sub = arg_max.substr(i, arg_max.length());
							//string motif_sub = act_mot.substr(0, arg_max.length() - i);
						
							int test = arg_max.length();

							string submotif_sub = arg_max.substr(0, arg_max.length() - i );
							string motif_sub = act_mot.substr( act_mot.length() - (arg_max.length() - i), act_mot.length());
		
							if (submotif_sub == motif_sub)
							{
								act_mot =  act_mot + arg_max.substr(arg_max.length() - i);
								cout << act_mot << endl;
								break;
							}

						}

						//jak sparsowa� se now� czesc motywu z motywem z 
						//prawej?
						//jak sparsowa� se now� czesc motywu z motywem?
						// tutaj for por�wnuj�cy aktuany motyw(ale tylko rozmiar okna) z dodawanym  podci�giem
						// to samo z prawej strony
						// jak sie nic nie zgadza no to dodajesz ca�y
						cout << arg_max << endl;
						right.clear();

					}
					else right_side = false;
				}
				else
				{
					right_side = false;
				}
			


		}


		cout << "********* ACT **************" << act_mot << endl;
		system("pause");
		return 0;
	}
}
//wierzcholek z najwieksz� liczba sasiadow jako pocz�tek kliki
//pierwszy s�siad tez do kliki
/*pierwszy sasiad tego sasiada wyzej taki co jest rozny od obecnych w klice  ,
trzeba sprawdzi� czy on ma na liscie s�siad�w tych wierzcholkow co sa juz w klice*/

//jak ma to do kliki

//czy klike budowa� na podstawie tej samej co graf ( string w stringu) ????

//warunek zakonczenia ? jak przejdziemy liste sasiadow tego pierwszego?
//to sie troche prosi o rekurencje ale nwm 

