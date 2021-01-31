#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <allocators>
#include <string>
#include <numeric> //accumulate
#include <algorithm>
#include <functional> //bind
#include <list> //list
#include<iterator>//back_inserter
#include <map> //map
#include <set> //set
#include <limits.h> //里边定义了一组常量，限定编译器运行的这台机器的不同整数数据类型的取值范围
#include <stdint.h>
#include <fstream> //文件操作
#include <cctype> // 判断char的性质 是大写小写空格等
#include <string>
#include <sstream>//向string写入数据，从string读取数据，就像string是一个IO流一样

//namespace Exercise
//{
//	int ivar = 0;
//	double dvar = 0;
//	const int limit = 1000;
//	float cout = 0;
//}
//namespace Exercise1
//{
//	int ivar = 1;
//	double dvar = 1;
//	const int limit = 10001;
//	float cout = 1;
//}

namespace Exercise
{
	extern int ivar ;
	extern double dvar ;
	extern const int limit ;
	extern float cout ;
}
namespace Exercise1
{
	extern int ivar ;
	extern double dvar ;
	extern const int limit ;
	extern float cout ;
}

#endif