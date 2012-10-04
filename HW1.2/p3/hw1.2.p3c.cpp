#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <class T> void mySwap(T &a, T &b){

	T tmp=a;
	a=b;
	b=tmp;

}

template <class T>
class Compare{

public:
	virtual bool operator() (T a, T b) const = 0;

};

template <class T>
class Less : public Compare<T>{

	bool operator() (T a, T b) const{
	
		return (a<b);

	}

};

template <class T>
class Greater : public Compare<T>{

	bool operator() (T a, T b) const{
	
		return (a>b);

	}

};


//
//DO NOT CHANGE FOLLOWING
//
template <class T>
void selectionSort(vector<T>& array, const Compare<T>& compare){

	for (size_t i = 0, n = array.size(); i < n - 1; ++i) {
	
		size_t pivot = i;
		for (size_t j = i+1; j < n; ++j) {
	
			if (!compare(array[pivot], array[j])) pivot = j;

		}

		if (pivot != i)	mySwap(array[pivot], array[i]);
	
	}

}
//
//DO NOT CHANGE ABOVE
//

int main(){

	int num;

//string part
	cout << "How many numbers? ";
	cin >> num;

	vector<string> strArr(num);

	for(int i=0; i<num; i++){
	
		cin >> strArr[i];
	
	}

	cout << "Before sort:" << endl;
	for(int i=0; i<num; i++){
	
		cout << strArr[i] << " ";
	
	}

	Less<string> less;
	selectionSort(strArr, less);

	cout << "\nAscending sort:" << endl;
	for(int i=0; i<num; i++){
	
		cout << strArr[i] << " ";
	
	}
	cout << endl;


//double part
	cout << "\nHow many numbers? ";
	cin >> num;

	vector<double> doubleArr(num);

	for(int i=0; i<num; i++){
	
		cin >> doubleArr[i];
	
	}

	cout << "Before sort:" << endl;
	for(int i=0; i<num; i++){
	
		cout << doubleArr[i] << " ";
	
	}

	Greater<double> greater;
	selectionSort(doubleArr, greater);


	cout << "\nDescending sort:" << endl;
	for(int i=0; i<num; i++){
	
		cout << doubleArr[i] << " ";
	
	}
	cout << endl;

	return 0;

}

