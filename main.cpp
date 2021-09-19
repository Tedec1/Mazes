/*
    CSCI 262 Data Structures, Spring 2019, Project 2 - mazes

    main.cpp

    Provides the user interface for launching the maze solver.

    C. Painter-Wakefield
*/

#include <iostream>
#include <fstream>
#include <string>

#include "maze_solver.h"

using namespace std;


// You should not modify any of this code.  Changing it may cause
// difficulty for the grader.  Touch at your own risk!


/*
 file_exists()

 open/close the file to test for existence.  This is lame, but
 the best we can do until the experimental filesystem library
 is standardized.
*/
bool file_exists(string filename) {
    ifstream test(filename);
    if (test) {
        test.close();
        return true;
    }
    return false;
}


/*
 main()

 Gets user inputs, creates and runs maze_solver object.
*/
int main(int argc, char* argv[]) {
	string infile;
	string s_or_q;
	bool do_pause = true;
	bool use_stack = true;

	// Check for command line arguments for maze filename and stack/queue
	// selection.  An optional third argument lets the grader circumvent
	// the pause between UI refreshes for automated testing purposes.
	if (argc >= 3) {
		infile = argv[1];
		s_or_q = argv[2];
		if (s_or_q != "Q" && s_or_q != "S") {
		    cerr << "Unrecognized argument: please specify stack (S) or queue (Q).  Exiting." << endl;
		}
		if (argc == 4 && string(argv[3]) == "false") do_pause = false;
		if (!file_exists(infile)) {
		    cerr << "Error opening file \"" << infile << "\" for reading, exiting." << endl;
		    return -1;
		}
	}
	else {
		cout << "Please enter a maze filename: ";
		getline(cin, infile);
		while (!file_exists(infile)) {
		    cout << "Error opening file \"" << infile << "\" for reading." << endl;
            cout << "Please enter a maze filename: ";
            getline(cin, infile);
        }
		cout << "Please enter (S) to use a stack, or (Q) to use a queue: ";
		getline(cin, s_or_q);
		while (s_or_q != "Q" && s_or_q != "S") {
            cout << "Input error: please specify stack (S) or queue (Q): ";
            getline(cin, s_or_q);
		}
	}

	use_stack = (s_or_q == "S");

	// create the maze_solver object and run it with the user inputs
	maze_solver solver(infile, use_stack, do_pause);
	solver.run();

	return 0;
}

