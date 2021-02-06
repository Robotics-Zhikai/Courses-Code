#pragma once

#include "main.h"

namespace MessageFolder
{

using namespace std;

class Message; //����������������������Ԫ�ǲ����õ�
class Folder
{
	friend void swap(Message& msg1, Message&msg2);
	friend void swap(Folder&, Folder&);
	friend Message; //��Ԫ�����໥�ġ������ҵ����ѣ������Ҳ�һ����������� ��������Ŀ���
public:
	Folder() = default;
	Folder(const Folder& f):msgs(f.msgs) //�½���һ��folder��Ȼ��Ѹ�folder��ӵ�msgs��ȥ
	{
		add_to_Message(*this);
	}
	Folder& operator=(const Folder& f)
	{
		remove_from_message();
		msgs = f.msgs;
		add_to_Message(f);
		return *this;
	}
	~Folder()
	{
		remove_from_message();
	}

	void print_debug();
private:
	set<Message*> msgs;

	void add_to_Message(const Folder& f);//����Folder���뵽f.msgs��folders��
	void remove_from_message();
	

	void addMsg(Message * m) { msgs.insert(m); }
	void remMsg(Message * m) { msgs.erase(m); }

};
void swap(Folder& f1, Folder& f2)
{
	f1.remove_from_message();
	f2.remove_from_message();
	swap(f1.msgs, f2.msgs);
	f1.add_to_Message(f1);
	f2.add_to_Message(f2);
}


}