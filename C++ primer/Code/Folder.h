#pragma once

#include "main.h"

namespace MessageFolder
{

using namespace std;

class Message; //必须在这里声明，否则友元是不管用的
class Folder
{
	friend void swap(Message& msg1, Message&msg2);
	friend void swap(Folder&, Folder&);
	friend Message; //友元不是相互的。你是我的朋友，但是我不一定是你的朋友 独立单向的控制
public:
	Folder() = default;
	Folder(const Folder& f):msgs(f.msgs) //新建了一个folder，然后把该folder添加到msgs中去
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

	void add_to_Message(const Folder& f);//将本Folder加入到f.msgs的folders中
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