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
#include <limits.h> //��߶�����һ�鳣�����޶����������е���̨�����Ĳ�ͬ�����������͵�ȡֵ��Χ
#include <stdint.h>
#include <fstream> //�ļ�����
#include <cctype> // �ж�char������ �Ǵ�дСд�ո��
#include <string>
#include <sstream>//��stringд�����ݣ���string��ȡ���ݣ�����string��һ��IO��һ��

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