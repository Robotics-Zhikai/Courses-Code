#pragma once
#include "main.h"
#include "reflect.h"
namespace abstract_factory
{
//抽象工厂类模式

class User
{
public:
	int ID;
	string name;
};

class Department
{
public:
	int ID;
	string name;
};

class Iuser
{
public:
	virtual void insert_user(User us) = 0;
	virtual User get_user(int ID) = 0;

};

class Idepartment
{
public:
	virtual void insert_department(Department dt) = 0;
	virtual Department get_department(int ID) = 0;
};

class AccessDepartment :public Idepartment
{
public:
	void insert_department(Department dt) override
	{
		cout << "AccessDepartment::insert_department" << endl;
	}
	Department get_department(int ID) override
	{
		cout << "AccessDepartment::get_department" << endl;
		return Department();
	}
};

class SqlserverDepartment :public Idepartment
{
public:
	void insert_department(Department dt) override
	{
		cout << "SqlserverDepartment::insert_department" << endl;
	}
	Department get_department(int ID) override
	{
		cout << "SqlserverDepartment::get_department" << endl;
		return Department();
	}
};

class AccessUser :public Iuser
{
public:
	void insert_user(User us)override
	{
		cout << "AccessUser::insert_user" << endl;
	}
	User get_user(int ID)override
	{
		cout << "AccessUser::get_user" << endl;
		return User();
	}
};

class SqlserverUser :public Iuser
{
public:
	void insert_user(User us)override
	{
		cout << "SqlserverUser::insert_user" << endl;
	}
	User get_user(int ID)override
	{
		cout << "SqlserverUser::get_user" << endl;
		return User();
	}
};

class factory_base
{
public:
	virtual shared_ptr<Iuser> return_Iuser() = 0;
	virtual shared_ptr<Idepartment> return_Idepartment() = 0;
};

class AccessFactory :public factory_base
{
public:
	shared_ptr<Iuser> return_Iuser() override
	{
		return shared_ptr<Iuser>(new AccessUser);
	}
	shared_ptr<Idepartment> return_Idepartment() override
	{
		return shared_ptr<Idepartment>(new AccessDepartment);
	}
};

class SqlserverFactory :public factory_base //为新产品族提供方便
{
public:
	shared_ptr<Iuser> return_Iuser() override
	{
		return shared_ptr<Iuser>(new SqlserverUser);
	}
	shared_ptr<Idepartment> return_Idepartment() override
	{
		return shared_ptr<Idepartment>(new SqlserverDepartment);
	}
};
//但不能为新的产品等级结构的增加提供这样的方便。


//利用简单工厂模式改进抽象工厂模式
/*
如果再要加一个数据库，那么只需要再继承一个factory_base 再定义相应的数据库User和数据库Department即可
这需要操作三个类。
且抽象工厂模式每次使用具体的函数时，都需要shared_ptr<factory_base> fcbase(new SqlserverFactory);
造成了文本里出现了很多次shared_ptr<factory_base> fcbase(new SqlserverFactory); 
基于上述原因，可以通过简单工厂模式对抽象工厂模式进行改进,以便能够解决文本中出现多次对应数据库的问题
*/
class Database
{
public:
	shared_ptr<factory_base> retfac()
	{
		if (DBname == "Sqlservel")
			return shared_ptr<factory_base>(new SqlserverFactory);
		else if (DBname == "Access")
			return shared_ptr<factory_base>(new AccessFactory);
		//else if (DBname == "whatever")
		//	return shared_ptr<factory_base>(new whateverFactory);

	}
private:
	string DBname = "Sqlservel"; //只需要在所有程序的一开始定义一个这个，那么以后所有用到Database的都用这个代替即可
};

Database Databasethis;//只要调用这个 就可以替代所有的

//开放-封闭原则告诉我们：对于扩展，我们开放；但对于修改，要尽量关闭

//上述代码还可以利用反射来进行改进 反射的实现需要用到单例模式
factory_base * Sqlservelret() { return new SqlserverFactory; }
factory_base * Accessret() { return new AccessFactory; }
RegisterAction sq("Sqlservel", (PTRCreateObject)Sqlservelret);
RegisterAction Ac("Access", (PTRCreateObject)Accessret);
//这完全可以放在配置文件里，与业务逻辑的实现无关

class DatabaseAdvance
{
public:
	shared_ptr<factory_base> retfac(string strdatabase)
	{
		//auto a = Reflect::getInstance().getClassByName(strdatabase);
		//auto a22 = Reflect::getInstance().getClassByName(strdatabase)();
		//auto a1 = (factory_base*)Reflect::getInstance().getClassByName(strdatabase);
		//auto a2 = (factory_base*)Reflect::getInstance().getClassByName(strdatabase)();//void*地址转化成factory_base*
		return shared_ptr<factory_base>((factory_base*)Reflect::getInstance().getClassByName(strdatabase)());
		//用到了单例模式，只能有一个map<string,PTRCreateObject> 实现反射机制
		//借助反射机制，就不需要调用多个case了，if  switch啥的
	}
};

/*
工厂方法也具有唯一性，一般情况下，一个具体工厂中只有一个工厂方法或者一组重载的工厂方法。
但是有时候我们需要一个工厂可以提供多个产品对象，而不是单一的产品对象。
当系统所提供的工厂所需生产的具体产品并不是一个简单的对象，
而是多个位于不同产品等级结构中属于不同类型的具体产品时需要使用抽象工厂模式。
抽象工厂模式是所有形式的工厂模式中最为抽象和最具一般性的一种形态。

当一个工厂等级结构可以创建出分属于不同产品等级结构的一个产品族中的所有对象时，
抽象工厂模式比工厂方法模式更为简单、有效率

当一个产品族中的多个对象被设计成一起工作时，它能够保证客户端始终只使用同一个产品族中的对象。
这对一些需要根据当前环境来决定其行为的软件系统来说，是一种非常实用的设计模式。

增加新的具体工厂和产品族很方便，无须修改已有系统，符合“开闭原则”。

缺点 
1.在添加新的产品对象时，难以扩展抽象工厂来生产新种类的产品，这是因为在抽象工厂角色中规定了所有可能被创建的产品集合，
要支持新种类的产品就意味着要对该接口进行扩展，而这将涉及到对抽象工厂角色及其所有子类的修改，显然会带来较大的不便。
2.开闭原则的倾斜性（增加新的工厂 只需要继承一下就行 和产品族容易，增加新的产品等级结构麻烦）。
*/

/*
对“开闭原则”的支持呈现倾斜性。
1.增加产品族：对于增加新的产品族，工厂方法模式很好的支持了“开闭原则”，
对于新增加的产品族，只需要对应增加一个新的具体工厂即可，对已有代码无须做任何修改。
2.增加新的产品等级结构：对于增加新的产品等级结构，需要修改所有的工厂角色，包括抽象工厂类，
在所有的工厂类中都需要增加生产新产品的方法，不能很好地支持“开闭原则”。

抽象工厂模式适用情况包括：一个系统不应当依赖于产品类实例如何被创建、组合和表达的细节；
系统中有多于一个的产品族，而每次只使用其中某一产品族；属于同一个产品族的产品将在一起使用；
系统提供一个产品类的库，所有的产品以同样的接口出现，从而使客户端不依赖于具体实现！！！。
*/
void test()
{
	//不同的数据库进行同样的逻辑操作用到的指令是不一样的，运用多态技术可以保持向底层的调用接口不变
	shared_ptr<factory_base> fcbase(new SqlserverFactory);

	//客户端认识的只有Iuser和Idepartment，具体用到什么数据库是不知道的
	fcbase->return_Iuser()->insert_user(User());
	fcbase->return_Iuser()->get_user(1);
	fcbase->return_Idepartment()->insert_department(Department());
	fcbase->return_Idepartment()->get_department(1);

	fcbase = shared_ptr<factory_base>(new AccessFactory); //任何用户代码都不需要修改 只需要改new AccessFactory即可
	fcbase->return_Iuser()->insert_user(User());
	fcbase->return_Iuser()->get_user(1);
	fcbase->return_Idepartment()->insert_department(Department());
	fcbase->return_Idepartment()->get_department(1);

	//经过简单工厂模式改进的抽象工厂模式，不需要修改new 但是需要修改Databasethis的成员变量
	Databasethis.retfac()->return_Iuser()->insert_user(User());
	Databasethis.retfac()->return_Iuser()->get_user(1);
	Databasethis.retfac()->return_Idepartment()->insert_department(Department());
	Databasethis.retfac()->return_Idepartment()->get_department(1);

	//经过反射改进的简单工厂和抽象工厂模式结合 不需要修改任何源代码 只需要输入参数即可
	DatabaseAdvance DatabaseAdvance;
	DatabaseAdvance.retfac("Sqlservel")->return_Iuser()->insert_user(User());
	DatabaseAdvance.retfac("Sqlservel")->return_Iuser()->get_user(1);
	DatabaseAdvance.retfac("Sqlservel")->return_Idepartment()->insert_department(Department());
	DatabaseAdvance.retfac("Sqlservel")->return_Idepartment()->get_department(1);

	DatabaseAdvance.retfac("Access")->return_Iuser()->insert_user(User());
	DatabaseAdvance.retfac("Access")->return_Iuser()->get_user(1);
	DatabaseAdvance.retfac("Access")->return_Idepartment()->insert_department(Department());
	DatabaseAdvance.retfac("Access")->return_Idepartment()->get_department(1);
}

//https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/abstract_factory.html
//大话设计模式

}