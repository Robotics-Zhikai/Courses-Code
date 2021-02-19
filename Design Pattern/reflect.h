#pragma once
#include "main.h"
/*

反射：如何通过类的名称字符串来生成类的对象，比如有一个类classA，那么如何通过类名称字符串"classA"来创建类的对象？
反射是指计算机程序在运行时可以访问、检测和修改它本身状态或行为的一种能力
实际上反射就是程序在运行时动态获取对象信息以及调用对象方法的能力
通过类名称创建类对象是反射的功能之一

在MATLAB中反射是不是相当于eval？
*/

//https://blog.csdn.net/K346K346/article/details/51698184
typedef void * (*PTRCreateObject)();
//定义一个函数指针，用于指向创建类实例的回调函数

class Reflect
{
private:
	map<string, PTRCreateObject> m_classMap;
	Reflect() = default;
public:
	PTRCreateObject getClassByName(string ClassName) //应该是调用map得到string对应的函数指针
	{
		map<string, PTRCreateObject>::iterator it ;
		it = m_classMap.find(ClassName);
		if (it == m_classMap.end())
			return NULL;
		else
			return it->second;
	}
	void registClass(string name, PTRCreateObject method)//将给定的类名称字符串和对应的创建类对象的函数保存到map中
	{
		m_classMap.insert({ name,method });
	}
	
	static Reflect& getInstance()//获取工厂类的单个实例对象
	{
		static Reflect slo_factory;
		return slo_factory;
	}
};

//注册动作类
class RegisterAction
{
public:
	RegisterAction(string classname, PTRCreateObject ptrfuc)
	{
		Reflect::getInstance().registClass(classname, ptrfuc);
	}
};

//test class
class TestClassA
{
public:
	void m_print()
	{
		cout << "hello TestClassA" << endl;
	}
};

TestClassA* createObjectClassA()
{
	return new TestClassA;
}

RegisterAction g_createObjectClassA("TestClassA", (PTRCreateObject)createObjectClassA);

//在abstract_factory中用到了反射机制