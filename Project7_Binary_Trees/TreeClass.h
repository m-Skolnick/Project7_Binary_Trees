//************************************  PROGRAM IDENTIFICATION  ***************************************
//*                                                                                                   *
//*   PROGRAM FILE NAME:  TreeClass.cpp                  ASSIGNMENT #: 6                              *
//*                                                                                                   *
//*                                                                                                   *
//*   USER DEFINED                                                                                    *
//*    MODULES    : insertNode - Inserts a node into the tree                                         *
//*                 patchParent - Patches the parent of the current node                              *
//*                 deleteNode - Deletes the node from the tree which matches the ID passed in        *
//*                 operateOnNode - Based on opcode, update or print the specified node               *
//*                 printEntireTree - Print each node of the tree                                     *
//*                 printEachNode - Print the data held in each node of the tree                      *
//*                 searchForNode - Return the node which ID matched the ID passed in                 *
//*                                                                                                   *
//*****************************************************************************************************
#ifndef TREECLASS_H
#define TREECLASS_H
#include "Variables.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
//*****************************************************************************************************
struct NodeType {
		// The structure "NodeType" holds each inventory file.
	int QOnHand, QOnOrder;
	string ID,Name;
	NodeType *Lptr,*Rptr,*parent;
};
//*****************************************************************************************************
class BinaryTreeClass {
public:
	BinaryTreeClass();
	bool insertNode(NodeType);
	bool printEntireTree();
	void printEachNode(NodeType*);
	void patchParent(NodeType*, NodeType*, NodeType*);
	bool deleteNode(string);
	bool operateOnNode(string, char, int);
	NodeType* searchForNode(string);
private:
	NodeType *RootPtr;
};
BinaryTreeClass iTree; //Initialize a tree to hold all inventory values
//*****************************************************************************************************
BinaryTreeClass::BinaryTreeClass() {
		// Initialize the Root Ptr to NULL
	RootPtr = NULL;
}
//*****************************************************************************************************
bool BinaryTreeClass::insertNode(NodeType newNode){
		// Receives – A new node to insert into tree
		// Task - Insert a node into the tree
		// Returns - A bool to indicate operation success or failure
		// Declare variables used in process
	bool inserted = false;
	NodeType  *newPtr, *currPtr;
	newPtr = new(NodeType);   
		//First search the tree. If the node is found matching this ID, return false
	if (searchForNode(newNode.ID)->ID != "NOT FOUND") {
		return false;
	}
		//If the node was not already found in the tree, insert it
	else if (newPtr != NULL){
			//Move values from node being passed in to new pointer
		newPtr->ID = newNode.ID;
		newPtr->Name = newNode.Name;
		newPtr->QOnHand = newNode.QOnHand;
		newPtr->QOnOrder = newNode.QOnOrder;  
			//Initialize left and right pointer of new node to null
		newPtr -> Lptr = NULL;    
		newPtr -> Rptr = NULL;      
		currPtr = RootPtr; //Set the current pointer to the root
		while (inserted == false){
			if (currPtr == NULL){ //If there is not yet a root value, set current Ptr to root
				RootPtr = newPtr;
				return true; //Return true to indicate that the node was successfully added
			}                                           
			else{ //If tree already contained a node, add new node at correct location
				if (newPtr->ID < currPtr ->ID) 
					//If new node ID is less than current, follow the left pointer
					if (currPtr->Lptr != NULL) {
						currPtr = currPtr->Lptr;
					}
					else{
						currPtr->Lptr = newPtr;
						return true;
					}
				else {
						//If new node ID is greater than current, follow the right pointer
					if (currPtr->Rptr != NULL) {
						currPtr = currPtr->Rptr;
					}
					else{
						currPtr->Rptr = newPtr;
						return true;
					}
				}
			}
		}  
	}   
	return false;
}  
//*****************************************************************************************************
inline void BinaryTreeClass::patchParent(NodeType *Newparnode, NodeType *parnode, NodeType *delnode){
		// Receives – The new parent node, current parent node, and the node to delete
		// Task - Patch the parent when deleting a node
		// Returns - Nothing
		// If there is no parent node, set the Root to be the new parent
	if (parnode == NULL) {
		RootPtr = Newparnode;
	}
	else{ //If the parent is not null, replace either the left or right pointer to patch in new parent
		if (parnode->Lptr == delnode) {
			parnode->Lptr = Newparnode;
		}
		else {
			parnode->Rptr = Newparnode;
		}
	}
}
//*****************************************************************************************************
inline bool BinaryTreeClass::deleteNode(string IDtoSearch){
		// Receives – The ID matching a node to delete
		// Task - Delete a node from the tree
		// Returns - A bool to indicate if node was deleted from tree
		// Declare local pointers 
	NodeType *delnode, *parnode, *node1, *node2, *node3;
	bool found = false; // Declare flag to indicate if the node to be deleted was found  
	delnode = RootPtr;// Set the pointers to start at the root
	parnode = NULL;
	// Search the tree until node to be deleted is found, or end of tree is reached
	while (found == false && delnode != NULL){	// Set flag to true if node is found
		if (IDtoSearch == delnode->ID) {
			found = true;
		}
		else { // Otherwise keep track of the parent node and move down correct branch of the tree 
			parnode = delnode;
			if (IDtoSearch < delnode->ID) {
				delnode = delnode->Lptr;
			}
			else{
				delnode = delnode->Rptr;
			}
		}
	}
	//  CASE 1 – Node is not in tree
	if (found == false) {
		return false;
	}
	else{
		if (delnode->Lptr == NULL)
			if (delnode->Rptr == NULL) // CASE 2 – Node has NO children
				patchParent(NULL, parnode, delnode);
			else  	 	     // CASE 3 – Node has ONE right child 
				patchParent(delnode->Rptr, parnode, delnode);
		else{
			if (delnode->Rptr == NULL)  // CASE 4 – Node has ONE left child    
				patchParent(delnode->Lptr, parnode, delnode);
			else { 		     // CASE 5 – Node has TWO children
				node1 = delnode;
				node2 = delnode->Lptr;
				node3 = node2->Rptr;
				while (node3 != NULL) {
					node1 = node2;
					node2 = node3;
					node3 = node3->Rptr;
				}
				if (node1 != delnode) {
					node1->Rptr = node2->Lptr;
					node2->Lptr = delnode->Lptr;
				}
				node2->Rptr = delnode->Rptr;
					//Patch the parent with the correct nodes
				patchParent(node2, parnode, delnode);
			}
		}
		return true;
	}
}
//*****************************************************************************************************
inline bool BinaryTreeClass::operateOnNode(string IDtoSearch, char operation, int value) {
		//Receives – The ID of the node to operate on, the type of operation, and the op value
		//Task - Operate on an individual node
		//Returns - A bool indicating whether operation was successful
	NodeType *node = new(NodeType);
	node = searchForNode(IDtoSearch);
	if (node->ID == "NOT FOUND") { //If the node was not found in the search, return false;
		return false;
	}
	else { //If the node was found, perform the specified operation
		if (operation == 'P') { //If the operation is a 'P' Print the node
				//Print a header for the item
			dataOUT << "	Item             Item                    Quantity       Quantity" << endl;
			dataOUT << "	ID Number        Description             On hand        On Order" << endl;
			dataOUT << "--------------------------------------------------------------------" << endl;
				//Print the item
			dataOUT << "    " << left << setw(17) << node->ID  << setw(22) << node->Name
			 << right << setw(7) << node->QOnHand << setw(15) << node->QOnOrder << endl;
			dataOUT << "--------------------------------------------------------------------" << endl;
			lineCount += 5; //Increment the line counter
		}//If the operation is an 'S', adjust the inventory to reflect a sale
		else if (operation == 'S') { 
			node->QOnHand -= value;
		}//If the operation is an 'R', adjust the inventory to reflect a shipment received
		else if (operation == 'R') { 
			node->QOnOrder -= value;
			node->QOnHand += value;
		}//If the operation is an 'O', adjust the inventory to reflect a restock transaction
		else if (operation == 'O') {
			node->QOnOrder += value;
		}
		return true; //Return true to indicate that the operation was successful
	}
}
//*****************************************************************************************************
inline bool BinaryTreeClass::printEntireTree(){
		//Receives – Nothing
		//Task - Print each item in the tree
		//Returns - A bool indicating whether operation was successful
		//If there are no nodes in the tree, alert the user
	if (RootPtr == NULL) {
		dataOUT << "There are no inventory items to print" << endl;
		lineCount++;
		return false;
	}
	else { //If tree is not empty, print each of the items in the tree
		printEachNode(RootPtr);
		return true;
	}
}
//*****************************************************************************************************
inline void BinaryTreeClass::printEachNode(NodeType *root) {
		//Receives – The pointer to a node in the tree
		//Task - Print each node in the tree
		//Returns - Nothing
	if (root != NULL) {
		printEachNode(root->Lptr); //Recursively call left pointers
			//Print the contents of the current node
		dataOUT << "    " << left << setw(17) << root->ID  << setw(22) << root->Name 
			 << right << setw(7) << root->QOnHand << setw(15) << root->QOnOrder << endl;
		lineCount++; //Increment the line counter
		printEachNode(root->Rptr); //Recursively call right pointers
	}
}
//*****************************************************************************************************
inline NodeType* BinaryTreeClass::searchForNode(string IDtoSearch) {
		//Receives – The ID of a node to search for
		//Task - Find the specified node in the tree
		//Returns - The node with the ID matching the node searched for
	NodeType *currPtr = new(NodeType);
	currPtr = RootPtr; //Start the search at the root of the tree
	bool found = false;
	while (!found && currPtr != NULL) {
			//If ID is less than current node, follow left pointer
		if (IDtoSearch < currPtr->ID) { 
			currPtr = currPtr->Lptr;
		}	//If ID is greater than current node, follow right pointer
		else if (IDtoSearch > currPtr->ID) { 
			currPtr = currPtr->Rptr;
		} //If ID is equal to current node, set found to true
		else if (IDtoSearch == currPtr->ID) {
			found = true;
		}		
	}
	if (currPtr == NULL) { //If the node was not found, set ID of node to be returned to "NOT FOUND"
		currPtr = new(NodeType);
		currPtr->ID = "NOT FOUND";
	}
		//Return the node which was found
	return currPtr;
}
//*****************************************************************************************************
#endif
