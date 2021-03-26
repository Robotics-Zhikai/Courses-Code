#pragma once
#include "main.h"
#include "Quote.h"
using namespace std;

class Basket
{
public:
	void add_item(const Quote& qt) //可以加入到不同打折类型的书籍
	{
		//items.insert(make_shared<Quote>(&qt)); //不能直接这样，因为如果是右值引用或者以后将要被销毁的变量，那么shareptr就指向了一个无效的地址
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
			size_t num = items.count(*it);//这个比较智能指针是否相同是通过compare比较的吗？
			sum += (*it)->net_prize(num); //这个是动态绑定调用虚函数
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