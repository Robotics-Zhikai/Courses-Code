#pragma once
#include "main.h"
#include "Quote.h"
using namespace std;

class Basket
{
public:
	void add_item(const Quote& qt) //���Լ��뵽��ͬ�������͵��鼮
	{
		//items.insert(make_shared<Quote>(&qt)); //����ֱ����������Ϊ�������ֵ���û����Ժ�Ҫ�����ٵı�������ôshareptr��ָ����һ����Ч�ĵ�ַ
		items.insert(shared_ptr<Quote>(qt.clone()));
	}
	void add_item(Quote&& qt)
	{
		items.insert(shared_ptr<Quote>(std::move(qt).clone()));
	}
	double total_receipt()
	{
		double sum = 0;
		for (auto it = items.cbegin(); it != items.cend(); it = items.upper_bound(*it))
		{
			cout << (*it)->ISBN() << endl;
			size_t num = items.count(*it);//����Ƚ�����ָ���Ƿ���ͬ��ͨ��compare�Ƚϵ���
			sum += (*it)->net_prize(num); //����Ƕ�̬�󶨵����麯��
		}
		return sum;
	}
private:
	static bool compare(const shared_ptr<Quote>&q1, const shared_ptr<Quote>&q2)
	{
		return q1->ISBN() < q2->ISBN();
	}
	multiset<shared_ptr<Quote>, decltype(compare)*> items{ compare };
};