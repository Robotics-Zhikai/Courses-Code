#pragma once
#include "main.h"
using namespace std;

class shape
{
public:
	using coord = pair<double, double>;
	shape(const string& name,const coord& coord,size_t dim) :Coordinate(coord), dimension(dim),name(name) {}
	virtual ~shape() = default;
	virtual double area() const = 0; //纯虚函数，抽象基类没有面积
	virtual double volume() const = 0;//纯虚函数，抽象基类没有体积
	virtual void coutname()const
	{
		cout << name << endl;
	}
	virtual void debug()const  //输出类的成员
	{
		cout << "Coordinate:" << Coordinate.first << " " << Coordinate.second << endl;
		cout << "dimension:" << dimension << endl;
		cout << "name:" << name << endl;
	}
private:
	pair<double, double> Coordinate = { 0,0 };
	size_t dimension ;
	string name;
};

class shape_2D:public shape
{
public:
	shape_2D(const string& name,const coord& coord) :shape(name,coord, 2) {}
	shape_2D() :shape("randname", make_pair(0, 0), 2) {}
	double volume()const final  //如果标记为final的虚函数弄成纯虚函数，那么所有由shape_2D派生出去的类都是抽象类，不能定义对象
	{
		return 0;
	}
	double area()const = 0;
	~shape_2D() = default;
};

class shape_3D :public shape
{
public:
	shape_3D(const string &name, const coord& coord) :shape(name, coord, 3) {}
	shape_3D() :shape("randname", make_pair(0, 0), 3) {}
	double volume() const = 0;
	double area() const = 0;
	~shape_3D() = default;
};

class circle :public shape_2D
{
public:
	circle(const string&name, const coord& coord, double r) :shape_2D(name, coord), radius(r) {}
	circle() = default;
	
	double area()const override
	{
		return pi*radius*radius;
	}
	void coutname()const override
	{
		cout << "circle name:";
		shape::coutname();
	}
	void debug()const override
	{
		shape::debug();
		cout << "radius:" << radius << endl;
	}
	~circle() = default;
private:
	double radius = 0;
};

class rectangle :public shape_2D
{
public:
	rectangle(const string&name, const coord& coord, double l, double h) :shape_2D(name, coord),length(l),height(h) {}
	rectangle() = default;
	double area()const override
	{
		return length*height;
	}
	void coutname()const override
	{
		cout << "rectangle name:";
		shape::coutname();
	}
	void debug()const override
	{
		shape::debug();
		cout << "length:" << length << endl;
		cout << "height:" << height << endl;
	}
	~rectangle() = default;
private:
	double length = 0;
	double height = 0;
};

class sphere:public shape_3D
{
public:
	sphere(const string& name,const coord& coord,double r) :shape_3D(name,coord),radius(r) {}
	sphere() = default;
	double volume()const override
	{
		return 4.0 / 3.0*pi*radius*radius;
	}
	double area()const override
	{
		return 4 * pi*radius*radius;
	}
	void coutname()const override
	{
		cout << "sphere name:";
		shape::coutname();
	}
	void debug()const override
	{
		shape::debug();
		cout << "radius:" << radius << endl;
	}
private:
	double radius = 0;
};

class cone :public shape_3D //圆锥
{
public:
	cone(const string& name, const coord& coord, double r,double h) :shape_3D(name, coord),radius(r),height(h) {}
	cone() = default;
	double volume()const override
	{
		return 1 / 3.0*pi*radius*radius;
	}
	double area()const override
	{
		cout << "圆锥的表面积怎么算？" << endl;
		return -1;
	}
	void coutname()const override
	{
		cout << "cone name:";
		shape::coutname();
	}
	void debug()const override
	{
		shape::debug();
		cout << "radius:" << radius << endl;
		cout << "height:" << height << endl;
	}
private:
	double radius = 0;
	double height = 0;
};

void printdebug(ostream& os, shape& sp)
{
	sp.debug();
}