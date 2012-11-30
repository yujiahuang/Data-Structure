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
			iterator& operator ++ () { _node+=1; return (*this); }
			iterator operator ++ (int) { iterator copied((*this)); this->_node+=1; return copied; }
			iterator& operator -- () { this->_node-=1; return (*this); }
			iterator operator -- (int) { iterator copied((*this)); this->_node-=1; return copied; }

			iterator operator + (int i) const { iterator copied((*this)); copied._node+=i; return copied; }
			iterator& operator += (int i) { this->_node+=i; return (*this); }

			iterator& operator = (const iterator& i) { this->_node=i._node; return (*this); }

			bool operator != (const iterator& i) const { return (this->_node)!=(i._node); }
			bool operator == (const iterator& i) const { return (this->_node)==(i._node); }

			private:
			T*    _node;
		};

		// TODO: implement these functions
		iterator begin() const { 

			if(_size!=0) return iterator(&_data[0]);
			else return this->end();

		}

		iterator end() const {
		
			if(_capacity!=0) return iterator(&_data[_size]);
			else return 0;
			
		}

		bool empty() const { return !_size; }
		size_t size() const { return _size; }

		T& operator [] (size_t i) { return _data[i]; }
		const T& operator [] (size_t i) const { return _data[i]; }

		void pop_front() {
		
			if(this->begin()!=0) erase(this->begin());
			for(size_t i=0; i<_size-2; i++){
			
				_data[i]=_data[i+1];

			}

		}

		void pop_back() {
		
			if(this->end()+(-1)!=0) erase(this->end()+(-1));

		}

		bool erase(iterator pos) {
		
			if(!empty()){
			
				delete pos._node;
				for(iterator i=pos; i!=end(); i++){
				
					*i=*(i+1);
				
				}
				
				this->_size-=1;
				return true;

			}
			else return false;
			
		}
		//TODO
		bool erase(const T&x) {
		
			bool returnedValue=false;
			int i=binarySearch(x);
			if(i!=-1){
			
				erase(iterator(&_data[i]));	
				returnedValue=true;
			
			}
			else returnedValue=false;
	
			return returnedValue;

		}

		bool insert(const T& x) {
		
			if(binarySearch(x)!=-1) return false;
			else {
			
				T* tmp=new T[_size+1];
				for(size_t i=0; i<_size; i++){
				
					tmp[i]=_data[i];
				
				}
				delete [] _data;
				_data=tmp;
				_data[_size]=x;
				++_size;
				++_capacity;
				mergeSort(_data, _size);
				return true;

			}
		
		}

		void clear() {
		
			_size=0;
		
		}

		// Nice to have, but not required in this homework...
		// void reserve(size_t n) { ... }
		// void resize(size_t n) { ... }

	private:
		T*           _data;
		size_t       _size;       // number of valid elements
		size_t       _capacity;   // max number of elements

		// [OPTIONAL TODO] Helper functions;
		
		// merge sort TODO
		void mergeSort(T* data, size_t size){
	
			if(size!=1){

			size_t leftS=size/2, rightS=size/2;
			if(size%2==1) ++leftS;
			T left[leftS], right[rightS];

			//copy
			size_t i=0;
			for(; i<leftS; i++){

				left[i]=data[i];

			}
			for(size_t j=0; j<rightS; j++){

				right[j]=data[i];
				i++;

			}

			//recursive
			mergeSort(left, leftS);
			mergeSort(right, rightS);

			//merge
			size_t leftI=0, rightI=0;
			i=0;
			for(; i<size && leftI<leftS && rightI<rightS; i++){
			
				if(left[leftI]<right[rightI]){
				
					data[i]=left[leftI];
					++leftI;
				
				}
				else{
				
					data[i]=right[rightI];
					++rightI;
				
				}
			
			}
			for(; i<size && leftI<leftS; i++){
				
					data[i]=left[leftI];
					leftI++;
			
			}
			for(; i<size && rightI<rightI; i++){
				
					data[i]=right[rightI];
					rightI++;
			
			}
			
			}
		
		}
		
		// binaery search TODO
		// return -1 if not found
		int binarySearch(const T& x){
		
			int high=_size-1, low=0;
			while(high>low){
			
				if(x==_data[(high+low)/2]) return high+low/2;
				else if(x<_data[(high+low)/2]){
				
					high=(high+low)/2;
				
				}
				else{
				
					low=(high+low)/2;
				
				}
			
			}
			return -1;

		}



};

#endif // ARRAY_H
