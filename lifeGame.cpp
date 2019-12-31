#include "lifeGame.h"

int LifeGame::get_x_size(){
	return _x_size;
}


int LifeGame::get_y_size(){
	return _y_size;
}


void LifeGame::reset_game(){
	curr_time = 0;
	set_all_dead(curr_matrix);
	set_all_dead(next_matrix);
	set_all_dead(prev_matrix);
	while(!stack_matrix.empty())
    	stack_matrix.pop();
}


void LifeGame::create_matrix(vector<vector<int>>* matrix){
	for(int x = 0; x < _x_size; x++){
		for (int y = 0; y < _y_size; y++)
			(*matrix)[x][y] = DEAD;
	}
}


void LifeGame::curr_matrix_print(){
	cout << "curr_time=" << curr_time << endl;
	for(int y = 0; y < _y_size; y++){
		for (int x = 0; x < _x_size; x++)
			cout << (*curr_matrix)[x][y] ;
		cout <<  endl;
	}
	cout << endl;
}


int LifeGame::arround_alive_num_Torus(int x1, int y1){
	int alive_count = 0;
	int x0 = (x1-1 + _x_size) % _x_size;                         
    int x2 = (x1+1) % _x_size;            // (x0,y0),(x1,y0),(x2,y0) 
    int y0 = (y1-1 + _y_size) % _y_size;  // (x0,y1),(x1,y1),(x2,y1)
    int y2 = (y1+1) % _y_size;            // (x0,y2),(x1,y2),(x2,y2)
    if ((*curr_matrix)[x0][y0] == ALIVE)
    	alive_count++;
   	if ((*curr_matrix)[x0][y1] == ALIVE)
    	alive_count++;
    if ((*curr_matrix)[x0][y2] == ALIVE)
    	alive_count++;
    if ((*curr_matrix)[x1][y0] == ALIVE)
    	alive_count++;
    if ((*curr_matrix)[x1][y2] == ALIVE)
    	alive_count++;
    if ((*curr_matrix)[x2][y0] == ALIVE)
    	alive_count++;
    if ((*curr_matrix)[x2][y1] == ALIVE)
    	alive_count++;
    if ((*curr_matrix)[x2][y2] == ALIVE)
    	alive_count++;
    return alive_count;
}


int LifeGame::arround_alive_num_KleinBT(int x1, int y1){
	int alive_count = 0;
	int x0 = (x1-1 + _x_size) % _x_size;                         
    int x2 = (x1+1) % _x_size;            // (x0,y0),(x1,y0),(x2,y0) 
    int y0 = (y1-1 + _y_size) % _y_size;  // (x0,y1),(x1,y1),(x2,y1)
    int y2 = (y1+1) % _y_size;            // (x0,y2),(x1,y2),(x2,y2)
    if (x1 == 0){
    	if ((*curr_matrix)[x0][(_y_size-y0-1) % _y_size] == ALIVE)
    		alive_count++;
        if ((*curr_matrix)[x0][(_y_size-y1-1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x0][(_y_size-y2-1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y0] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y1] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y2] == ALIVE)
        	alive_count++;
    }
    else if (x1 == _x_size-1){
    	if ((*curr_matrix)[x0][y0] == ALIVE)
    		alive_count++;
        if ((*curr_matrix)[x0][y1] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x0][y2] == ALIVE)
        	alive_count++; 
        if ((*curr_matrix)[x2][(_y_size-y0-1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][(_y_size-y1-1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][(_y_size-y2-1) % _y_size] == ALIVE)
        	alive_count++;
    }
    else{
	    if ((*curr_matrix)[x0][y0] == ALIVE)
	    	alive_count++;
	   	if ((*curr_matrix)[x0][y1] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x0][y2] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y0] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y1] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y2] == ALIVE)
	    	alive_count++;
    }
    if ((*curr_matrix)[x1][y0] == ALIVE)
	    alive_count++;
	if ((*curr_matrix)[x1][y2] == ALIVE)
	    alive_count++;
    return alive_count;
}


int LifeGame::arround_alive_num_twist_Cylinder(int x1, int y1){
	int alive_count = 0;
	int x0 = (x1-1 + _x_size) % _x_size;                         
    int x2 = (x1+1) % _x_size;            // (x0,y0),(x1,y0),(x2,y0) 
    int y0 = (y1-1 + _y_size) % _y_size;  // (x0,y1),(x1,y1),(x2,y1)
    int y2 = (y1+1) % _y_size;            // (x0,y2),(x1,y2),(x2,y2)
    if (x1 == 0){
    	if ((*curr_matrix)[x1][(_y_size/2+y0) % _y_size] == ALIVE)
    		alive_count++;
        if ((*curr_matrix)[x1][(_y_size/2+y1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x1][(_y_size/2+y2) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y0] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y1] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y2] == ALIVE)
        	alive_count++;
    }
    else if (x1 == _x_size-1){
    	if ((*curr_matrix)[x0][y0] == ALIVE)
    		alive_count++;
        if ((*curr_matrix)[x0][y1] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x0][y2] == ALIVE)
        	alive_count++; 
        if ((*curr_matrix)[x1][(_y_size/2+y0) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x1][(_y_size/2+y1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x1][(_y_size/2+y2) % _y_size] == ALIVE)
        	alive_count++;
    }
    else{
	    if ((*curr_matrix)[x0][y0] == ALIVE)
	    	alive_count++;
	   	if ((*curr_matrix)[x0][y1] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x0][y2] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y0] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y1] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y2] == ALIVE)
	    	alive_count++;
    }
    if ((*curr_matrix)[x1][y0] == ALIVE)
	    alive_count++;
	if ((*curr_matrix)[x1][y2] == ALIVE)
	    alive_count++;
    return alive_count;
}


int LifeGame::arround_alive_num_twist_Torus(int x1, int y1){
	int alive_count = 0;
	int x0 = (x1-1 + _x_size) % _x_size;                         
    int x2 = (x1+1) % _x_size;            // (x0,y0),(x1,y0),(x2,y0) 
    int y0 = (y1-1 + _y_size) % _y_size;  // (x0,y1),(x1,y1),(x2,y1)
    int y2 = (y1+1) % _y_size;            // (x0,y2),(x1,y2),(x2,y2)
    if (x1 == 0){
    	if ((*curr_matrix)[x0][(_y_size/3+y0) % _y_size] == ALIVE)
    		alive_count++;
        if ((*curr_matrix)[x0][(_y_size/3+y1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x0][(_y_size/3+y2) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y0] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y1] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][y2] == ALIVE)
        	alive_count++;
    }
    else if (x1 == _x_size-1){
    	if ((*curr_matrix)[x0][y0] == ALIVE)
    		alive_count++;
        if ((*curr_matrix)[x0][y1] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x0][y2] == ALIVE)
        	alive_count++; 
        if ((*curr_matrix)[x2][(_y_size/3*2+y0) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][(_y_size/3*2+y1) % _y_size] == ALIVE)
        	alive_count++;
        if ((*curr_matrix)[x2][(_y_size/3*2+y2) % _y_size] == ALIVE)
        	alive_count++;
    }
    else{
	    if ((*curr_matrix)[x0][y0] == ALIVE)
	    	alive_count++;
	   	if ((*curr_matrix)[x0][y1] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x0][y2] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y0] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y1] == ALIVE)
	    	alive_count++;
	    if ((*curr_matrix)[x2][y2] == ALIVE)
	    	alive_count++;
    }
    if ((*curr_matrix)[x1][y0] == ALIVE)
	    alive_count++;
	if ((*curr_matrix)[x1][y2] == ALIVE)
	    alive_count++;
    return alive_count;
}



void LifeGame::rule(int x, int y, int mode){
	int alive_count;
	switch (mode){
		case TP_Torus:
			alive_count = arround_alive_num_Torus(x, y);
			break;
		case TP_KleinBottle:
			alive_count = arround_alive_num_KleinBT(x, y);
			break;
		case TP_twisted_Cylinder:
			alive_count = arround_alive_num_twist_Cylinder(x, y);
			break;
		case TP_twisted_Torus:
			alive_count = arround_alive_num_twist_Torus(x, y);
			break;
	}
	if ((*curr_matrix)[x][y] == DEAD){
		if (alive_count == 3)
			(*next_matrix)[x][y] = ALIVE;
		else
			(*next_matrix)[x][y] = DEAD;		
	}
	else{
		if (alive_count == 2 || alive_count == 3)
			(*next_matrix)[x][y] = ALIVE;
        else
        	(*next_matrix)[x][y] = DEAD;
	}
}


void LifeGame::set_curr_matrix(vector<vector<int>>* list1){//list{{1,2},{2,2},{3,2},{1,3},{2,4}};
	for(auto itr = list1->begin(); itr != list1->end(); ++itr){
		(*curr_matrix)[(*itr)[0]][(*itr)[1]] = ALIVE;
	}
}


void LifeGame::set_all_dead(vector<vector<int>>* matrix){
	for (int y = 0; y < _y_size; y++){
		for (int x = 0; x < _x_size; x++){
			(*matrix)[x][y] = DEAD;
		}
	}
}


void LifeGame::copy_matrix(vector<vector<int>>* src, vector<vector<int>>* dest){
	for (int y = 0; y < _y_size; y++){
		for (int x = 0; x < _x_size; x++){
			(*dest)[x][y] = (*src)[x][y];
		}
	}
}


void LifeGame::next_time(int mode){
	curr_time++;
	stack_matrix.push(*curr_matrix);
	set_all_dead(next_matrix);
	for(int y = 0; y < _y_size; y++){
		for (int x = 0; x < _x_size; x++){
			rule(x, y, mode);
		}
	}
	copy_matrix(next_matrix, curr_matrix);
}


void LifeGame::prev_time(){
	if (curr_time < 1)
		return;
	if (stack_matrix.empty())
		return;
	vector<vector<int>> prev_matrix = stack_matrix.top();
	stack_matrix.pop();
	copy_matrix(&prev_matrix, curr_matrix);
	curr_time--;
}


void LifeGame::cell_change(vector<vector<int>>* matrix, int x, int y, int live_dead){
	(*matrix)[x][y] = live_dead;
}
        

/*
int main() {
	LifeGame lgame(15, 12);
	vector<vector<int>> list{{1,2},{2,2},{3,2},{1,1},{2,0}};
	lgame.set_curr_matrix(&list);

	cout << "TP_Torus" << endl;
	lgame.curr_matrix_print();  
    for (int i; i < 20; i++){
    	lgame.next_time(TP_Torus);
    	lgame.curr_matrix_print();  
    }
    for (int i; i < 20; i++){
    	lgame.prev_time();
    	lgame.curr_matrix_print();  
    }

    lgame.reset_game();
    lgame.set_curr_matrix(&list);
    cout << "TP_KleinBottle" << endl;
	lgame.curr_matrix_print();  
    for (int i; i < 20; i++){
    	lgame.next_time(TP_KleinBottle);
    	lgame.curr_matrix_print();  
    }

    lgame.reset_game();
    lgame.set_curr_matrix(&list);
    cout << "TP_twisted_Cylinder" << endl;
	lgame.curr_matrix_print();  
    for (int i; i < 20; i++){
    	lgame.next_time(TP_twisted_Cylinder);
    	lgame.curr_matrix_print();  
    }

    lgame.reset_game();
    lgame.set_curr_matrix(&list);
    cout << "TP_twisted_Torus" << endl;
	lgame.curr_matrix_print();  
    for (int i; i < 20; i++){
    	lgame.next_time(TP_twisted_Torus);
    	lgame.curr_matrix_print();  
    }


	return 0;
}/**/