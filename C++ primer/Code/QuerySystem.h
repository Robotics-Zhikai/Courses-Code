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
			os << "(line " << *it << ") " << (*que.text)[*it] << endl;
		}
		return os;
	}
public:
	QueryResult() = default;
	QueryResult(const string& instr,const shared_ptr<StrVec>& intextptr,
		const shared_ptr<set<size_t>>& setptr) :word(instr), text(intextptr), setptr(setptr) {}
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
	QueryResult query(const string & word)const //最好把查询函数定义成const，减少任何改变数据库的风险
	{
		static shared_ptr<set<size_t>> nodata(new set<size_t>); //这个是必须的，因为shared ptr在销毁时会调用delete，必须实现有new的出现 
		auto found = mapping.find(word);
		if (found != mapping.end())
			return QueryResult(word, text, found->second /*mapping[word]*/);//如果只有这个语句的话，索引map会自动生成一个empty的mappedtype 造成索引set.size时出错
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

class Query;
class Query_base //不希望用户或派生类直接使用Query_base，因此所有的成员都非public
{
	friend Query;
protected:
	virtual ~Query_base() = default; //但这个作为根节点要用到虚析构函数，因此是protected成员
private:
	virtual QueryResult eval(const TextQuery& t)const = 0;
	virtual string rep()const = 0; //如果去掉virtual的话会报错仅虚拟函数允许纯说明符

};

class WordQuery:public Query_base
{
	friend class Query;
private:
	WordQuery(const string & s) :word(s) { }
	QueryResult eval(const TextQuery& t)const override { return t.query(word); }
	string rep()const override { return "(" + word + ")"; }

	string word;

	static int wr;
	static void sad() //静态成员不能有this指针 因此也就不能调用非静态成员了
	{
		wr = 3;
		//word = string("saf"); //报错非静态成员必须与特定对象相对 这也就是没有this的问题
		//string asg = word;  //报错同上
	}
};

class Query
{
	friend Query operator~(const Query & qe);
	friend Query operator&(const Query & l, const Query& r);
	friend Query operator|(const Query & l, const Query& r);
public:
	Query(const string &str) :q(new WordQuery(str)) {};
	QueryResult eval(const TextQuery& t)const { return q->eval(t); }
	string rep()const { return q->rep(); }
	~Query() { cout << "~Query()" << endl; };
private:
	Query(const shared_ptr<Query_base>& q) :q(q) {} //不希望用户代码能随便定义Query
	shared_ptr<Query_base> q;
};

class HistoryQuery//构建一个历史库
{
public:
	void addQuery(const Query& qe)
	{
		his_database.push_back(qe);
	}
	Query& operator[](size_t n)
	{
		return his_database[n];
	}
	size_t size()const { return his_database.size(); }
private:
	vector<Query> his_database;
};

class NotQuery :public Query_base
{
	friend Query operator~(const Query & qe);
private:
	NotQuery(const Query& qe) :qe(qe) {}

	string rep()const override
	{
		return "~(" + qe.rep() + ")";
	}
	QueryResult eval(const TextQuery& t)const override
	{
		QueryResult tmpresult = qe.eval(t);
		auto text = tmpresult.get_file();
		set<size_t>::iterator beg = tmpresult.begin();
		set<size_t>::iterator end = tmpresult.end();
		set<size_t> set_not;
		for (size_t i = 0; i < text->size(); i++)
		{
			if (beg!=end && i == *beg)
				++beg;
			else
				set_not.insert(i);
		}
		return QueryResult(rep(),text,shared_ptr<set<size_t>>(new set<size_t>(std::move(set_not))));
	}

	Query qe;
};

class BinaryQuery:public Query_base
{
protected: //若定义为private，那么此类的派生类不能调用rep(),而由于没有重写eval，则此类还是抽象基类，不能随便让用户定义成员，因此是protected类
	BinaryQuery(const Query& l, const Query& r, string o) :lhs(l), rhs(r), opSym(o) {}
	string rep()const override { return "(" +lhs.rep()+" " + opSym + " " + rhs.rep()+")"; }

	Query lhs, rhs;
	string opSym; // 指明二元运算的符号
};

class AndQuery :public BinaryQuery
{
	friend Query operator&(const Query & l, const Query& r);
private:
	AndQuery(const Query& l, const Query& r):BinaryQuery(l,r,"&") {}
	QueryResult eval(const TextQuery& t)const override
	{
		QueryResult lhsresult = lhs.eval(t);
		QueryResult rhsresult = rhs.eval(t);
		set<size_t> setresult;
		set_intersection(lhsresult.begin(), lhsresult.end(), rhsresult.begin(), rhsresult.end(), inserter(setresult, setresult.begin()));
		return QueryResult(rep(), lhsresult.get_file(), shared_ptr<set<size_t>>(new set<size_t>(std::move(setresult))));
	}
};

class OrQuery :public BinaryQuery
{
	friend Query operator|(const Query & l, const Query& r);
private:
	OrQuery(const Query&l, const Query&r) :BinaryQuery(l, r, "|") {}
	QueryResult eval(const TextQuery& t)const override
	{
		QueryResult lhsresult = lhs.eval(t);
		QueryResult rhsresult = rhs.eval(t);
		set<size_t> setresult(lhsresult.begin(), lhsresult.end());
		setresult.insert(rhsresult.begin(), rhsresult.end());
		return QueryResult(rep(), lhsresult.get_file(), shared_ptr<set<size_t>>(new set<size_t>(std::move(setresult))));
	}
};

Query operator~(const Query & qe)
{
	shared_ptr<Query_base> p(new NotQuery(qe));
	return Query(p);
}

Query operator&(const Query & l,const Query& r)
{
	return Query(shared_ptr<Query_base>(new AndQuery(l, r)));
}

Query operator|(const Query & l, const Query& r)
{
	return Query(shared_ptr<Query_base>(new OrQuery(l, r)));
}

}

