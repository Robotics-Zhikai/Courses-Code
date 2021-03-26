#pragma once
#include "main.h"
namespace singleton_pattern
{
//单例模式
class singleton
{
public:
	static shared_ptr<singleton> get_instance(double num1,double num2)
	{
		if (instance == nullptr)
			instance = shared_ptr<singleton>(new singleton(num1,num2));
		return instance;
	}
	static shared_ptr<singleton> get_instance()
	{
		if (instance == nullptr)
			instance = shared_ptr<singleton>(new singleton());
		return instance;
	}
	void showmessage()
	{
		cout << num1 << "," << num2 << endl;
	}
private:
	double num1 = 0;
	double num2 = 0;
	singleton() = default;
	singleton(double num1, double num2) :num1(num1), num2(num2) {}
	
	static shared_ptr<singleton> instance;
};
shared_ptr<singleton> singleton::instance = nullptr;

/*
对于系统中的某些类来说，只有一个实例很重要，例如，一个系统中可以存在多个打印任务，但是只能有一个正在工作的任务；
一个系统只能有一个窗口管理器或文件系统；一个系统只能有一个计时工具或ID（序号）生成器。

如何保证一个类只有一个实例且这个实例易于被访问(最好弄成static的)
一个更好的解决办法是让类自身负责保存它的唯一实例。这个类可以保证没有其他实例被创建，并且它可以提供一个访问该实例的方法

单例模式定义：单例模式确保某一个类只有一个实例，而且自行实例化并向整个系统提供这个实例，这个类称为单例类，提供全局访问的方法

单例类有私有构造函数，确保用户无法通过new关键字直接实例化它

优点：
允许可变数目的实例，可以基于单例模式进行拓展，适用于单力控制相似的方法来获得指定个数的对象实例

缺点：
1、由于单例模式没有抽象层，因此单例类的扩展有很大的困难
2、与单一职责原则冲突，一个类应该只关心内部逻辑，而不关心外面怎么样来实例化
且单例类既充当了工厂角色，提供了工厂方法，同时又充当了产品角色，包含一些业务方法，
将产品的创建（get_instance）和产品本身的功能（showmessage）融合在一起
3、现在很多面向对象语言（Java、C#)的运行环境都提供了自动垃圾回收的技术，因此，如果实例化的对象长时间不被利用
系统会认为它是垃圾，会自动销毁并回收资源，下次利用时又将重新实例化，这将导致对象状态的丢失
*/
void test()
{
	singleton s = *singleton::get_instance(4,6); //默认合成的拷贝构造函数是public的，因此可以这样调用得到一个s
	s.get_instance(3, 5)->showmessage();//即便改变了3,5 由于单例模式的固有特性，还是不会改变数字的
	singleton::get_instance(5,6)->showmessage();
	singleton::get_instance()->showmessage();
}

//https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/singleton.html
//还涉及到懒汉饿汉等模式 暂时还没有学会 可能涉及到操作系统的知识



}//end singleton_pattern