#pragma once
#include "main.h"
namespace singleton_pattern
{
//����ģʽ
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
����ϵͳ�е�ĳЩ����˵��ֻ��һ��ʵ������Ҫ�����磬һ��ϵͳ�п��Դ��ڶ����ӡ���񣬵���ֻ����һ�����ڹ���������
һ��ϵͳֻ����һ�����ڹ��������ļ�ϵͳ��һ��ϵͳֻ����һ����ʱ���߻�ID����ţ���������

��α�֤һ����ֻ��һ��ʵ�������ʵ�����ڱ�����(���Ū��static��)
һ�����õĽ���취�����������𱣴�����Ψһʵ�����������Ա�֤û������ʵ���������������������ṩһ�����ʸ�ʵ���ķ���

����ģʽ���壺����ģʽȷ��ĳһ����ֻ��һ��ʵ������������ʵ������������ϵͳ�ṩ���ʵ����������Ϊ�����࣬�ṩȫ�ַ��ʵķ���

��������˽�й��캯����ȷ���û��޷�ͨ��new�ؼ���ֱ��ʵ������

�ŵ㣺
����ɱ���Ŀ��ʵ�������Ի��ڵ���ģʽ������չ�������ڵ����������Ƶķ��������ָ�������Ķ���ʵ��

ȱ�㣺
1�����ڵ���ģʽû�г���㣬��˵��������չ�кܴ������
2���뵥һְ��ԭ���ͻ��һ����Ӧ��ֻ�����ڲ��߼�����������������ô����ʵ����
�ҵ�����ȳ䵱�˹�����ɫ���ṩ�˹���������ͬʱ�ֳ䵱�˲�Ʒ��ɫ������һЩҵ�񷽷���
����Ʒ�Ĵ�����get_instance���Ͳ�Ʒ����Ĺ��ܣ�showmessage���ں���һ��
3�����ںܶ�����������ԣ�Java��C#)�����л������ṩ���Զ��������յļ�������ˣ����ʵ�����Ķ���ʱ�䲻������
ϵͳ����Ϊ�������������Զ����ٲ�������Դ���´�����ʱ�ֽ�����ʵ�������⽫���¶���״̬�Ķ�ʧ
*/
void test()
{
	singleton s = *singleton::get_instance(4,6); //Ĭ�ϺϳɵĿ������캯����public�ģ���˿����������õõ�һ��s
	s.get_instance(3, 5)->showmessage();//����ı���3,5 ���ڵ���ģʽ�Ĺ������ԣ����ǲ���ı����ֵ�
	singleton::get_instance(5,6)->showmessage();
	singleton::get_instance()->showmessage();
}

//https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/singleton.html
//���漰������������ģʽ ��ʱ��û��ѧ�� �����漰������ϵͳ��֪ʶ



}//end singleton_pattern