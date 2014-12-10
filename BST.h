//DO NOT CHANGE THIS FILE
//Author: Bo Brinkman
//Date: 2013/10/01

#include "SSet.h"

template <class Key, class T>
class Node {
public:
  Key k;
  T data;
  Node* left;
  Node* right;
};

template <class Key, class T>
class BST : public SSet <Key,T> {
 public:
  BST();
  ~BST();
  
  //Return the number of items currently in the SSet
  virtual unsigned long size();

  //Add a new item, x, with Key k.
  // If an item with Key k already exists, overwrite it
  virtual void add(Key k, T x);

  //Remove the item with Key k. If there is no such item, do nothing.
  virtual void remove(Key k);

  //Return the item with Key k. 
  // If there is no such item, throw an exception.
  virtual T find(Key k);
  //Return true if there is an item with Key k in the table. If not,
  // return false
  virtual bool keyExists(Key k);

  //If there is a key in the set that is > k,
  // return the first such key. If not, return k
  virtual Key next(Key k);
  //If there is a key in the set that is < k,
  // return the first such key. If not, return k
  virtual Key prev(Key k);

private:
  Node<Key,T>* root;

  virtual unsigned long size(Node<Key,T>* r);
  //These are the recursive versions of each of your methods.
  // You should return the address of the new root node, whether
  // or not the root node changes.
  virtual Node<Key,T>* add(Key k, T x, Node<Key,T>* r);
  virtual Node<Key,T>* remove(Key k, Node<Key,T>* r);

  //This one returns the address of the found node, NULL
  // if not found
  virtual Node<Key,T>* find(Key k, Node<Key,T>* r);

  //Find the item in the sub-tree rooted at r which has the smallest key
  virtual Node<Key,T>* min(Node<Key,T>* r);

  //Find the item in the sub-tree rooted at r which has the largest key
  virtual Node<Key,T>* max(Node<Key,T>* r);

  //Find the next/prev node, and return its address
  virtual Node<Key,T>* next(Key k, Node<Key,T>* r);
  virtual Node<Key,T>* prev(Key k, Node<Key,T>* r);

};

#define NULL 0
#include <string>

template <class Key, class T>
BST<Key, T>::BST(){
	root = NULL;
}

template <class Key, class T>
BST<Key, T>::~BST(){
	//Found this on stack overflow
	while (root != NULL) {
		root = remove(root->k, root);
	}
	root = NULL;
}

//Return the number of items currently in the SSet
template <class Key, class T>
unsigned long BST<Key, T>::size(){
	return size(root);
}

template <class Key, class T>
unsigned long BST<Key, T>::size(Node<Key, T>* r){
	if (r == NULL)
		return 0;

	return 1 + size(r->left) + size(r->right);
}

//Add a new item, x, with Key k.
// If an item with Key k already exists, overwrite it
template <class Key, class T>
void BST<Key, T>::add(Key k, T x){
	root = add(k, x, root);
}

//Remove the item with Key k. If there is no such item, do nothing.
template <class Key, class T>
void BST<Key, T>::remove(Key k){
	if (!keyExists(k))
		throw std::string("ERROR: there is no such key");

	root = remove(k, root);
}

//Return the item with Key k. 
// If there is no such item, throw an exception.
template <class Key, class T>
T BST<Key, T>::find(Key k){
	Node<Key, T> *tempNode = find(k, root);

	if (tempNode == NULL) {
		throw std::string("Error: there is no such key");
	}

	return tempNode->data;
}

//Return true if there is an item with Key k in the table. If not,
// return false
template <class Key, class T>
bool BST<Key, T>::keyExists(Key k){
	if (find(k, root) == NULL)
		return false;

	return true;
}

//If there is a key in the set that is > k,
// return the first such key. If not, return k
template <class Key, class T>
Key BST<Key, T>::next(Key k){
	Node<Key, T>* tempNode = next(k, root);

	if (tempNode == NULL){
		return k;
	}
	return tempNode->k;
}

template <class Key, class T>
Node<Key, T>* BST<Key, T>::next(Key k, Node<Key, T>* r){
	if (r == NULL){
		return NULL;
	}

	if (k < r->k){
		Node<Key, T>* tempNode = next(k, r->left);
		if (tempNode == NULL){
			return r;
		}
		return tempNode;
	}
	else{
		Node<Key, T>* tempNode = next(k, r->right);
		return tempNode;
	}
}

//If there is a key in the set that is < k,
// return the first such key. If not, return k
template <class Key, class T>
Key BST<Key, T>::prev(Key k){
	Node<Key, T> *tempNode = prev(k, root);

	if (tempNode == NULL) {
		return k;
	}

	return tempNode->k;
}

template <class Key, class T>
Node<Key, T>* BST<Key, T>::prev(Key k, Node<Key, T>* r){
	if (r == NULL) {
		return NULL;
	}

	if (k > r->k) {
		Node<Key, T> *temp = prev(k, r->right);
		if (temp == NULL) {
			return r;
		}

		return temp;
	}
	else {
		Node<Key, T> *temp = prev(k, r->left);
		if (temp == NULL) {
			return NULL;
		}

		return temp;
	}
}


template <class Key, class T>
Node<Key, T>* BST<Key, T>::add(Key k, T x, Node<Key, T>* r){
	if (r == NULL){
		Node<Key, T>* tempNode = new Node<Key, T>();
		tempNode->k = k;
		tempNode->data = x;
		tempNode->left = NULL;
		tempNode->right = NULL;
		return tempNode;
	}

	else if (r->k == k) {
		r->data = x;
		return r;
	}

	else if (k < r->k) {
		r->left = add(k, x, r->left);
		return r;
	}
	else {
		r->right = add(k, x, r->right);
		return r;
	}
}

template <class Key, class T>
Node<Key, T>* BST<Key, T>::remove(Key k, Node<Key, T>* r){
	if (r == NULL) {
		return NULL;
	}

	Node<Key, T> *temp;

	if (r->k == k) {
		if (r->left == NULL && r->right == NULL) {
			delete r;
			r = NULL;
			return NULL;
		}

		if (r->right != NULL) {
			temp = min(r->right);
		}
		else {
			temp = max(r->left);
		}

		Node<Key, T> tempNode;
		tempNode.k = temp->k;
		tempNode.data = temp->data;
		remove(temp->k, r);
		r->k = tempNode.k;
		r->data = tempNode.data;

		return r;
	}

	if (r->k < k) {
		temp = remove(k, r->right);
		r->right = temp;
	}
	else {
		temp = remove(k, r->left);
		r->left = temp;
	}
}

template <class Key, class T>
Node<Key, T>* BST<Key, T>::find(Key k, Node<Key, T>* r){
	if (r == NULL) {
		return NULL;
	}

	if (r->k == k) {
		return r;
	}

	if (k < r->k) {
		return find(k, r->left);
	}
	else {
		return find(k, r->right);
	}
}

template <class Key, class T>
Node<Key, T>* BST<Key, T>::max(Node<Key, T>* r){
	if (r == NULL) {
		throw std::string("Error: NULL root passed");
	}

	if (r->right == NULL) {
		return r;
	}
	return max(r->right);
}

template <class Key, class T>
Node<Key, T>* BST<Key, T>::min(Node<Key, T>* r){
	if (r == NULL) {
		throw std::string("Error: NULL root passed");
	}

	if (r->left == NULL) {
		return r;
	}
	return min(r->left);
}

