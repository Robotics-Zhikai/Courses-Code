#pragma once
#include "main.h"
#include <iostream>
using namespace std;

std::string * psstore;
class HasPtr
{
public:
	~HasPtr()
	{
		psstore = ps;
		//delete ps;
	}
	HasPtr(int num) :ps(new std::string()), i(num) 
	{
		//std::cout << ps;
	}
	HasPtr(const std::string &s = std::string()) :ps(new std::string(s)), i(0) {}
	HasPtr(const HasPtr& has,int s=1) :ps(new std::string(*has.ps+"2")),i(has.i) {}
	//�������һ���µ�string��ַ��ps���õ�ַָ�򱻿�����has��string+"2"
	//������Ǳ������Լ��ϳɵĿ������캯���Ļ���ֻ�´��ps����Ҳ����˵���������µ�ps��ֵ����ȵ�
	//������ϲ���Ĭ��ֵ��int s����ô�Ͳ��ǿ������캯���ˣ���ֻ��ʹ�ñ������Լ��ṩ�Ŀ������캯����Ч���ǿ���ps����
	void pulicfuc() {};
	HasPtr& operator= (const HasPtr& hasptr) 
	{ 
		*ps = *hasptr.ps; //����ָ��Ķ��� ��������ps����
		//ps = hasptr.ps; //��������þ͸�Ĭ�ϺϳɵĿ�����ֵ�����һ����
		i = hasptr.i; 
		std::cout << "�ҵĿ�����ֵ����" << std::endl; 
		return *this; 
	}
	//void testmoren(std::string pss = *ps);//��ᱨ����Ϊ�Ǿ�̬���ݳ�Ա������ΪĬ�ϲ��� ����̬���ݳ�Ա������ΪĬ�ϲ���
	constexpr static double teststatic = 1.2; //
	static constexpr double teststatic2 = 2.1;//���δʵ��Ⱥ�˳�򲻻�Ӱ��
	constexpr static double teststatic3 = 2.1;//
	static const int teststatic4 = 1;//ֻ�����β�����static const�����β���ʼ��������Ҫ�����ڳ�ʼ������static constexpr
	static const double teststatic5;
	static const std::vector<double> teststatic6;
	static const double teststatic7;
	static double teststatic8;
private:
	std::string *ps;
	int i;
	
};
const std::vector<double> HasPtr::teststatic6(10);
//const double HasPtr::teststatic5 = 1.2;
const static double teststatic = 4 ;

//constexpr static double HasPtr::teststatic = 1.2;//����˴�����ָ���洢�� staticֻ���������ڲ����������
constexpr double HasPtr::teststatic;//�������ھ�̬���ݳ�Ա����ʼ���ˣ�ҲӦ�����ⲿ����һ�¸ó�Ա
//constexpr double HasPtr::teststatic = 1.5;//�������ڲ��Ѿ���ʼ�����ˣ��ᱨ���ض��壬��γ�ʼ����
//const double HasPtr::teststatic;// �����ڲ�����constexpr������ ��˲�����const����

const double HasPtr::teststatic7 = 3.5;
//constexpr double HasPtr::teststatic7 = 3; //�ڲ�����const�����ģ��ⲿ��������Ļ������ض��壬��ͬ�Ĵ洢��

double HasPtr::teststatic8;


class HasPtrValue //��Ϊ��ֵ����
{
	friend void swap(HasPtrValue & h1, HasPtrValue& h2); //��������ʱ���õ� ����Ч��
public:
	HasPtrValue(const string & s = string()) :ps(new string(s)), i(0) {}
	//�������ԣ������±ߵ�Ĭ�ϲ�����Ҳ�������������캯��
	HasPtrValue(const HasPtrValue& hpv,int test = 3) :ps(new string(*hpv.ps)), i(hpv.i+ test) {}//�����ð��֮���ʼ�����������ִ��Ĭ�ϳ�ʼ��
	//HasPtrValue& operator=(const HasPtrValue & hpv)
	//{
	//	*ps = *hpv.ps;
	//	i = hpv.i;
	//	cout << "HasPtrValue& operator=(const HasPtrValue & hpv)" << endl;
	//	return *this;
	//}
	HasPtrValue(HasPtrValue&& hpv)noexcept
		:ps(hpv.ps),i(hpv.i)
	{
		hpv.ps = nullptr;
	}
	HasPtrValue& operator=(HasPtrValue hpv) 
		//ֵ����һ����ʱ������Ȼ��swap ������Ȼ��Ƚ����ϱ�ע�͵����Ǹ�operator=����һ�ο������캯����
		//���Ƕ�����swap���������һ�δ�����ظ��ڶദ�õ����
	{
		swap( *this,hpv); //���������� ����
		cout << "HasPtrValue& operator=(HasPtrValue hpv)" << endl;
		return *this;
	}
	//HasPtrValue& operator=(HasPtrValue&& hpv) //������operatorͬʱ���ֵĻ��ᱨ��operator=����ȷ����Ϊ������ֵ�������ܵ���
	//{
	//	swap(*this, hpv);
	//	return *this;
	//}


	bool operator<(const HasPtrValue& h2)const
	{
		return *ps < *h2.ps;
	}
	~HasPtrValue()
	{
		delete ps;
	}
private:
	string * ps;
	int i;
};
void swap(HasPtrValue & h1, HasPtrValue& h2) //������Ĭ�����������һ��tempȻ�󿽱���������ڴ����ķѵ�ʱ��
{
	//using std::swap;
	swap(h1.ps, h2.ps);
	swap(h1.i, h2.i);
	cout << "swap(HasPtrValue & h1, HasPtrValue& h2)" << endl;
}

class HasPtrptr //��Ϊ��ָ����� �е�����ָ�����˼
{
public:

	explicit HasPtrptr(const string & s = string()) :ps(new string(s)), i(1) 
	{
		refcount[ps]++;
	}
	HasPtrptr(const HasPtrptr& hpp) //����һ��ָ����ͬ��ַ�Ķ���
	{
		refcount[hpp.ps]++;
		ps = hpp.ps;
		i = hpp.i;
	}
	HasPtrptr& operator=(const HasPtrptr & hpp)
	{
		refcount[hpp.ps]++;

		if (--refcount[ps] == 0)
		{
			auto it = refcount.find(ps);
			delete it->first;
			refcount.erase(it);
		}

		ps = hpp.ps;
		i = hpp.i;
		return *this;
	}
	~HasPtrptr()
	{
		if (--refcount[ps] == 0)
		{
			auto it = refcount.find(ps);
			delete it->first;
			refcount.erase(it);
		}		
	}

private:
	string * ps;
	int i;
	static map<string*,int> refcount ;//�����ַ����������ӳ�� Ҳ�������ü��� 
	//ʵ�����ö�̬�����ָ�����ʵ��ͬ����Ч��������ָ��ָ������ĸı���Էֲ�ʽ���� ���������ֵ�Ļ���������зֲ�ʽ����
};
map<string*, int> HasPtrptr::refcount ;
