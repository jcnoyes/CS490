/******************************************************************************
* Joseph Noyes                                                                *
* CS 490 - Program 1                                                          *
* Program that will create a hash table that uses the prime-number remainder  *
* hashing function.  It will look up data in the table, and handle any        *
* collisions with an overflow area.  It will obtain the instructions from a   *
* file named indat.txt, and create two output files - logout.txt and          *
* summary.txt.                                                                *
******************************************************************************/

#define hashSize 19 //obtained from the professor in advanced
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <fstream>

using namespace std;

//define the struct that will be used to hold information in the hash table
struct tableDataEntry
{
	int key;		//holds the hash key
	float data;		//holds the data
};

/* define a struct to hold data that will be passed through functions.
This data is used for the summary.  Holds #of probes, dataValue,
number of look ups, and number of inserts */
struct data
{
	bool checkInsert;	//reports error for insert
	bool checkLookUp;	//reports error for lookup
	bool collision;		//reports if there was a collision during insertion
	char command;		//holds the command (I or L)
	int keyValue;		//the key value of the command
	int originalKey;	//original key value
	int tableLocation;	//location of where the value is inserted
	float dataValue;	//value of the data from a lookup or lookup

	//the values that are used for the summary
	int numLookups;		//total number of look ups
	int numInserts;		//total number of inserts
	int probeCount;		//number of probes per command
	int totalProbe;		//total number of probes
	float lookUpProbe;	//total probes from lookups
	float insertProbes;	//total probes from inserts
	float avgInsertProbes;	//average number of insert probes
	float avgLookUpProbes;	//average number of look up probes
	int overflowSlot;	//if in overflow, place where its slot is
	int synonym;		//number of synonyms - entries with the same hash number

};

//function prototypes
bool initHashTable(tableDataEntry tableArray[], int size);
bool ObtainAndImplementFileCommands(tableDataEntry tableArray[], int size, data *pdata);
bool parseLine(string line, data *pdata);
void generateKey(data *pdata);
bool insert(tableDataEntry tableArray[], data *pdata, int size);
bool lookup(tableDataEntry tableArray[], data *pdata, int size);
void printHashTable(tableDataEntry tableArray[], int size);
void writeLog(data *pdata, ofstream& logout);
void writeSummary(data *pdata, tableDataEntry tableArray[], int size);

//main function
int main()
{
	//variables needed
	bool valid = false;

	//get size
	int overflow = (hashSize/4);  // M/4
	int hashAndOverflow = hashSize + overflow;  // M + M/4

	//create the hashtable
	tableDataEntry *hashTable = new tableDataEntry[hashAndOverflow];

	//initializing hashtable, all key values to -1
	cout << "Intializing hashtable.  Prime number value: " << hashSize << endl;
	cout << "Total size of table with overflow: " << hashAndOverflow << endl << endl;
	valid = initHashTable(hashTable, hashAndOverflow);  //function initializes table

	//create the struct to hold the data, will be passed around through functions
	data *programData = new data;
	programData->numLookups = 0;
	programData->numInserts = 0;
	programData->probeCount = 0;
	programData->totalProbe = 0;
	programData->originalKey = 0;
	programData->tableLocation = 0;
	programData->synonym = 0;
	programData->lookUpProbe = 0;
	programData->insertProbes = 0;
	programData->avgInsertProbes = 0;
	programData->avgLookUpProbes = 0;
	programData->checkInsert = true;

	//start reading the file
	cout << "Starting file read..." << endl;
	valid = ObtainAndImplementFileCommands(hashTable, hashAndOverflow, programData);

	if(valid == false)
	{
		writeSummary(programData, hashTable, hashAndOverflow);
		if(programData->checkInsert != false)
		{
			cout << "There was an error reading input.dat ";
			cout << "please check to make sure file exists" << endl << endl;
		}
		else
		{
			cout << "The hash table was not able to hold all the data, please make" << endl;
			cout << "the M size larger." << endl;
			cout << "Press enter to continue...";
		}
		cin.ignore();
		return 0;
	}

	//write the summary file
	writeSummary(programData, hashTable, hashAndOverflow);

	//print out hashtable and summary to standard out
	printHashTable(hashTable, hashAndOverflow);
	cout << endl << "Summary" << endl << endl;
	cout << "Number of inserts = " << programData->numInserts << endl;
	cout << "Number of lookups = " << programData->numLookups << endl;
	cout << "Total insert probes = " << programData->insertProbes << endl;
	cout << "Total look up probes = " << programData->lookUpProbe << endl;
	cout << "Total probes = " << programData->totalProbe << endl;
	cout << "Average number of insert probes = " << programData->avgInsertProbes << endl;
	cout << "Average number of look up probes = " << programData->avgLookUpProbes << endl;
	cout << "Total synonyms = " << programData->synonym << endl << endl;
	cout << "Summary saved as summary.txt" << endl;
	cout << "Log file saved as logout.txt" << endl << endl;
	cout << "Press enter to exit program..." << endl;
	cin.ignore();

	return 0;
}

//function to initialize hash table
bool initHashTable(tableDataEntry tableArray[], int size)
{
	for(int i = 0; i < size ; i++)
	{
		//make all null keys equal to -1, data to 0.  This shows they are null
		tableArray[i].key = -1;
		tableArray[i].data = 0;
	}

	return true;
}

// function to obtain the lines in the file and determine which command to implement
bool ObtainAndImplementFileCommands(tableDataEntry tableArray[], int size, data *pdata)
{
	//variables needed
	string inputFile = "indat.txt";
	string line = "";
	bool validRead = false;
	bool valid = false;
	string outLog = "logout.txt";
	
	//make outfile streams
	ofstream logFile(outLog);

	//write basic header for log file
	logFile << "Joseph Noyes" << endl << "CS 490 Program 1" << endl << endl;

	//open the file for reading, read each line
	ifstream readFile(inputFile);

	//check if file exists
	if(readFile.is_open() == false) 
		{
			validRead = false;
			return validRead;
		}

	else //file is open and can be read
	{
		//read each file line by line
		while(getline(readFile,line))
		{
			validRead = parseLine(line, pdata);

			//if read wasn't successful, return false
			if(validRead == false)
			{
				return validRead;
			}

			//do the division, get the remainder
			generateKey(pdata);

			//if read was successful check which operation to perform
			if(pdata->command == 'I') //insert
			{
				valid = insert(tableArray, pdata, size);
				if(valid == false)
				{
					pdata->checkInsert = false;
					writeLog(pdata, logFile);
					logFile.close();
					return valid;
				}
			}
			else if(pdata->command == 'L') //lookup
			{
				valid = lookup(tableArray, pdata, size);
				if(valid == false)
				{
					//change checkLookUp to false for log file writing
					pdata->checkLookUp = false;
				}
			}

			//write log and reset variables
			pdata->totalProbe += pdata->probeCount;
			writeLog(pdata, logFile);
			pdata->probeCount = 0;
			pdata->collision = false;
		}

	}
	logFile.close();
	return validRead;
}

//function used to parse the command, key value, and data
bool parseLine(string line, data *pdata)
{
	//variables needed
	istringstream stringStream;
	stringStream.str(line);
	char com = ' ';
	int key;
	float data;

	//obtain the variables from string
	stringStream >> com;
	stringStream >> key;

	//if command was invalid, return false, invalid read
	if(com != 'I')
	{
		if(com != 'L')
		{
			return false;
		}
	}

	//get the data value if inserting
	if(com == 'I')
	{
		stringStream >> data;
		pdata->dataValue = data;
	}

	pdata->command=com;
	pdata->originalKey=key;

	return true; //everything is ok, return true
}

//function that generates the key
void generateKey(data *pdata)
{
	int key = pdata->originalKey;

	//get the remainder from key/hashSize
	key = key%hashSize;

	pdata->keyValue = key;
	return;
}

//function to insert data into the hashtable
bool insert(tableDataEntry tableArray[], data *pdata, int size)
{
	//obtain neccessary values
	int keyValue = pdata->keyValue;
	float dataValue = pdata->dataValue;

	//insert into array if it is empty
	if(tableArray[keyValue].key == -1)
	{
		tableArray[keyValue].data=dataValue;
		tableArray[keyValue].key=keyValue;
		pdata->numInserts = pdata->numInserts + 1;
		pdata->probeCount = pdata->probeCount + 1;
		pdata->insertProbes = pdata->insertProbes + pdata->probeCount;
		pdata->tableLocation = keyValue;
		return true;
	}
	else if(tableArray[keyValue].key == keyValue) //if slot isn't empty, put it in overflow
	{
		//check the overflow area for a spot to insert
		pdata->collision = true;
		pdata->synonym = pdata->synonym + 1;
		int overflowArea = hashSize;
		for(int i = overflowArea; i < size; i++)
		{
			pdata->probeCount = pdata->probeCount + 1;
			pdata->insertProbes = pdata->insertProbes + 1;
			if(tableArray[i].key == -1)
			{
				tableArray[i].data=dataValue;
				tableArray[i].key=keyValue;
				pdata->numInserts = pdata->numInserts + 1;
				pdata->tableLocation = i;
				return true;
			}
		}
	}		
	
	return false; //could not place data into table
}

//function used to look up the data in the hashtable
bool lookup(tableDataEntry tableArray[], data *pdata, int size)
{
	pdata->numLookups =  pdata->numLookups + 1;
	int lookUpKey = pdata->keyValue;
	pdata->checkLookUp = true;

	//look up value was not present where it was suppose to be
	if(tableArray[lookUpKey].key == -1)
	{
		pdata->checkLookUp = false;
		pdata->probeCount = pdata->probeCount+1;
		pdata->lookUpProbe = pdata->lookUpProbe + 1;

		//serach overflow sections
		int overflow = hashSize;
		for( int i = overflow; i < size; i++)
		{
			pdata->probeCount = pdata->probeCount + 1;
			pdata->lookUpProbe = pdata->lookUpProbe + 1;
			if(tableArray[i].key == lookUpKey)
			{
				//found data in overflow section
				pdata->checkLookUp = true;
				pdata->dataValue = tableArray[i].data;
				pdata->keyValue = tableArray[i].key;
				pdata->tableLocation = i;
				return true;
			}
		}

		//data was not found, give values of -1
		pdata->dataValue = -1;
		pdata->keyValue = -1;
		pdata->tableLocation = -1;
		return false;
	}
	//value was found in the table
	if(pdata->checkLookUp = true)
	{
		//get data and place them in the struct to write to log
		pdata->dataValue = tableArray[lookUpKey].data;
		pdata->keyValue = tableArray[lookUpKey].key;
		pdata->tableLocation = lookUpKey;
		pdata->probeCount = pdata->probeCount + 1; //increment probe count
		pdata->lookUpProbe = pdata->lookUpProbe + 1;
	}

	return true;
}

//function that writes to the log, uses the program data.
void writeLog(data *pdata, ofstream& logout)
{
	//make ofstream
	string starDivider = "******************************************************";
	
	//write out information to the log file
	logout << starDivider << endl;
	logout << "Command: " << pdata->command << " " << pdata->originalKey << " ";
	if(pdata->command == 'I')
	{
		logout << pdata->dataValue << endl;
		logout << "Item added: key = " << pdata->originalKey << ", data value = " << 
			pdata->dataValue << endl;

		if(pdata->checkInsert == false)
		{
			logout << "Unable to insert data, overflow section full" << endl;
			logout << "Teminating program...";
			return;
		}
	}

	else if(pdata->command == 'L')
	{
		if(pdata->checkLookUp == true)
		{
			logout << endl << "Lookup key: " << pdata->keyValue << ", data value = " <<
				pdata->dataValue << endl;
		}
		else
		{
			logout << "Error looking up " << pdata->originalKey << " data not found." << endl;
		}
	}

	logout << "Hash value: " << pdata->keyValue << endl;
	logout << "Number of probes: " << pdata->probeCount << endl;
	logout << "Table index: " << pdata->tableLocation << endl;

	if(pdata->collision == true)
	{
		logout << "Collision encountered while inserting this data." << endl;
		logout << "Data was moved to overflow section." << endl;
	}

}

//function to print the hash table out
void printHashTable(tableDataEntry tableArray[], int size)
{
	for(int i = 0; i < size; i++)
	{
		cout << tableArray[i].key << "\t" << tableArray[i].data << endl;
	}

}

//function to write the summary to summary.txt
void writeSummary(data *pdata, tableDataEntry tableArray[], int size)
{
	//variables needed
	string summaryFile = "summary.txt";
	string starDiv = "**********************************************************";
	ofstream summaryStream(summaryFile);

	//print hashtable lables
	summaryStream << "Joseph Noyes" << endl << "CS 490 Program 1" << endl;
	summaryStream << starDiv << endl << endl;
	summaryStream << "Table Index \tData Entry" << endl; 
	//print the hashtable to file
	for(int i = 0; i < size; i++)
	{
		summaryStream << tableArray[i].key << "\t\t" << tableArray[i].data << endl;
	}

	//print the statistics from pdata to summary.txt
	summaryStream << endl << starDiv << endl;
	summaryStream << "Summary:" << endl;
	summaryStream << starDiv << endl;
	summaryStream << "Number of table inserts: " << pdata->numInserts << endl;
	summaryStream << "Number of table look ups: " << pdata->numLookups << endl;
	summaryStream << "Number of synonyms: " << pdata->synonym << endl;
	summaryStream << "Number of probes: " << pdata->totalProbe << endl;
	summaryStream << "Number of insert probes: " << pdata->insertProbes << endl;
	summaryStream << "Number of look up probes: " << pdata->lookUpProbe << endl;
	summaryStream << starDiv << endl;
	
	//calculate the average
	//protection from dividing by zero
	if(pdata->numInserts == 0)
	{
		pdata->numInserts = 1;
	}

	if(pdata->numLookups == 0)
	{
		pdata->numLookups = 1;
	}

	//actual average calculation
	pdata->avgInsertProbes = pdata->insertProbes/pdata->numInserts;
	pdata->avgLookUpProbes = pdata->lookUpProbe/pdata->numLookups;

	//print out averages
	summaryStream << "Average number of insert probes: " << pdata->avgInsertProbes << endl;
	summaryStream << "Average number of look up probes: " << pdata->avgLookUpProbes << endl;

	if(pdata->checkInsert == false)
	{
		summaryStream << "Error Overflow section of the table is maxed out" << endl; 
		summaryStream << "unable to insert any more data."  << endl;
	}
	summaryStream << starDiv << endl;

	//close the summaryStream
	summaryStream.close();
	return;
}