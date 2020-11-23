#ifndef CP5_ex13_5_h
#define CP5_ex13_5_h
#include <string>
#include <iostream>

class HasPtr
{
public:
	HasPtr(int num) :ps(new std::string()), i(num) {}
	HasPtr(const std::string &s = std::string()) :ps(new std::string(s)), i(0) {}
	HasPtr(const HasPtr& has,int s=1) :ps(new std::string(*has.ps+"2")),i(has.i) {}
	//随机分配一个新的string地址给ps，该地址指向被拷贝的has的string+"2"
	//而如果是编译器自己合成的拷贝构造函数的话，只会拷贝ps本身，也就是说被拷贝与新的ps的值是相等的
	//如果加上不带默认值的int s，那么就不是拷贝构造函数了，就只能使用编译器自己提供的拷贝构造函数，效果是拷贝ps本身
private:
	std::string *ps;
	int i;
};






#endif