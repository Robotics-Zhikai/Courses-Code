#include <iostream>
#include "main.h"
#include "another.h"
#include "CLASS.h"
#include "Sales_data.h"
#include "CP5_ex7_53.h"
#include "Account.h"
#include "StrBlob.h"
#include "CP5_ex13_18.h"
#include "HasPtr.h"
#include "QuerySystem.h"
#include "Folder.h"
#include "Message.h"
#include "StrVec.h"
#include "String.h"
#include "Quote.h"
#include "CP5_ex15_20.h"
#include "CP5_ex15_21_22.h"
#include "Basket.h"
using namespace std;

class Y;
class X
{
public:
	Y & Yreference(Y tmpY);
	X * dataX;
	int d;
	X & s;
};

class Y
{
	Y(X xdata)
	{
		Ydata = xdata.d;
	}
	int Ydata;
};



class screen
{
public:
	screen() = default; //加上这个即便有其他构造函数也可以进行编译器自己合成的默认构造函数
	screen(int data1 = 5) :data(data1){} //在有screen()=default存在的情况下 直接声明时会不知道到底该调用哪个函数
	//同时有这两个构造函数的写法是非常错误的，因为默认构造函数不知道到底该调哪个 但是编译通过是没有问题的

	screen(int s,int s1) :data(1), DATA2(3), DATA1(DATA2) {} //这样初始化是错误的，由于成员初始化是有顺序的
	//这样初始化的话，会出现变量内存储的是垃圾值
	screen & display()
	{
		return *this;
	}
	screen(std::istream & is) { int x; is >> x; }
	//const screen & display() const { return *this; }
	screen display() const { return *this; }
	void changedata(int s) { data=s ; }
	int data = 6;
private:
	int DATA1 = 7;
	int DATA2 = 8;
};



namespace Exercise
{
	int ivar = 0;
	double dvar = 0;
	const int limit = 1000;
	float cout = 0;
}
namespace Exercise1
{
	int ivar = 1;
	double dvar = 1;
	const int limit = 10001;
	float cout = 1;
}

int ivar = 10;

int inlinetest()
{
	return 2;
}

void NotThrow() throw()
{
	throw exception("sss") ;
}

int & CastConstZK(const int &tmp)
{
	return const_cast<int &>(tmp); //把const属性抛掉了，不推荐，因为入口的参数应该是不可改变的
}



typedef string Type;
Type initVal();
class Excercise 
{
public :
	//typedef double Type; //如果加在这里的话就可以隐藏了外层的typedef
	Type SetVal(Type parm);
	Type initVal() { ::Type s; return s; };
private:
	int val;
	typedef double Type;
	Type nima; //重定义名字 是不允许的 但是编译器不会报错 
};

//Excercise::Type initVal() //加在这里的话由于Type是private 不可访问 但是如果再加一个Excercise限定符 就可以访问了
//{
//
//}

Type Excercise::SetVal(::Type parm) //Type是string类型的
{
	::Type str; //必须得加全局域，否则与类中的函数是不兼容的。
	initVal();
	return str;
}

typedef int Notclass;
void func()
{
	//在普通函数里 这样是合法的 但是在类中 类似这样的用法会由于类先处理声明再处理定义的特殊设定而导致非法。
	Notclass s1 = 2;
	typedef double Notclass;
	Notclass s = 1;
}


class Nodefault
{
public:
	~Nodefault() { cout << "析构" << endl; }
	Nodefault(const std::string &);
	Nodefault(int i):data(i){ cout << "执行实际干活的函数了" << endl; };
	Nodefault() :Nodefault(1) { cout << "执行到委托构造函数了" << endl; } //委托构造函数
	const Nodefault & returnRef(const Nodefault& constref) { return constref; }
	Nodefault & returnrefnotconst(Nodefault & ref) { return ref; }
	int data;
};
class C
{
public:
	C() : data() {}
//public:
private:
	Nodefault data;
};

namespace n13_13
{
	struct X {
		X() { std::cout << "X()" << std::endl; }
		X(const X&) { std::cout << "X(const X&)" << std::endl; }
		X& operator=(const X&) { std::cout << "X& operator=(const X&)" << std::endl; return *this; }
		~X() { std::cout << "~X()" << std::endl; }
	};

	void f(const X &rx, X x)
	{
		std::vector<X> vec;
		vec.reserve(2);
		vec.push_back(rx);
		vec.push_back(x);
	}
}

namespace ex2_33
{
	int i = 0, &r = i;
	auto a = r;
	const int ci = i, &cr = ci;
	auto b = ci;
	auto c = cr;//忽略顶层const
	auto d = &i;
	auto e = &ci;//保留底层const
	const auto f = ci;//忽略顶层const 需要显示的加上const
	auto &g = ci;
	//auto &h = 42;//右边这是一个顶层const auto会忽略掉顶层const 这个会报错

	const int i35 = 42;
	auto j = i35; const auto &k = i35; auto *p = &i35;
	const auto j2 = i35, &k2 = i35;

	decltype (i35) s = 0;//如果使用的表达式是一个变量，则decltype返回该变量的类型（包括顶层const和引用在内）
	//引用从来都是作为其所指对象的同义词出现，只有decltype例外
	const int &iii = 2;
	decltype (iii) sad = i35;
}

namespace ex2_36
{
	int a = 3, b = 4;
	decltype(a) c = a;
	decltype((b)) d = a;

	int a1 = 5;
	int a2 = a1;//初始化变量在编译期就能够确定值了。

	auto a3 = a1;
	decltype(a1) a4 = a1; //这两个类型一样

	const int a5 = 6;
	auto a6 = a5;
	decltype(a5) a7 = a5; //这两个类型就不一样了
}

namespace ex6_54
{
	int fucplus(int a, int b) { return a + b; }
	int fucminus(int a, int b) { return a - b; }
	int fucmultiple(int a, int b) { return a*b; }
	int fucdivid(int a, int b) { return a / b; }

	vector<int(*)(int, int)> vec_pfuc = {fucplus,fucminus,fucmultiple,fucdivid};
	void testvec_pfuc(int a,int b)
	{
		for (int i = 0; i < vec_pfuc.size(); i++)
		{
			std::cout << vec_pfuc[i](a, b) << std::endl; 
			//比如用在足式机器人步态序列的生成上，核心就是一行步态生成代码，但是有其他的循环调用语句
			//如果想要弄不同的步态，如果不是像现在这样调用的话需要写好几个重复度很高的函数
			//但是有了这个方法，就可以极大地简化代码量。
		}
	}
	decltype(testvec_pfuc(1,2)) s();//decltype返回假定调用这个函数后返回的类型
	decltype(testvec_pfuc) s1;//decltype返回函数名对应的函数类型，其实本质上是返回一个变量的类型
	//s1 为void(int,int)类型的函数类型
	using Fp = int(*)(int, int);
	using F = int(int, int);

	Fp returnP2fuc(F fucname)//自动转化为函数指针
	{
		return fucname; //返回一个函数指针 不能返回函数类型
	}
	int(*sss(F fucname))(int, int) //这样是可以的
	{
		return fucname;
	}
	//int(*)(int, int) ssss(F fucname) //这样会报错
	//{

	//}
	int useFp(F fucname,int a,int b)
	{
		return returnP2fuc(fucname)(a, b);
	}
}

namespace ex12_2
{
	vector<int>ss(10);
	//ss[1] = 3; //这样是会报错的 不能在namespace中赋值
}

namespace ex12_6
{
	vector<int> *returnvecint()
	{
		return new vector<int>;
	}
	shared_ptr<vector<int>> returnvecintIntelligence()
	{
		//return new vector<int>;//不能用new初始化，因为不存在对应的构造函数
		//这是因为智能指针对于内置指针类型是explicit的，不能作为拷贝构造函数返回
		return make_shared<vector<int>>();
	}

	vector <int> *readinput(vector<int> * input)
	{
		cout << "输入10个int数据" << endl;
		for (int i = 0; i < 10; i++)
		{
			int tmp;
			cin >> tmp;
			input->push_back(tmp);
		}
		return input;
	}
	void readinputIntelligence(const shared_ptr<vector<int>> & input)
	{
		cout << "输入10个int数据" << endl;
		for (int i = 0; i < 10; i++)
		{
			int tmp;
			cin >> tmp;
			input->push_back(tmp);
		}
	}

	void printvecint(vector<int> * &input)//这样可以作为指针的引用直接修改指针的值
	{
		for (int i = 0; i < input->size(); i++)
		{
			cout << (*input)[i] << " ";
		}
		delete input;
		input = nullptr; 
	}
	void printvecintIntelligence(shared_ptr<vector<int>> & input)
	{
		for (int i = 0; i < input->size(); i++)
		{
			cout << (*input)[i] << " ";
		}
		input = make_shared<vector<int>>(); //指向了一个临时对象 这样的话就把分配的内存释放自由了
	}
}

namespace ex10_9
{
	template<typename Ty>
	void ElimDups(vector<Ty>& input) //消除重复数据
	{
		sort(input.begin(), input.end());//如果去掉此行的话unique不能很好的去除
		auto unieued = unique(input.begin(), input.end());
		input.erase(unieued, input.cend());
	}
}

namespace section10_3_1
{
	bool Isshorter(const string&s1, const string &s2)
	{
		if (s1.size() < s2.size())
			return 1;
		else
			return 0;
	}

	bool compareIsbn(const Sales_data&s1, const Sales_data&s2)
	{
		return s1.isbn().size() > s2.isbn().size();
	}

	Sales_data d1("aa"), d2("aaaa"), d3("aaa"), d4("z"), d5("aaaaz");
	std::vector<Sales_data> Sales_datav{ d1, d2, d3, d4, d5 };

	auto ex10_13 = std::vector<std::string>{ "aaaaassaa","a", "as", "aaaaaabba","aasss",  "aaa" };
	bool IsMore5(const string&s1)
	{
		if (s1.size() > 5)
			return 1;
		else
			return 0;
	}
}

namespace section10_3_2
{
	
	auto f_ex10_14 = 
		[](int a, int b)
	{
		return a + b; 
	};

	auto ex10_15(int a)
	{
		return [a](int b) {return a + b; };
	}

	void biggies(vector<string>& words,vector<string>::size_type sz)//求大于等于一个给定长度的单词有多少,并打印
	{
		sort(words.begin(), words.end(), [](const string& s1, const string& s2) {return s1.size() < s2.size(); });//按长度排序
		auto ele = find_if(words.begin(), words.end(), [sz](const string&s1) {return s1.size() > sz ? 1 : 0; });
		auto count = words.end() - ele;
		cout << "大于" << sz << "长度的单词有" << count << "个,分别是" << endl;
		for_each(ele, words.end(), [](const string&s1) {cout << s1 << " "; });
	}

	void ex10_21(int in)
	{
		auto f = [&in]()->bool //如果不指明bool，会推断出函数的返回类型是int lambda
		{
			if (in > 0)
				in--;
			else if (in < 0)
				in++;
			else
				return 1;
			return 0;
		};

		while (!f())
			cout << in << endl;;
	}


	
	
}

namespace sec10_3_4
{
	using namespace std::placeholders;
	bool Iflowerthansz(const string& s1, int sz)
	{
		if (s1.size() <= sz)
			return 1;
		else
			return 0;
	}

	size_t CheckSizeLambda(const vector<int> & num, const string& s1) //返回索引值
	{
		return find_if(num.begin(), num.end(), [&s1](int n) {return s1.size() < n; })-num.begin(); //lambda是一个可调用对象

	}

	bool CheckSize(string::size_type num, const string& s1)
	{
		if (s1.size() < num)
			return 1;
		else
			return 0;
	}
	size_t CheckSizeBind(const vector<int>&num, const string&s1) //返回索引值
	{
		return find_if(num.begin(), num.end(), bind(CheckSize, _1, s1)) - num.begin();
	}

	//void foreach_exe(string::size_type num, const string& s1)
	//{
	//	if (!CheckSize(num, s1))
	//		cout << s1 << endl;
	//}
	void biggies(const vector<string>& str, string::size_type num) //求大于等于一个给定长度的单词有多少,并打印
	{
		auto foreach_exe = [&](const string& s1)
		{
			if (!CheckSize(num, s1))
				cout << s1 << endl;
		};
		for_each(str.begin(), str.end(), foreach_exe); //用lambda可调用对象
		//for_each(str.begin(), str.end(), bind(foreach_exe, num, _1)); //bind生成一个可调用对象  注释掉的这两个可以组合着用
	}
}

namespace sec10_4_1
{
	std::vector<int> vec{ 1, 1, 3, 3, 5, 5, 7, 7, 9 };
	std::list<int> lst;

	vector<int> ex10_28 = { 1,2,3,4,5,6,7,8,9 };
	vector<int> insertercopy, backcopy, frontcopy;
}

namespace ex12_26
{
	void input_string_output_reversestring(size_t n)
	{
		string * newtest = new string[n]();//只能是空括号，不能是其他
		*(newtest + 3) = string("gtr");

		allocator<string> tmpalloc;
		string * beginptr = tmpalloc.allocate(n);
		//*beginptr = string("asdasf"); //在这里会运行时报错，因为从来没有construct过。
		string * q = beginptr;
		string s;
		for (int i = 0; i < n; i++)
		{
			cin >> s;
			tmpalloc.construct(beginptr++,s);
		}
		while (q != beginptr)
		{
			cout << *(--beginptr) << endl;
			tmpalloc.destroy(beginptr);
		}
		*(beginptr+3) = string("asdasf"); //在这里就不会报错，因为已经construct了一次了
	}
}

namespace sec11_1
{
	vector<string> testdata = { "example.","example,","Example","gasdas","example","example","examp,le" };
	void CountString(const vector<string> &input) //单词计数程序
	{
		map<string, size_t> record;
		for (size_t i = 0; i < input.size(); i++)
			record[input[i]]++;
		for (auto &r : record)
		{
			cout << r.first << "单词出现" << r.second << "次" << endl;		
		}
	}

	void CountStringIgnorePuc(vector<string> input)
	{
		map<string, size_t> record;
		char * tmpstring = NULL;
		for (size_t i = 0; i < input.size(); i++)
		{
			tmpstring = new char[input[i].size()+1];
			size_t cout = 0;
			for (auto c : input[i])
			{
				if (ispunct(c))
					continue;
				if (isupper(c))
					c = tolower(c);
				*(tmpstring + (cout++)) = c;
			}
			*(tmpstring + cout) = '\0';
			input[i] = string(tmpstring);
			delete[]tmpstring;
			record[input[i]]++;
		}
			
	}
}

namespace sec11_2_1
{
	void ex_11_7_add_family(map<string, vector<string>>& families,const string& firstname,const vector<string>& lastnames )
	{
		//auto sda = families[firstname]; //只要调用这个方括号，就自动把字符串加入到了map中
		//sda.empty();
		if (!families[firstname].empty())
			for (size_t i = 0; i < lastnames.size(); i++)
				families[firstname].push_back(lastnames[i]);
		else
			families[firstname] = lastnames;
	}
	void printfamilies(const map<string, vector<string>>& families)
	{
		for (auto it = families.begin(); it != families.end(); it++) //map可以用双向迭代器访问
		{
			cout << it->first << endl;
			for (const auto &c : it->second)
				cout << c << " ";
			cout << endl;
		}
	}
	void test_ex_11_7()
	{
		map<string, vector<string>> families;
		ex_11_7_add_family(families, "wang", vector<string>{"zk", "s", "xk"});
		ex_11_7_add_family(families, "wang", vector<string>{"zk1", "s1", "xk1"});
		ex_11_7_add_family(families, "zhang", vector<string>{"zk1", "s1", "xk1"}); 
		ex_11_7_add_family(families, "zhang", vector<string>{"zk11", "s11", "xk11"});
		printfamilies(families);
	}
	void unique_vector_str(vector<string>& input)
	{
		sort(input.begin(), input.end()); //用标准库算法进行去重
		auto uniqueptr = unique(input.begin(), input.end());
		input.erase(uniqueptr, input.end());
	}
}

namespace sec11_2_2
{
	map<vector<int>::iterator, int> sasf;
	
	map<list<int>::iterator, int> agsg;
	list<int> test1122 = { 1,2,3,4 };
	vector<int> vectest1122 = { 1,2,3,4 };
	bool compisbn(const Sales_data& s1, const Sales_data& s2)
	{
		return s1.isbn() < s2.isbn();
	}
}

namespace sec_11_2_3
{
	vector <pair<string,int>> combiPair(const vector<string>& s, const vector<int>& integar)
	{
		vector <pair<string, int>> result;
		for (size_t i = 0; i < s.size(); i++)
			result.push_back({ s[i],integar[i] });
		return result;
	}
	class families //有点像数据库的意思了 主键和数据
	{
		using Datatype = map<string,vector<pair<string, string>>>;
	public:
		void addchild(const string& firstname, const string& lastname, const string& birthday)
		{
			Data[firstname].push_back(make_pair(lastname, birthday));
		}
		void printData()const
		{
			for (auto const & s : Data)
			{
				cout << s.first << " ";
				for (auto const & s1 : s.second)
					cout << s1.first << " " << s1.second;
				cout << endl;
			}
		}
	private:
		Datatype Data;
	};

	void test()
	{
		auto testmakepair = make_pair(Sales_data(), int(3));
		auto test11_12 = sec_11_2_3::combiPair({ "wang","zhao" }, { 1,2 });
		sec_11_2_3::families ex11_14;
		ex11_14.addchild("wang", "zk", "98.1");
		ex11_14.addchild("wang", "dsa", "99.2");
		ex11_14.addchild("zhang", "zk", "98.1");
		ex11_14.addchild("zhang", "dsa", "99.2");
	}
}

namespace sec11_3_5
{
	multimap<string, string> bookstore = {
		{ "alan", "DMA" },
		{ "pezy", "LeetCode" },
		{ "alan", "CLRS" },
		{ "wang", "FTP" },
		{ "pezy", "CP5" },
		{ "wang", "CPP-Concurrency" }
	};
	void eraseKey(multimap<string, string>& bookstore, multimap<string, string>::key_type value)
	{
		auto countnum = bookstore.count(value);
		multimap<string, string>::iterator ptrbegin = bookstore.find(value);
		for (int i = 0; i < countnum; i++)
			bookstore.erase(ptrbegin++);
	}
	void printbookstore(const multimap<string, string>& bookstore)
	{
		bool flag = 1;
		for (auto it = bookstore.begin(); it != bookstore.end(); it++)
		{
			static string tmpstore = it->first;
			if (tmpstore != it->first||flag)
			{
				flag = 0;
				tmpstore = it->first;
				cout << tmpstore << endl;
			}
			cout << it->second << endl;
		}
	}
	
	void test()
	{
		map<string, vector<int>> ex11_28 = { { "Alan",{ 1,2,3,4,5, } },{ "John",{ 1,5,6,7,8 } } };
		map<string, vector<int>>::iterator finded = ex11_28.find("Alan");
		eraseKey(bookstore, "wan1g");
		printbookstore(bookstore);
	}
}

namespace sec11_3_6
{
	ifstream ifs_map("./word_transformation.txt"), ifs_content("./given_to_transform.txt");

	void skipSpace(const string & str, string::iterator & it)
	{
		for (; it != str.end(); it++)
			if (!isspace(*it))
				break;
	}
	void skipWord(const string& str, string::iterator&it)
	{
		if (it!=str.end()&&isspace(*it))
			skipSpace(str, it);
		for (; it != str.end(); it++)
			if (isspace(*it))
				break;
	}


	map<string, string> getmap(ifstream& ifsmap)
	{
		map<string, string> transformMap;
		string linestr;
		while (getline(ifs_map, linestr))
		{
			string::iterator it = linestr.begin();
			string::iterator keyend = linestr.end();
			string::iterator mappedbegin = linestr.end();
			skipSpace(linestr, it);
			string::iterator keybegin = it;
			for ( ; it != linestr.end(); it++)
			{
				if (isspace(*it))
				{
					keyend = it;
					break;
				}
			}
			skipSpace(linestr, it);
			mappedbegin = it;
			
			string::reverse_iterator rit = linestr.rbegin();
			if (mappedbegin != linestr.end())
			{
				for (; rit != linestr.rend(); rit++)
					if (!isspace(*rit))
						break;
			}
			string key(keybegin, keyend);
			string mappedvalue(mappedbegin, rit.base());
			transformMap[key] = mappedvalue;
		}
		return transformMap;
	}

	

	void TranslateWords(const map<string, string> &maptrans, ifstream& given,ofstream& outfile)
	{
		string linestr;
		while (getline(given, linestr))
		{
			string::iterator it = linestr.begin();

			while (it != linestr.end())
			{
				skipSpace(linestr, it);
				auto wordbegin = it;
				skipWord(linestr, it);
				auto wordend = it;
				string word(wordbegin, wordend);
				
				map<string, string>::const_iterator foundword = maptrans.find(word);
				if (foundword != maptrans.end())
					outfile << maptrans.find(word)->second << " ";
				else
					outfile << word << " ";
			}
			outfile << endl;
		}
	}

	void test()
	{
		auto maptrans = getmap(ifs_map);
		TranslateWords(maptrans, ifs_content, ofstream("ex11_33out.txt"));
	}
}

namespace sec11_3_3
{
	void test()
	{
		vector<int> sec11_3_3 = { 1,2,3,4,6,7,3,2,2,1,6,3 }; //顺序容器不能添加入const int 因为allocator不能加入const元素
		sec11_3_3.erase(sec11_3_3.begin(), sec11_3_3.begin() + 1);
		map<string, string> maptesttype;
		map<string, string>::mapped_type sgagah = maptesttype[string("fag")];
		map<string, string>::value_type ahasfhdg = *maptesttype.begin();
		map<string, string>::key_type ahhsli = maptesttype.begin()->first;
	}
}

namespace QuerySystem
{
	void testQuerySystem(string path)
	{
		QueryResult queryres;
		{
			TextQuery test(path); //即便这个销毁了，还是能够引用它内部的元素
			queryres = test.query("Daddy");
			queryres = test.query("Daddy");
			//set<decltype(vector<int>().size())> //这样可以自动推断
		}
		print(std::cout, queryres);
	}
}

namespace testhasptr_chap13
{
	void test()
	{
		HasPtrValue test11("215");
		HasPtrValue test22 = test11;

		{
			HasPtrptr test1(string("1"));
			HasPtrptr test2(string("2"));
			HasPtrptr test3(string("3"));
			HasPtrptr test4 = test1;
			HasPtrptr test6 = test2;
			{
				HasPtrptr test5 = test2;
			}
		}
		
	}
}

namespace sec12_1_6
{
	ifstream ifs("./storyDataFile.txt");
	
	void ex12_20()
	{
		string line;
		vector<string> vecline;
		while (getline(ifs, line))
			vecline.push_back(line);
		const StrBlob Blob(vecline);
		for (auto it = Blob.begin(); it != Blob.end(); it.incr())
		{
			cout << it.deref() << endl;
		}
	}
}

namespace ex13_28
{
	class TreeNode
	{
	public:
		TreeNode() :left(NULL), right(NULL) {}
	private:
		string value;
		int count;
		TreeNode * left;
		TreeNode * right;
	};
}

namespace sec13_3
{
	void ex13_31()
	{
		//vector<HasPtrValue> wt = { "4","5","ah","5y" }; //由于编译器只进行一步转换 所以这样是不对的
		vector<HasPtrValue> wt = { string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y") };
		//不同规模的数据sort时调用的函数是不同的，小规模只调用operator= 大规模时不光调用operator=还要调用swap
		sort(wt.begin(), wt.end());
	}
}

namespace sec13_5
{
	void ex13_42()
	{
		{
			string str1 = "ga";
			String teststr = "ga";
			teststr.print();
			String teststr1 = "";
			teststr1.print();
			String teststr2 = teststr;
			teststr2.print();
			teststr2 = teststr1;
			teststr2.print();
		}
		

		//String teststr2;

		StrVec test;
		StrVec test1 = {};
		StrVec test2 = { "s","f" };
		test2.print();
		test2.push_back("gag");
		test2.print();
		test2.resize(100);
		test2.print();
		test2.resize(2);
		test2.print();
		test2.reserve(200);
		test2.print();
	}
}

#include "Message.h"
#include "Folder.h"
namespace MessageFolder
{

	void test()
	{
		Message safga;
		
	}
}

namespace ex13_48
{
	void test()
	{
		StrVec af;
		af.push_back("wot");
		char text[] = "world";

		String && tmp = "hello"; //这个会调用String的构造函数 但是越过之后不会调用析构函数,因为有一个左值引用了
		String tmp1 = tmp; //因为变量表达式是左值，所以调用的是拷贝构造函数而不是移动构造函数
		String tmp2 = tmp;
		String tmp3 = std::move(tmp); //调用move显示的将左值转化为右值引用 进而调用移动构造函数
		String tmp4 = std::move(tmp);
		String tmp5 = std::move(String("he")); //这个即会调用移动构造函数 也会调用char*的构造函数 还会调用析构函数
		String tmp6 = "he"; // 但是这个的话编译器直接用char*初始化构造了，没有析构

		String tmp7 = "h1";
		//(tmp7.testcopymove()).testcopymove(); //由于testcopymove声明了左值，所以不能用右值调用testcopymove  会报错
		tmp7.testcopymove(); //在调用点处调用构造函数，构造了一个临时变量 然后由于没有左值引用该临时变量，就被销毁了
		String tmp8 = tmp7.testcopymove(); //编译器略过了tmp8的拷贝构造函数，直接用tmp7.testcopymove()的返回值构造tmp8
		String * tmp9 = &(tmp7.testcopymove());
		//在调用点处调用构造函数，构造了一临时变量，然后取该临时变量的地址赋给tmp9.但是由于是临时变量，在程序越过这个语句时，
		//临时变量销毁，tmp9所引用的地址是无效的。

		{
			String("tmp1");
			String && str = String("tmp");
			String & str1 = str;
			str1 = "t12";
			cout << "ssaf" << endl;
		}

		String s0;
		String s1("hello");
		String s2(s0);
		String s3 = s1;
		String s4(text);
		s2 = s1;

		std::vector<String> svec;
		svec.reserve(8);
		svec.push_back(s0); //每次push_back都要拷贝构造一次
		svec.push_back(s1);
		svec.push_back(s2);
		svec.push_back(s3);
		svec.push_back(s4); //左值输入调用的是拷贝构造函数
		svec.push_back("good job"); //右值引用调用的是移动构造函数 
		svec.push_back("good job1");
	}
}

namespace sec15_2_2
{
	void ex15_6()
	{
		Quote tmpQuote0("book0",13.4);
		Bulk_quote tmp1("book1",54.56,40,0.5);
		//tmp1.net_prize(5,5);
		tmp1.net_prize(5);
		print_total(cout, tmpQuote0, 10);
		print_total(cout, tmp1, 54); //这个输入的是动态类型
	}
	void ex15_7()
	{
		Quote q("textbook", 10.60);
		Bulk_quote bq("textbook", 10.60, 10, 0.3);
		bq.net_prize(1, 2);
		bq.net_prize(2);
		Quote& sf = bq;
		sf.net_prize(1);//通过引用调用虚函数
		//sf.net_prize(2, 3); //虽然是引用，动态类型，运行时决定，但是这是用在虚函数上的，因此不能用非虚函数，这句话会报错
		Bulk_quote& sff = bq; //这样相当于把sff看成了一个对象
		sff.net_prize(1);
		sff.net_prize(3, 2); //但是这样的话就可以调用了。因此注意理解：当我们使用“基类”的引用或指针调用一个虚成员函数时会执行动态绑定

		q.testvirtual();
		bq.testvirtual();
		//调用带有默认参数的testvirtual，不是通过基类引用和指针调用的，所以默认参数是各自的默认参数 9 和19  但是对于通过基类引用
		//和指针调用的就所有默认参数都是9 见print_total函数

		Strategy_ex15_7 lq("Bible", 10.60, 10, 0.3);
		testclass lq1("Bible1", 10.60, 10, 0.3);

		lq1.net_prize(5,3);
		print_total(std::cout, lq1, 5);
		print_total(std::cout, q, 5);
		print_total(std::cout, bq, 5);
		print_total(std::cout, lq, 5);

		print_total(std::cout, q, 15);
		print_total(std::cout, bq, 15);
		print_total(std::cout, lq, 15);
	}

	void test()
	{
		ex15_7();
		ex15_6();
	}
}

namespace ex15_11
{
	void printdebug(Quote& item)
	{
		item.debug();
	}
	void test()
	{
		pro_quote pro_quote1;
		//pro_quote1.ISBN();//因为是protected继承，所以即便是基类的public，也还是不能调用的.要想调用的话，需要弄using
		//pro_quote1.bookNo //protected继承，private仍然是private，不能调用

		Quote q("aaa", 10.60);
		Bulk_quote bq("bbb", 111, 10, 0.3);
		Strategy_ex15_7 lq("ccc", 222, 10, 0.3);
		
		cout << "/////////////////////////////////////////////////////////" << endl;
		Quote & r1 = q;
		r1.debug();
		Quote & r2 = bq;
		r2.debug();
		Quote & r3 = lq;
		r3.debug(); //当且仅当对通过引用或指针调用虚函数时，才会在运行时解析该调用，只有在此时动态类型与静态类型才不一样。
		//这跟下一栏的还不一样，因为本栏都是通过引用调用的，而下一栏根本不是引用，是赋值！！！！
		cout << "/////////////////////////////////////////////////////////" << endl;
		Quote& r = q;
		r.debug();
		r = bq; //调用的是拷贝赋值函数,在编译阶段就知道调用哪个。只会赋值基类部分，派生类会被忽略掉
		r.debug();
		r = lq; //也就是说是通过普通类型非引用非指针来调用的！！！！！！！
		r.debug(); //编译器在编译阶段就能确定是Quote类型的，因此调用的debug都是对应派生类的基类debug
		cout << "/////////////////////////////////////////////////////////" << endl;
		printdebug(q);
		printdebug(bq);
		printdebug(lq); //编译器编译printdebug(Quote& item)时，不知道具体的基类引用绑定在什么派生类上，因此留给运行调用时决定。动态类型，调用的是各自
						//派生类的debug函数
		cout << "/////////////////////////////////////////////////////////" << endl;
	}

}

namespace ex15_20
{
	void test()
	{
		pub_derv pub_derv;
		priv_derv priv_derv;
		prot_derv prot_derv;
		derived_from_public derived_from_public;
		derived_from_private derived_from_private;
		derived_from_protected derived_from_protected;
		//derived_from_protected.pub_mem(); //protected类型，不可以被成员函数访问到

		//如果外界调用引用的话，有一个派生类向基类的转化
		pub_derv.memfcn(pub_derv);
		pub_derv.memfcn(derived_from_public);
		derived_from_public.memfcn(pub_derv);
		derived_from_public.memfcn(derived_from_public);
	}
}

namespace ex15_22
{
	void test()
	{
		//shape shapetmp; //报错不允许使用抽象基类的对象
		//shape_2D shape2dtmp;//报错不允许使用抽象基类的对象
		//shape_3D shape3dtmp; //报错不允许使用抽象基类的对象
		circle circletmp("sad", { 1,1 },3);
		circletmp.coutname();
		printdebug(cout, circletmp);
		cout << circletmp.volume() << endl;
		cout << circletmp.area() << endl;
		rectangle rectangletmp("sga", { 3,4 }, 5, 6);
		rectangletmp.coutname();
		printdebug(cout, rectangletmp);
		cout << rectangletmp.volume() << endl;
		cout << rectangletmp.area() << endl;
		sphere spheretmp("sphere", { 5,4 }, 3);
		spheretmp.coutname();
		printdebug(cout, spheretmp);
		cout << spheretmp.volume() << endl;
		cout << spheretmp.area() << endl;
		cone conetmp("cone", { 3,2 }, 6, 8);
		conetmp.coutname();
		printdebug(cout, conetmp);
		cout << conetmp.volume() << endl;
		cout << conetmp.area() << endl;
		//似乎没有必要弄受保护的成员
	}
}

namespace sec15_6
{
	class base
	{
	public:
		virtual int fcn()
		{
			cout << "base" << endl;
			return 1;
		}
	};
	class D1 :public base
	{
	public:
		int fcn(int);
		virtual void f2() {}
	};
	class D1_5 :public base
	{
	public:
		int fcn()
		{
			cout << "class D1_5 :public base" << endl;
			return 1;
		}
	};
	class D2_5 :public D1_5
	{
	public:
		void nothing();
	};

	class D2 :public D1
	{
	public:
		int fcn(int);
		int fcn();
		void f2()
		{
			return;
		}
	};

	void test()
	{
		D1 D1;
		//D1.fcn(); //直接通过派生类希望调用基类的同名函数，但是已经被fcn(int)隐藏了
		base * tmp = &D1;
		//tmp->fcn(6); //由于tmp的静态类型是base，因此fcn(int)找不到
		tmp->fcn();
		//在编译时先通过base找到fcn，发现fcn是一个没有输入参数的虚函数，说明要运行时调用，发生了动态绑定。
		//运行时进入D1，然后首先遇到fcn(int)，但是发现其不是一个没有输入参数的虚函数，因此进入基类继续寻找，直到找到一个没有输入参数的虚函数

		D2_5 D2_5;
		base * tmp1 = &D2_5;
		tmp1->fcn(); 
		//在编译时先通过base找到fcn()，发现其是不带参数的虚函数，则编译器产生的代码将在运行时确定到底该运行虚函数的哪个版本。
		//在运行时，发现动态类型是D2_5，扫一遍D2_5发现没有不带参数的fcn，然后进入基类，发现有一个不带参数的fcn，这时候就调用这个fcn
		//需要注意的是，这个fcn不是base域的，而是base的直接派生域的 因此P549的名字查找与继承无论如何都是必要的步骤

	}
}

namespace ex15_26
{
	Bulk_quoteNew test()
	{
		{
			Bulk_quoteNew Bulk_quoteNew1;
			Bulk_quoteNew Bulk_quoteNew2("twet", 46, 3, 4);
			Bulk_quoteNew Bulk_quoteNew3 = Bulk_quoteNew2;
			Bulk_quoteNew3 = Bulk_quoteNew1;
		}
		Bulk_quoteNew Bulk_quoteNew4;
		return Bulk_quoteNew4;
	}
}

namespace sec15_8
{
	void test()
	{
		vector<Quote> vecquote;
		Bulk_quoteNew Bulk_quoteNew1("sga", 3, 4, 0.5);
		for (int i = 0; i < 10; i++)
		{
			vecquote.push_back(Bulk_quoteNew1);
			cout << endl; //从输出窗口可以很明显的看到vector的行为，每次pushback都要先把旧元素移动到新的空间然后拷贝构造一个新元素
		}
			
		double sum = 0;
		for (int i = 0; i < vecquote.size(); i++)
			sum += vecquote[i].net_prize(6);
		cout << sum << endl;

		vector<shared_ptr<Quote>> vecquote1;
		shared_ptr<Bulk_quoteNew> Bulk_quoteNew_ptr = make_shared<Bulk_quoteNew>("sga", 3, 4, 0.5);
		for (int i = 0; i < 10; i++)
		{
			vecquote1.push_back(Bulk_quoteNew_ptr);
			cout << endl;
		}
			
		sum = 0;
		for (int i = 0; i < vecquote1.size(); i++)
			sum += vecquote1[i]->net_prize(6); //这个是动态绑定 前提一定是指针或引用+虚函数
		cout << sum << endl;
	}
}

namespace ex15_30
{
	void test()
	{
		Bulk_quote asga;
		asga.net_prize(1, 1);
		asga.net_prize(2);
		Quote* sag = &asga;
		sag->net_prize(3);


		Basket basket;
		for (unsigned i = 0; i != 10; ++i)
			basket.add_item(Bulk_quote("Bible", 20.6, 20, 0.3)); //20本为打折限度，0.3为折扣量 206
		for (unsigned i = 0; i != 10; ++i)
			basket.add_item(Bulk_quoteNew("C++Primer", 30.9, 5, 0.4)); //5本为打折限度，0.4为折扣量 10*30.9*0.6=185.4
		for (unsigned i = 0; i != 10; ++i)
			basket.add_item(Quote("CLRS", 40.1)); //只按原价销售 401

		cout << basket.total_receipt() << endl;
	}
}

namespace QuerySystem
{
	void sec15_9()
	{
		TextQuery tQuery("test.txt");
		HistoryQuery HistoryQuery;
		{
			Query q = Query("fiery")&Query("bird") | Query("wind");
			HistoryQuery.addQuery(q);
			q = (~Query("she"));
			HistoryQuery.addQuery(q);

			q = Query("her"); //这行出去只调用一次析构函数 ，因为是拷贝赋值函数，临时变量出去这行后任然会被销毁

			Query q1 = Query("her"); //这行出去不调用析构函数，因为是拷贝构造，编译器略过，不销毁临时变量，成了左值，局部变量出作用域后才调用析构函数

			q = ~Query("her");//因为是拷贝赋值函数 ~Query("her")有两个临时对象，出去这行后会调用两次析构函数
			HistoryQuery.addQuery(q);

			q = ~Query("her") & (~Query("she")); //所有这些表达式都没有执行查询操作，只是把查询语句存在这里了
			HistoryQuery.addQuery(q);

			q = ~Query("her") | (~Query("she"));
			HistoryQuery.addQuery(q);
		} //比如有一个场景，在这里边查询并输出了，实时查询结果查询完毕后就销毁了，然后在以后的某一天想看看historyquery
		
		for (int i = 0; i < HistoryQuery.size(); i++)
		{
			print(cout, HistoryQuery[i].eval(tQuery));
		}
	}
}


namespace EffectiveCpp //保存一些在看EffectiveC++时的一些例子
{
	namespace basederive
	{
		
		class base
		{
		public:
			virtual void print()
			{
				cout << "base" << endl;
			}
			virtual void printthis()
			{
				cout << "base:" << this << endl;
			}
			int m;
			virtual void privatevirtual()
			{
				cout << "base privatevirtual" << endl;
			}
			void protectfuc(int d)
			{

			}
		private:
			class tmp
			{
				int m;
			};
			void copyfuc(int x)
			{

			}
		protected:
			void protectfuc() //在两个不同的权限域中也是可以进行重载的
			{

			}
		};
		class derive :public base
		{
		public:
			//class tmpderive :public tmp {}; //会报错tmp不可访问 但当把tmp放到public时就不会报错了，正常的继承
			void print() override
			{
				cout << "derive" << endl;
			}
			void printthis()override
			{
				cout << "derive:" << this << endl;
			}
			int n;
			void copyfuc() //这个抛开权限和名字查找不说，违反了绝不重新定义继承而来的non virtual函数的准则
			{
				protectfuc(2);
				protectfuc(); //在两个不同的权限域中也是可以进行重载的
			}
		private:
			virtual void privatevirtual() override
			{
				cout << "derive privatevirtual" << endl;
			}
		};

		void fcn(shared_ptr<base> ptr)
		{
			cout << "fcn(shared_ptr<base> ptr)" << ptr.use_count() << endl;
			ptr->print();
		}
		void fcn1(const shared_ptr<base>& ptr) //如果把const去掉的话编译器会报错，不能传入继承类的智能指针
		{
			cout << "fcn1(const shared_ptr<base>& ptr)" << ptr.use_count() << endl;
			ptr->print();
		}

		void test()
		{
			shared_ptr<derive> d(new derive);
			fcn(d); //这样的话输出是derive 进入函数后引用计数是2 fcn引用计数要加1好理解，因为是值传递
			fcn1(d); //这样的话输出还是derive 进入函数后引用计数是2  这个不太好理解，继承类的智能指针传引用给基类智能指针后引用计数加1
			//？？？？？？？？？？？？？？？？？？？？ 可能要等到看到智能指针的实现和理解更底层的内存机制才能得到解答了
			//https://stackoverflow.com/questions/56798756/why-is-shared-ptr-counter-incremented-when-passed-as-const-reference-to-base-cla
			//Stack Overflow的解释 因为这俩是完全不同的类型，有一个共享，所以就增1了 还是不太懂 之后再看吧
			shared_ptr<base> b(new base);
			fcn(b); //输出是base 进入函数后引用计数是2
			fcn1(b); //输出是base 进入函数后引用计数是1 这个由于是完全相同的类进行引用，因此引用计数没有变化
		}
	}

	void test()
	{
		basederive::base b;
		cout << &b << endl;
		b.printthis(); //这两个输出的地址完全一样
		basederive::derive d;
		basederive::base * bptr;
		bptr = &d;
		cout << bptr << endl;
		bptr->printthis(); //这两个输出的地址也是完全一样 也就是说当用指针访问类的某一成员函数时this指针的值和该指针的值是一样的
		bptr->privatevirtual(); //动态调用了derive的private的函数 可以看做是通过共有函数接口调用的private的函数 但是具体是怎么操作的还是不知道
		//虚函数的继承不受private public的影响

		//d.protectfuc(); //这个会报错不可访问 
		d.protectfuc(1); //这个正常 可见必须是有访问权限的相同作用域才能进行重载

		basederive::test(); //条款20 以const&替换value 测试智能指针和多态
		int ii5;
		(ii5 = 3) = 4; //i的结果最终为4；因此可见条款10，operator=应该返回一个引用，而不是值，不是void
		const string("sag") = string("aha"); //实际上就是一个右值
		string("asd") = string(""); //这样的赋值是可以的，本质原因是operator=返回的值不是const的引用
		string & af = string("asd");
		af = "asg";
	}
}
void main()
{
	try
	{

		EffectiveCpp::test();
		
		QuerySystem::sec15_9();
		ex15_30::test();
		sec15_8::test();
		ex15_26::test();
		sec15_6::test();
		ex15_22::test();
		ex15_20::test();
		ex15_11::test();

		sec15_2_2::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		//ex13_48
		ex13_48::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		
		////////////////////////////////////////////////////////////////////////////////////////////
		//sec12.5
		sec13_5::ex13_42();
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//sec12.3
		QuerySystem::testQuerySystem(string("./storyDataFile.txt"));
		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//sec13.3
		sec13_3::ex13_31();

		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//sec12.3
		sec12_1_6::ex12_20();
		////////////////////////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////////////////////////
		//chapter13
		testhasptr_chap13::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.6节
		sec11_3_6::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.5节
		sec11_3_5::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.3节
		sec11_3_3::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.1节
		map<string, vector<string>> sasf = { {string("sdafa0"),vector<string>{ "asfga0s","asgagh0" }} };
		//*sasf.begin() = make_pair(string("sdafa"),string{ "asfgas","asgagh" }); //由于关键字是const类型的，因此不能直接赋值
		(*sasf.begin()).second = { "asf" };
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.2.3节
		sec_11_2_3::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.2.2节
		
		//sec11_2_2::agsg[sec11_2_2::test1122.begin()] = 5; //会报错没有定义小于号
		sec11_2_2::sasf[sec11_2_2::vectest1122.begin() + 3] = 8;
		sec11_2_2::sasf[sec11_2_2::vectest1122.begin() + 4] = 9; //尾后迭代器
		//sec11_2_2::test1122.begin()<sec11_2_2::test1122.end() //list的迭代器没有小于操作，只有++ --。

		multiset<Sales_data, bool(*)(const Sales_data&, const Sales_data&)> multi1122(sec11_2_2::compisbn); 
		//有一个好处是，compisbn所在的位置可以调用其他内容但是类型相同的函数
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//11.2.1节
		sec11_2_1::test_ex_11_7();
		vector<string> vecstr = { "af", "af", "asfa", "af", "h23","asfa" };
		sec11_2_1::unique_vector_str(vecstr);
		set<string> dd = { "af", "af", "asfa", "af", "h23","asfa" };
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.1节
		sec11_1::CountString(sec11_1::testdata);
		sec11_1::CountStringIgnorePuc(sec11_1::testdata);

		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//ex12_26
		allocator<string> sec12_2_2;
		auto beginptr = sec12_2_2.allocate(100);
		sec12_2_2.construct(beginptr , "dasd");
		ex12_26::input_string_output_reversestring(10);
		////////////////////////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////////////////////////
		//12.1.5节
		//int * sec12_1_5addr = new int(32);
		//{
		//	unique_ptr<int> sec12_1_5_intell(sec12_1_5addr);
		//	{
		//		unique_ptr<int> sec12_1_5_intell1(sec12_1_5addr); //同一块内存释放了两次，编译没错，但是运行时触发断点出错
		//	}
		//}
		
		

		//unique_ptr<int> ex12_16(new int(32));
		//unique_ptr<int> ex_12_16_1(ex12_16); //报错是已删除的函数
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.4.3节
		for_each(sec10_4_1::vec.crbegin(), sec10_4_1::vec.crend(), [](int n) {cout << n << " "; });
		cout << endl;
		auto it = sec10_4_1::vec.cend();
		while (it != sec10_4_1::vec.cbegin())
			cout << *(--it) << " ";
		cout << endl;
		std::list<int> ex10_36 = { 1, 2, 3, 4, 0, 5, 6 };
		auto last0 = find(ex10_36.rbegin(), ex10_36.rend(), 0);

		vector<int> ex10_37 = { 1,5,12,4,4,3,7,8,9,6 };
		list<int> reverse;
		copy(ex10_37.cbegin() + 3, ex10_37.cbegin() + 8, front_inserter(reverse));

		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//10.4.1节
		sort(sec10_4_1::vec.begin(), sec10_4_1::vec.end());
		unique_copy(sec10_4_1::vec.begin(), sec10_4_1::vec.end(), front_inserter(sec10_4_1::lst));
		copy(sec10_4_1::ex10_28.begin(), sec10_4_1::ex10_28.end(), inserter(sec10_4_1::insertercopy, sec10_4_1::insertercopy.begin()));
		//copy(sec10_4_1::ex10_28.begin(), sec10_4_1::ex10_28.end(), front_inserter(sec10_4_1::insertercopy)); // vector不能push front
		////////////////////////////////////////////////////////////////////////////////////////////




		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.4节
		cout << count_if(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), bind(sec10_3_4::Iflowerthansz, sec10_3_4::_1, 6)) << endl;
		cout << sec10_3_4::CheckSizeLambda(vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8}, string("gasdf")) << endl;
		cout << sec10_3_4::CheckSizeBind(vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8}, string("gasdf")) << endl;
		sec10_3_4::biggies(section10_3_1::ex10_13, 4);
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.3节
		//ex 10.20
		cout << count_if(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), [](const string&s1)->bool {if (s1.size() > 6) return 1; else return 0; }) << endl;
		section10_3_2::ex10_21(10);
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.2节
		cout << section10_3_2::ex10_15(3)(4) << endl;
		section10_3_2::biggies(section10_3_1::ex10_13, 4);
		sort(section10_3_1::Sales_datav.begin(), section10_3_1::Sales_datav.end(), [](const Sales_data&s1, const Sales_data&s2) {return s1.isbn().size() < s2.isbn().size(); });
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.1节
		std::vector<std::string> v{ "1234", "1234", "1234", "Hi", "alan", "wang" };
		ex10_9::ElimDups(v);
		stable_sort(v.begin(), v.end(), section10_3_1::Isshorter);

		sort(section10_3_1::Sales_datav.begin(), section10_3_1::Sales_datav.end(), section10_3_1::compareIsbn);
		//sort(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), section10_3_1::Isshorter);
		partition(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), section10_3_1::IsMore5);

		////////////////////////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////////////////////////
		//10.2节
		vector<double> ex10_4 = { 1.1,3,2,5,6,2,1,5,2,3,1 };
		cout << accumulate(ex10_4.cbegin(), ex10_4.cend(), 0.0) << endl;
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.1
		vector<int> ex10_1_1 = { 1,3,2,1,2,3,4,1,5,7,8,5,23,7 };
		cout << count(ex10_1_1.cbegin(), ex10_1_1.cbegin()+3, 1) << endl;
		vector<string> ex10_1_2 = { "asda s","asda s","s" };
		cout << count(ex10_1_2.cbegin(), ex10_1_2.cbegin() + 3, "asda s") << endl;

		////////////////////////////////////////////////////////////////////////////////////////////
		
		
		auto sdsad = new auto(Sales_data());
		////////////////////////////////////////////////////////////////////////////////////////////
		//12.1.3节
		//shared_ptr<int> tmpsharedptr1 = new int(43); //看源码的话发现地址到智能指针的构造函数是explicit的，因此不能用拷贝构造函数，这涉及到隐式转化
		shared_ptr<int> tmpsharedptr2(new int(43)); //这样就可以
		{
			shared_ptr<int> tmpsharedptr3(shared_ptr<int>(new int(43))); //默认的析构函数是delete int* ，如果指向的不是动态内存，需要自定义一个删除器

		}
		



		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习12.6
		vector <int> * ex12_6p = ex12_6::returnvecint();
		ex12_6p = ex12_6::readinput(ex12_6p);
		ex12_6::printvecint(ex12_6p);

		shared_ptr<vector<int>> ex12_6pIntelligence = ex12_6::returnvecintIntelligence();
		ex12_6::readinputIntelligence(ex12_6pIntelligence);
		ex12_6::printvecintIntelligence(ex12_6pIntelligence);
		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//练习12.2
		cout << ex12_2::ss[0] << endl;
		ex12_2::ss[1] = 2;
		cout << ex12_2::ss[1] << endl;

		StrBlob StrBlob1;
		StrBlob1.push_back("ssss");
		StrBlob1.front() = "ssss1";
		
		const StrBlob StrBlob2({"uyffhgf","sfafag"});
		//StrBlob2.front() = "uyffhgfmodify";//因为是const，所以不能修改元素 但若将重载的front返回类型的const去掉，就能修改了，虽然不会报错但与用户初衷相悖
		cout << StrBlob2[1] << endl;
		//StrBlob2[1] = 1; //这会报错，因为已经在成员函数中重载了只读类型

		StrBlob StrBlob3 = StrBlob2; //这会调用编译器默认合成的拷贝构造函数
		StrBlob3.push_back("FASFASF");//StrBlob2是const类型的，意味着不能通过StrBlob2改变共享区域的数据，而能通过其他Blob改变共享区域的数据


		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//练习6.54
		ex6_54::testvec_pfuc(50, 2);
		ex6_54::useFp(ex6_54::fucminus, 48, 3);
		ex6_54::useFp(ex6_54::fucmultiple, 48, 3);
		cout << ex6_54::returnP2fuc(ex6_54::fucmultiple) << endl;
		int i = 1;
		cout << &i << endl; //函数的指针和变量的指针占用的内存是一样的
		ex6_54::Fp s0 = ex6_54::fucminus;
		//ex6_54::F s1 = ex6_54::fucminus;//会报错，因为不能直接给函数类型赋值 没有定义这样的类型的值是什么，不想函数指针那样
		//当我们把函数名作为一个值使用时，该函数自动的转化成指针
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习2.36
		++ex2_36::c;
		++ex2_36::a;
		++ex2_36::d;
		++ex2_36::a1;
		++ex2_36::a2;
		int e236 = 3;
		int e237 = e236;
		++e236;
		++e237;
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习2.33
		ex2_33::a = 42;
		ex2_33::b = 42;
		ex2_33::c = 42;
		//ex2_33::d = 42; //推断是int*类型的，会报错无法将int类型的值分配到int *类型
		//ex2_33::e = 42;	//推断是const int*类型的，会报错无法将int类型的值分配到const int *类型
		//ex2_33::f = 42;	//报错表达式必须是可修改的左值。
		
		int * s = nullptr;
		bool(*p)(const char a, const char b);
		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习13.18
		Employee Employee1("wzk1");
		//Employee Employee2 = Employee1;//会报错拷贝构造函数是被删除的函数。
		Employee Employee3("wzk3");
		//Employee3 = Employee1;//本身合成的拷贝赋值函数就是删除的



		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习13.11 13.13
		HasPtr hasptr1311 = HasPtr("2");//又略去了拷贝构造函数

		HasPtr("3");
		cout << *psstore << endl; 
		//上一步中创建的临时变量被销毁时，会调用析构函数，析构函数中应该加一个delete动态分配的对象
		//因为当指向一个对象的引用或指针离开作用域时，析构函数不会执行
		//且因为隐式销毁一个内置指针类型的成员不会delete它所指向的对象
		//当显示的加入delete ps时，上述cout会出错。

		n13_13::X *px = new n13_13::X;
		n13_13::f(*px, *px);
		delete px;
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//练习7.57
		Account("wzk0");
		Account("wzk1");
		Account("wzk2");
		Account Account1("wzk3");
		Account Account2 = "232"; 
		//给构造函数加上explicit后，本语句就非法了
		//这个跳过了拷贝构造函数，本来应该是先通过字符串输入的构造函数构造一临时变量，
		//然后在通过拷贝构造函数创建对象的，这样的话NumID++2，但实际上是++1,因此就是跳过了拷贝构造函数
		//但跳过拷贝构造函数不是所有编译器都必须实现的事项，因此，为了避免numID++2，把字符串输入的构造声明为explicit
		//以避免隐式转换造成numID分配浪费
		Account Account3 = Account2;

		Account Account4("Account4");
		Account4 = Account1;
		long n = Account::read_NumID();
		Account Account5("Account5");
		Account &Account6 = Account5;
		Account &Account7 = Account4;
		Account6 = Account7;

		int intnum00 = 2;
		int intnum000 = 3;
		int &intnum01 = intnum00;
		int &intnum02 = intnum000;
		intnum01 = intnum02;
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//练习7.58
		const vector<double> vectordouble1;
		//vectordouble1.push_back(2.1); // 会报错 因为不能往一个const类型的容器中加东西

		HasPtr hs0;
		const vector<double> &vectordouble3 = hs0.teststatic6;
		const vector<double> &vectordouble2 = HasPtr::teststatic6;
		cout << HasPtr::teststatic << endl;
		cout << teststatic << endl;

		cout << HasPtr::teststatic7 << endl;

		HasPtr::teststatic8 = 2;

		//cout << HasPtr::teststatic5 << endl; //如果定义了static，但是没有初始化，会报错无法解析的外部命令

		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习13.5
		HasPtr hasptr0("hasptr0");
		HasPtr hasptr1 = hasptr0; 
		//由于自定义拷贝构造函数的缘故，导致hasptr1.ps值不等于hasptr0.ps，
		//指向的是一个经过加工的string，这样可以明确地与编译器合成的进行比较
		//当不自定义拷贝构造函数时，用编译器自己的，效果是拷贝地址本身
		HasPtr hasptr1_1 = 5;
		//跳过了拷贝构造函数，并没有执行自定义的拷贝构造函数，而是直接调用入口参数为int的构造函数
		//如果没跳过的话，应该+"2"的，但是没有加
		//这也是符合逻辑的，因为我本意是想把5对应的对象赋给hasptr1_1，已经执行了一遍构造了，就不必要再执行一遍
		HasPtr hasptr1_2 = hasptr1_1;//这就用的是自定义的拷贝构造函数，会+"2"
		HasPtr hasptr1_3(6, 3); //并不会影响拷贝构造函数作为正常构造函数进行最优参数匹配发挥其功能
		HasPtr hasptr2("ptr2");
		(hasptr2 = hasptr0).pulicfuc();//这个是拷贝赋值运算符
		//若是默认合成的拷贝赋值运算符则完全与hasptr0相等,但是对象本身地址不同，因而就叫“拷贝”
		//若是自定义的，就是自定义的那些操作，但有个风险是，需要自己控制所有成员变量的拷贝赋值，如有遗漏，则该遗漏的成员变量保持不变
		cout << &hasptr0 << " " << &hasptr2 << "大小是" << sizeof(&hasptr2)<< endl;
		int intvalue = 0;
		int *intvalueadd = &intvalue;
		cout << intvalueadd << "大小是"<<sizeof(intvalueadd)<<endl; 
		//通过对比输出的地址格式，以及地址的大小，发现指向不同对象的指针所占内存空间是一样的
		////////////////////////////////////////////////////////////////////////////////////////////
		
		
		////////////////////////////////////////////////////////////////////////////////////////////
		//练习7.53
		Debug Debug0;
		Debug Debug1(true,false,true);
		Debug Debug2(2, 3, 1);

		double debuga=3, debugb=2, debugc=1;
		Debug Debug3(debuga, debugb, debugc);
		//要想发挥constexpr的属性，必须在声明变量时前边加上constexpr。
		//否则如果不加的话，还是可以通过变量赋值的，constexpr形同虚设
		cout << Debug3.get_bw();
		Debug3.set_bw(0);  //这样是可以修改的
		cout << Debug3.get_bw();

		//constexpr Debug Debug4_1(debuga, debugb, debugc); //会报错表达式中必须含有常量
		constexpr Debug Debug4(3, 2, 1); //这样由常量赋值后，就正确了。
		Debug4.get_bw();
		//Debug4.set_bw(0); //这个是报错的，因为Debug4声明保证了其内部的值在编译时就已经确定，程序运行时是不能搞的
		
		Debug Debug5(true);
		bool boolva = 1;
		Debug Debug6(boolva);
		const Debug Debug7(true);//不是在编译期就确定值的常量 跟constexpr没什么关系
		//constexpr Debug Debug8(true);//报错：表达式必须含有常量值
		//在构造函数中，并没有对Debug(bool b ) :bw(b), io(b), other(b)这个构造函数声明constexpr
		//而如果在构造函数中声明了constexpr，则上边这一行不报错。意味着可以构造constexpr类型

		//综上，constexpr是一个比较广泛的选项，可以进一步提升代码安全性也可以退一步不声明constexpr当成普通的来处理
		////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////
		//练习7.49
		Sales_data Sales_data0("333");
		Sales_data & Sales_data1 = Sales_data0.returnRef(Sales_data("333"));
		//期望返回一个临时变量的引用，虽然不会报错，但是Sales_data1的值是默认构造构造出来的，没有"333"
		//上边这个似乎会拷贝构造一个 目前还不清楚为什么返回一个局部引用还work
		Sales_data & Sales_data2 = Sales_data0.returnRef(Sales_data0);
		//期望返回Sales_data0的引用，因为其不是一个临时变量，因此真实值确实是"333"

		Sales_data Sales_data3;
		Sales_data & Sales_data4 = Sales_data0.combine(Sales_data3);

		Sales_data Sales_data5("345");
		Sales_data Sales_data6 = Sales_data0.returnValue(Sales_data5);

		double sad = 4;
		//如果加上explicit，那么将不能按照下边的操作进行隐式转化
		Sales_data0.combine(3.3);//自动隐式转化成了一个const Sales_data
		Sales_data0.combine(sad);//临时变量一定是const型的 因为即便是double型变量进行隐式转换，调用的构造函数都是const类型
		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		int n1118 = 1;
		int &n11181 = n1118;
		int &n11182 = n11181;
		n1118 = 3; //可以对引用多次命名

		Nodefault moren;
		const Nodefault &moren1 = moren.returnRef(moren);
		const Nodefault &moren2 = moren.returnRef(2);
		//把整数隐式转换为了临时的类，如果没有输入参数为整数的构造函数的话会报错没有适当的参数类型转换

		//Nodefault & moren3 = moren.returnrefnotconst(3);
		////上式是会报错的，报错必须是可修改的左值。
		////由C++primer P55可知道，常引用可以绑定在临时量上，但是非常引用不可以绑定在临时量上。
		////上式是先隐式构造了一临时量，然后期望将该临时量绑定在一非常量引用上，这是非法的

		Nodefault moren5;
		Nodefault & moren6 = moren.returnrefnotconst(moren5); //这样就可以了 将非常量引用绑定在了变量上

		moren5.data++;
		const Nodefault&ref2tmp = Nodefault(6); 
		//在内存中有的，因为可以绑定常引用，没有调用析构函数 结合下一条语句可见临时量的释放与否是灵活的
		Nodefault(6); //临时变量出了这条语句后就不可访问了, 释放了内存了,调用了析构函数

		Nodefault(6).returnRef(moren1); //临时变量只能是只读的
		Nodefault(6).returnrefnotconst(moren5).data++; //就是把moren5的data++了一下
		//Nodefault(6).returnRef(moren1).data++; //由于const的限制，使得不能被修改
		//Nodefault(6).data++; //报错表达式必须是可修改的左值，是因为是临时变量，不能被修改

		////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////
		C ctmp;
		cout << endl;
		vector<C> tmpvectornodefault(10,ctmp);
		Nodefault nodefaulttmp;
		vector<Nodefault> tmpvectornodefault1(10, nodefaulttmp);
		cout << endl;
		vector<Nodefault> tmpvectornodefault2(10); //这样的话是构造十次
		cout << endl;
		vector<Nodefault> tmpvectornodefault3(10, Nodefault(1)); //这样的只构造一次，然后复制十次
		////////////////////////////////////////////////////////////////////////////////////////////



		////这下边这个本来应该是未定义的行为的，但是可能VS2015编译器有一些优化，就可以对声明为const的值进行修改
		////但我认为，未定义的行为可能是本来声明为const，但是有这些行为使得const的值变化了，造成难以预计的程序运行后果
		////意味着程序具有设计缺陷，编译可能在所有平台都是可以通过的，只不过与标准相悖
		//const int j = 3; // j 声明为const常量
		//int* pj = const_cast<int*>(&j);
		//*pj = 4;         // 用去const的指针去修改一个原本就是常量的j的值，undefined behavior!

		const int intnum = 5;
		const int intconst1 = intnum;
		//intconst1 = 34; //报错必须是可修改的左值，可以通过引用的const_cast来修改 但是很不推荐！
		//const_cast<int> (intconst1) //报错必须是指针、引用 因为只能修改底层const属性
		CastConstZK(intconst1) = 34; //这样一下就把本来应该是const的值给修改了


		const int * intconst = &intnum;
		const int & ref = intnum;
		//const int & &ss = ref; //这样就会报错不能使用对引用的引用
		const int & ss = ref; //这样不会报错对引用的引用
		const_cast<int &>(ref) = 100;
		//int * intnotconstnotcast = intconst;
		*(const_cast<int *>(intconst)) =1;
		int * intnotconst = const_cast <int *> (intconst);
		*intnotconst = 6;

		//NotThrow();
		vector<int> vectorint(1,1);
		vectorint.push_back(10);
		const screen constscreen(5);
		screen tmpscreen2(3,4);
		//screen tmpscreen3; //此时会报错包含多个默认构造函数

		/*screen tmpscreen4(); //此时会报错必须定义类类型，因为他把tmpscreen4()看成了函数声明
		cout << tmpscreen4.data << endl;*/

		screen tmpscreen(2);
		screen tmpscreen1(3);
		tmpscreen1 = tmpscreen; //似乎编译器自己重载了一个赋值operator
		constscreen.display() = tmpscreen; //这个非引用 为什么还能做左值 因为编译器把该表达式翻译为了operator = 
										   //在effective C++中有表述
		constscreen.display().changedata(100); // 不会对constscreen做任何操作

		cout << testinline() << endl;
		cout << inlinetest() << endl;

		int a = 5;
		int * Locate_a = &a;
		int ** locloc_a = &Locate_a;
		int *** loclocloc_a = &locloc_a;
		cout << Locate_a << endl;
		cout << locloc_a << endl;
		cout << loclocloc_a << endl;

		//using namespace Exercise;
		using Exercise::ivar;
		//using Exercise::dvar;
		using Exercise::limit;

		fuc();
		using Exercise::cout;
		std::cout << cout << endl;

		double dvar = 3.1415;

		int iobj = limit + 1;
		++ivar;
		++::ivar;
	}
	catch (exception d)
	{
		cout << d.what();
	}
	
	system("pause");
}