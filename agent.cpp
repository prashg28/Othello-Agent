#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<sstream>
#include<ctime>
#include<string>
#define Ninfi "-Infinity"
#define Pinfi  "Infinity"
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define PRINT
//#define GAME
#ifdef GAME
int depth_log;
int endc=0;
int endab = 0;
class LOG
{
public:
	int w, d;
	LOG(int weight, int depth)
	{
		w = weight; d = depth;
	}

};
std::vector<LOG> weight_log;
#endif
using namespace std;
char player = NULL;
char opponent = NULL;
int cutoff = -1;
bool alpha_beta = false;

vector<string> log_vec;
ofstream out("output.txt");
ifstream in("input.txt");

int weight[8][8] = {
		{ 99, -8, 8, 6, 6, 8, -8, 99 },
		{ -8, -24, -4, -3, -3, -4, -24, -8 },
		{ 8, -4, 7, 4, 4, 7, -4, 8 },
		{ 6, -3, 4, 0, 0, 4, -3, 6 },
		{ 6, -3, 4, 0, 0, 4, -3, 6 },
		{ 8, -4, 7, 4, 4, 7, -4, 8 },
		{ -8, -24, -4, -3, -3, -4, -24, -8 },
		{ 99, -8, 8, 6, 6, 8, -8, 99 },
};
class Reverci
{
public:
	char x;
};
class Position
{
public:
	int x, y,val;
	bool operator<(const Position &p) const;
};
class Node
{
public:
	Reverci tempArray[8][8];
	int alpha;
	int beta;
	int min;
	int max;
	int val,calval;
	int depth;
	int x, y;
	int best_x, best_y;
	bool isMax  ;
	bool pass ;
	bool parent_pass;
	bool grandParent_pass;
	bool endGame;
	vector <Position> node_pos;
};
bool Position::operator<(const Position & p)const
{
	if (this->val < p.val)
		return true;
	else if (this->val > p.val)
		return false;
	else
	{
		if (this->x > p.x)
			return true;
		else if(this->x < p.x)
			return false;
		else
		{
			if (this->y > p.y)
				return true;
			else
				return false;
		}
	}
}
struct maxtrix_sort
{
	bool operator() (const Position& pos1, const Position& pos2) 
	{
		if (pos1.x > pos2.x)
			return true;
		else if (pos1.x < pos2.x)
			return false;
		else
		{
			if (pos1.y > pos2.y)
				return true;
			else
				return false;
		}
	}
};

Node min_value(Node state);
Node max_value(Node state);
Node min_value_Comp(Node state);
Node max_value_Comp(Node state);
void swap(char *player, char *opponent);
void displayArray(Reverci array[8][8]);
template <typename T>
string NumberToString(T Number)
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}
bool boardState(Reverci array[8][8])
{
	int pcount=0, ocount=0,tcount=0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (array[i][j].x == player)
				pcount++;
			if (array[i][j].x == opponent)
				ocount++;
			if (array[i][j].x != '*')
				tcount++;
		}
	}
	if (tcount == pcount || tcount == ocount)
		return false;
	else
		return true;
}
void inputArray(ifstream &in, Reverci array[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		in >> array[i][j].x;
		}
}
void eliminateDuplicates(vector <Position> &pos)
{
	for (size_t i = 0; i < pos.size(); i++)
	{
		int current_x = pos.at(i).x;
		int current_y = pos.at(i).y;

		for (size_t j = i; j < pos.size(); j++)
		{
			if (j == i)
			{
				continue;
			}
			else
			{
				int temp_x = pos.at(j).x;
				int temp_y = pos.at(j).y;
				if (temp_x == current_x && temp_y == current_y)
				{
					vector<Position>::iterator iter = pos.begin() + j;
					pos.erase(iter);
					j--;
				}
			}
		}
	}
}
void findIndex(vector <Position> &pos, int i, int j, Reverci array[8][8])
{
	Position temp_pos;
	if (j < 8 && j >= 0 && i < 8 && i >= 0)
	{
		if (array[i][j + 1].x == opponent)//Right
			for (int k = j + 1; k < 8; k++)
			{
			if (array[i][k].x == '*')
			{
				temp_pos.x = i;
				temp_pos.y = k;
		//		temp_pos.val = sum - weight[i][k];
				pos.push_back(temp_pos);
				break;
			}
			else if (array[i][k].x == player)
				break;
			}
		if (array[i][j - 1].x == opponent)//left
			for (int k = j - 1; k >= 0; k--)
			{
			if (array[i][k].x == '*')
			{
				temp_pos.x = i;
				temp_pos.y = k;
		//		temp_pos.val = sum - weight[i][k];
				pos.push_back(temp_pos);
				break;
			}
			else if (array[i][k].x == player)
				break;
			}
		if (array[i + 1][j].x == opponent)//bottom
			for (int k = i + 1; k < 8; k++)
			{
			if (array[k][j].x == '*')
			{
				temp_pos.x = k;
				temp_pos.y = j;
			//	temp_pos.val = sum - weight[k][j];
				pos.push_back(temp_pos);
				break;
			}
			else if (array[k][j].x == player)
				break;
			}

		if (array[i - 1][j].x == opponent)//top
			for (int k = i - 1; k >= 0; k--)
			{
			if (array[k][j].x == '*')
			{
				temp_pos.x = k;
				temp_pos.y = j;
			//	temp_pos.val = sum - weight[k][j];
				pos.push_back(temp_pos);
				break;
			}
			else if (array[k][j].x == player)
				break;
			}
		if (array[i + 1][j + 1].x == opponent)//Right_top
			for (int k = j + 1, l = i + 1; k < 8 && l < 8; k++, l++)
			{
			if (array[l][k].x == '*')
				{
			temp_pos.x = l;
			temp_pos.y = k;
		//	temp_pos.val = sum - weight[l][k];
			pos.push_back(temp_pos);
			break;
				}
			else if (array[l][k].x == player)
				break;
			}
		if (array[i - 1][j + 1].x == opponent)//Right_bottom
			for (int k = j + 1, l = i - 1; k < 8 && l>=0; k++, l--)
			{
			if (array[l][k].x == '*')
			{
				temp_pos.x = l;
				temp_pos.y = k;
			//	temp_pos.val = sum - weight[l][k];
				pos.push_back(temp_pos);
				break;
			}
			else if (array[l][k].x == player)
				break;
			}
		if (array[i + 1][j - 1].x == opponent)//left_bottom
			for (int k = j - 1, l = i + 1; k >= 0 && l < 8; k--, l++)
			{
			if (array[l][k].x == '*')
			{
				temp_pos.x = l;
				temp_pos.y = k;
			//	temp_pos.val = sum - weight[l][k];
				pos.push_back(temp_pos);
				break;
			}
			else if (array[l][k].x == player)
				break;
			}
		if (array[i - 1][j - 1].x == opponent)//left_top
			for (int k = j - 1, l = i - 1; k >= 0 && l >= 0; k--, l--)
			{
			if (array[l][k].x == '*')
			{
				temp_pos.x = l;
				temp_pos.y = k;
		//		temp_pos.val = sum - weight[l][k];
				pos.push_back(temp_pos);
				break;
			}
			else if (array[l][k].x == player)
				break;
			}


	}
	eliminateDuplicates(pos);
}
void colorChange(int i, int j, Reverci array[8][8])
{
	if (j < 8 && j >= 0 && i < 8 && i >= 0)
	{
		if (array[i][j + 1].x == opponent)//Right
		{
			for (int k = j + 1; k < 8; k++)
			{
				if (array[i][k].x == player)
				{
					for (int l = k - 1; l >= j + 1; l--)
						array[i][l].x = player;
					break;
				}
				else if (array[i][k].x == '*')
					break;
			}
		}
		if (array[i][j - 1].x == opponent)//left
		{
			for (int k = j - 1; k >= 0; k--)
			{
				if (array[i][k].x == player)
				{
					for (int l = k + 1; l <= j - 1; l++)
						array[i][l].x = player;
					break;
				}
				else if (array[i][k].x == '*')
					break;
			}
		}
		if (array[i + 1][j].x == opponent)//bottom
		{
			for (int k = i + 1; k < 8; k++)
			{
				if (array[k][j].x == player)
				{
					for (int l = k - 1; l >= i + 1; l--)
						array[l][j].x = player;
				}
				else if (array[k][j].x == '*')
					break;
			}
		}

		if (array[i - 1][j].x == opponent)//top
		{
			for (int k = i - 1; k >= 0; k--)
			{
				if (array[k][j].x == player)
				{
					for (int l = k + 1; l <= i - 1;l++)
					array[l][j].x = player;
					break;
				}
				else if (array[k][j].x == '*')
					break;
			}
		}
		if (array[i + 1][j + 1].x == opponent)//Right_top
		{
			for (int k = j + 1, l = i + 1; k < 8 && l < 8; k++, l++)
			{
				if (array[l][k].x == player)
				{
					for (int m = k - 1, n = l - 1; m >= j + 1, n >= i + 1;m--,n--)
					array[n][m].x = player;
				}
				else if (array[l][k].x == '*')
					break;
			}
		}
		if (array[i - 1][j + 1].x == opponent)//Right_bottom
		{
			for (int k = j + 1, l = i - 1; k < 8 && l >= 0; k++, l--)
			{
				if (array[l][k].x == player)
				{
					for (int m = k - 1, n = l + 1; m >= j + 1, n <= i - 1;m--,n++)
						array[n][m].x = player;
					break;
				}
				else if (array[l][k].x == '*')
					break;
			}
		}
		if (array[i + 1][j - 1].x == opponent)//left_bottom
		{
			for (int k = j - 1, l = i + 1; k >= 0 && l < 8; k--, l++)
			{
				if (array[l][k].x == player)
				{
					for (int m = k + 1, n = l - 1; m <= j - 1, n >= i + 1; m++, n--)
						array[n][m].x = player;
					break;
				}
				else if (array[l][k].x == '*')
					break;
			}
		}
		if (array[i - 1][j - 1].x == opponent)//left_top
		{
			for (int k = j - 1, l = i - 1; k >= 0 && l >= 0; k--, l--)
			{
				if (array[l][k].x == player)
				{
					for (int m = k + 1, n = l + 1; m <= j - 1, n <= i - 1;m++,n++)
					array[n][m].x = player;
				}
				else if (array[l][k].x == '*')
					break;
			}

		}
	}
}
int findValue(Reverci array[8][8])
{
	int sum = 0;
	//swap(&player, &opponent);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		if (array[i][j].x == player)
			sum += weight[i][j];
		else if (array[i][j].x == opponent)
			sum -= weight[i][j];
		}
	//swap(&player, &opponent);
	return sum;
}
void findPosition(vector <Position> &pos, Reverci array[8][8])
{
	/*int sum = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		if (array[i][j].x == opponent)
			sum += weight[i][j];
		else if (array[i][j].x == player)
			sum -= weight[i][j];
		}*/

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		if (array[i][j].x == player)
			findIndex(pos, i, j,array);
		}
}
void findPositionTemp(vector <Position> &pos, Reverci array[8][8])
{
	swap(&player, &opponent);
	findPosition(pos, array);
	swap(&player, &opponent);
}
void displayArray(Reverci array[8][8])
{
	for (int i = -1; i < 8; i++)
	{
		std::cout << endl << i + 1 << "|" << " ";
		for (int j = 0; j < 8; j++)
		{	
			if (i == -1)
				cout << char('a' + j)<<"|" << " ";
			else
				cout << array[i][j].x << "|" << " ";
		}
	}
	cout << endl << "----------------------------------------------------------------------------" << endl;
}
void displayArray_file(Reverci array[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			out << array[i][j].x;
		}
		out << endl ;
	}
}
void displaySelection(vector <Position> &position)
{
	cout << endl;
	for (vector<Position>::iterator pos = position.begin(); pos != position.end(); ++pos)
	{
		cout << " X : " << pos->x << " y: " << pos->y << " val : " << pos->val<<" -->> " <<char('a' + (pos->y)) << pos->x + 1 << endl;
	}
}
void arrayCopy(Reverci dest[8][8], Reverci src[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		dest[i][j] = src[i][j];
		}
}
void swap(char *player, char *opponent)
{
	char temp = *player;
	*player = *opponent;
	*opponent = temp;
}
void print_AlphaBeta(Node state)
{
	cout << ",";
	if (state.alpha == INT_MIN)
	{
		cout << Ninfi;
	}
	else
	{
		cout << state.alpha;
	}
	cout << ",";
	if (state.beta == INT_MAX)
	{
		cout << Pinfi;
	}
	else
	{
		cout << state.beta;
	}

}
void print_AlphaBeta_File(Node state, string *log)
{
	//out << ",";
	*log += ",";
	if (state.alpha == INT_MIN)
	{
		//out << Ninfi;
		*log += Ninfi;
	}
	else
	{
		//out << state.alpha;
		*log += NumberToString(state.alpha);
	}
	//out << ",";
	*log += ",";
	if (state.beta == INT_MAX)
	{
		//out << Pinfi;
		*log += Pinfi;
	}
	else
	{
		//out << state.beta;
		*log += NumberToString(state.beta);
	}

}
void print_max(Node state)
{
	if (state.depth == cutoff && state.pass != true)
	{
		cout << "root";
	}
	else if (state.pass == true)
	{
		cout << "pass";
	}
	else
	{
		cout << char('a' + (state.y)) << state.x + 1;
	}
	cout << "," << cutoff - state.depth << ",";
	if (state.val == INT_MIN)
	{
		cout << Ninfi;
	}
	else
	{
		cout << state.val;
	}
	if (alpha_beta)
	{
		print_AlphaBeta(state);
	}
	cout << endl;
}
void print_min(Node state)
{
	if (state.depth == cutoff && state.pass != true)
	{
		cout << "root";
	}
	else if (state.pass == true)
	{
		cout << "pass";
	}
	else
	{
		cout << char('a' + (state.y)) << state.x + 1;
	}
	cout << "," << cutoff - state.depth << ",";
	if (state.val == INT_MAX)
	{
		cout << Pinfi;
	}
	else
	{
		cout << state.val;
	}
	if (alpha_beta)
	{
		print_AlphaBeta(state);
	}
	cout << endl;
}
void print_min_file(Node state)
{
	string log;
	//print_min(state);
	if (state.depth == cutoff)
	{
		//out << "root";
		log += "root";
	}
	else if (state.pass == true)
	{
		//out << "pass";
		log += "pass";
	}
	else if (state.parent_pass == true)
	{
		log += "pass";
	}
	else
	{
		//out << char('a' + (state.y)) << state.x + 1;
		log += char('a' + (state.y)) +NumberToString(state.x + 1);
	}
	//out << "," <<to_string(cutoff - state.depth) << ",";
	//if (state.pass == true)
	//	log += "," + to_string(cutoff - state.depth) + ",";
	//else
	log += "," + NumberToString(cutoff - state.depth) + ",";
	if (state.val == INT_MAX)
	{
		//out << Pinfi;
		log += Pinfi;
	}
	else if (state.val == INT_MIN)
	{
		//out << Ninfi;
		log += Ninfi;
	}
	else
	{
		//out << state.val;
		log += NumberToString(state.val);
	}
	if (alpha_beta)
	{
		print_AlphaBeta_File(state, &log);
	}
	log += "\n";
	log_vec.push_back(log);
	//out << endl;

}
void print_max_file(Node state)
{
	string log;
	//print_max(state);
	if (state.depth == cutoff)
	{
		//out << "root";
		log += "root";
	}
	else if (state.pass == true)
	{
		//out << "pass";
		log += "pass";
	}
	else if (state.parent_pass == true)
	{
		log += "pass";
	}
	else
	{
		//out << char('a' + (state.y)) << state.x + 1;
		log+= char('a' + (state.y)) + NumberToString(state.x + 1);
	}
	//if (state.pass == true)
	//	log += "," + to_string(cutoff - state.depth + 1) + ",";
	//else
	//out << "," << to_string(cutoff - state.depth) << ",";
	log+= ","+NumberToString(cutoff - state.depth)+ ",";
	if (state.val == INT_MIN)
	{
		//out << Ninfi;
		log += Ninfi;
	}
	else if (state.val == INT_MAX)
	{
		//out << Pinfi;
		log += Pinfi;
	}
	else
	{
		//out << state.val;
		log += NumberToString(state.val);
	}
	if (alpha_beta)
	{
		print_AlphaBeta_File(state, &log);
	}
	log += "\n";
	log_vec.push_back(log);
	//out << endl;
}
void display_log_file()
{
	for (size_t i = 0; i < log_vec.size();i++)
	{
		out<<log_vec[i];
	}
}
Node max_value(Node state)
{
	swap(&player, &opponent);
	if (state.depth < 0)
	{
		state.val = state.calval;
		if (state.pass == false)
#ifdef PRINT
		print_max_file(state);
#endif
		return state;
	}
	else if (!boardState(state.tempArray))
	{
		state.val = state.calval;
#ifdef PRINT
		print_max_file(state);
#endif
		return state;
	}
	
	Node temp_state;
	findPosition(state.node_pos, state.tempArray);
	sort(state.node_pos.begin(), state.node_pos.end(), maxtrix_sort());
	//displaySelection(state.node_pos);


	if (state.node_pos.empty())
	{
		if (state.grandParent_pass == true)
		{
			state.val = state.calval;
#ifdef PRINT
			print_max_file(state);
#endif
			return state;
		}
#ifdef PRINT
		print_max_file(state);
#endif	
		arrayCopy(temp_state.tempArray, state.tempArray);
		//displayArray(temp_state.tempArray);
		findPositionTemp(temp_state.node_pos, temp_state.tempArray);
		if (!temp_state.node_pos.empty())
		{
		//	print_max_file(state);
			temp_state.pass = false;
		}
		else
			temp_state.pass = true;
		temp_state.isMax = false;
		if (state.pass == true)
			temp_state.parent_pass = true;
		else
			temp_state.parent_pass = false;
		temp_state.depth = state.depth - 1;
		temp_state.val = INT_MAX;
		temp_state.endGame = false;
		temp_state.calval =state.calval;
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		//cout <<endl<<"-----------------"<<temp_state.val<<endl;
		//cout <<"MAX : " <<" X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node min = min_value(temp_state);
		swap(&player, &opponent);
		state.val = min.val;
		//if (min.pass == true && state.pass == true)
		//{
		//	state.endGame = true;
		//	return state;
		//}
		//if (min.pass == true)
		//{
			if (min.val > state.val)
			{
				state.val = min.val;
				state.best_x = min.x;
				state.best_y = min.y;
				//cout << "MAX : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
			}
			if (alpha_beta)// && !min.endGame)
			{
				if (state.val < state.beta)
				{
					state.alpha = MAX(state.alpha, state.val);
#ifdef PRINT
					print_min_file(state);
#endif
					return state;
				}
				else
#ifdef PRINT
					print_min_file(state);
#endif
			}
			else
			{
#ifdef PRINT
				print_min_file(state);
#endif
			}
//		}
//		else	if (min.endGame)
//			{
//#ifdef PRINT
//			print_min_file(state);
//#endif
//			}
			return state;
	}
#ifdef PRINT
	print_max_file(state);
#endif
	while (!state.node_pos.empty())
	{
		arrayCopy(temp_state.tempArray,state.tempArray);
		temp_state.tempArray[state.node_pos.back().x][state.node_pos.back().y].x = player ;
		colorChange(state.node_pos.back().x, state.node_pos.back().y, temp_state.tempArray);
		//displayArray(temp_state.tempArray);
		temp_state.isMax = false;
		temp_state.pass = false;
		temp_state.parent_pass = false;
		temp_state.depth = state.depth - 1;
		temp_state.x = state.node_pos.back().x;
		temp_state.y = state.node_pos.back().y;
		temp_state.best_x = state.node_pos.back().x;
		temp_state.best_y = state.node_pos.back().y;
		temp_state.endGame = false;
		temp_state.val = INT_MAX;
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		temp_state.calval = findValue(temp_state.tempArray);//state.node_pos.back().val;
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		//cout <<endl<<"-----------------"<<temp_state.val<<endl;
		//cout <<"MAX : " <<" X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node min = min_value(temp_state);
		swap(&player, &opponent);
		if (min.val > state.val)
		{
			state.val = min.val;
			state.best_x = min.x;
			state.best_y = min.y;
			//cout << "MAX : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		}
		if (alpha_beta)// && !min.endGame)
		{
			if (state.val >= state.beta)
			{
				//state.alpha = MAX(state.alpha, state.val);
#ifdef PRINT
				print_max_file(state);
#endif
			//	state.alpha = MAX(state.alpha, state.val);
				return state;
			}
			else
				state.alpha = MAX(state.alpha, state.val);
		}
#ifdef PRINT
			print_max_file(state);
#endif
			state.node_pos.pop_back();
	}
	return state;
}
Node min_value(Node state)
{
	swap(&player, &opponent);
	if (state.depth < 0 )
	{
		state.val = state.calval;
		if (state.pass == false)
#ifdef PRINT
		print_min_file(state);
#endif
		return state;
	} 
	else if (!boardState(state.tempArray))
	{
		state.val = state.calval;
#ifdef PRINT
			print_min_file(state);
#endif
		return state;
	}
	Node temp_state;
	findPosition(state.node_pos, state.tempArray);
	sort(state.node_pos.begin(), state.node_pos.end(), maxtrix_sort());
	//displaySelection(state.node_pos);
	if (state.node_pos.empty())
	{
		if (state.grandParent_pass == true)
		{
			state.val = state.calval;
#ifdef PRINT
			print_min_file(state);
#endif
			return state;
		}
#ifdef PRINT
		print_min_file(state);
#endif
		arrayCopy(temp_state.tempArray, state.tempArray);
		//displayArray(temp_state.tempArray);
		findPositionTemp(temp_state.node_pos, temp_state.tempArray);
		if (!temp_state.node_pos.empty())
		{
		//	print_max_file(state);
			temp_state.pass = false;
		}
		else
			temp_state.pass = true;
		temp_state.isMax = true;
		temp_state.pass = true;
		temp_state.depth = state.depth - 1;
		if (state.pass == true)
			temp_state.parent_pass = true;
		else
			temp_state.parent_pass = false;
		temp_state.endGame = false;
		temp_state.calval = state.calval;
		temp_state.val = INT_MIN;
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		//cout << endl << "-----------------" << temp_state.val<<endl;
		//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node max = max_value(temp_state);
		swap(&player, &opponent);
		state.val = max.val;
	/*	if (max.pass == true && state.pass == true)
		{
			state.endGame = true;
			return state;
		}
		*/
			if (max.val < state.val)
			{
				state.val = max.val;
				state.best_x = max.x;
				state.best_y = max.y;
				//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
			}
			if (alpha_beta)//&& ! max.endGame)
			{
				if (state.val > state.alpha)
				{
					state.beta = MIN(state.beta, state.val);
#ifdef PRINT
					print_min_file(state);
#endif
					return state;
				}
				else
#ifdef PRINT
					print_min_file(state);
#endif

			}
			else
			{
#ifdef PRINT
				print_min_file(state);
#endif
			}
			/*else
				if (max.endGame)
				{
				#ifdef PRINT
					print_min_file(state);
#endif
				}*/
		return state;
	}
#ifdef PRINT
	print_min_file(state);
#endif	
	while (!state.node_pos.empty())
	{
		arrayCopy(temp_state.tempArray, state.tempArray);
		temp_state.tempArray[state.node_pos.back().x][state.node_pos.back().y].x = player;
		colorChange(state.node_pos.back().x, state.node_pos.back().y, temp_state.tempArray);
		//displayArray(temp_state.tempArray);
		temp_state.isMax = true;
		temp_state.pass = false;
		temp_state.parent_pass = false;
		temp_state.depth = state.depth - 1;
		temp_state.x = state.node_pos.back().x;
		temp_state.y = state.node_pos.back().y;
		temp_state.best_x = state.node_pos.back().x;
		temp_state.best_y = state.node_pos.back().y;
		temp_state.endGame = false;
		temp_state.val = INT_MIN;
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		swap(&player, &opponent);
		temp_state.calval = findValue(temp_state.tempArray);//state.node_pos.back().val;
		swap(&player, &opponent);
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		//cout << endl << "-----------------" << temp_state.val<<endl;
		//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node max = max_value(temp_state);
		swap(&player, &opponent);
		if (max.val < state.val)
		{
			state.val = max.val;
			state.best_x = max.x;
			state.best_y = max.y;
			//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		}
		if (alpha_beta)// && !max.endGame)
		{
			if (state.val <= state.alpha)
			{
				//state.beta = MIN(state.beta, state.val);
#ifdef PRINT
				print_min_file(state);
#endif
			//	state.beta = MIN(state.beta, state.val);
				return state;
			}
			else
				state.beta = MIN(state.beta, state.val);
		}
#ifdef PRINT
		print_min_file(state);
#endif
		state.node_pos.pop_back();
	}
	return state;
}
void findMinMax(Reverci array[8][8], int depth)
{
	Node Root;
	Root.depth = depth;
	Root.alpha = INT_MIN;
	Root.beta = INT_MAX;
	Root.val = INT_MIN;
	Root.isMax = true;
	Root.endGame = false;
	arrayCopy(Root.tempArray, array);
	findPosition(Root.node_pos, Root.tempArray);
	if (Root.node_pos.empty())
		Root.pass = true;
	else
		Root.pass = false;
	Root.grandParent_pass = false;
	Root.parent_pass = false;
	//	Root.pass = false;
	Root.calval = findValue(Root.tempArray);
	if (cutoff > -1 && boardState(Root.tempArray))
	{
		swap(&player, &opponent);
		Node result = max_value(Root);
		//swap(&player, &opponent);
		if (result.pass == false)
		{
			//std::cout << char('a' + (result.best_y)) << result.best_x + 1 << " -> " << result.val << endl;
			//print_max_file(result);
			Root.tempArray[result.best_x][result.best_y].x = player;
			colorChange(result.best_x, result.best_y, Root.tempArray);
		}
		else
		{
			swap(&player, &opponent);
			if (result.val > Root.val)
			{
				Root.val = result.val;
				Root.best_x = result.x;
				Root.best_y = result.y;
				//	cout << "MAX : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
			}
			if (alpha_beta)
			{
				if (!result.endGame && Root.beta > result.val)
					Root.alpha = MAX(Root.alpha, Root.val);
				//		print_max_file(Root);
			}
		}
#ifdef GAME
		if (result.pass == true)
		{
			endab++;
		}
		out << 4 << endl;
		out << opponent << endl << 200 << endl;
		displayArray(Root.tempArray);
#endif
	}
	else
	{
		Root.val = Root.calval;
		print_max_file(Root);
	}
	displayArray_file(Root.tempArray);
	out << "Node," << "Depth," << "Value" << endl;
	display_log_file();
}
void findAlphaBeta(Reverci array[8][8], int depth)
{
	Node Root;
	Root.depth = depth;
	alpha_beta = true;
	Root.alpha = INT_MIN;
	Root.beta = INT_MAX;
	Root.val = INT_MIN;
	Root.isMax = true;
	Root.endGame = false;
	arrayCopy(Root.tempArray, array);
	findPosition(Root.node_pos, Root.tempArray);
	if (Root.node_pos.empty())
		Root.pass = true;
	else
		Root.pass = false;
	Root.grandParent_pass = false;
	Root.parent_pass = false;
	//	Root.pass = false;
	Root.calval = findValue(Root.tempArray);
	if (cutoff > -1 && boardState(Root.tempArray))
	{
		swap(&player, &opponent);
		Node result = max_value(Root);
		if (result.pass == false)
		{
			//std::cout << char('a' + (result.best_y)) << result.best_x + 1 << " -> " << result.val << endl;
			//print_max_file(result);
			Root.tempArray[result.best_x][result.best_y].x = player;
			colorChange(result.best_x, result.best_y, Root.tempArray);
		}
		else
		{
			swap(&player, &opponent);
			if (result.val > Root.val)
			{
				Root.val = result.val;
				Root.best_x = result.x;
				Root.best_y = result.y;
			//	cout << "MAX : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
			}
			if (alpha_beta)
			{
				if(!result.endGame && Root.beta > result.val)
					Root.alpha = MAX(Root.alpha, Root.val);
		//		print_max_file(Root);
			}
		}
#ifdef GAME
		if (result.pass == true)
		{
			swap(&player,&opponent);
			endab++;
		}
		out << 4 << endl;
		out << opponent << endl << 200 << endl;
		displayArray(Root.tempArray);
#endif
	}
	else
	{
			Root.val = Root.calval;
			print_max_file(Root);
	}
		displayArray_file(Root.tempArray);
		out << "Node," << "Depth," << "Value," << "Alpha," << "Beta" << endl;
	display_log_file();
	alpha_beta = false;
}
//########################-----Competition-------###############################
int player_count=0, opponent_count=0;
int findValue_Comp(Reverci array[8][8])
{
	int sum = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		if (array[i][j].x == player)
			sum += weight[i][j];
		else if (array[i][j].x == opponent)
			sum -= weight[i][j];
		}
	return sum;
}
void count_player_opponent(Reverci array[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		if (array[i][j].x == player)
			player_count++;
		else if (array[i][j].x == opponent)
			opponent_count++;
		}
}
int count_heu(Reverci array[8][8])
{
	count_player_opponent(array);
	int temp = (100 * (player_count - opponent_count)) / (player_count + opponent_count);
	player_count = 0;
	opponent_count = 0;
	return temp;
}
int count_boundary(Reverci array[8][8])
{
	int p_count = 0, o_count = 0;
	if (array[0][0].x == player)
	{
		for (int i = 0; i < 8 && i!=1&&i!=7; i++)
		{
			if (array[0][i].x == player) 
				p_count++;
			if (array[i][0].x == player)
				p_count++;
			if (array[0][i].x == opponent)
				o_count++;
			if(array[i][0].x == opponent)
				o_count++;
		}
	}
		if (array[0][7].x == player)
		{
			for (int i = 0; i < 8; i++)
			{
				if (array[0][i].x == player)
					p_count++;
				if(array[i][7].x == player)
					p_count++;
				if (array[0][i].x == opponent)
					o_count++;
				if( array[i][7].x == opponent)
					o_count++;
			}
		}
		if (array[7][0].x == player)
		{
			for (int i = 0; i < 8; i++)
			{
				if (array[7][i].x == player)
					p_count++;
				if(array[i][0].x == player)
					p_count++;
				if (array[7][i].x == opponent)
					o_count++;
				if(array[i][0].x == opponent)
					o_count++;
			}
		}
		if (array[7][7].x == player)
		{
			for (int i = 0; i < 8; i++)
			{
				if (array[7][i].x == player)
					p_count++;
				if(array[7][0].x == player)
					p_count++;
				if (array[7][i].x == opponent)
					o_count++;
				if(array[7][0].x == opponent)
					o_count++;
			}
		}
		//cout << endl<<"count : " << p_count << " : "<< o_count << endl;
			return p_count - o_count;
}
void findPosition_Comp(vector <Position> &pos, Reverci array[8][8])
{
	swap(&player,&opponent);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
		if (array[i][j].x == player)
			findIndex(pos, i, j, array);
		}
	swap(&player, &opponent);
}
int count_corner(Reverci array[8][8])
{
	int p_count = 0, o_count = 0;
	// 0,0
	if (array[0][0].x == player)
		p_count++;
	else if (array[0][0].x == opponent)
		o_count++;
	// 7,7
	if (array[7][7].x == player)
		p_count++;
	else if (array[7][7].x == opponent)
		o_count++;
	// 0,7
	if (array[0][7].x == player)
		p_count++;
	else if (array[0][7].x == opponent)
		o_count++;
	// 7,0
	if (array[7][0].x == player)
		p_count++;
	else if (array[7][0].x == opponent)
		o_count++;
	return (p_count - o_count);
}
int count_edge(Reverci array[8][8])
{
	int p_count= 0, o_count = 0;
	if (array[0][0].x == '*')
	{
		if (array[0][1].x == player)
			p_count++;
		else if (array[0][1].x == opponent)
			o_count++;
		if (array[1][1].x == player)
			p_count++;
		else if (array[1][1].x == opponent)
			o_count++;
		if (array[1][0].x == player)
			p_count++;
		else if (array[1][0].x == opponent)
			o_count++;
	}

	if (array[7][7].x == '*')
	{
		if (array[7][6].x == player)
			p_count++;
		else if (array[7][6].x == opponent)
			o_count++;
		if (array[6][6].x == player)
			p_count++;
		else if (array[6][6].x == opponent)
			o_count++;
		if (array[6][7].x == player)
			p_count++;
		else if (array[6][7].x == opponent)
			o_count++;
	}

	if (array[7][0].x == '*')
	{
		if (array[6][0].x == player)
			p_count++;
		else if (array[6][0].x == opponent)
			o_count++;
		if (array[6][1].x == player)
			p_count++;
		else if (array[6][1].x == opponent)
			o_count++;
		if (array[7][1].x == player)
			p_count++;
		else if (array[7][1].x == opponent)
			o_count++;
	}

	if (array[0][7].x == '*')
	{
		if (array[0][6].x == player)
			p_count++;
		else if (array[0][6].x == opponent)
			o_count++;
		if (array[1][6].x == player)
			p_count++;
		else if (array[1][6].x == opponent)
			o_count++;
		if (array[1][7].x == player)
			p_count++;
		else if (array[1][7].x == opponent)
			o_count++;
	}

	return (o_count - p_count);
}
Node max_value_Comp(Node state)
{
	swap(&player, &opponent);
	if (state.depth < 0)
	{
		state.val = state.calval;
		if (state.pass == false)
		return state;
	}
	else if (!boardState(state.tempArray))
	{
		state.val = state.calval;
		return state;
	}

	Node temp_state;
	findPosition(state.node_pos, state.tempArray);
	sort(state.node_pos.begin(), state.node_pos.end(), maxtrix_sort());
	//displaySelection(state.node_pos);


	if (state.node_pos.empty())
	{
		if (state.grandParent_pass == true)
		{
			state.val = state.calval;
			return state;
		}

		arrayCopy(temp_state.tempArray, state.tempArray);
		//displayArray(temp_state.tempArray);
		findPositionTemp(temp_state.node_pos, temp_state.tempArray);
		if (!temp_state.node_pos.empty())
		{
			temp_state.pass = false;
		}
		else
			temp_state.pass = true;
		temp_state.isMax = false;
		if (state.pass == true)
			temp_state.parent_pass = true;
		else
			temp_state.parent_pass = false;
		temp_state.depth = state.depth - 1;
		temp_state.val = INT_MAX;
		temp_state.endGame = false;
		temp_state.calval =state.calval;
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		//cout <<endl<<"-----------------"<<temp_state.val<<endl;
		//cout <<"MAX : " <<" X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node min = min_value_Comp(temp_state);
		swap(&player, &opponent);
		state.val = min.val;
		//if (min.pass == true && state.pass == true)
		//{
		//	state.endGame = true;
		//	return state;
		//}
		//if (min.pass == true)
		//{
		if (min.val > state.val)
		{
			state.val = min.val;
			state.best_x = min.x;
			state.best_y = min.y;
			//cout << "MAX : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		}
		
		return state;
	}
	while (!state.node_pos.empty())
	{
		arrayCopy(temp_state.tempArray, state.tempArray);
		temp_state.tempArray[state.node_pos.back().x][state.node_pos.back().y].x = player;
		colorChange(state.node_pos.back().x, state.node_pos.back().y, temp_state.tempArray);
		//displayArray(temp_state.tempArray);
		temp_state.isMax = false;
		temp_state.pass = false;
		temp_state.parent_pass = false;
		temp_state.depth = state.depth - 1;
		temp_state.x = state.node_pos.back().x;
		temp_state.y = state.node_pos.back().y;
		temp_state.best_x = state.node_pos.back().x;
		temp_state.best_y = state.node_pos.back().y;
		temp_state.endGame = false;
		temp_state.val = INT_MAX;
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		int valueNode = findValue(temp_state.tempArray);
		int countPlayer = count_heu(temp_state.tempArray);
		int cornerVal = count_corner(temp_state.tempArray);
		int countE = count_edge(temp_state.tempArray);
		temp_state.calval = (valueNode) +countPlayer +(10 * cornerVal)+(5*countE);
		//state.node_pos.back().val;
		//cout << "####" << valueNode <<" "<< countPlayer << endl;
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		//cout <<endl<<"-----------------"<<temp_state.val<<endl;
		//cout <<"MAX : " <<" X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node min = min_value_Comp(temp_state);
		swap(&player, &opponent);
		if (min.val > state.val)
		{
			state.val = min.val;
			state.best_x = min.x;
			state.best_y = min.y;
			//cout << "MAX : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		}
		if (alpha_beta)// && !min.endGame)
		{
			if (state.val >= state.beta)
			{
				state.alpha = MAX(state.alpha, state.val);
				//	state.alpha = MAX(state.alpha, state.val);
				return state;
			}
			else
				state.alpha = MAX(state.alpha, state.val);
		}
		state.node_pos.pop_back();
	}
	return state;
}
Node min_value_Comp(Node state)
{
	swap(&player, &opponent);
	if (state.depth < 0)
	{
		state.val = state.calval;
		if (state.pass == false)
		return state;
	}
	else if (!boardState(state.tempArray))
	{
		state.val = state.calval;
		return state;
	}
	Node temp_state;
	findPosition(state.node_pos, state.tempArray);
	sort(state.node_pos.begin(), state.node_pos.end(), maxtrix_sort());
	//displaySelection(state.node_pos);
	if (state.node_pos.empty())
	{
		if (state.grandParent_pass == true)
		{
			state.val = state.calval;
			return state;
		}
		arrayCopy(temp_state.tempArray, state.tempArray);
		//displayArray(temp_state.tempArray);
		findPositionTemp(temp_state.node_pos, temp_state.tempArray);
		if (!temp_state.node_pos.empty())
		{
			//	print_max_file(state);
			temp_state.pass = false;
		}
		else
			temp_state.pass = true;
		temp_state.isMax = true;
		temp_state.pass = true;
		temp_state.depth = state.depth - 1;
		if (state.pass == true)
			temp_state.parent_pass = true;
		else
			temp_state.parent_pass = false;
		temp_state.endGame = false;
		temp_state.calval = state.calval;
		temp_state.val = INT_MIN;
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		//cout << endl << "-----------------" << temp_state.val<<endl;
		//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node max = max_value_Comp(temp_state);
		swap(&player, &opponent);
		state.val = max.val;
		/*	if (max.pass == true && state.pass == true)
		{
		state.endGame = true;
		return state;
		}
		*/
		if (max.val < state.val)
		{
			state.val = max.val;
			state.best_x = max.x;
			state.best_y = max.y;
			//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		}
		if (alpha_beta)//&& ! max.endGame)
		{
			if (state.val > state.alpha)
			{
				state.beta = MIN(state.beta, state.val);
				return state;
			}
		}
		else
		/*else
		if (max.endGame)
		{
		#ifdef PRINT
		print_min_file(state);
		#endif
		}*/
		return state;
	}

	while (!state.node_pos.empty())
	{
		arrayCopy(temp_state.tempArray, state.tempArray);
		temp_state.tempArray[state.node_pos.back().x][state.node_pos.back().y].x = player;
		colorChange(state.node_pos.back().x, state.node_pos.back().y, temp_state.tempArray);
		//displayArray(temp_state.tempArray);
		temp_state.isMax = true;
		temp_state.pass = false;
		temp_state.parent_pass = false;
		temp_state.depth = state.depth - 1;
		temp_state.x = state.node_pos.back().x;
		temp_state.y = state.node_pos.back().y;
		temp_state.best_x = state.node_pos.back().x;
		temp_state.best_y = state.node_pos.back().y;
		temp_state.endGame = false;
		temp_state.val = INT_MIN;
		if (state.pass && state.parent_pass)
			temp_state.grandParent_pass = true;
		else
			temp_state.grandParent_pass = false;
		swap(&player, &opponent);
		int valueNode = findValue(temp_state.tempArray);
		swap(&player, &opponent);
		int countPlayer = count_heu(temp_state.tempArray);
		int cornerVal = count_corner(temp_state.tempArray);
		int countE = count_edge(temp_state.tempArray);
		temp_state.calval = (valueNode) + countPlayer +(10 * cornerVal) + (5*countE);
		//state.node_pos.back().val;
		//cout << "####" << valueNode << " "<< countPlayer<<endl;
		if (alpha_beta)
		{
			temp_state.alpha = state.alpha;
			temp_state.beta = state.beta;
		}
		//cout << endl << "-----------------" << temp_state.val<<endl;
		//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		Node max = max_value_Comp(temp_state);
		swap(&player, &opponent);
		if (max.val < state.val)
		{
			state.val = max.val;
			state.best_x = max.x;
			state.best_y = max.y;
			//cout << "MIN : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
		}
		if (alpha_beta)// && !max.endGame)
		{
			if (state.val <= state.alpha)
			{
				state.beta = MIN(state.beta, state.val);
				//	state.beta = MIN(state.beta, state.val);
				return state;
			}
			else
				state.beta = MIN(state.beta, state.val);
		}
		state.node_pos.pop_back();
	}
	return state;
}
void findAlphaBeta_Comp(Reverci array[8][8], int depth)
{
	Node Root;
	Root.depth = depth;
	alpha_beta = true;
	Root.alpha = INT_MIN;
	Root.beta = INT_MAX;
	Root.val = INT_MIN;
	Root.isMax = true;
	Root.endGame = false;
	arrayCopy(Root.tempArray, array);
	findPosition(Root.node_pos, Root.tempArray);
	if (Root.node_pos.empty())
		Root.pass = true;
	else
		Root.pass = false;
	Root.grandParent_pass = false;
	Root.parent_pass = false;
	//	Root.pass = false;
	Root.calval = findValue(Root.tempArray);
	if (cutoff > -1 && boardState(Root.tempArray))
	{
		swap(&player, &opponent);
		Node result = max_value_Comp(Root);
		if (result.pass == false)
		{
#ifndef GAME
			out << char('a' + (result.best_y)) << result.best_x + 1 /*<< " -> " << result.val */<< endl;
#endif
			//print_max_file(result);
			/*Root.tempArray[result.best_x][result.best_y].x = player;
			colorChange(result.best_x, result.best_y, Root.tempArray);*/
}
		else
		{
			swap(&player, &opponent);
			if (result.val > Root.val)
			{
				Root.val = result.val;
				Root.best_x = result.x;
				Root.best_y = result.y;
				//	cout << "MAX : " << " X : " << state.node_pos.back().x << " y: " << state.node_pos.back().y << " val : " << state.node_pos.back().val << " -->> " << char('a' + (state.node_pos.back().y)) << state.node_pos.back().x + 1 << endl;
			}
		}
#ifdef GAME
		if (result.pass == true)
		{
			endc++;
		}
		out << 3 << endl;
		out << opponent << endl << 6 << endl;
		if (result.pass == false)
		{
			Root.tempArray[result.best_x][result.best_y].x = player;
			colorChange(result.best_x, result.best_y, Root.tempArray);
		}
		displayArray_file(Root.tempArray);
#endif
	}
#ifdef GAME
	LOG temp(Root.calval, Root.depth - cutoff);
	weight_log.push_back(temp);
#endif
	/*displayArray_file(Root.tempArray);
	out << "Node," << "Depth," << "Value," << "Alpha," << "Beta" << endl;*/
	//display_log_file();
	alpha_beta = false;
}
void competition(Reverci array[8][8], float seconds)
{
	//chrono::time_point<std::chrono::system_clock> start, end;
	//start = chrono::system_clock::now();
	cutoff = 3;
	findAlphaBeta_Comp(array, cutoff);
	/*end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	time_t end_time = chrono::system_clock::to_time_t(end);*/

	/*cout << "finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";
	cout << "Time Remaining  : " << seconds - elapsed_seconds.count()<<endl;*/
}
//###############################################################################
int main()
{
	int task = -1;
	float seconds;
	vector <Position> pos;
	Reverci array[8][8];
	in >> task;
	in >> player;
	if (player == 'X')
		opponent = 'O';
	else
		opponent = 'X';
	if (task == 4)
		in >> seconds;
	else
	     in >> cutoff;
	cutoff-=1;
	inputArray(in,array);
	//displayArray(array);
	in.close();
	switch (task)
	{
	case 1:
		findPosition(pos,array);
		if (pos.empty())
		{
			out << "pass";
			break;
		}
		sort(pos.begin(), pos.end());
		//std::cout <<char('a' + (pos.back().y))<< pos.back().x+1;
		array[pos.back().x][pos.back().y].x = player;
		colorChange(pos.back().x, pos.back().y, array);
		displayArray_file(array);
		//displaySelection(pos);
		break;
	case 2:
		findMinMax(array,cutoff);
		break;
	case 3 : 
		findAlphaBeta(array, cutoff);
#ifdef GAME
		std::cout << endl << "Let me find a safe zone !!!" << endl;
		out.close();
		in.close();
		in.open("output.txt");
		out.open("input.txt");
		if (endc == 1 && endab == 1)
		{
			cout << endl << "war ends" << endl;
			out.close();
			out.open("log.txt");
			for (int i = 0; i < weight_log.size(); i++)
				out << i << " " << weight_log[i].w << endl;
			break;
		}
		endc = 0;
		main();
#endif
		break;
	case 4 :
		std::cout << endl << "Let the War Begin !!!" << endl;
		competition(array, seconds);
#ifdef GAME
		out.close();
		in.close();
		out.open("output.txt");
		in.open("input.txt");
		if (endc == 1 && endab == 1)
		{
			cout << endl << "war ends"<<endl;
			out.close();
			out.open("log.txt");
			for (int i = 0; i < weight_log.size(); i++)
				out << i << " " << weight_log[i].w << endl;
			break;
		}
		endab = 0;
		main();
#endif
		break;
	}
	in.close();
	out.close();
	return 0;
}
