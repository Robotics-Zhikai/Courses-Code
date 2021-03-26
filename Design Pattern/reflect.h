#pragma once
#include "main.h"
/*

���䣺���ͨ����������ַ�����������Ķ��󣬱�����һ����classA����ô���ͨ���������ַ���"classA"��������Ķ���
������ָ���������������ʱ���Է��ʡ������޸�������״̬����Ϊ��һ������
ʵ���Ϸ�����ǳ���������ʱ��̬��ȡ������Ϣ�Լ����ö��󷽷�������
ͨ�������ƴ���������Ƿ���Ĺ���֮һ

��MATLAB�з����ǲ����൱��eval��
*/

//https://blog.csdn.net/K346K346/article/details/51698184
typedef void * (*PTRCreateObject)();
//����һ������ָ�룬����ָ�򴴽���ʵ���Ļص�����

class Reflect
{
private:
	map<string, PTRCreateObject> m_classMap;
	Reflect() = default;
public:
	PTRCreateObject getClassByName(string ClassName) //Ӧ���ǵ���map�õ�string��Ӧ�ĺ���ָ��
	{
		map<string, PTRCreateObject>::iterator it ;
		it = m_classMap.find(ClassName);
		if (it == m_classMap.end())
			return NULL;
		else
			return it->second;
	}
	void registClass(string name, PTRCreateObject method)//���������������ַ����Ͷ�Ӧ�Ĵ��������ĺ������浽map��
	{
		m_classMap.insert({ name,method });
	}
	
	static Reflect& getInstance()//��ȡ������ĵ���ʵ������
	{
		static Reflect slo_factory;
		return slo_factory;
	}
};

//ע�ᶯ����
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

//��abstract_factory���õ��˷������