#pragma once

#include <string>
#include <iostream>

class Employee
{
public:
	//Employee() = default; //����������øú���ʱ�ᱨ��Employee����ɾ����Ĭ�Ϲ��캯��
	Employee(const std::string& nameinput) :name(nameinput), ID(++ID_distribute) {}
	//�������õ���ʽ���Ա�����п�������
	Employee(Employee& emp) = delete;
private:
	std::string name;
	const long ID;//�����������Ա�Ĵ��ڣ��ϳɵ�Ĭ�Ϲ��캯����ɾ���� �ϳɵĿ�����ֵ����Ҳ��ɾ����
	//���ǺϳɵĿ������캯���Ǵ��ڵģ�����Ա����Ϣ����˵���ܱ��������죬���Ӧ����Ϊdelete
	static long ID_distribute;
};

long Employee::ID_distribute = 0;

