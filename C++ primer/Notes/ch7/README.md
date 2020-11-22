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

