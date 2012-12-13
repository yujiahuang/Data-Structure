/****************************************************************************
	FileName     [ dlist.h ]
	PackageName  [ util ]
	Synopsis     [ Define doubly linked list package ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2005-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
	friend class DList<T>;
	friend class DList<T>::iterator;

	DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
		_data(d), _prev(p), _next(n) {}

	T              _data;
	DListNode<T>*  _prev;
	DListNode<T>*  _next;
};


template <class T>
class DList
{
	public:
		DList() {
			_head = new DListNode<T>(T());
			_head->_prev = _head->_next = _head; // _head is a dummy node
		}
		~DList() { clear(); delete _head; }

		// DO NOT add any more data member or function for class iterator
		class iterator
		{
			friend class DList;

			public:
			iterator(DListNode<T>* n= 0): _node(n) {}
			iterator(const iterator& i) : _node(i._node) {}
			~iterator() {} // Should NOT delete _node

			// TODO: implement these overloaded operators
			const T& operator * () const { return _node->_data; }
			T& operator * () { return _node->_data; }
			iterator& operator ++ ()  { _node=_node->_next; return (*this); }
			iterator operator ++ (int)  { iterator copied((*this)); ++(*this); return copied; }
			iterator& operator -- ()  { _node=_node->_prev; return (*this); }
			iterator operator -- (int)  { iterator copied((*this)); --(*this); return copied; }

			iterator& operator = (const iterator& i)  { this->_node=i._node; return (*this); }

			bool operator != (const iterator& i)  { return (this->_node)!=(i._node); }
			bool operator == (const iterator& i)  { return (this->_node)==(i._node); }

			private:
			DListNode<T>* _node;
		};

		// TODO: implement these functions

		iterator begin() const {     

			return iterator(_head);

		}

		iterator end() const {  

			return iterator(_head->_prev);

		}

		bool empty() const { return (_head==_head->_next)&&(_head==_head->_prev); }
		size_t size() const { 
		
			size_t count=0;
			DListNode<T>* node=_head->_next;
			while(node!=_head){
			
				count++;
				node=node->_next;
			
			}
		
			return count; 
		
		}

		void pop_front() {
		
			DListNode<T>* tmp=_head->_next;
			DListNode<T>* dummy=_head->_prev;
			erase(begin());
			_head=tmp;
			_head->_prev=dummy;
			dummy->_next=_head;	
	
		}
		void pop_back() {
		
			DListNode<T>* tmp=_head->_prev->_prev->_prev;
			erase(iterator(_head->_prev->_prev));
			_head->_prev->_prev=tmp;
			tmp->_next=_head->_prev;
		
		}

		// return false if nothing to erase
		bool erase(iterator pos) {
		
			if(!empty()){

				if(pos._node==_head){

					_head->_next->_prev=_head->_prev;
					_head->_prev->_next=_head->_next;
					DListNode<T>* tmp=_head->_next;
					delete _head;	
					_head=tmp;

				}
				else{

					DListNode<T>* next=pos._node->_next;
					DListNode<T>* prev=pos._node->_prev;
					next->_prev=prev;
					prev->_next=next;
					delete pos._node;

				}

				return true;

			}
			else return false; 
		
		}
		
		bool erase(const T& x) { 
		
			iterator pos=sequentialSearch(x);
			if(pos._node!=0){
			
				erase(pos);
				return true;

			}
			else return false; 
		
		}

		// return false if insertion fails
		bool insert(const T& x) {
		
			iterator pos=sequentialSearch(x);
			if(pos._node==0){
			
				DListNode<T>* node=_head;
				while(node->_data<x && node!=_head->_prev){

					node=node->_next;
			
				}

				if(node==_head || empty()){

					DListNode<T>* newNode = new DListNode<T>(x);
					newNode->_prev=_head->_prev;
					_head->_prev=newNode;
					newNode->_next=_head;
					_head=newNode;
					_head->_prev->_next=_head;

				}
				else{

					DListNode<T>* newNode = new DListNode<T>(x);
					newNode->_prev=node->_prev;
					node->_prev->_next=newNode;
					newNode->_next=node;
					node->_prev=newNode;

				}

				return true;
			
			}
			else return false; 
		
		}

		void clear() { 
		
			DListNode<T>* dummy=_head->_prev;
			DListNode<T>* node=_head->_next;
			while(node!=dummy){
			
				erase(iterator(node->_prev));
				node=node->_next;
			
			}
			erase(iterator(dummy->_prev));
			_head=dummy;
			_head->_prev=_head;
			_head->_next=_head;
		
		} // delete all nodes except for the dummy node

	private:
		DListNode<T>*  _head;  // = dummy node if list is empty

		// [OPTIONAL TODO] helper functions;
		// return iterator(0) if not found
		iterator sequentialSearch(const T& x){
		
			DListNode<T>* node=_head;
			while(!(node->_data==x) && node!=_head->_prev){
			
				node=node->_next;
			
			}
			if(node!=_head->_prev){

				return iterator(node);
			
			}
			else return iterator(0);
		}

};

#endif // DLIST_H
