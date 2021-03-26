#pragma once

#include "main.h"

namespace MessageFolder
{
	using namespace std;
	
class Folder;
class Message
{
	friend void swap(Message& msg1, Message&msg2);
	friend class Folder;
public:
	explicit Message(const string& str = "") :contents(str) {}
	Message(const Message& msg):contents(msg.contents),folders(msg.folders)//拷贝时，拷贝副本和原始的将是不同的msg对象
		//如何保证Message在多个folder都同步变化呢？就是通过set<Folder*> folders来保证的
	{
		//经过初始化后，新建的Msg已经有了指向的folder，但是folder还没有该msg
		add_to_folders(*this);  //如果使用默认的拷贝构造函数，则不会调用add_to_folders，folders没有及时更新
	}
	Message(Message&& msg) :contents(std::move(msg.contents)),folders(std::move(msg.folders))
		//相当于剪切，把原始对象消除，移动到新对象
	{
		for (auto f : folders)
		{
			f->remMsg(&msg);
			f->addMsg(this);
		}
		msg.folders.clear(); //移后源对象不能对其值做任何假设，确保销毁msg是无害的，因为msg的析构函数遍历folders，希望能确定set是空的
	}

	Message& operator=(const Message& msg) //拷贝赋值时，原对象的所有内容都赋给了新对象，原对象不消失，新对象原来的内容消失
		//这个不用拷贝并交换技术来实现，如果用的话，首先会调用拷贝构造函数，把this add_to_folders
		//然后再调用swap的话，又需要调用this remove_from_Folders，多此一举
	{
		remove_from_Folders();//把当前的msg存在的文件都移走
		contents = msg.contents;
		folders = msg.folders;
		add_to_folders(msg);
		return *this;
	}
	Message& operator=(Message&& msg)//原对象销毁 新对象原来的内容消失
	{
		//之所以拷贝赋值函数没有检查自赋值（浪费资源的问题），是因为原对象不销毁，不存在两次remmsg的问题
		if (&msg != this) //直接检查自赋值情况，可以避免f->remMsg执行两次 浪费资源 虽然运行没问题
		{
			remove_from_Folders();//把当前的msg存在的文件都移走
			contents = std::move(msg.contents);
			folders = std::move(msg.folders);
			for (auto f : folders)
			{
				f->remMsg(&msg);
				f->addMsg(this);
			}
		}
		return *this;
	}
	~Message() //内容和folders会自动消除，但是folders引用的不会自动消除 只能是手动调用。
	{
		remove_from_Folders();
	}

	void save(Folder&);
	void remove(Folder&); //这是真正的remove file，不光message对file的引用被消除，file对message的引用也消除

	void print_debug();
private:
	string contents;
	set<Folder*> folders;

	void add_to_folders(const Message&);
	void remove_from_Folders();

	void addfldr(Folder *f) { folders.insert(f); } //只是把folder的指针加入进去了，并没有保证引用关系
	void remfldr(Folder *f) { folders.erase(f); }
};
void swap(Message& msg1, Message&msg2)
//swap想做的事情是：把contents交换，folders交换
//如果是默认的swap函数，那么需要拷贝set<Folder*>一次，然后两次赋值；拷贝构造的话很有可能动态分配内存，资源耗费巨大
//因此考虑自己定义一个swap,避免拷贝构造
{
	//for (auto m : msg1.folders) 
	//	m->remMsg(&msg1); //只有在folder也声明友元，才能这样调用 而事实上 下边的这两行可以代替相同的功能而不用在folder声明友元
	msg1.remove_from_Folders();
	msg2.remove_from_Folders(); //这两行语句把folders指向message的关系解耦了
	swap(msg1.contents, msg2.contents);
	swap(msg1.folders, msg2.folders);
	msg1.add_to_folders(msg1);
	msg2.add_to_folders(msg2);
	//这样的话swap就不会调用拷贝构造函数了 而只是调用关联容器的insert erase
}

void Message::save(Folder & f)
{
	addfldr(&f);
	f.addMsg(this);
}

void Message::remove(Folder& f)
{
	remfldr(&f);
	f.remMsg(this);
}

void Message::add_to_folders(const Message& msg) //将本msg this加入到msg指示的各folder中
{
	for (auto it = msg.folders.begin(); it != msg.folders.end(); it++)
		(*it)->addMsg(this);
}

void Message::remove_from_Folders()
{
	for (auto it = folders.begin(); it != folders.end(); it++)
		(*it)->remMsg(this);
}

void Folder::add_to_Message(const Folder& f)//将本Folder加入到f.msgs的folders中
{
	for (auto it = f.msgs.begin(); it != f.msgs.end(); it++)
		(*it)->addfldr(this);
}
void Folder::remove_from_message()
{
	for (auto it = msgs.begin(); it != msgs.end(); it++)
		(*it)->remfldr(this);
}
}
