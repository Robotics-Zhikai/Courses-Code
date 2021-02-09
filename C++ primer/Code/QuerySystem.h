#pragma once
#include "main.h"
#include "StrVec.h"
namespace QuerySystem
{


using namespace std;

class QueryResult //�����ѯ�����������ѯ���ַ������ı����ݵ�ַ�������кŵĵ�ַ
{
	friend ostream& print(ostream& os, const QueryResult & que)
	{
		os << que.word << "����" << que.setptr->size() << "��" << endl;
		size_t countline = 0;
		for (auto it = que.setptr->begin(); it != que.setptr->end(); it++)
		{
			os << "(line " << countline++ << ") " << (*que.text)[*it] << endl;
		}
		return os;
	}
public:
	QueryResult() = default;
	QueryResult(const string& instr, shared_ptr<StrVec>& intextptr,
		shared_ptr<set<size_t>>& setptr) :word(instr), text(intextptr), setptr(setptr) {}
	set<size_t>::iterator begin()const { return setptr->begin(); }
	set<size_t>::iterator end()const { return setptr->end(); }
	shared_ptr<StrVec> get_file()const { return text; }
private:
	string word;
	shared_ptr<StrVec> text;
	shared_ptr<set<size_t>> setptr;
};


class TextQuery //һ��TextQuery����һ���ı���ӳ�䣬����˵Ӧ��Ҫ�󿽱����캯���Ϳ�����ֵ������delete��
{
public:
	using textptr = shared_ptr<StrVec>;
	using mappingptr = shared_ptr<map<string, set<size_t>>>; 

	TextQuery(const string& path):text(new StrVec)
	{
		ifstream file(path);
		readText(file);
	}
	QueryResult query(const string & word) //��ðѲ�ѯ���������const�������κθı����ݿ�ķ���
	{
		static shared_ptr<set<size_t>> nodata(new set<size_t>); //����Ǳ���ģ���Ϊshared ptr������ʱ�����delete������ʵ����new�ĳ��� 
		if (mapping.find(word) != mapping.end())
			return QueryResult(word, text, mapping[word]);//���ֻ��������Ļ�������map���Զ�����һ��empty��mappedtype �������set.sizeʱ����
		else
			return QueryResult(word, text, nodata);
	}

private:
	textptr text;
	//mappingptr mapping; //���������map�����ó�����ָ�������ô����ζ��TextQuery��ֻ�𵽶�ȡ���ݵ��ڲ������ã����𵽲�ѯ������
	//��ѯ��ѹ���·ŵ���QueryResult�У�û�кܺõĽ���
	//��������Ҫ��ѯ�ܶ൥�ʲ���ѯ��Ϻ�textquery���ͷţ��ڲ����е��ļ����ݶ�û�б��ͷţ���������set��Ϊ����ָ��ʱ��������ѡ����ͷ��ڴ棬������ڴ������ʡ�
	map<string, shared_ptr<set<size_t>>> mapping;


	void readText(ifstream & filein)
	{
		string word;
		string linestr;
		size_t countline = 0;
		while (getline(filein, linestr))
		{
			text->push_back(linestr);
			istringstream record(linestr);
			while (record >> word)
			{
				auto found = mapping.find(word);
				if (found == mapping.end()) //˵��û�ҵ�
					mapping[word] = shared_ptr<set<size_t>>(new set<size_t>);
				mapping[word]->insert(countline);//����mappedtype��set���ͣ����Բ�����ͬһ���ж���ظ�Ԫ�ص��ظ�����
			}
			countline++;
		}
	}
};


}

