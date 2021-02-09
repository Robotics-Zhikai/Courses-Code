#pragma once
#include "main.h"
using namespace std;
class Quote
{
	friend double print_total(ostream &os, const Quote& item, size_t n);
public:
	Quote() = default;
	Quote(const string& bn, double pr) :bookNo(bn),prize(pr){}
	string ISBN()const { return bookNo; } //ISBN���ܱ��ı�

	virtual ~Quote() = default; // ���ڵ㶨��һ������������
	virtual double net_prize(size_t n)const //��ζ����������Ҫoverride������� ��Ϊ�������п��ܸı���prize��ֵ
	{
		return prize * n;
	}
private:
	string bookNo;
protected:
	double prize = 0.0;
};

//���㲢��ӡ���۸���������ĳ���鼮���÷���
double print_total(ostream &os, const Quote& item, size_t n)
//�����ʵ����friendҲ����,��Ϊ���ǵ��õ�public��װ
{
	os << item.ISBN() << endl;
	double ret = item.net_prize(n); //���ݶ�̬���������ã�item������Quote���࣬Ҳ������������������
	os << "������" << n << "���ļ۸���" << ret << endl;
	return ret;
}

class Bulk_quote:public Quote
{
	friend double print_total(ostream &os, const Quote& item, size_t n);
public:
	Bulk_quote() = default;
	Bulk_quote(const string & str, double pri,size_t minct,double dc ):Quote(str,pri),min_qty(minct),discount(dc){}
	double net_prize(size_t n,double n1)const //��������ĵ�����������ĳ�Ա����
	{
		if (n >= min_qty)
			return n*prize*(1 - discount);
		else
			return n*prize;
	}
	double net_prize(size_t n/*,int sfag*/)const override 
		//override���Ǳ���ģ���������������ȫ��ͬ�Ҳ���override����ôĬ���Ǹ����˻����ͬ��������
		//���ǵ�����д��ʱ������д�����ϱߵ�������ڲ���������Ҫ���ǵĻ����麯����ƥ�䣬��ô����override����ܱ���
		//ָ������д����
	{
		if (n >= min_qty)
			return n*prize*(1 - discount);
		else
			return n*prize;
	}
private:
	size_t min_qty = 0; //ʹ���ۿ����ߵ���͹�����
	double discount = 0.0; //��С����ʾ���ۿ۶�
};

class Strategy_ex15_7 :public Quote
{
public:
	Strategy_ex15_7() = default;
	Strategy_ex15_7(const string& bookname, double bookprize, size_t numup, double dc)
		:Quote(bookname, bookprize), numupper(numup), discount(dc) {}
	double net_prize(size_t n) const override
	{
		if (n <= numupper)
			return n*prize*(1 - discount);
		else
			return numupper*prize*(1 - discount) + (n - numupper)*prize;
	}
private:
	size_t numupper = 0; // ����һ�����Ͻ磬�������Ͻ�ʱ�������ְ�ԭ�����ۣ������������
	double discount = 0.0; // �����ۿ۶�

};

class test :public Strategy_ex15_7 //��Ӽ̳�Quote�����Է��������Ӷ�����Ӷ�����Ӷ����public��protect����
{
public:
	double teteag = prize;
	string adhADSF = ISBN();
	test(const string& bookname, double bookprize, size_t numup, double dc)
		:Strategy_ex15_7(bookname, bookprize, numup, dc) {}
	//test()
	//	:Quote("g",4) {}//��������ʹ�ü�ӷ��������ࡣ
	//�������ŵ����һ����ֻ��Ҫ��֤����������Ĺ����ϵ���Ϳ��԰�
	//�����̳������๹����� Ҳ����ֱ�ӵ���ֱ�Ӽ̳еĹ��캯��
};