/****************************************************************************
	FileName     [ array.h ]
	PackageName  [ util ]
	Synopsis     [ Define dynamic array package ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2005-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
	public:
		Array() : _data(0), _size(0), _capacity(0) {}
		~Array() { delete []_data; }

		// DO NOT add any more data member or function for class iterator
		class iterator
		{
			friend class Array;

			public:
			iterator(T* n= 0): _node(n) {}
			iterator(const iterator& i): _node(i._node) {}
			~iterator() {} // Should NOT delete _node

			// TODO: implement these overloaded operators
			const T& operator * () const { return (*this); }
			T& operator * () { return (*_node); }
			iterator& operator ++ () { this->_node+=1; return (*this); }
			iterator operator ++ (int) { iterator copied=(*this); this->_node+=1; return (*copied); }
			iterator& operator -- () { this->_node-=1; return (*this); }
			iterator operator -- (int) { iterator copied=(*this); this->_node-=1; return (*copied); }

			iterator operator + (int i) const { iterator copied=(*this); copied._node+=i; return copied; }
			iterator& operator += (int i) { this->_node+=i; return (*this); }

			iterator& operator = (const iterator& i) { this->_node=i._node; return (*this); }

			bool operator != (const iterator& i) const { return (this->_node)!=(i._node); }
			bool operator == (const iterator& i) const { return (this->_node)==(i._node); }

			private:
			T*    _node;
		};

		// TODO: implement these functions
		iterator begin() const { 

			if(_size!=0) return iterator(_data[0]);
			else return this->end();

		}

		iterator end() const {
		
			if(_capacity!=0) return iterator(_data[_size]);
			else return 0;
			
		}

		bool empty() const { return !_size; }
		size_t size() const { return _size; }

		T& operator [] (size_t i) { return _data[i]; }
		const T& operator [] (size_t i) const { return _data[i]; }

		void pop_front() {
		
			if(this->begin()) erase(this->begin());
			for(size_t i=0; i<_size-2; i++){
			
				_data[i]=_data[i+1];

			}

		}

		void pop_back() {
		
			if(this->end()) erase(this->end());
		
		}

		bool erase(iterator pos) {
		
			if(!empty()){
			
				delete pos._node;
				this->_size-=1;
			
			}
			else return false;
			
		}
		//TODO
		bool erase(const T&x) { return false; }
		bool insert(const T& x) { return false; }

		void clear() { }

		// Nice to have, but not required in this homework...
		// void reserve(size_t n) { ... }
		// void resize(size_t n) { ... }

	private:
		T*           _data;
		size_t       _size;       // number of valid elements
		size_t       _capacity;   // max number of elements

		// [OPTIONAL TODO] Helper functions;
		// binaery search TODO
		// merge sort TODO

};

#endif // ARRAY_H
