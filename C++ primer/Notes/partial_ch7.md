# 第七章部分笔记
一些有效的简短总结
## 7.5.4 隐式的类类型转换
### 笔记
* 抑制构造函数定义的隐式转换用explicit声明，实际上就是禁止编译器自作主张，未经人允许就调用explicit声明的构造函数
### 练习
* 7.48 
假定`Sales_data`的构造函数不是`explicit`的，则下述定义将执行什么样的操作？

```cpp
string null_isbn("9-999-9999-9");
Sales_data item1(null_isbn);
Sales_data item2("9-999-99999-9");
```
和sales_data的构造函数是不是explicit的无关，因为都是显示的调用入口参数为string类型的构造函数；而与string的构造函数有关，如果string声明了一个explicit的话，就不能隐式的把"9-999-99999-9"转化为一个string的对象，见上边的第三行。
* 7.49
对于`combine`函数的三种不同声明，当我们调用`i.combine(s)`时分别发生什么情况？其中`i`是一个`Sales_data`，而` s`是一个`string`对象。

```cpp
(a) Sales_data &combine(Sales_data); //字符串隐式转换到Sales_data类 能否成功取决于是否声明了explicit
(b) Sales_data &combine(Sales_data&);//string类型是const的，不能传递到非const的引用，这里其实并没有涉及到两步隐式转换的问题，因为如果把输入参数转化为const Sales_data，是完全正确的，编译器将string类型自动转化到Sales_data类型（这里把引用绑定到转化的临时Sales_data上并不是一层新的隐式转换，即引用和类型是等价的）。
(c) Sales_data &combine(const Sales_data&) const;//指向常量的this指针，不能在该函数内部对类的成员变量进行修改，这与函数的功能相悖。
```

若在输入参数为string的构造函数上加一explicit，那么就不能隐式的转换了。

## 7.5.6 字面值常量类
### 笔记
* constexpr可以保证在编译时即知道所声明的所有变量的值，且在整个程序的运行过程中值保持不变。可以极大的保证安全性，不像const一样只保证一定义了就不变（可能在每次程序的运行过程中随着环境的改变该值也会改变），constexpr可以在编译阶段即固化一个值。就像机器人三定律固化在机器人的硬件上一样，不因外部环境的改变而改变。
* constexpr声明构造函数时，要想在声明一个变量用到编译阶段检查的安全性，必须在声明一个对象时前边加constexpr。详见代码。书上有讲：constexpr构造函数用于生成constexpr对象以及constexpr函数的参数或返回类型，这就意味着，当用constexpr声明时，用的是constexpr那套东西，不用constexpr声明时就是正常的那一套。
### 练习
* 7.54
`Debug`中以 `set_` 开头的成员应该被声明成`constexpr` 吗？如果不，为什么？

不应该，如果被生成constexpr，则只能由return语句存在，且就被搞成一个隐式const了，不能修改成员数据。


