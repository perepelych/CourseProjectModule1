#include <fstream>
#include <iostream>
#include <unistd.h>


bool file_check(std::ifstream & file);
char** inicialization(int&);
char** create_arr(int&, int&);
int cells(char**, int, int);
void new_world(char**, char**, int&);
void change_world(char**&, char**&);
void print(char **,int&, int&);
void exit_repeat_world(char**, char**, bool&);

char symbol = '*';
int pole_height = 0;
int pole_width = 0;

char** inicialization(int& alive_cells)
{
	char** world = nullptr;
	std::ifstream file("in.txt");
	if (file_check(file))
	{
		file.close();
		return nullptr;
	}
	file.clear();
	file.seekg(0);
	file >> pole_height >> pole_width;
	world = create_arr(pole_height, pole_width);
	for (int i = 0; i < pole_height; i++)
		for (int k = 0; k < pole_width; k++)
			world[i][k] = '-';
	int y = 0, x = 0;
	while (file >> y >> x)
	{
		world[y][x] = symbol;
		alive_cells++;
	}
	file.close();
	return world;
}

bool file_check(std::ifstream& file)
{
	if (!file.is_open())
	{
		std::cout << "Error: can't open file";
		return true;
	}

	int  y, x = 0;
	while (file >> y) 
    x++;
	if (x < 2)
	{
		std::cout << "Error: empty file or few data for twi-dim array";
		return true;
	}

	file.clear();	file.seekg(0);
	char s;
	while (file >> s)
		if (((int)s < 0x30) || ((int)s > 0x39))
		{
			std::cout << "Error: not all units digital";			
			return true;
		}

	file.clear();	file.seekg(0);
	x = 0;
	while (file >> y)	x++;
	if (x%2!= 0)
	{
		std::cout << "Error: odd units in file";
		return true;
	}

	file.clear();	file.seekg(0);
	int Y, X;
	file >> y >> x;
	while (file >> Y >> X)
	{
		if (Y > y-1 || X > x-1)
		{
			std::cout << "Error: out of array";			
			return true;
		}
	}
  return false;
}

char** create_arr(int& pole_height, int& pole_width)
{
	char**arr = new char* [pole_height];
	for (int i = 0; i < pole_height; i++)
		arr[i] = new char[pole_width];
	return arr;
}

int cells(char** world, int coord_Y, int coord_X)
{
	int cells_count = 0;
	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			if ((coord_Y + y < 0) || (coord_X + x < 0) || (coord_Y + y > pole_height - 1) || (coord_X + x > pole_width - 1) || (((coord_Y + y) == coord_Y) && ((coord_X + x) == coord_X)))
				continue;
			if (world[coord_Y + y][coord_X + x] == symbol)
				cells_count++;
		}
	}
	return cells_count;
};

void new_world(char** world, char** n_world, int& alive_cells)
{
	alive_cells = 0;
	for (int y = 0; y < pole_height; y++)
	{
		for (int x = 0; x < pole_width; x++)
		{
			int cells_count = cells(world, y, x);
			n_world[y][x] = '-';
			if (world[y][x] == '-' && (cells_count == 3))
			{
				n_world[y][x] = symbol; 
        alive_cells++;
			}
			if ((cells_count < 2) || (cells_count > 3))
				n_world[y][x] = '-';
			else if (world[y][x] == symbol)
			{
				n_world[y][x] = symbol; 
        alive_cells++;
			}
		}
	}
};

void change_world(char**& n_world, char**& world)
{
	char** ptr;
	ptr = n_world;
	n_world = world;
	world = ptr;
};

void print(char** arr, int& evo, int& alive)
{
	for (int y = 0; y < pole_height; y++)
	{
		std::cout << " ";
		for (int x = 0; x < pole_width; x++)
		{
			std::cout << arr[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << " Generation: " << evo << ". Alive cells: " << alive << std::endl;

};

void exit_repeat_world(char** new_world, char** world, bool& game_over)
{
	int signal = 0;
	for (int y = 0; y < pole_height; y++)
	{
		for (int x = 0; x < pole_width; x++)
		{
			if (new_world[y][x] == world[y][x])
				signal++;
		}
	}
	if (signal == pole_height * pole_width)
	{
		std::cout << " The world has stagnated. Game over";
		game_over = false;
	}
};

int main()
{	
  system("clear");
	char** world;
	int evolution(1);
	int alive_cells = 0;
	bool game_over = true;
	if ((world = inicialization(alive_cells)) == nullptr)
    return 0;
	char** n_world = create_arr(pole_height, pole_width);  
	print(world, evolution, alive_cells);
	if (alive_cells == 0)
	  {game_over = false; std::cout << " Game over";}
    sleep(1);
	while (game_over)    
	{
		++evolution;
		system("clear");
		new_world (world, n_world, alive_cells);
		print(n_world, evolution, alive_cells);
		if (alive_cells == 0)
		{game_over = false; std::cout << " Game over";}
		if (game_over == true)
		  exit_repeat_world(n_world, world, game_over);
		  change_world(n_world, world);
      sleep(1);
	}
	for (int i = 0; i < pole_height; i++)
	{
		delete[] world[i];
		delete[] n_world[i];
	}
	delete[] world;
	delete[] n_world;
};