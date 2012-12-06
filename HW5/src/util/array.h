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
			const T& operator * () const { return (*_node); }
			T& operator * () { return (*_node); }
			iterator& operator ++ () { _node++; return (*this); }
			iterator operator ++ (int) { iterator copied((*this)); ++(*this); return copied; }
			iterator& operator -- () { _node--; return (*this); }
			iterator operator -- (int) { iterator copied((*this)); --(*this); return copied; }

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

			if(_capacity==0 || _size==0) return end();
			else return iterator(&_data[0]);

		}

		iterator end() const {
		
			if(_capacity!=0) return iterator(&_data[_size-1]+1);
			else return 0;
			
		}

		bool empty() const { return (_size==0); }
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
			
				for(iterator i=pos; (i+1)!=end(); i++){
				
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
			else{
				
				if(_capacity==_size){

					T* tmp;
					if(_size!=0){

						tmp=new T[2*_size];
						_capacity=2*_size;

					}
					else{

						tmp=new T[1];
						_capacity=1;

					}				

					for(size_t i=0; i<_size; i++){

						tmp[i]=_data[i];

					}
					delete [] _data;
					_data=tmp;

				}

				size_t i=0;
				for(; i<_size && _data[i]<x; i++);
				for(size_t j=_size; j>i; j--){

					_data[j]=_data[j-1];

				}
				_data[i]=x;
				++_size;
/*
				_data[_size]=x;
				++_size;
				mergeSort(_data, _size);
*/
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
		
		//swap
		void swap(T& a, T& b){

			T tmp = a;
			a=b;
			b=tmp;

		}

		// merge sort TODO
		void mergeSort(T* data, size_t size){
			if(size<2);	
			else if(size==2){

				if(data[1]<data[0]) swap(data[0], data[1]);

			}
			else{

				size_t leftS=size/2;
				size_t rightS=size-leftS;
				T left[leftS], right[rightS];

				//copy
				for(size_t i=0; i<leftS; i++){

					left[i]=data[i];

				}
				for(size_t j=0; j<rightS; j++){

					right[j]=data[j+leftS];

				}

				//recursive
				mergeSort(left, leftS);
				mergeSort(right, rightS);

				//merge
				size_t leftI=0, rightI=0, i=0;
				while(i<size && leftI<leftS && rightI<rightS){

					if(left[leftI]<right[rightI]){

						data[i]=left[leftI];
						leftI++;

					}
					else{

						data[i]=right[rightI];
						rightI++;

					}
					i++;

				}
				while(i<size && leftI<leftS){
	
					data[i]=left[leftI];
					leftI++;
					i++;

				}
				while(i<size && rightI<rightS){

					data[i]=right[rightI];
					rightI++;
					i++;

				}

			}
	
		}
		
		// binary search TODO
		// return -1 if not found
		int binarySearch(const T& x){
		
			int high=_size-1, low=0;
			while(high>=low){
			
				if(x==_data[(high+low)/2]) return (high+low)/2;
				else if(x<_data[(high+low)/2]){
				
					high=(high+low)/2-1;
				
				}
				else{
				
					low=(high+low)/2+1;
				
				}
			
			}
			return -1;

		}



};

#endif // ARRAY_H
