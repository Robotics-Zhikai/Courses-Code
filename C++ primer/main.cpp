#include <iostream>
#include "main.h"
#include "another.h"
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


void main()
{
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