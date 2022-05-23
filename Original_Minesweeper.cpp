// Original_Minesweeper.cpp : Defines the entry point for the console application.
//


#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include<sstream>
#include <iterator>
#include <windows.h>
#include <time.h>


using namespace std;
enum Minefield {
	EMPTY,
	ALL_CLEAR,
	MINE
};
struct Position
{
	int x;
	int y;
};
struct Velocity
{
	int x;
	int y;
};
Position p;
Velocity v;
void gravity();
void inertia();
void layMines(int);
void showGame(int, int);
void showMines(int, int);
void checkIfClear(int, int);
void seeder();
int neighborhood(int, int);
bool checkIfInputIsTwoCoordinates(string);
bool checkInputIsNumeric(string input);
bool checkCoordinatesAreWithinMinefield(string input);
Position getCoordinates(string);
bool does_exist(const vector<vector<Minefield>>&, Minefield);

vector<Minefield> row(30, EMPTY);
vector<vector<Minefield>> minefield(30, row);
vector<vector<Minefield>> example_minefield(30, row);
bool firstGo=true;

int main()
{
	Position player{ 1,1 };
	cout << "Welcome to Minesweeper 2021\n\nAlex Mair\n" << endl;
	Sleep(1000);
	cout << "Choose level : 2 - Easy 3 - Hard\n" << endl;
	int op = 0;
	string error_msg;
	do {
		cin >> op;
		if (op == 2)
		{
			layMines(100);
		}
		else if (op == 3)
		{
			layMines(200);
		}
		else
		{
			cout << "valid options are 2 - Easy 3 - Hard\n" << endl;
		}
	} while (op != 2 && op != 3);
	cin.ignore();
	cin.clear();
	
	//showMines(player.x, player.y);

	
	while (true)
	{
		system("CLS");
		cout << "Minesweeper 2021\n\nAlex Mair\n" << endl;
		cout << "To play, type in the coordinates\nof the square you want to minesweep in the form \"x y\" (e.g \"1 1\" or \"2 3\")\n" << endl;
		
		showGame(player.x, player.y);
		cout << error_msg;
		cout << "enter coordinates" << endl;
		error_msg="";
		string input="";
		
		getline(cin, input);
		
		if (!checkIfInputIsTwoCoordinates(input)
			|| !checkInputIsNumeric(input))
		{
			//cout<<"First if statement"<<endl;
			error_msg="Input must be two numeric coordinates\n";
			
			
		}
		else if (!checkCoordinatesAreWithinMinefield(input))
		{
			//cout<<"Second if statement"<<endl;
			error_msg = "The coordinates you entered are outside the minefield\n";
			
		}
		else{
			
			//cout<<"Third if statement"<<endl;
			player = getCoordinates(input);
		}
		
		
		
			
		
		if (minefield[player.y][player.x] == MINE)
		{
			system("CLS");
			showMines(player.x, player.y);
			cout << "You lost!" << endl;
			Sleep(1000);
			break;
		}
		if (!does_exist(minefield, EMPTY))
		{
			cout << "You won!" << endl;
			Sleep(1000);
			break;
		}
		checkIfClear(player.x, player.y);
		
	}
    return 0;
}

void showGame(int x, int y)
{
	for (size_t i = 0; i < minefield.size(); i++)
	{
		for (size_t j = 0; j < minefield.size(); j++)
		{
			if (j == 0 && i == 0)
			{
				cout << "[O]";
			}
			
			else if (j == 0)
			{
				if (i > 0 && i < 10)
				{
					cout << " "+to_string(i);
				}
				if(i>9 &&i<100)
				{
					cout << to_string(i);
				}
			}
			
			else if (i == 0 && j>0)
			{
				if (i == x || j<10)
				{
					cout << " " + to_string(j) + " ";
				}
				else
				{
					cout << "" + to_string(j) + " ";
				}
			}
			
			

			else if (j == x && i == y)
			{
				cout << ">-0";
			}
			
			else if (neighborhood(i, j) > 0 && minefield[i][j]==ALL_CLEAR)
			{
				cout << " " + to_string(neighborhood(i, j)) << " ";
			}
			else if (minefield[i][j] == ALL_CLEAR)
			{
				cout << "   ";
			}
			else
			{
				cout << "[ ]";
			}
			
		}
		cout << endl;
	}
}

void showMines(int x, int y)
{
	for (size_t i = 0; i < minefield.size(); i++)
	{
		for (size_t j = 0; j < minefield.size(); j++)
		{
			if (minefield[i][j] == MINE && x == j && y == i)
			{
				cout << "\\|/";
			}
			else if (minefield[i][j] == MINE)
			{
				cout << "(X)";
			}
			else
			{
				cout << "[ ]";
			}
		}
		cout << endl;
	}
}
void checkIfClear(int x, int y)
{
	//cout<<"checkIfClear"<<endl;
	
	int n = 1;
	bool mine_nearby = false;
	while (!mine_nearby && n<50){
			//cout<<"n="<<n<<endl;
		for (size_t i = y-n; i < y+n; i++)
		{
			for (size_t j = x - n; j <= x + n; j++)
			{
				if (((i >= y - n || i <= y + n) && (j == x + n || j == x - n)
					|| (j <= x + n || j >= x - n) && (i == y - n || i == y + n))
					&& minefield[i][j] == MINE)
				{
					mine_nearby = true;
				}
				else
				{
					minefield[i][j] = ALL_CLEAR;
				}
				
			}
		}
		n++;
	}

	
}

void gravity()
{
	srand(time(NULL));
	int a = rand() % 9 + 1;
	v.x -= 1;
	v.y += 1;
}

void layMines(int iterations)
{
	srand(time(NULL));
	
	v.x = 2;
	v.y = -5;
	for (size_t i = 0; i < iterations; i++)
	{
		inertia();
		gravity();
		seeder();
		
	}
}
void inertia()
{
	if (p.x + v.x <= 0 || p.x + v.x > minefield.size())
	{
		v.x = -v.x;
	}

	if (p.y + v.y <= 0 || p.y + v.y > minefield.size())
	{
		v.y = -v.y;
	}

	p.x += v.x;
	p.y += v.y;
}

void seeder()
{
	for (size_t i = 0; i < minefield.size(); i++)
	{
		for (size_t j = 0; j < row.size(); j++)
		{
			if (i == p.x && j == p.y)
			{
				minefield[i][j] = MINE;
			}
		}
	}
}

int neighborhood(int i, int j)
{
	int  neighbors = 0;

	if (i > 0 && i < minefield.size() - 1 && j > 0 && j < minefield.size() - 1)
	{
		for (size_t k = i - 1; k <= i + 1; k++)
		{
			for (size_t l = j - 1; l <= j + 1; l++)
			{
				if (minefield[k][l]==MINE && !(k == i && l == j))
				{

					neighbors++;
				}
			}
		}
	}
	return neighbors;
}

bool checkIfInputIsTwoCoordinates(string input)
{
	stringstream iss(input);

	vector<string>coordinates(istream_iterator<string>{iss}, std::istream_iterator<std::string>());
	//cout<<"checkIfInputIsTwoCoordinates"<<endl;

	if (coordinates.size() == 2)
	{
		return true;
	}
	return false;
}

bool checkInputIsNumeric(string input)
{
	stringstream iss(input);

	vector<string>coordinates(istream_iterator<string>{iss}, std::istream_iterator<std::string>());
	cout<<"checkInputIsNumeric"<<endl;


	try {
		stoi(coordinates[0]);
		stoi(coordinates[1]);

	}
	catch (invalid_argument)
	{
		return false;
	}
	return true;
}

bool checkCoordinatesAreWithinMinefield(string input)
{
	stringstream iss(input);
	vector<string>coordinates(istream_iterator<string>{iss}, std::istream_iterator<std::string>());
	//cout<<"checkCoordinatesAreWithinMinefield"<<endl;

	
	return stoi(coordinates[0]) > 0 && stoi(coordinates[0]) < minefield.size()
		&& stoi(coordinates[1]) > 0 && stoi(coordinates[1]) < minefield.size();
	
}

Position getCoordinates(string input)
{
	stringstream iss(input);

	vector<string>coordinates(istream_iterator<string>{iss}, std::istream_iterator<std::string>());
	
	

		Position position{ stoi(coordinates[0]),stoi(coordinates[1]) };

		if (position.x > 0 && position.x < minefield.size()
			&& position.y > 0 && position.y < minefield.size())
		{
			//cout<< " getCoordinates Position"<<endl;
			return position;
		}
		else {
			//cout<< " getCoordinates Exception"<<endl;
			throw exception();
		}
	

}

bool does_exist(const vector<vector<Minefield>>&  v, Minefield item) {

	vector<vector<Minefield>>::const_iterator row;

	for (row = v.begin(); row != v.end(); row++) {
		if (find(row->begin(), row->end(), item) != row->end())
			return true;
	}

	return false;
}
