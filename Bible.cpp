// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	ifstream stream;
	bool isOpen = false;
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; }

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 

    // TODO: scan the file to retrieve the line that holds ref ...
	string line;
	stream.open(infile, ios::in);
	isOpen = stream.is_open();
    // update the status variable
	status = OTHER; // placeholder until retrieval is attempted

	//Iterate through Bible until verse is found
	if (isOpen && status == OTHER) {
		bool chapFound = false;
		while (getline(stream, line)) {
			Ref currentRef = Ref(line);
			//Update chapFound if chapter is found
			if (currentRef.getBook() == ref.getBook() && currentRef.getChap() == ref.getChap()) {
				chapFound = true;
			}
			//If the chapter has been found, but currentRef moved on to the next chapter, there is no verse
			if (chapFound && (currentRef.getChap() > ref.getChap())) {
				status = NO_VERSE;
				//break;
			}
			//If the verse is found, break the loop
			if (currentRef == ref) {
				status = SUCCESS;
				break;
			}
			//If currentRef moves on to the next book, there is no chapter
			if (currentRef.getBook() > ref.getBook()) {
				status = NO_CHAPTER;
				break;
			}
		}
	}
	// create and return the verse object
	Verse aVerse;   // default verse, to be replaced by a Verse object
	                // that is constructed from a line in the file.
	aVerse = Verse(line);
    return(aVerse);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(Ref ref, LookupResult& status) {
	string line;
	Ref currentRef;
	isOpen = stream.is_open();
	if (isOpen) {
		//if the end of the file has not been reached, return the next verse
		getline(stream, line);
		currentRef = Ref(line);
		//print name of new book
		if (currentRef.getBook() > ref.getBook()) {
			cout << endl;
			currentRef.display();
			cout << endl;
		}
		//print name of new chapter
		if (currentRef.getVerse() == 1 && currentRef.getBook() == ref.getBook()) {
			cout << "\nChapter " << currentRef.getChap() << endl;
		}
	}
	else {
		//If the end of the file has been reached, start at the beginning
		currentRef = Ref(1, 1, 1);
		return lookup(currentRef, status);
	}
	Verse aVerse = Verse(line);
	return (aVerse);
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
	if (status == NO_BOOK) {
		return("Error: invalid book");
	}
	if (status == NO_CHAPTER) {
		return ("Error: invalid chapter");
	}
	if (status == NO_VERSE) {
		return ("Error: invalid verse");
	}
	return ("none");
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
//Ref Bible::next(const Ref ref, LookupResult& status) {}

// OPTIONAL: Return the reference before the given ref
//Ref Bible::prev(const Ref ref, LookupResult& status) {}
