#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Value_Output.H>

#include<fstream>
#include<string>

#include "lifeGame.h"
#include "inih/INIReader.h"
      

// default
int cellSize = 24;
int width=53;
int height=24;
// See https://www.fltk.org/doc-1.3/drawing.html
int alive_color = FL_BLUE;
int screen_color = 246;// light blue
int line_color = FL_BLACK;

string font_name= "arial";
int font_size = 14;
int time_font_size = 16;
int interval_slider_font_size = 10;

string scheme_name = "gleam";

int screen_width;
int screen_height;
int button_width;
int button_height = 30;
int SP =10;

LifeGame *lg;
bool pause = true;
Fl_Toggle_Button *play_button;

Fl_Hor_Value_Slider* interval_slider;
double interval = 0.1;

Fl_Choice *topology_choice;
int topology_nr = TP_Torus;

Fl_Value_Output* time_count_box;
char File_open_filename[128];

Fl_Box* open_filename_box;
string open_filename;

void play_pause_button_setPause();
void reset();


void set_open_filename_box(string text) {
    sprintf(File_open_filename, "File: %s", text.c_str()); 
    open_filename_box->label(File_open_filename);
}


void set_curr_time_ch() {
    time_count_box->value(lg->curr_time);
}


class Screen : public Fl_Widget {
protected:
    int cellSize_div2 = cellSize/2;
    int cellSize_div2_subt2 = cellSize/2-2;
    int button_height_add_SP = button_height+SP;
    
    int handle(int e) {
        if (e == FL_PUSH){
            if((Fl::event_x() - SP) < 0 or (Fl::event_y() - (button_height_add_SP)) < 0)
                return 0;
            int x = (Fl::event_x() - SP) / cellSize;
            int y = (Fl::event_y() - (button_height_add_SP)) / cellSize;
            //cout << "(" << Fl::event_x() <<"," << Fl::event_y() << "), "<< "(x,y) = (" << x << "," << y << ")"  <<endl;
            if (x < 0 or x > width-1 or y < 0 or y > height -1)
                return 0;
            if ((*(lg->curr_matrix))[x][y] == ALIVE)
                lg->cell_change(lg->curr_matrix, x, y, DEAD);
            else
                lg->cell_change(lg->curr_matrix, x, y, ALIVE);
            redraw();
        }    
        return 0;
    }
   
    void draw() {
        fl_color(screen_color);
        fl_rectf(SP, button_height+SP, screen_width, screen_height);
       
        fl_begin_line();
        fl_color(line_color);
        int y = button_height + SP;
        for (int row = 0; row < height+1; row++)
            fl_line(SP, y+row*cellSize, screen_width+SP, y+row*cellSize);
        int x = 0;
        for (int col = 0; col < width+1; col++)
            fl_line(x+col*cellSize+SP, SP+button_height, x+col*cellSize+SP, screen_height+SP+button_height);
        fl_end_line();    
        fl_color(alive_color);
        for (int row = 0; row < height; row++)
            for (int col = 0; col < width; col++){
                if ((*(lg->curr_matrix))[col][row] == ALIVE){
                    fl_begin_polygon();
                    fl_arc(col*cellSize+SP +cellSize/2 , row*cellSize+SP+button_height +cellSize/2, cellSize/2-2, 0.1, 360.0);
                    fl_end_polygon();
                }          
            }     
    }

public:
    Screen(int X,int Y,int W,int H) : Fl_Widget(X,Y,W,H) {
    }
};

Screen *lg_screen;



bool set_live_points(string filename){
    ifstream ifs(filename);
    char str[256];
    if (!ifs) {
        cerr << "Failed to open file." << endl;
        return false;
    }    
    int x, y;
    lg->reset_game();
    while (ifs.getline(str, 256)) { 
        sscanf(str, "(%d,%d)", &x, &y);
        if (x < 0 || lg->get_x_size()-1 < x || y < 0 || lg->get_y_size()-1 < y)
            return false;
        (*(lg->curr_matrix))[x][y] = ALIVE;        
    }
    lg_screen->redraw();
    return true;
}


string GetFileName(const string &path){
    size_t pos1; 
    pos1 = path.rfind('\\');
    if(pos1 != string::npos){
        return path.substr(pos1+1, path.size()-pos1-1);
    }
     return path;
}


string GetExtension(const string &path){
    size_t pos1; 
    pos1 = path.rfind('.');
    if(pos1 != string::npos){
        return path.substr(pos1, path.size()-pos1);
    }
     return "";
}


static void open_file(Fl_Widget *w, void *) {
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Open Game of Life file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fnfc.filter("Life of Game file\t*.lg");
    fnfc.directory(".\\lgfiles\\");
    int result = fnfc.show();
    if (result == -1){
        cout << "ERROR:" << fnfc.errmsg() << endl;
        return;
    }
    else if (result == 1)
        return;
    open_filename = fnfc.filename();

    if(!set_live_points(open_filename)){
        fl_alert( "Warning: Bad File: Some point data is out of range ");
        reset();
        return;
    }
    set_curr_time_ch();
    play_pause_button_setPause();
    if (pause)
        play_button->value(0);
    set_open_filename_box(GetFileName(open_filename));
}


string get_alive_points(){
    string buf ="";
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++){           
            if ((*(lg->curr_matrix))[x][y] == ALIVE){
                cout << x << y << endl;
                buf += "(" + to_string(x) + "," + to_string(y) + ")\n"; 
            }                             
        }
    return buf;
}


static void save_as_file(Fl_Widget *w, void *) {
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Save As Game of Life file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    fnfc.filter("Life of Game file\t*.lg");
    fnfc.directory(".\\lgfiles\\");
    int result = fnfc.show();
    if (result == -1){
        cout << "ERROR:" << fnfc.errmsg() << endl;
        return;
    }
    else if (result == 1)
        return;
    string fname = fnfc.filename();
    string extension = GetExtension(fname);
    if (extension == "")
        fname += ".lg";
    else if(extension != ".lg"){
        int pos = fname.size() - extension.size();
        fname.replace(pos,extension.size(),".lg");
    }
    ofstream ofs(fname);
    if (!ofs) {
        cerr << "Failed to open file." << endl;
        return;
    }
    ofs << get_alive_points();
    set_open_filename_box(GetFileName(fname));
}


static void quit(Fl_Widget *w, void *) { 
    exit(0);
}


void nexttime_button_cb(Fl_Widget *,void *) {
    lg->next_time(topology_nr);
    lg_screen->redraw();
    set_curr_time_ch();
}


void prevtime_button_cb(Fl_Widget *,void *) {
    lg->prev_time();
    lg_screen->redraw();
    set_curr_time_ch();
}


static void Timer_CB(void *data) {              // timer callback
    lg->next_time(topology_nr);
    lg_screen->redraw();
    Fl::repeat_timeout(interval, Timer_CB, data);
    set_curr_time_ch();
}


void play_pause_button_setPause(){
    Fl::remove_timeout(Timer_CB);
    play_button->label("&Play");
    pause = true;
}
        

void play_pause_button_setStart(){
    Fl::add_timeout(interval, Timer_CB);
    play_button->label("&Pause");
    pause = false; 
}


void play_button_cb(Fl_Widget *,void *) {
    if (pause) 
        play_pause_button_setStart();
    else
        play_pause_button_setPause();  
}


void set_interval(Fl_Widget *o, void *) {
    Fl_Hor_Value_Slider *slider = (Fl_Hor_Value_Slider *)o;
    interval = slider->value();
}


void choice_cb(Fl_Widget *, void *v) {
    topology_nr = (int) (fl_intptr_t)v; 
}


void reset(){
    lg->reset_game();
    lg_screen->redraw();
    set_curr_time_ch();
    topology_nr = TP_Torus;
    topology_choice->value(TP_Torus);
    play_pause_button_setPause();
    if (pause)
        play_button->value(0);
    set_open_filename_box("No name");

}


void reset_button_cb(Fl_Widget *, void *v) {
    reset();
}


Fl_Menu_Item choices[] = {
    {"Trus", 0, choice_cb, (void*)TP_Torus},
    {"KleinBottle", 0, choice_cb, (void*)TP_KleinBottle},
    {"1/2 twisted Cylinder", 0, choice_cb, (void*)TP_twisted_Cylinder},
    {"1/3 twisted Torus", 0, choice_cb, (void*)TP_twisted_Torus},
    {0}
};



int main(int argc, char **argv) {

    INIReader reader("lifeGameFLTK.ini");
    if (reader.ParseError() < 0) {
        cout << "Can't load 'lifeGameFLTK.ini'\n";
    }
    else{
        cellSize = reader.GetInteger("screen", "cellSize", -1);
        width = reader.GetInteger("screen", "width", -1);
        height = reader.GetInteger("screen", "height", -1);
        alive_color = reader.GetInteger("color", "alive_color", -1);
        screen_color = reader.GetInteger("color", "screen_color", -1);
        line_color = reader.GetInteger("color", "line_color", -1);
        font_name = reader.Get("font", "font_name", "UNKNOWN");
        font_size = reader.GetInteger("font", "font_size", -1);
        time_font_size = reader.GetInteger("font", "time_font_size", -1);
        interval_slider_font_size = reader.GetInteger("font", "interval_slider_font_size", -1);
        scheme_name = reader.Get("scheme", "scheme_name", "UNKNOWN");
    }

    screen_width = width * cellSize;
    screen_height = height * cellSize;
    button_width = screen_width/24;

    Fl::scheme(scheme_name.c_str());

    char title[] = "Conway\'s Game of Life in FLTK";
    Fl_Double_Window *win = new Fl_Double_Window(screen_width + SP*2, screen_height+button_height+SP*2, title);
    win->color(FL_DARK1);    


    Fl_Menu_Button *filebar = new Fl_Menu_Button(SP, 0, button_width*2, button_height, "&File"); 

    play_button = new Fl_Toggle_Button(SP+button_width*2, 0, button_width, button_height, "&Play");
    Fl_Repeat_Button *nexttime_button = new Fl_Repeat_Button(SP+button_width*3, 0, button_width, button_height, "Nex&t");
    Fl_Repeat_Button *prevtime_button = new Fl_Repeat_Button(SP+button_width*4, 0, button_width,button_height, "Pre&v");
    Fl_Button *reset_button = new Fl_Button(SP+button_width*5, 0, button_width, button_height, "&Reset");

    Fl_Box *slider_title = new Fl_Box(FL_FLAT_BOX,SP+button_width*6, 0, button_width*2, button_height, "Interval(sec):");
    slider_title->align(FL_ALIGN_INSIDE|FL_ALIGN_RIGHT);
    Fl_Hor_Value_Slider slider1(SP+button_width*8, 0, button_width*4.8, button_height, "");    
    
    topology_choice = new Fl_Choice(SP+button_width*12.8, 0, button_width*3.2, button_height);
    
    open_filename_box = new Fl_Box(FL_FLAT_BOX, SP+button_width*16, 0, button_width*5.5, button_height, "");
    Fl_Box *time_count_box_title = new Fl_Box(FL_FLAT_BOX, screen_width - button_width*3 +SP, 0, button_width,button_height, "Time");
    time_count_box_title->align(FL_ALIGN_INSIDE|FL_ALIGN_RIGHT);

    time_count_box = new Fl_Value_Output(screen_width - button_width*2 +SP, 0, button_width*2, button_height, "");
    time_count_box->align(FL_ALIGN_INSIDE|FL_ALIGN_RIGHT);


    filebar->add("&Open",  "^o", open_file);
    filebar->add("save&As",  "^a", save_as_file, 0, FL_MENU_DIVIDER);
    filebar->add("&Quit",  "^q", quit);
    
    lg = new LifeGame(width, height);
    lg->reset_game();
    lg_screen  = new Screen(0, 0, screen_width+SP*2, screen_height+button_width+SP*2);
    
    
    interval_slider = &slider1;
    slider1.align(FL_ALIGN_LEFT);
    slider1.step(0.001);
    slider1.bounds(0.0,0.200);
    slider1.callback(set_interval);
    slider1.value(interval);
    
    topology_choice->menu(choices); 
    
    set_curr_time_ch();
    time_count_box->align(FL_ALIGN_INSIDE|FL_ALIGN_LEFT);
    time_count_box->textsize(time_font_size);
    interval_slider->textsize(interval_slider_font_size);
    
    open_filename = "No name";
    set_open_filename_box(open_filename);
    open_filename_box->align(FL_ALIGN_INSIDE|FL_ALIGN_LEFT);    

    nexttime_button->callback(nexttime_button_cb);    
    prevtime_button->callback(prevtime_button_cb);    
    play_button->callback(play_button_cb);    
    reset_button->callback(reset_button_cb);

    Fl::set_font(FL_HELVETICA, font_name.c_str());
    filebar->labelsize(font_size);
    play_button->labelsize(font_size);
    nexttime_button->labelsize(font_size);
    prevtime_button->labelsize(font_size);
    reset_button->labelsize(font_size);
    slider_title->labelsize(font_size);
    topology_choice->textsize(font_size);
    time_count_box_title->labelsize(font_size);
    open_filename_box->labelsize(font_size);

    win->end();
    win->show();    
    
    return(Fl::run());
}
