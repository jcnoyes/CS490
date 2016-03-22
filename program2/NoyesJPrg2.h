/******************************************************************************
* Joseph Noyes                                                                *
* Program 2 - Page Table                                                      *
* Program simulates a page table that uses a second chance algorithm. Counts  *
* the number of page faults, page fault frequency, and statistics about the   *
* data set entered.                                                           *
******************************************************************************/

class list_element
{
public:
private:
	bool isHead;			//tells if it is at the head of the list
	bool isTail;			//tells if it is at the end of the list
	int dataValue;			//the value of the page
	bool refBit;			//the reference bit, if 0, it can be replaced
	list_element *next;		//the next item in the list, null if it is at the tail

};