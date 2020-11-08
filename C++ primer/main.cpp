#include <iostream>
#include "main.h"
#include "another.h"
#include "CLASS.h"
using namespace std;

namespace Exercise
{
	int ivar = 0;
	double dvar = 0;
	const int limit = 1000;
	float cout = 0;
}
namespace Exercise1
{
	int ivar = 1;
	double dvar = 1;
	const int limit = 10001;
	float cout = 1;
}

int ivar = 10;

int inlinetest()
{
	return 2;
}

void main()
{
	cout << testinline() << endl;
	cout << inlinetest() << endl;
	
	int a = 5;
	int * Locate_a = &a;
	int ** locloc_a = &Locate_a;
	int *** loclocloc_a = &locloc_a;
	cout << Locate_a << endl;
	cout << locloc_a << endl;
	cout << loclocloc_a << endl;
	
	//using namespace Exercise;
	using Exercise::ivar;
	//using Exercise::dvar;
	using Exercise::limit;
	
	fuc();
	using Exercise::cout;
	std::cout << cout << endl;

	double dvar = 3.1415;

	int iobj = limit + 1;
	++ivar;
	++::ivar;

}