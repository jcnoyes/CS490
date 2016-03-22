/******************************************************************************
* Joseph Noyes                                                                *
* Program 2 - Page Table                                                      *
* Program simulates a page table that uses a second chance algorithm. Counts  *
* the number of page faults, page fault frequency, and statistics about the   *
* data set entered.                                                           *
******************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include "NoyesJPrg2.h"

using namespace std;

//function prototypes
bool readFile(list_element *h, string fname, message *m);
bool addToList(list_element *h, message *m, int pageValue);

//start of execution
void main()
{
	//variables needed
	string filename;
	list_element *head_ds1_3 = NULL;

	//Greet the user and ask for a file name
	cout << "Hello, welcome to Joseph Noyes' program 2" << endl;
	cout << "Please enter a file to read: ";
	cin >> filename;

	//read in from file

	//ready the message object for first read, set page size to 3
	message *dataSet1_Message = new message;
	dataSet1_Message->pageSize = 3;
	dataSet1_Message->dataSetNum = "Data Set 1";

	readFile(head_ds1_3, filename, dataSet1_Message);
}

//function used to read in data from file
bool readFile(list_element *h, string fname, message *m)
{
	//start an input file stream, obtain intergers seperated by commas
	ifstream inputfile(fname);

	//check if file exists
	if(inputfile.is_open() == false)
	{
		cout << "Error opening file, please make sure it exists." << endl;
		return false;
	}

	else //file is open
	{
		string line = "";
		while(getline(inputfile, line)!= NULL)
		{
			int itemCount = 0;
			//parse line by commas, assumed it is a csv file
			for(int i = 0; i != line.length(); i++)
			{
				if(line[i] != ',')
				{
					cout << line[i] << " ";
					itemCount++;
				}
			}
			//place itemCount in message
			cout << endl << "Number of items: " << itemCount;
			m->itemCount = itemCount;

			return true;
		}
	}

}

//function used to add an element to the list
bool addToList(list_element *h, message *m, int pageValue)
{
	//if first item in the list
	if(h == NULL)
	{
		h = new list_element;
		h->dataValue = pageValue;
		h->isHead = true;
		h->refBit = true;
		h->next = NULL;
		m->pageFaults = 1;
	}
	/*else go through each item in the list, try to find it
	if it does not exist in the list, page fault and add it */
	else
	{
		
	}
	return true;
}