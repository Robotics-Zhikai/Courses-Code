#pragma once
#include "main.h"
#include "StrVec.h"
namespace QuerySystem
{


using namespace std;

class QueryResult //储存查询结果，包含查询的字符串和文本数据地址和所处行号的地址
{
	friend ostream& print(ostream& os, const QueryResult & que)
	{
		os << que.word << "出现" << que.setptr->size() << "次" << endl;
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


class TextQuery //一个TextQuery管理一个文本及映射，按理说应该要求拷贝构造函数和拷贝赋值函数是delete的
{
public:
	using textptr = shared_ptr<StrVec>;
	using mappingptr = shared_ptr<map<string, set<size_t>>>; 

	TextQuery(const string& path):text(new StrVec)
	{
		ifstream file(path);
		readText(file);
	}
	QueryResult query(const string & word) //最好把查询函数定义成const，减少任何改变数据库的风险
	{
		static shared_ptr<set<size_t>> nodata(new set<size_t>); //这个是必须的，因为shared ptr在销毁时会调用delete，必须实现有new的出现 
		if (mapping.find(word) != mapping.end())
			return QueryResult(word, text, mapping[word]);//如果只有这个语句的话，索引map会自动生成一个empty的mappedtype 造成索引set.size时出错
		else
			return QueryResult(word, text, nodata);
	}

private:
	textptr text;
	//mappingptr mapping; //如果把整个map都设置成智能指针管理，那么就意味着TextQuery类只起到读取数据到内部的作用，不起到查询的作用
	//查询的压力下放到了QueryResult中，没有很好的解耦
	//当遇到需要查询很多单词并查询完毕后，textquery被释放，内部所有的文件内容都没有被释放，而当仅把set作为智能指针时，就是有选择的释放内存，提高了内存利用率。
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
				if (found == mapping.end()) //说明没找到
					mapping[word] = shared_ptr<set<size_t>>(new set<size_t>);
				mapping[word]->insert(countline);//由于mappedtype是set类型，所以不会有同一行有多个重复元素的重复计数
			}
			countline++;
		}
	}
};


}

