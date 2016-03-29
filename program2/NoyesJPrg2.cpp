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
void createMessage(int table_size, string descrition, message *m);

//start of execution
void main()
{
	//variables needed
	string filename;
	string outfile;
	list_element *head_ds1_3 = NULL;
	list_element *head_ds1_5 = NULL;
	list_element *head_ds1_7 = NULL;

	//Greet the user and ask for a file name
	cout << "Hello, welcome to Joseph Noyes' program 2" << endl;
	cout << "Please enter the name of the first file to read: ";
	cin >> filename;
	cout << "Please enter the name for the output file: ";
	cin >> outfile;

	//ready the message object for first data set, page size 3
	message *dataSet1_Message_3 = new message();
	createMessage(3,"Data Set 1, Resident Set = 3", dataSet1_Message_3);
	readFile(head_ds1_3, filename, dataSet1_Message_3);

	//ready the message object for first data set, page size 5
	message *dataSet1_Message_5 = new message();
	createMessage(5, "Data Set 1, Resident Set = 5", dataSet1_Message_5);
	readFile(head_ds1_5, filename, dataSet1_Message_5);

	//ready the message object for first data set, page size 7
	message *dataSet1_Message_7 = new message();
	createMessage(7, "Data Set 1, Resident Set = 5", dataSet1_Message_7);
	readFile(head_ds1_7, filename, dataSet1_Message_7);
	cout << "done";
}

//function used to read in data from file
bool readFile(list_element *&h, string fname, message *m)
{
	cout << "Starting to process file: " << fname << " for " << m->dataSetDesc << endl;
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
				if(line[i] != ',' && line[i] != ' ' && line[i] != '\n' && line[i] != '\r')
				{
					int value = line[i] - 48;  //convert char to int by subtracting 48
					addToList(h, m, value);
					m->itemCount++;
				}
			}
		}
		inputfile.close(); //close file

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
			if(p->dataValue == pageValue)
			{
				return true;
			}
			p = p->next;  //increment to go through list
		}

		//once it has found tail, insert new list_element
		p->next = new list_element();
		p->next->dataValue = pageValue;
		p->next->refBit = true;
		m->currentInTable += 1;
		m->pageFaults += 1;
		p->next->next=NULL;

		//clean up
		p = NULL;
		delete p;
	}
	else  //table is full, need to go through and find element or have a page fault
	{
		bool found = false;
		bool done = false;
		list_element *p = head;

		while(p != NULL)  //go through list
		{
			if(p->dataValue == pageValue)
			{
				//found dataValue
				found = true;
				p->refBit = true;
				return true;
			}
			p=p->next;
		}
		p=head;
		while(done == false)
		{
			if(p->refBit == 0)
			{
				//found first reference bit of 0
				//do not do anything
				done = true;
			}
			else
			{
				do
				{
					//turn off reference bit, place page at the end of the table
					p->refBit = false;
					list_element *nextp = p;

					//place p at the end of the list
					while(nextp->next != NULL)  //increment through list
					{
						nextp=nextp->next;
					}
				
					//place head at the end
					list_element *newTail = new list_element();
					newTail->dataValue = head->dataValue;
					newTail->next = NULL;
					newTail->isHead = false;
					newTail->refBit = false;

					nextp->next = newTail;

					list_element *newHead = new list_element();
					newHead->dataValue = head->next->dataValue;
					newHead->next = head->next->next;
					newHead->isHead = true;

					head = NULL;
					delete head;
					head = newHead;
					if(p->next != NULL)
					{
						p=p->next;
					}
				}
					while(p->refBit != false);
					done = true;
			}

		p=p->next;
	}

		if(found == false)
		{
			//page fault occurred
			m->pageFaults++;
			p = head;
			
			while(p->next != NULL)
			{
				p=p->next;
			}

			//add new element to the list
			p->next = new list_element();
			p->next->dataValue = pageValue;
			p->next->next = NULL;
			p->next->refBit=true;
			
			//delete head
			list_element *temp = head;
			head = head->next;
			head->isHead = true;
			delete temp;
		}
		else
		{
			return true;
		}
	}
	return true;
}

//function to create a message used for reading file, saves table size, page faults, etc.
void createMessage(int table_size, string descrition, message *m)
{
	m->pageTableSize=table_size;
	m->pageFaults = 0;
	m->itemCount = 0;
	m->dataSetDesc = descrition;
	m->currentInTable=0;
};