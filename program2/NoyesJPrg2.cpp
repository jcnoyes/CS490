/******************************************************************************
* Joseph Noyes                                                                *
* Program 2 - Page Table                                                      *
* Program simulates a page table that uses a second chance algorithm. Counts  *
* the number of page faults, page fault frequency, and statistics about the   *
* data set entered.                                                           *
******************************************************************************/

#include <iostream>
#include <string>
#include "NoyesJPrg2.h"

using namespace std;

//start of execution
void main()
{
	//variables needed
	string filename;
	list_element *head = NULL;

	//Greet the user and ask for a file name
	cout << "Hello, welcome to Joseph Noyes' program 2" << endl;
	cout << "Please enter a file to read: ";
	cin >> filename;
}