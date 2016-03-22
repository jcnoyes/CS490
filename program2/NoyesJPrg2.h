/******************************************************************************
* Joseph Noyes                                                                *
* Program 2 - Page Table                                                      *
* Program simulates a page table that uses a second chance algorithm. Counts  *
* the number of page faults, page fault frequency, and statistics about the   *
* data set entered.                                                           *
******************************************************************************/

#include <string>
using namespace std;

//list_element object, used to place in list
class list_element
{
public:
	bool isHead;			//tells if it is at the head of the list
	bool isTail;			//tells if it is at the end of the list
	int dataValue;			//the value of the page
	bool refBit;			//the reference bit, if 0, it can be replaced
	list_element *next;		//the next item in the list, null if it is at the tail

};

//message object, used to keep track of data being passed around
class message
{
public:
	int itemCount;			//the number of items in a list
	int pageFaults;			//number of page faults encounted
	int pageSize;			//the size of the page table
	string dataSetNum;		//string telling user how big the dataset is
};