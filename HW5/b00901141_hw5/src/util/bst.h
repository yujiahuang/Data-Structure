/****************************************************************************
	FileName     [ bst.h ]
	PackageName  [ util ]
	Synopsis     [ Define binary search tree package ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2005-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode{
	// TODO: design your own class!!
	friend class BSTree<T>;
	friend class BSTree<T>::iterator;

	BSTreeNode(const T& d, BSTreeNode<T>* r = 0, BSTreeNode<T>* l = 0, BSTreeNode<T>* p = 0):
		_data(d), _right(r), _left(l), _parent(p) {}
	
	T              	_data;
	BSTreeNode<T>*  _right;
	BSTreeNode<T>*  _left;
	BSTreeNode<T>* 	_parent;

};

template <class T>
class BSTree
{
	// TODO: design your own class!!
public:
	BSTree(){
	
		_root=0;
	
	}
	~BSTree(){
	
		delete _root;
	
	}

	class iterator {
	
		friend class BSTree;
		
		public:
		iterator(BSTreeNode<T>* n=0): _node(n), _isEnd(false) {}
		iterator(const iterator& i): _node(i._node), _isEnd(i._isEnd) {}
		~iterator() {}

		const T& operator * () const { return _node->_data; }
		T& operator * () { return _node->_data; }
		iterator& operator ++ ()  { 
		
			if(_node->_right!=0) _node=findMin(_node->_right);
			else {
		
				BSTreeNode<T>* tmp=_node;
				while(tmp->_parent!=0 && !(tmp->_data < tmp->_parent->_data) ){
				
					tmp=tmp->_parent;
				
				}
				if(tmp->_parent!=0) _node=tmp->_parent;
				else{
				
						iterator copied((*this));
						copied._isEnd=true;
						(*this)=copied;
					
					}

			}
		
		
		
		/*
			if(_node->_right!=0) _node=findMin(_node->_right);
			else if(_node->_parent!=0){
				
				if(_node==_node->_parent->_left) _node=_node->_parent;
				else{
		
					BSTreeNode<T>* tmp=_node->_parent;
					while(  ( (tmp->_parent)!=0 ) && !( (tmp)==(tmp->_parent->_left) )  ){
				
						tmp=tmp->_parent;
						cout << "climb";

					}
					if(tmp->_parent!=0){
					
						_node=tmp->_parent;
						cout << "//" << _node->_data << "//";

					}
					else{
				
						cout << "end";
						iterator copied((*this));
						copied._isEnd=true;
						(*this)=copied;
					
					}

				}

			}
*/
			return (*this);
		
		}
		
		iterator operator ++ (int)  { 
		
			iterator copied((*this)); 
			++(*this); 
			return copied; 
		
		}

		iterator& operator -- ()  { 
		
			if(_isEnd==true) _isEnd=false;
			else if(_node->_left!=0) _node=findMax(_node->_left);
			else {
		
				BSTreeNode<T>* tmp=_node;
				while(tmp->_parent!=0 && (tmp->_data < tmp->_parent->_data) ){
				
					tmp=tmp->_parent;
				
				}
				if(tmp->_parent!=0) _node=tmp->_parent;
				else{
			
					//then something's wrong
					
				}

			}

			return (*this);

		}

		iterator operator -- (int)  { 
			
			iterator copied((*this)); 
			--(*this); 
			return copied; 
			
		}

		iterator& operator = (const iterator& i)  { this->_node=i._node; this->_isEnd=i._isEnd;  return (*this); }
		
		bool operator != (const iterator& i)  { return ( (this->_isEnd!=i._isEnd) || !((this->_node)==(i._node)) ); }
		bool operator == (const iterator& i)  { return ((this->_node)==(i._node)) && (_isEnd==i._isEnd); }
		
		private:
		BSTreeNode<T>* _node;
		bool _isEnd;

		//helper function
		BSTreeNode<T>* findMin(BSTreeNode<T>* node){
		
			if(node->_left!=0) return findMin(node->_left);
			else return node;
		
		}
		
		BSTreeNode<T>* findMax(BSTreeNode<T>* node){
		
			if(node->_right!=0) return findMax(node->_right);
			else return node;
		
		}

	};

	iterator begin() const { 
	
		if(empty()) return end();
		else return findMin(_root); 
	
	}

	iterator end() const { 
	
		if(empty()) return iterator();
		else{

			iterator last=findMax(_root);
			iterator copied(last);
			copied._isEnd=true;
			return copied;

		}

	}

	bool empty() const { 
	
		return (_root==0); 	
		
	}

	size_t size() const {

		if(empty()) return 0;
		else return count(_root);

		
	}

	void pop_front() {
	
		erase(findMin(_root));
	
	}

	void pop_back() {
	
		erase(findMax(_root));
	
	}

	// return false if nothing to erase
	bool erase(iterator pos) {
	
		if(!empty()){

			if(pos._node->_right!=0){

				iterator min = findMin(pos._node->_right);
				pos._node->_data = min._node->_data;	
				erase(min);

			}
			else if(pos._node->_left!=0){ //no right tree

				iterator max = findMax(pos._node->_left);
				pos._node->_data = max._node->_data;	
				erase(max);

			}
			else{ //no child

				char isRight; // 0: left 1: right x: no parent
				if(pos._node->_parent==0) isRight='x';
				else if(pos._node==pos._node->_parent->_left) isRight='0';
				else isRight='1';

				if(isRight=='1'){
				
					pos._node->_parent->_right=0;
					delete pos._node;
				
				}
				else if(isRight=='0'){
				
					pos._node->_parent->_left=0;
					delete pos._node;
				
				}
				else{
				
					delete pos._node;
					_root=0;
				
				} 

			}
			return true;

		}
		else return false;

	}

	bool erase(const T& x) { 
	
		iterator i=binarySearch(_root, x);	
		if(i._node==0){
		
			return false;

		}
		else{
	
			erase(i);	
			return true; 
		
		}

	}

	bool insert(const T& x) {

		return insertWithRoot(_root, x);

	}


	void clear() {

		while(_root->_left!=0 || _root->_right!=0){
		
			erase(iterator(_root));
	
		}
		delete _root;
		_root=0;

	} // delete all nodes except for the dummy node

	void print(){

		printInorder(_root);

	}

private:
	BSTreeNode<T>* _root;

	//helper function
	iterator findMin(BSTreeNode<T>* root) const{
	
		if(root->_left==0) return iterator(root);
		else return findMin(root->_left);
	
	}
	iterator findMax(BSTreeNode<T>* root) const{
	
		if(root->_right==0) return iterator(root);
		else return findMax(root->_right);
	
	}

	iterator binarySearch(BSTreeNode<T>* root, const T& x){ // return iterator(0) if not found
		
		if(root==0) return iterator(0);
		else if(root->_data==x) return iterator(root);
		else if(root->_data<x){
	
			return binarySearch(root->_right, x);

		}
		else{
			
			return binarySearch(root->_left, x);

		}

	}
	void printInorder(BSTreeNode<T>* root){
	
		if(root!=0){
		
			printInorder(root->_left);
			cout << root->_data << " ";
			printInorder(root->_right);

		}

	}

	size_t count(BSTreeNode<T>* root) const{
	
		int count=0;
		if(root!=0){
		
			size_t left=this->count(root->_left);
			size_t right=this->count(root->_right);
			count=left+right+1;

		}
		return count;
	
	}

	bool insertWithRoot(BSTreeNode<T>*& root, const T& x){
	
		iterator i=binarySearch(root, x);
		if(i==0){
	
			if(root==0){
			
				BSTreeNode<T>* newNode = new BSTreeNode<T>(x);
				root=newNode;

			}
			else if(root->_data<x){
			
				if(root->_right!=0){
				
					insertWithRoot(root->_right, x);

				}
				else{
				
					BSTreeNode<T>* newNode = new BSTreeNode<T>(x);
					newNode->_parent=root;
					root->_right=newNode;
				
				}

			}
			else{
			
				if(root->_left!=0){

					insertWithRoot(root->_left, x);

				}
				else{
				
					BSTreeNode<T>* newNode = new BSTreeNode<T>(x);
					newNode->_parent=root;
					root->_left=newNode;
				
				}

			}
			return true;

		}
		else return false; 
		
	}

};

#endif // BST_H
