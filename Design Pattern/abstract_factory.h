#pragma once
#include "main.h"
#include "reflect.h"
namespace abstract_factory
{
//���󹤳���ģʽ

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

class SqlserverFactory :public factory_base //Ϊ�²�Ʒ���ṩ����
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
//������Ϊ�µĲ�Ʒ�ȼ��ṹ�������ṩ�����ķ��㡣


//���ü򵥹���ģʽ�Ľ����󹤳�ģʽ
/*
�����Ҫ��һ�����ݿ⣬��ôֻ��Ҫ�ټ̳�һ��factory_base �ٶ�����Ӧ�����ݿ�User�����ݿ�Department����
����Ҫ���������ࡣ
�ҳ��󹤳�ģʽÿ��ʹ�þ���ĺ���ʱ������Ҫshared_ptr<factory_base> fcbase(new SqlserverFactory);
������ı�������˺ܶ��shared_ptr<factory_base> fcbase(new SqlserverFactory); 
��������ԭ�򣬿���ͨ���򵥹���ģʽ�Գ��󹤳�ģʽ���иĽ�,�Ա��ܹ�����ı��г��ֶ�ζ�Ӧ���ݿ������
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
	string DBname = "Sqlservel"; //ֻ��Ҫ�����г����һ��ʼ����һ���������ô�Ժ������õ�Database�Ķ���������漴��
};

Database Databasethis;//ֻҪ������� �Ϳ���������е�

//����-���ԭ��������ǣ�������չ�����ǿ��ţ��������޸ģ�Ҫ�����ر�

//�������뻹�������÷��������иĽ� �����ʵ����Ҫ�õ�����ģʽ
factory_base * Sqlservelret() { return new SqlserverFactory; }
factory_base * Accessret() { return new AccessFactory; }
RegisterAction sq("Sqlservel", (PTRCreateObject)Sqlservelret);
RegisterAction Ac("Access", (PTRCreateObject)Accessret);
//����ȫ���Է��������ļ����ҵ���߼���ʵ���޹�

class DatabaseAdvance
{
public:
	shared_ptr<factory_base> retfac(string strdatabase)
	{
		//auto a = Reflect::getInstance().getClassByName(strdatabase);
		//auto a22 = Reflect::getInstance().getClassByName(strdatabase)();
		//auto a1 = (factory_base*)Reflect::getInstance().getClassByName(strdatabase);
		//auto a2 = (factory_base*)Reflect::getInstance().getClassByName(strdatabase)();//void*��ַת����factory_base*
		return shared_ptr<factory_base>((factory_base*)Reflect::getInstance().getClassByName(strdatabase)());
		//�õ��˵���ģʽ��ֻ����һ��map<string,PTRCreateObject> ʵ�ַ������
		//����������ƣ��Ͳ���Ҫ���ö��case�ˣ�if  switchɶ��
	}
};

/*
��������Ҳ����Ψһ�ԣ�һ������£�һ�����幤����ֻ��һ��������������һ�����صĹ���������
������ʱ��������Ҫһ�����������ṩ�����Ʒ���󣬶����ǵ�һ�Ĳ�Ʒ����
��ϵͳ���ṩ�Ĺ������������ľ����Ʒ������һ���򵥵Ķ���
���Ƕ��λ�ڲ�ͬ��Ʒ�ȼ��ṹ�����ڲ�ͬ���͵ľ����Ʒʱ��Ҫʹ�ó��󹤳�ģʽ��
���󹤳�ģʽ��������ʽ�Ĺ���ģʽ����Ϊ��������һ���Ե�һ����̬��

��һ�������ȼ��ṹ���Դ����������ڲ�ͬ��Ʒ�ȼ��ṹ��һ����Ʒ���е����ж���ʱ��
���󹤳�ģʽ�ȹ�������ģʽ��Ϊ�򵥡���Ч��

��һ����Ʒ���еĶ��������Ƴ�һ����ʱ�����ܹ���֤�ͻ���ʼ��ֻʹ��ͬһ����Ʒ���еĶ���
���һЩ��Ҫ���ݵ�ǰ��������������Ϊ�����ϵͳ��˵����һ�ַǳ�ʵ�õ����ģʽ��

�����µľ��幤���Ͳ�Ʒ��ܷ��㣬�����޸�����ϵͳ�����ϡ�����ԭ�򡱡�

ȱ�� 
1.������µĲ�Ʒ����ʱ��������չ���󹤳�������������Ĳ�Ʒ��������Ϊ�ڳ��󹤳���ɫ�й涨�����п��ܱ������Ĳ�Ʒ���ϣ�
Ҫ֧��������Ĳ�Ʒ����ζ��Ҫ�Ըýӿڽ�����չ�����⽫�漰���Գ��󹤳���ɫ��������������޸ģ���Ȼ������ϴ�Ĳ��㡣
2.����ԭ�����б�ԣ������µĹ��� ֻ��Ҫ�̳�һ�¾��� �Ͳ�Ʒ�����ף������µĲ�Ʒ�ȼ��ṹ�鷳����
*/

/*
�ԡ�����ԭ�򡱵�֧�ֳ�����б�ԡ�
1.���Ӳ�Ʒ�壺���������µĲ�Ʒ�壬��������ģʽ�ܺõ�֧���ˡ�����ԭ�򡱣�
���������ӵĲ�Ʒ�壬ֻ��Ҫ��Ӧ����һ���µľ��幤�����ɣ������д����������κ��޸ġ�
2.�����µĲ�Ʒ�ȼ��ṹ�����������µĲ�Ʒ�ȼ��ṹ����Ҫ�޸����еĹ�����ɫ���������󹤳��࣬
�����еĹ������ж���Ҫ���������²�Ʒ�ķ��������ܺܺõ�֧�֡�����ԭ�򡱡�

���󹤳�ģʽ�������������һ��ϵͳ��Ӧ�������ڲ�Ʒ��ʵ����α���������Ϻͱ���ϸ�ڣ�
ϵͳ���ж���һ���Ĳ�Ʒ�壬��ÿ��ֻʹ������ĳһ��Ʒ�壻����ͬһ����Ʒ��Ĳ�Ʒ����һ��ʹ�ã�
ϵͳ�ṩһ����Ʒ��Ŀ⣬���еĲ�Ʒ��ͬ���Ľӿڳ��֣��Ӷ�ʹ�ͻ��˲������ھ���ʵ�֣�������
*/
void test()
{
	//��ͬ�����ݿ����ͬ�����߼������õ���ָ���ǲ�һ���ģ����ö�̬�������Ա�����ײ�ĵ��ýӿڲ���
	shared_ptr<factory_base> fcbase(new SqlserverFactory);

	//�ͻ�����ʶ��ֻ��Iuser��Idepartment�������õ�ʲô���ݿ��ǲ�֪����
	fcbase->return_Iuser()->insert_user(User());
	fcbase->return_Iuser()->get_user(1);
	fcbase->return_Idepartment()->insert_department(Department());
	fcbase->return_Idepartment()->get_department(1);

	fcbase = shared_ptr<factory_base>(new AccessFactory); //�κ��û����붼����Ҫ�޸� ֻ��Ҫ��new AccessFactory����
	fcbase->return_Iuser()->insert_user(User());
	fcbase->return_Iuser()->get_user(1);
	fcbase->return_Idepartment()->insert_department(Department());
	fcbase->return_Idepartment()->get_department(1);

	//�����򵥹���ģʽ�Ľ��ĳ��󹤳�ģʽ������Ҫ�޸�new ������Ҫ�޸�Databasethis�ĳ�Ա����
	Databasethis.retfac()->return_Iuser()->insert_user(User());
	Databasethis.retfac()->return_Iuser()->get_user(1);
	Databasethis.retfac()->return_Idepartment()->insert_department(Department());
	Databasethis.retfac()->return_Idepartment()->get_department(1);

	//��������Ľ��ļ򵥹����ͳ��󹤳�ģʽ��� ����Ҫ�޸��κ�Դ���� ֻ��Ҫ�����������
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
//�����ģʽ

}