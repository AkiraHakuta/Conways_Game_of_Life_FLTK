#ifndef LIFEGAME_H
#define LIFEGAME_H

#include <iostream>
#include <vector>
#include <stack>
using namespace std;


const int ALIVE = 1;
const int DEAD = 0;

//topology_nr
enum {TP_Torus, TP_KleinBottle, TP_twisted_Cylinder, TP_twisted_Torus};

class LifeGame{
private:	
	int _x_size;
	int _y_size;
	vector<vector<int>>* next_matrix = new vector<vector<int>>;
	vector<vector<int>>* prev_matrix = new vector<vector<int>>;
	stack<vector<vector<int>>> stack_matrix;
	void create_matrix(vector<vector<int>>* matrix);
	int arround_alive_num_Torus(int x1, int y1);
	int arround_alive_num_KleinBT(int x1, int y1);
	int arround_alive_num_twist_Cylinder(int x1, int y1);
	int arround_alive_num_twist_Torus(int x1, int y1);
	void rule(int x, int y, int mode);
	void copy_matrix(vector<vector<int>>* src, vector<vector<int>>* dest);
	void set_all_dead(vector<vector<int>>* matrix);

public:
	int get_x_size();
	int get_y_size();	
	vector<vector<int>>* curr_matrix = new vector<vector<int>>;
	int topology_nr;
	LifeGame(int x_size=14, int y_size=10){
		_x_size = x_size;
		_y_size = y_size;
		curr_matrix->assign(_x_size, vector<int>(_y_size, DEAD));
		next_matrix->assign(_x_size, vector<int>(_y_size, DEAD));
		prev_matrix->assign(_x_size, vector<int>(_y_size, DEAD));
		/*
		cout << "_x_size=" << _x_size << ", _y_size=" << _y_size << endl;
		vector<vector<int>> list{{1,2},{2,2},{3,2},{1,1},{2,0}};
		set_curr_matrix(&list);/**/
	};

	int curr_gen = 0;	
	void reset_game();	
	void curr_matrix_print();
	void set_curr_matrix(vector<vector<int>>* list);
		
	void next_gen(int mode);
	void prev_gen();
	void cell_change(vector<vector<int>>* matrix, int x, int y, int live_dead);
};

#endif

