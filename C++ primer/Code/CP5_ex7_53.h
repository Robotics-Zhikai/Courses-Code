#ifndef CP5_ex7_53_h
#define CP5_ex7_53_h

class Debug
{
public:
	/*constexpr*/ explicit Debug(bool b ) :bw(b), io(b), other(b) {  }
	constexpr Debug(bool h, bool i, bool o) : bw(h), io(i), other(o) {/*std::cout << "ss" << std::endl;*/ /*会报错语句不能出现在constexpr函数中*/ } //意味着必须在编译期给个交代
	//但是，要想发挥constexpr的属性，必须在声明变量时前边加上constexpr，否则如果不加的话，还是可以通过变量赋值的
	constexpr Debug():bw(true),io(true),other(true) {} //只要声明了constexpr，就必须在编译期就给成员变量赋初值
	//Debug(bool h, bool i, bool o) : bw(h), io(i), other(o) {}//报错重定义

	void set_bw(bool b) { bw = b; } //这个因为要修改成员数据，所以不能被constexpr限定，因为constexpr中有且只能有return
	constexpr bool get_bw() { return bw; }
private:
	bool bw;
	bool io;
	bool other;
};

#endif