/*
    CSCI 262 Data Structures, Spring 2019, Project 2 - mazes

    maze_solver.cpp

    Code for the maze_solver class.  This class will use stack-based depth
    first search or queue-based breadth first search to find a solution (if
    one exists) to a simple maze.

    C. Painter-Wakefield
*/

#include "maze_solver.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;
// read the complete assignment instructions before beginning.  Also look
// at maze_solver.h - you will need to modify it as well.  In this file you
// will need to complete several methods, and add any others as needed for your
// solution. 


/*
 _read_maze()

 Read in maze information (rows, columns, maze) from the provided input stream.
*/
void maze_solver::_read_maze(istream& in) {
    string p;
    int i = 0;
    while(!in.eof()){
        getline(in,p,'\n');
        if(i == 0){
            i++;
            continue;
        }
        if(p.length() > 0){
            _rows = p.length() - 1;
        }
        _maze.push_back(p);
        i++;
    }
    _columns = _maze.size() - 1;
}


/*
 _write_maze()

 Output the (partially or totally solved) maze on cout.
*/
void maze_solver::_print_maze() {
    cout << "rows: " << _rows << "\n";
    cout << "columns: " << _columns << "\n";

    for(const string& s: _maze){
        cout << s << '\n';
    }
}


/*
 _initialize()

 Find the start point.  Push or enqueue the start point.
*/
void maze_solver::_initialize() {
    point* start;
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _columns; ++j) {
            if(_maze[i][j] == 'o'){
                if(_use_stack){
                    _stk.push(point(i,j,_maze[i][j]));
                } else {
                    _q.push(point(i,j,_maze[i][j]));
                }
                start = new point(i,j,_maze[i][j]);
            }
        }
    }
    cout << "starting point: (" << start->x << ", " << start->y << ")\n";
}


/*
 _step()

 Take *one* step towards solving the maze, setting _no_more_steps and
 _goal_reached as appropriate.  This implements the essential maze search
 algorithm; take the next location from your stack or queue, mark the
 location with '@', add all reachable next points to your stack or queue,
 etc.

 Notes:

 - Your initial point should have been pushed/enqueued in the _initialize()
   method.

 - Set the _no_more_steps variable to true when there are no more reachable
   points, or when the maze was successfully solved.

 - Set the _goal_reached variable to true if/when the maze was successfully
   solved.

 - You should write an '@' over every location you have previously visited,
   *except* for the start and goal positions - they should remain 'o' and '*',
   respectively.

 - Since the run() method will call _print_maze() in between calls to _step(),
   you probably want to try to make progress on each call to step.  On some
   mazes, depending on your approach, it may be possible to add the same
   point multiple times to your stack or queue - i.e., if a point is reachable
   from multiple other points.  When this happens, your code will work, but
   it may make very slow progress.  To fix the problem, simply run a loop to
   remove any already visited points from your stack or queue at the beginning
   or end of _step().

 - There are many approaches to dealing with invalid/unreachable points; you
   can choose to add them and then remove them next time you get into _step(),
   you can choose to not add them in the first place, etc.

 - It is strongly recommended that you make helper methods to avoid code
   duplication; e.g., a function to detect that a point is out of bounds is
   a real help on the mazes with no walls.  You can also think about pulling
   out methods that do the actual acting on stacks/queues - this will reduce
   code duplication, since everything is identical between the two except
   for whether you use your stack or queue.  E.g., you could have methods
   named "_push", "_pop", "_next" which use the appropriate data structure.
*/



bool maze_solver::_is_valid_point(point a){
    if(a.x >= _rows || a.x < 0 || a.y >= _columns || a.y < 0 || a.visited || *a.val == '#') {
        return false;
    } else if (*a.val == '*') {
        _no_more_steps = true;
        _goal_reached = true;
        return false;
    }
    return true;
}

vector<maze_solver::point> maze_solver::_get_valid_points(point a) {
    vector<point> valid_points;
    point* p;
    for (vector<int> const &b: _positions) {
       if(_is_valid_point(point(a.x + b[0],a.y + b[1],_maze[a.x + b[0]][a.y + b[1]]))){
           p = new maze_solver::point(a.x + b[0],a.y + b[1],_maze[a.x + b[0]][a.y + b[1]]);
           valid_points.push_back(*p);
       }
    }
    return valid_points;
}

void maze_solver::_add_valid_points(stack<point>& stk){
    point* p;
    vector<point> points_to_add;
    while (!_stk.empty()){
        p = &_stk.top();
        if(*p->val == '.'){
            *p->val = '@';
        }
        points_to_add.insert(points_to_add.cend(),_get_valid_points(*p).cbegin(),_get_valid_points(*p).cend());
        // TODO: get top point,"visit" the point, find valid points, add all new points to one vector, when stk or q is empty add next points to the stack or q;
        _stk.pop();
    }

}
void maze_solver::_add_valid_points(queue<point>& q) {

}

void maze_solver::_step() {
    if(_use_stack){
        _add_valid_points(_stk);
    } else {
        _add_valid_points(_q);
    }
}



/***************************************************************************
    You should not need to modify code below this point.  Touch at your own
    risk!
****************************************************************************/

/*
 maze_solver constructor

 You should not need to modify this.  It opens the maze file and passes
 the input stream to read_maze(), along with setting up some internal state
 (most importantly, the variable telling you whether to use a stack or
 a queue).
*/
maze_solver::maze_solver(string infile, bool use_stak, bool pause) {
	_use_stack = use_stak;
	_do_pause = pause;

	_no_more_steps = false;
	_goal_reached = false;

	// parse out maze name for later use in creating output file name
	int pos = infile.find(".");
	if (pos == string::npos) {
		_maze_name = infile;
	} else {
		_maze_name = infile.substr(0, pos);
	}

	// open input file and read in maze
	ifstream fin(infile);

	_read_maze(fin);

	fin.close();
}

/*
 run()

 Initializes the maze_solver, then runs a loop to keep moving forward towards
 a solution.

 While more steps are possible (while no_more_steps == false), run() calls
 step(), then calls print_maze(), then pause().  Once there are no more steps,
 it prints a success/failure message to the user (based on the goal_reached
 flag) and writes the final maze to a solution file.
*/

void maze_solver::run() {
    _initialize();

	cout << "Solving maze '" << _maze_name << "'." << endl;
	cout << "Initial maze: " << endl << endl;
	_print_maze();
	cout << endl;
	_pause();

	// main loop
	while (!_no_more_steps) {
		_step();
		cout << endl;
		_print_maze();
		cout << endl;
		_pause();
	}

	// final output to user
	cout << "Finished: ";
	if (_goal_reached) {
		cout << "goal reached!" << endl;
	} else {
		cout << "no solution possible!" << endl;
	}
}


/*
 pause()

 Pauses execution until user hits enter (unless do_pause == false).
*/
void maze_solver::_pause() {
	if (!_do_pause) return;
	string foo;
	cout << "Hit Enter to continue...";
	getline(cin, foo);
}



