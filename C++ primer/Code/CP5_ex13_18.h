#pragma once

#include <string>
#include <iostream>

class Employee
{
public:
	//Employee() = default; //加上这个在用该函数时会报错Employee是已删除的默认构造函数
	Employee(const std::string& nameinput) :name(nameinput), ID(++ID_distribute) {}
	//采用引用的形式可以避免进行拷贝构造
	Employee(Employee& emp) = delete;
private:
	std::string name;
	const long ID;//由于有这个成员的存在，合成的默认构造函数是删除的 合成的拷贝赋值函数也是删除的
	//但是合成的拷贝构造函数是存在的，但雇员的信息按理说不能被拷贝构造，因此应声明为delete
	static long ID_distribute;
};

long Employee::ID_distribute = 0;

