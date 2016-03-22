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
bool readFile(list_element *&h, string fname, message *m);
bool addToList(list_element *&head, message *m, int pageValue);

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
	dataSet1_Message->pageTableSize = 3;
	dataSet1_Message->pageFaults = 0;
	dataSet1_Message->dataSetNum = "Data Set 1";

	readFile(head_ds1_3, filename, dataSet1_Message);
}

//function used to read in data from file
bool readFile(list_element *&h, string fname, message *m)
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
					int value = line[i] - 48;  //convert char to int by subtracting 48
					addToList(h, m, value);
					itemCount++;
				}
			}
			inputfile.close(); //close file
			//place itemCount in message
			cout << endl << "Number of items: " << itemCount;
			m->itemCount = itemCount;

		}
		return true;
	}

}

//function used to add an element to the list
bool addToList(list_element *&head, message *m, int pageValue)
{
	//if first item in the list
	if(head == NULL)
	{
		head = new list_element();
		head->dataValue = pageValue;
		head->isHead = true;
		head->refBit = true;
		head->next = NULL;
		m->pageFaults = 1;
		m->currentInTable = 1;
	}
	/*else go through each item in the list, try to find it
	if it does not exist in the list, page fault and add it */

	else if(m->currentInTable != m->pageTableSize) //if table is not filled up yet
	{
		//check there are max number of elements in list, if not max,
		//just add the next list as the new tail.
		list_element *p = head;
		while(p->next != NULL)  //go through the list once
		{
			p = p->next;  //increment to go through list
		}

		//once it has found tail, insert new list_element
		p->next = new list_element();
		p->next->dataValue = pageValue;
		p->next->refBit = true;
		m->currentInTable += 1;
		m->pageFaults += 1;
	}
	else  //table is full, need to go through and find element or have a page fault
	{
		bool found = false;
		list_element *p = head;
		while(p != NULL)
		{
			if(p->dataValue == pageValue)
			{
				//found dataValue
				found = true;
				p->refBit = true;
			}
			else
			{
				//turn off reference bit
				p->refBit = false;
			}

			//increment
			p=p->next;
		}

		/* after going through list once, if dataValue wasn't found, delete head
		head->next becomes new head, and add new list_element to the end. */
		if(found == false)
		{
			//delete head
			list_element *temp = head;
			head = head->next;
			head->isHead = true;
			delete temp;

			//increment and add new item to the end
			list_element *p = head;
			while(p->next != NULL)
			{
				p=p->next;
			}

			//add new element to the list
			p->next = new list_element();
			p->next->dataValue = pageValue;
			p->next->next = NULL;
			p->refBit=true;
		}
	}
	return true;
}