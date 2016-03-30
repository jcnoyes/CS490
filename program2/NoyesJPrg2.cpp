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
bool readFile(list_element *&h, string fname, message *m, ofstream& outf, int timesR[]);
bool addToList(list_element *&head, message *m, int pageValue, ofstream& outf);
void createMessage(int table_size, string descrition, message *m);
void writeDataSetHeader(ofstream& outstream, message *m);
void writeSummaryToFile(ofstream& outfile, message *m);
void writeDetailedData(ofstream& outstream, pageReferenceInfo pinfo, int outCounter, list_element *le);
void writeSummaryTableHeaders(ofstream& outSum, string dataSetName, int timesR[]);
void writeSummaryTables(ofstream& outSum, message *m);

//start of execution
void main()
{
	//variables needed
	string filename;
	string outfile;

	//linked lists for data set 1 and various resident sizes
	list_element *head_ds1_3 = NULL;
	list_element *head_ds1_5 = NULL;
	list_element *head_ds1_7 = NULL;

	//linked lists for data set 2 adn various resident sizes
	list_element *head_ds2_3 = NULL;
	list_element *head_ds2_5 = NULL;
	list_element *head_ds2_7 = NULL;

	//times referenced array for each data set
	int timesReferencedS1_3[10] = {0,0,0,0,0,0,0,0,0,0};
	int timesReferencedS1_5[10] = {0,0,0,0,0,0,0,0,0,0};
	int timesReferencedS1_7[10] = {0,0,0,0,0,0,0,0,0,0};

	int timesReferencedS2_3[10] = {0,0,0,0,0,0,0,0,0,0};
	int timesReferencedS2_5[10] = {0,0,0,0,0,0,0,0,0,0};
	int timesReferencedS2_7[10] = {0,0,0,0,0,0,0,0,0,0};

	//Greet the user
	cout << "Hello, welcome to Joseph Noyes' program 2" << endl;

	//now ask the user for the first input file
	cout << "Please enter the name of the first file to read: ";
	cin >> filename;

	//now ask for an output file
	cout << "Please enter the name for the output file: ";
	cin >> outfile;
	ofstream output(outfile);

	//ready the message object for first data set, resident size 3
	message *dataSet1_Message_3 = new message();
	createMessage(3,"Data Set 1, Resident Set = 3", dataSet1_Message_3);

	//start process for data set 1 resdent size 3
	writeDataSetHeader(output, dataSet1_Message_3);
	readFile(head_ds1_3, filename, dataSet1_Message_3, output, timesReferencedS1_3);
	writeSummaryToFile(output, dataSet1_Message_3);

	//ready the message object for first data set, resident size 5
	message *dataSet1_Message_5 = new message();
	createMessage(5, "Data Set 1, Resident Set = 5", dataSet1_Message_5);

	//start process for data set 1 resident size 5
	writeDataSetHeader(output, dataSet1_Message_5);
	readFile(head_ds1_5, filename, dataSet1_Message_5, output, timesReferencedS1_5);
	writeSummaryToFile(output, dataSet1_Message_5);

	//ready the message object for first data set, resident size 7
	message *dataSet1_Message_7 = new message();
	createMessage(7, "Data Set 1, Resident Set = 7", dataSet1_Message_7);

	//start process for data set 1 resident size 7
	writeDataSetHeader(output, dataSet1_Message_7);
	readFile(head_ds1_7, filename, dataSet1_Message_7, output, timesReferencedS1_7);
	writeSummaryToFile(output, dataSet1_Message_7);

	//prompt user for the second input file
	cout << "Please enter the name of the second input file: ";
	cin >> filename;

	//ready the message for second data set, resident size 3
	message *dataSet2_Message_3 = new message();
	createMessage(3, "Data Set 2, Resident Set = 3", dataSet2_Message_3);

	//start process for data set 2 resident size 3
	writeDataSetHeader(output, dataSet2_Message_3);
	readFile(head_ds2_3, filename,dataSet2_Message_3, output, timesReferencedS2_3);
	writeSummaryToFile(output, dataSet2_Message_3);

	//ready the message for second data set, resident size 5
	message *dataSet2_Message_5 = new message();
	createMessage(5, "Data Set 2, Resident Set = 5", dataSet2_Message_5);

	//start process for data set 2, resident size 5
	writeDataSetHeader(output, dataSet2_Message_5);
	readFile(head_ds2_5, filename, dataSet2_Message_5, output, timesReferencedS2_5);
	writeSummaryToFile(output, dataSet2_Message_5);

	//ready the message for second data set, resident size 7
	message *dataSet2_Message_7 = new message();
	createMessage(7, "Data Set 2, Resident Set = 7", dataSet2_Message_7);

	//start process for data set 2, resident size 7
	writeDataSetHeader(output, dataSet2_Message_7);
	readFile(head_ds2_7, filename, dataSet2_Message_7, output, timesReferencedS2_7);
	writeSummaryToFile(output, dataSet2_Message_7);

	//close the output file
	output.close();

	//create a summary table
	cout << "Creating a summary table..." << endl;
	ofstream summaryTable("Noyes_SummaryTable.txt");

	//write to summary file for dataSet 1
	writeSummaryTableHeaders(summaryTable, "Data Set 1", timesReferencedS1_3);
	writeSummaryTables(summaryTable, dataSet1_Message_3);
	writeSummaryTables(summaryTable, dataSet1_Message_5);
	writeSummaryTables(summaryTable, dataSet1_Message_7);

	//write to summary file for dataSet 2
	writeSummaryTableHeaders(summaryTable, "Data Set 2", timesReferencedS2_3);
	writeSummaryTables(summaryTable, dataSet2_Message_3);
	writeSummaryTables(summaryTable, dataSet2_Message_5);
	writeSummaryTables(summaryTable, dataSet2_Message_7);
	summaryTable.close();
}

//function used to read in data from file
bool readFile(list_element *&h, string fname, message *m, ofstream& outf, int timesR[])
{
	cout << "Starting to process file: " << fname << " for " << m->dataSetDesc << endl << endl;
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
		//get each line in the file
		while(getline(inputfile, line)!= NULL)
		{
			//parse line by commas, assumed it is a csv file
			for(int i = 0; i != line.length(); i++)
			{
				//only get the number from the file
				if(line[i] != ',' && line[i] != ' ' && line[i] != '\n' && line[i] != '\r')
				{
					int value = line[i] - 48;  //convert char to int by subtracting 48
					addToList(h, m, value, outf);  //add to link list
					
					//update values
					timesR[value]++;
					m->itemCount++;
					double pf = m->pageFaults;
					double count = m->itemCount;
					m->pageFaultRatio = pf/count;
				}
			}
		}
		inputfile.close(); //close file

		return true; //read of file was successful
	}

}

//function used to add an element to the list
bool addToList(list_element *&head, message *m, int pageValue, ofstream& outf)
{
	pageReferenceInfo prf;
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
		
		//make detailed output if resident size is 5
		if(m->pageTableSize == 5)
		{
			prf.pageReplaced = -1;
			prf.referenced = pageValue;
			writeDetailedData(outf, prf, m->itemCount, head);
		}
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
				//found a duplicate in the first n elements
				if(m->pageTableSize == 5)
				{
					prf.pageReplaced = -1;
					prf.referenced = pageValue;
					writeDetailedData(outf, prf, m->itemCount, head);
				}
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

		//clean up pointers
		p = NULL;
		delete p;

		//if resident set size is 5, output detailed information
		if(m->pageTableSize == 5)
		{
			prf.pageReplaced = -1;
			prf.referenced = pageValue;
			writeDetailedData(outf, prf, m->itemCount, head);
		}
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
				p->refBit = true; // set reference bit to true

				//leave detailed information for resident set = 5 and itemCount is less than 25
				if(m->pageTableSize == 5 && m->itemCount < 25)
				{
					prf.pageReplaced = -1;
					prf.referenced = pageValue;
				    writeDetailedData(outf, prf, m->itemCount, head);
				}

				return true;
			}
			p=p->next;  //increment through the list
		}

		p=head;  //dataValue wasn't found, have to place it in list
		while(done == false)
		{
			if(p->refBit == 0)
			{
				//found first reference bit of 0
				//do not do anything, will replace this head
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

					//add new head to the head of the list
					list_element *newHead = new list_element();
					newHead->dataValue = head->next->dataValue;
					newHead->next = head->next->next;
					newHead->isHead = true;

					//clean up
					head = NULL;
					delete head;
					head = newHead;

					if(p->next != NULL)
					{
						p=p->next;
					}
				}while(p->refBit != false); //find first false refBit
				done = true; //once out of do while loop
			}
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
			
		//if resident set is 5, write out more detailed data for first 25
		if(m->pageTableSize == 5 && m->itemCount < 25)
		{
			pageReferenceInfo prf;
			prf.pageReplaced = temp->dataValue;
			prf.referenced = pageValue;
			writeDetailedData(outf, prf, m->itemCount, head);
		}	

		//clean up
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
	//set message values
	m->pageTableSize=table_size;
	m->pageFaults = 0;
	m->itemCount = 0;
	m->dataSetDesc = descrition;
	m->currentInTable=0;
};

//functions to write to the output file
void writeSummaryToFile(ofstream& outfile, message *m)
{
	//write number of page faults, references and page fault ratio
	outfile << "Number of Page Faults: " << m->pageFaults << endl;
	outfile << "Number of page references made: " << m->itemCount << endl;
	outfile << "Page fault ratio: " << m->pageFaultRatio << endl << endl;
};

//writes the header for each data set and resident size
void writeDataSetHeader(ofstream& outstream, message *m)
{
	outstream << "*************************************************************************" << endl;
	outstream << m->dataSetDesc << "\n";
	outstream << "*************************************************************************" << endl;
}

//function that provides more detailed output for page table size of 5
//only does this detailed output for the first 25 references
void writeDetailedData(ofstream& outstream, pageReferenceInfo pinfo, int outCounter, list_element *le)
{
	//start by making the headers
	if(outCounter == 0)
	{
		outstream << "Detailed infomration about the first 25 page references" << endl;
		outstream << "Page Referenced\t\tPage Replaced(if any)\t\tCurrent Page List" << endl;
	}

	//print out the page data
	outstream << pinfo.referenced << "\t\t\t";  //page referenced
	if(pinfo.pageReplaced != -1)		//if a page was replaced, output it
	{
		outstream << "\t" << pinfo.pageReplaced << "\t";
	}
	else  //output a tab
	{
		outstream << "\t\t";
	}
	
	outstream << "\t\t";

	//print current elements in the list
	while(le != NULL)
	{
		outstream << le->dataValue << " ";
		le = le->next;
	}
	outstream << endl;
}

//function to write the summary table's headers
void writeSummaryTableHeaders(ofstream& outSum, string dataSetName, int timesR[])
{
	outSum << "********************************************************************" << endl;
	outSum << dataSetName << endl;
	outSum << "********************************************************************" << endl;

	//print out the times each page was referenced
	for(int i = 0; i < 10; i++)
	{
		outSum << "Page " << i << " was referenced " << timesR[i] << " times." << endl;
	}
	outSum << "********************************************************************" << endl;
	outSum << endl << "Resident Set Size\tNumber of faults\tPage Fault Frequency" << endl;
}

void writeSummaryTables(ofstream& outSum, message *m)
{
	outSum << m->pageTableSize << "\t\t\t" << m->pageFaults << "\t\t\t" << m->pageFaultRatio << endl;
}