/****************************************************************************
	FileName     [ myHash.h ]
	PackageName  [ util ]
	Synopsis     [ Define Hash and Cache ADT ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2009-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef MY_HASH_H
#define MY_HASH_H

#include <vector>

using namespace std;

//--------------------
// Define Hash classes
//--------------------
// To use Hash ADT, you should define your own HashKey class.
// It should at least overload the "()" and "==" operators.

class HashKey{

	public:
		
		HashKey() {}
		HashKey(CirGate* g, GateType type) : _fanins(g->getFaninList()), _gateT(type){
				
			// make sure [0] is smaller than [1]
			if(_fanins.size()==2){
			
				if(_fanins[0]->gate()->getId() > _fanins[1]->gate()->getId()){
				
					CirGateV* tmp = _fanins[0];
					_fanins[0]=_fanins[1];
					_fanins[1]=tmp;
				
				}
			
			}

		}

		size_t operator() () const { 
	
			if(_fanins.size()!=0){

				CirGate* gate0=_fanins[0]->gate();
				CirGate* gate1=_fanins[1]->gate();
				return (_fanins[0]->isInv()+1)*(gate0->getId()) + (_fanins[1]->isInv()+1)*(gate1->getId()); 

			}
			else return 0;

		}
		
		bool operator == (const HashKey& k) {
	
			if(_fanins.size()==2 && k._fanins.size()==2){

				bool type = (k._gateT == _gateT);
				bool fanin0 = (k.getFanins()[0]->getGateV() == _fanins[0]->getGateV());
				bool fanin1 = (k.getFanins()[1]->getGateV() == _fanins[1]->getGateV());
				return (type && fanin0 && fanin1); 
			
			}
			else return false;

		}

		const vector<CirGateV*>& getFanins() const { return _fanins; }

	private:
		vector<CirGateV*> _fanins;
		GateType _gateT;
		HashKey* _prev;
		HashKey* _next;

};

template <class HashKey, class HashData>
class Hash{

	typedef pair<HashKey, HashData> HashNode;

	public:
	Hash() : _numBuckets(0), _buckets(0) {
	
		_first=_dummyEnd=new HashKey();
	
	}
	Hash(size_t b) : _numBuckets(0), _buckets(0) { init(b); }
	~Hash() { reset(); }

	// TODO: implement the Hash<HashKey, HashData>::iterator
	// o An iterator should be able to go through all the valid HashNodes
	//   in the Hash
	// o Functions to be implemented:
	//   - constructor(s), destructor
	//   - operator '*': return the HashNode
	//   - ++/--iterator, iterator++/--
	//   - operators '=', '==', !="
	//
	// (_bId, _bnId) range from (0, 0) to (_numBuckets, 0)
	//
	class iterator{

		friend class Hash<HashKey, HashData>;

		public:
		iterator(HashNode* n= 0): _node(n) {}
		iterator(const iterator& i) : _node(i._node) {}
		~iterator() {}

		const HashNode& operator * () const { return *_node; }
		HashNode& operator * () { return *_node; }
		iterator& operator ++ ()  {
		
			_node.first=*(_node.first._next); return (*this); 
			
		}
		iterator operator ++ (int)  { iterator copied((*this)); ++(*this); return copied; }
		iterator& operator -- ()  {

			_node.first=*(_node.first._next); return (*this); 
		
		}
		iterator operator -- (int)  { iterator copied((*this)); --(*this); return copied; }

		iterator& operator = (const iterator& i)  { this->_node=i._node; return (*this); }

		bool operator != (const iterator& i)  { return (this->_node)!=(i._node); }
		bool operator == (const iterator& i)  { return (this->_node)==(i._node); }

		private:
		HashNode* _node;

	};

	// TODO: implement these functions
	//
	// Point to the first valid data
	iterator begin() const { return iterator(_first); }
	// Pass the end
	iterator end() const { return iterator(_dummyEnd); }
	// return true if no valid data
	bool empty() const { return (size()==0); }
	// number of valid data
	size_t size() const {
	
		size_t s = 0; 
		for(size_t i=0; i<_numBuckets; i++){
		
			s+=_buckets[i].size();
		
		}
		return s;
		
	}
	size_t numBuckets() const { return _numBuckets; }

	vector<HashNode>& operator [] (size_t i) { return _buckets[i]; }
	const vector<HashNode>& operator [](size_t i) const { return _buckets[i]; }

	void init(size_t b) {
	
		_buckets=new vector<HashNode>[_numBuckets];
		_first=_dummyEnd=new HashKey();

	}
	void reset() { 
	
		delete [] _buckets;
		delete _first;
		delete _dummyEnd;

	}

	// check if k is in the hash...
	// if yes, update n and return true;
	// else return false;
	bool check(const HashKey& k, HashData& n) { 
	
		size_t bucketN = bucketNum(k);
		for(typename vector<HashNode>::iterator it=(_buckets[bucketN]).begin(); it!=(_buckets[bucketN]).end(); it++){
		
			if(k==(*it).first){
			
				(*it).second=n;
				return true;

			}

		}

		return false; 
		
	}

	// return true if inserted successfully (i.e. k is not in the hash)
	// return false is k is already in the hash ==> will not insert
	bool insert(const HashKey& k, const HashData& d) { 

		// check exists
		size_t bucketN = bucketNum(k);
		for(typename vector<HashNode>::iterator it=(_buckets[bucketN]).begin(); it!=(_buckets[bucketN]).end(); it++){
		
			if(k==(*it).first){
			
				return false;

			}

		}

		forceInsert(k,d);
		return true; 
	
	}

	// return true if inserted successfully (i.e. k is not in the hash)
	// return false is k is already in the hash ==> still do the insertion
	bool replaceInsert(const HashKey& k, const HashData& d) { 
	
		if(insert(k,d)) return true;
		else{

			check(k,d);
			return false; 
		
		}

	}

	// Need to be sure that k is not in the hash
	void forceInsert(const HashKey& k, const HashData& d) {
	
		size_t bucketN = bucketNum(k);
		_buckets[bucketN].push_back(HashNode(k,d));

		if(!empty()){

			_dummyEnd->_prev->_next=&k;
			_dummyEnd->_prev=&k;

		}
		else{
		
			_first=&k;
			_first->_next=_dummyEnd;
			_dummyEnd->_prev=_first;
		
		}

	}

	private:
	// Do not add any extra data member
	size_t                   _numBuckets;
	vector<HashNode>*        _buckets;
	HashKey*								 _first;
	HashKey*								 _dummyEnd;

	size_t bucketNum(const HashKey& k) const {
		return (k() % _numBuckets); }

};


//---------------------
// Define Cache classes
//---------------------
// To use Cache ADT, you should define your own HashKey class.
// It should at least overload the "()" and "==" operators.
//
// class CacheKey
// {
// public:
//    CacheKey() {}
//    
//    size_t operator() () const { return 0; }
//   
//    bool operator == (const CacheKey&) const { return true; }
//       
// private:
// }; 
// 
template <class CacheKey, class CacheData>
class Cache
{
	typedef pair<CacheKey, CacheData> CacheNode;

	public:
	Cache() : _size(0), _cache(0) {}
	Cache(size_t s) : _size(0), _cache(0) { init(s); }
	~Cache() { reset(); }

	// NO NEED to implement Cache::iterator class

	// TODO: implement these functions
	//
	// Initialize _cache with size s
	void init(size_t s) { }
	void reset() { }

	size_t size() const { return _size; }

	CacheNode& operator [] (size_t i) { return _cache[i]; }
	const CacheNode& operator [](size_t i) const { return _cache[i]; }

	// return false if cache miss
	bool read(const CacheKey& k, CacheData& d) const { return false; }
	// If k is already in the Cache, overwrite the CacheData
	void write(const CacheKey& k, const CacheData& d) { }

	private:
	// Do not add any extra data member
	size_t         _size;
	CacheNode*     _cache;
};


#endif // MY_HASH_H
