//************************************  PROGRAM IDENTIFICATION  ***************************************
//*                                                                                                   *
//*   PROGRAM FILE NAME:  Project7.cpp          ASSIGNMENT #:  7            Grade: _________          *
//*                                                                                                   *
//*   PROGRAM AUTHOR:     __________________________________________                                  *
//*                                     Micaiah Skolnick                                              *
//*                                                                                                   *
//*   COURSE #:  CSC 36000 11                            DUE DATE:  May 05, 2017                      *
//*                                                                                                   *
//*****************************************************************************************************
//***********************************  PROGRAM DESCRIPTION  *******************************************
//*                                                                                                   *
//*   PROCESS: The program processes each line of the input file based on the operation code. If the  *
//*            opcode is an 'I', an inventory item is entered into the tree. For opcode = 'D', the    *
//*            item is deleted from the tree. opcode = 'P' the item is printed, or the whole tree is  *
//*            printed. opcode 'S', 'O', and 'R' have to do with adjusting quantity on hand, and      *
//*            quantity ordered for the specific item referenced. As each operation is completed      *
//*            a binary tree, with processes in a separate header file, is manipulated.               *
//*            Variables used by every function are stored in a file named "Variables.H"              *
//*                                                                                                   *
//*   USER DEFINED                                                                                    *
//*    MODULES     : Footer - Prints a footer to signify end of program output                        *
//*                  Header - Prints a header to signify start of program output                      *
//*                  newPage - Inserts blank lines until the start of a new page                      *
//*                  processData - Process each line of data from input file                          *
//*                  printHeaderForTree - Prints a header for each entire tree output                 *
//*                  printMessage - Prints message specific to the operation which was just completed *
//*                  printList - Prints the entire array of elements with correct spacing             *
//*                  main - Calls each function in order in order to accomplish the desired task      *
//*                                                                                                   *
//*****************************************************************************************************
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <sstream>
#include "TreeClass.h"
#include "Variables.h"
using namespace std;
//*****************************************************************************************************
void newPage(ofstream&dataOUT) {
		// Receives – the output file
		// Task - Insert blank lines to fill the rest of the current page
		// Returns - Nothing
	while (lineCount < LINESPERPAGE) {
		dataOUT << endl;
		lineCount++;
	}
	lineCount = 0; // Reset the line count to 0 for next page.
}
//*************************************  FUNCTION HEADER  *********************************************
void Header(ofstream &Outfile)
{       // Receives – the output file
		// Task - Prints the output preamble
		// Returns - Nothing
	Outfile << setw(30) << "Micaiah Skolnick ";
	Outfile << setw(17) << "CSC 36000";
	Outfile << setw(15) << "Section 11" << endl;
	Outfile << setw(27) << "Spring 2017";
	Outfile << setw(30) << "Assignment #7" << endl;
	Outfile << setw(35) << "---------------------------------- - ";
	Outfile << setw(35) << "---------------------------------- - " << endl << endl;
	lineCount += 4;
	return;
}
//************************************* END OF FUNCTION HEADER  ***************************************
//*************************************  FUNCTION FOOTER  *********************************************
void Footer(ofstream &Outfile){
		// Receives – the output file
		// Task - Prints the output salutation
		// Returns - Nothing
	Outfile << endl;
	Outfile << setw(35) << "-------------------------------- - " << endl;
	Outfile << setw(35) << " | END OF PROGRAM OUTPUT | " << endl;
	Outfile << setw(35) << "-------------------------------- - " << endl;
	lineCount += 4;
	return;
}
//************************************* END OF FUNCTION FOOTER  ***************************************
void printMessage(char opType, string IDtoPrint, bool opSuccess) {
		//Receives – The type of operation, ID to print, and whether operation was successful
		//Task - Print a message to the user indicating whether operation succeeded
		//Returns - Nothing
	if (opSuccess) { //Print the corresponding message if the operation was successful
		if (opType == 'I') {
			dataOUT << "Item ID Number " << IDtoPrint << " successfully entered into database.";
		}
		else if (opType == 'D') {
			dataOUT << "Item ID Number " << IDtoPrint << " successfully deleted from database.";
		}
		else if (opType == 'S' || opType == 'R'){
			dataOUT << "Quantity on Hand for item " << IDtoPrint << " successfully updated.";
		}
		else if (opType == 'O') {
			dataOUT << "Quantity on order for item " << IDtoPrint << " successfully updated.";
		}
		else {	return; }		
	}
	else { //Print the corresponding message if the operation failed
		if (opType == 'I') {
			dataOUT << "ERROR - Attempt to insert a duplicate item " 
				<< IDtoPrint << " into the database.";
		}
		else if (opType == 'D') {
			dataOUT << "ERROR - Attempt to delete an item (" 
				<< IDtoPrint << ") not in the database list.";
		}
		else if (opType == 'P') {
			dataOUT << "Item " << IDtoPrint << " not in database. Print failed.";
		}
		else {
			dataOUT << "Item " << IDtoPrint << " not in database. Data not updated.";
		}
	}
	//Add a line of dashes after printing output message
	dataOUT << endl << "--------------------------------------------------------------------" << endl; 	
	lineCount+=2; //Increment the line counter
}
//*****************************************************************************************************
void printHeaderForTree() {
		//Receives – Nothing
		//Task - Prints a header for when any node from the tree is printed
		//Returns - Nothing
	dataOUT << "	               JAKE’S HARDWARE INVENTORY REPORT" << endl;
	dataOUT << "	Item             Item                    Quantity       Quantity" << endl;
	dataOUT << "	ID Number        Description             On hand        On Order" << endl;
	dataOUT << "--------------------------------------------------------------------" << endl;
	lineCount += 4; // Increment line counter
}
//*****************************************************************************************************
void processData() {
		//Receives – Nothing
		//Task - Process data from the input file
		//Returns - Nothing
		//Declare local variables needed for processing
	char code1,code2,newName[22];
	bool opSuccess = false;
	int newQuantity;
	string IDtoSearch;
	dataIN >> ws >> code1; //Seed read first command code
	while (code1 != 'Q') { //Keep reading until the sentinel of 'Q' is reached
		NodeType newNode;
		if (code1 == 'I') { //If the code is 'I' Insert a node				
			dataIN >> IDtoSearch >> ws; //Get the ID of the node to insert
			newNode.ID = IDtoSearch; //Set the ID to the ID of the new Node
			dataIN.getline(newName, 21); //Read in the new name to a character array
			newNode.Name = newName; //Add char array to node as the name
			dataIN >> ws>> newNode.QOnHand >> newNode.QOnOrder; // Get the QOnHand and QOrdered
				//Insert the node into the tree if it is not already there
			opSuccess = iTree.insertNode(newNode);			
		}
		else if (code1 == 'D') { //If opcode is 'D', delete a node
			dataIN >> IDtoSearch >> ws; //Read in the ID of the node to delete
			dataIN.getline(newName, 21); // Read in the name to a character array			
			opSuccess = iTree.deleteNode(IDtoSearch);//Delete the node matching this ID		
		}
		else if (code1 == 'P') { //If opcode is 'P' print either one node, or whole tree
			dataIN >> code2; //Get opcode indicating whether one node or whole tree should be printed
			if (code2 == 'E') { //If second opcode is 'E' print the whole tree
				newPage(dataOUT);//Space out to new page for entire printing of tree
				printHeaderForTree(); //Print a header for the tree printout
				opSuccess = iTree.printEntireTree(); //Print entire tree
				newPage(dataOUT);//Space out rest of page for more output
			}
			if (code2 == 'N') { //If second opcode is 'N' only print one node
				dataIN >> IDtoSearch; //Get ID of node to print
				opSuccess = iTree.operateOnNode(IDtoSearch, code1, 0);//Print an individual node
			}			
		}
		else { //For all other opcodes, follow the same procedure
			dataIN >> IDtoSearch >> newQuantity; //Read in the ID of node to operate on, and value
					//Update the specified node with whatever new value was specified
				opSuccess = iTree.operateOnNode(IDtoSearch, code1, newQuantity);
		}
		printMessage(code1, IDtoSearch, opSuccess); //Print a message to alert user of op outcome
		dataIN >> ws >> code1;  //Read in the next code
	}
}
//*****************************************************************************************************
int main() {
		// Receives – Nothing
		// Task - Call each necessary function of the program in order
		// Returns - Nothing
		// Declare variables used in process	 
	dataIN.open("tree_in.txt"); // Open the file containing data
	dataOUT.open("dataOUT.doc"); // Create and open the file to write data to
	lineCount = 0; //Initialize lineCount to 0
	LINESPERPAGE = 52; //Intialize lines per page to 52
	Header(dataOUT); // Print data header.
	processData(); // Process the data from the input file
	Footer(dataOUT); // Print footer. 
	dataIN.close(); // Close input data file. 
	dataOUT.close(); // Close output data file.
	return 0;
}