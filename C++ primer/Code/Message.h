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
	Message(const Message& msg):contents(msg.contents),folders(msg.folders)//����ʱ������������ԭʼ�Ľ��ǲ�ͬ��msg����
		//��α�֤Message�ڶ��folder��ͬ���仯�أ�����ͨ��set<Folder*> folders����֤��
	{
		//������ʼ�����½���Msg�Ѿ�����ָ���folder������folder��û�и�msg
		add_to_folders(*this);  //���ʹ��Ĭ�ϵĿ������캯�����򲻻����add_to_folders��foldersû�м�ʱ����
	}
	Message(Message&& msg) :contents(std::move(msg.contents)),folders(std::move(msg.folders))
		//�൱�ڼ��У���ԭʼ�����������ƶ����¶���
	{
		for (auto f : folders)
		{
			f->remMsg(&msg);
			f->addMsg(this);
		}
		msg.folders.clear(); //�ƺ�Դ�����ܶ���ֵ���κμ��裬ȷ������msg���޺��ģ���Ϊmsg��������������folders��ϣ����ȷ��set�ǿյ�
	}

	Message& operator=(const Message& msg) //������ֵʱ��ԭ������������ݶ��������¶���ԭ������ʧ���¶���ԭ����������ʧ
		//������ÿ���������������ʵ�֣�����õĻ������Ȼ���ÿ������캯������this add_to_folders
		//Ȼ���ٵ���swap�Ļ�������Ҫ����this remove_from_Folders�����һ��
	{
		remove_from_Folders();//�ѵ�ǰ��msg���ڵ��ļ�������
		contents = msg.contents;
		folders = msg.folders;
		add_to_folders(msg);
		return *this;
	}
	Message& operator=(Message&& msg)//ԭ�������� �¶���ԭ����������ʧ
	{
		//֮���Կ�����ֵ����û�м���Ը�ֵ���˷���Դ�����⣩������Ϊԭ�������٣�����������remmsg������
		if (&msg != this) //ֱ�Ӽ���Ը�ֵ��������Ա���f->remMsgִ������ �˷���Դ ��Ȼ����û����
		{
			remove_from_Folders();//�ѵ�ǰ��msg���ڵ��ļ�������
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
	~Message() //���ݺ�folders���Զ�����������folders���õĲ����Զ����� ֻ�����ֶ����á�
	{
		remove_from_Folders();
	}

	void save(Folder&);
	void remove(Folder&); //����������remove file������message��file�����ñ�������file��message������Ҳ����

	void print_debug();
private:
	string contents;
	set<Folder*> folders;

	void add_to_folders(const Message&);
	void remove_from_Folders();

	void addfldr(Folder *f) { folders.insert(f); } //ֻ�ǰ�folder��ָ������ȥ�ˣ���û�б�֤���ù�ϵ
	void remfldr(Folder *f) { folders.erase(f); }
};
void swap(Message& msg1, Message&msg2)
//swap�����������ǣ���contents������folders����
//�����Ĭ�ϵ�swap��������ô��Ҫ����set<Folder*>һ�Σ�Ȼ�����θ�ֵ����������Ļ����п��ܶ�̬�����ڴ棬��Դ�ķѾ޴�
//��˿����Լ�����һ��swap,���⿽������
{
	//for (auto m : msg1.folders) 
	//	m->remMsg(&msg1); //ֻ����folderҲ������Ԫ�������������� ����ʵ�� �±ߵ������п��Դ�����ͬ�Ĺ��ܶ�������folder������Ԫ
	msg1.remove_from_Folders();
	msg2.remove_from_Folders(); //����������foldersָ��message�Ĺ�ϵ������
	swap(msg1.contents, msg2.contents);
	swap(msg1.folders, msg2.folders);
	msg1.add_to_folders(msg1);
	msg2.add_to_folders(msg2);
	//�����Ļ�swap�Ͳ�����ÿ������캯���� ��ֻ�ǵ��ù���������insert erase
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

void Message::add_to_folders(const Message& msg) //����msg this���뵽msgָʾ�ĸ�folder��
{
	for (auto it = msg.folders.begin(); it != msg.folders.end(); it++)
		(*it)->addMsg(this);
}

void Message::remove_from_Folders()
{
	for (auto it = folders.begin(); it != folders.end(); it++)
		(*it)->remMsg(this);
}

void Folder::add_to_Message(const Folder& f)//����Folder���뵽f.msgs��folders��
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
