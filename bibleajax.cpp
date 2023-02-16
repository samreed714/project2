/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 * 
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching 
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <fstream>
#include <string> 
#include <stdlib.h>
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

int main() {
  /* A CGI program must send a response header with content type
   * back to the web client before any other output.
   * For an AJAX request, our response is not a complete HTML document,
   * so the response type is just plain text to insert into the web page.
   */
  cout << "Content-Type: text/plain\n\n";
  
  Cgicc cgi;  // create object used to access CGI request data

  // GET THE INPUT DATA
  // browser sends us a string of field name/value pairs from HTML form
  // retrieve the value for each appropriate field name
  form_iterator st = cgi.getElement("search_type");
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");
  form_iterator version = cgi.getElement("version");

  // Convert and check input data
  bool validInput = false;
  string errorMessage = "Invalid Chapter";
  string errorDesc = " ";
  if (chapter != cgi.getElements().end()) {
	 int chapterNum = chapter->getIntegerValue();
	 if (chapterNum > 150) {
		 //cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
         errorDesc = string("The chapter number ") + to_string(chapterNum) +string(" is too high.");
	 }
	 else if (chapterNum <= 0) {
		 //cout << "<p>The chapter must be a positive number.</p>" << endl;
         errorDesc = "The chapter must be a positive number";
	 }
	 else
		 validInput = true;
  }
  
  /* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */
  //Book should be greater than 0 and less than 66
  int b = book->getIntegerValue();
  if (b > 66) {
      validInput = false;
      errorMessage = "Invalid Book";
      //cout << "<p>There are only 66 books of the Bible.</p>";
      errorDesc = "There are only 666 books of the Bible.";
  }

  //already checked
  int c = chapter->getIntegerValue();

  //Verse should be greater than 0 and less than 176
  int v = verse->getIntegerValue();
  if (v > 176) {
      validInput = false;
      errorMessage = "Invalid Verse";
      //cout << "<p>No chapters of the Bible have more than 176 verses.</p>";
      errorDesc = "No chapters of the Bible have more than 176 verses";
  }
  if (v < 1) {
      validInput = false;
      errorMessage = "Invalid Verse";
      //cout << "<p>Verse must be greater than 0</p>";
      errorDesc = "Verse must be greater than 0";
  }

  //Number of verses should not be less than 1. If it is, only print 1 verse
  int v2 = nv->getIntegerValue();
  if (v2 < 1) {
      cout << "<p style=\"color:Red;\">Number of verses should be 1 or more. Printing 1 verse: </p>" << endl;
  }

  //Version number should 1-5
  int versionInt = version->getIntegerValue();
  if (versionInt > 5) {
      cout << "<p style=\"color:Red;\">There are only 5 versions. Using WEB version</p>";
  }
  if (versionInt < 1) {
      cout << "<p style=\"color:Red;\">Version cannot be less than 0. Using WEB version</p>";
  }

  /* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
   *        TO LOOK UP THE REQUESTED VERSES
   */

  // Create Bible object to process the raw text file
  Bible webBible;

  //set webBible to correct version
  if (versionInt == 1) {
      webBible = Bible("/home/class/csc3004/Bibles/web-complete");
  }
  else if (versionInt == 2) {
      webBible = Bible("/home/class/csc3004/Bibles/dby-complete");
  }
  else if (versionInt == 3) {
      webBible = Bible("/home/class/csc3004/Bibles/kjv-complete");
  }
  else if (versionInt == 4) {
      webBible = Bible("/home/class/csc3004/Bibles/webster-complete");
  }
  else if (versionInt == 5) {
      webBible = Bible("/home/class/csc3004/Bibles/ylt-complete");
  }
  //Default to WEB if version is invalid
  else {
      webBible = Bible("/home/class/csc3004/Bibles/web-complete");
  }

  LookupResult result;

  string bookNames[66] = {
        "Genesis",
        "Exodus",
        "Leviticus",
        "Numbers",
        "Deuteronomy",
        "Joshua",
        "Judges",
        "Ruth",
        "1 Samuel",
        "2 Samuel",
        "1 Kings",
        "2 Kings",
        "1 Chronicles",
        "2 Chronicles",
        "Ezra",
        "Nehemiah",
        "Esther",
        "Job",
        "Psalms",
        "Proverbs",
        "Ecclesiastes",
        "Song of Solomon",
        "Isaiah",
        "Jeremiah",
        "Lamentations",
        "Ezekiel",
        "Daniel",
        "Hosea",
        "Joel",
        "Amos",
        "Obediah",
        "Jonah",
        "Micah",
        "Nahum",
        "Habakkuk",
        "Zephaniah",
        "Haggai",
        "Zechariah",
        "Malachi",
        "Matthew",
        "Mark",
        "Luke",
        "John",
        "Acts",
        "Romans",
        "1 Corinthians",
        "2 Corinthians",
        "Galatians",
        "Ephesians",
        "Phillipians",
        "Colossians",
        "1 Thessalonians",
        "2 Thessalonians",
        "1 Timothy",
        "2 Timothy",
        "Titus",
        "Philemon",
        "Hebrews",
        "James",
        "1 Peter",
        "2 Peter",
        "1 John",
        "2 John",
        "3 John",
        "Jude",
        "Revelation"
  };
  string versionNames[5] = {
      "WEB",
      "DBY",
      "KJV",
      "WBSTR",
      "YLT"
  };

  // Create a reference from the numbers
  Ref ref(b, c, v);
  Ref currentRef = ref.next();
  Verse lookedUpVerse = webBible.lookup(ref, result);
  Verse currentVerse;

  /* SEND BACK THE RESULTS
   * Finally we send the result back to the client on the standard output stream
   * in HTML text format.
   * This string will be inserted as is inside a container on the web page, 
   * so we must include HTML formatting commands to make things look presentable!
   */

  //check for errors from lookup
  if (result != SUCCESS) {
      if (result == NO_BOOK) {
          errorMessage = "Invalid Book";
      }
      if (result == NO_CHAPTER) {
          errorMessage = "Invalid Chapter";
      }
      if (result == NO_VERSE) {
          errorMessage = "Invalid Verse";
      }
      validInput = false;
  }
  if (validInput) {
	cout << "Search Type: <b>" << **st << "</b>" << endl;
	cout << "<p>Your result: "
		 << bookNames[b - 1] << " " << **chapter << ":" << **verse << " (" << versionNames[versionInt - 1] << "): " 
		<< "<em>" << lookedUpVerse.getVerse() << "</em>";
    //Print the rest of the verses. i = 1 becase one verse has already been displayed
    for (int i = 1; i < v2; i++) {                                           
        currentVerse = webBible.nextVerse(currentRef, result);
        cout << " <sup><b>" << currentRef.getVerse() << "</sup></b>";
        cout << "<em> " << currentVerse.getVerse() << "</em>";
        currentRef = currentRef.next();
    }
    cout << "</p>" << endl;

  }
  else {
	  cout << "<p style=\"color:Red;\">Error: <em>" << errorMessage << "</em></p>" << endl;
      cout << "<p style=\"color:Red;\">Description: <em>" << errorDesc << "</em></p>" << endl;
  }
  return 0;
}
