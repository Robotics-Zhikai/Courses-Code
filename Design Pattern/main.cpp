#include "main.h"

#include "simple_factory.h"
#include "factory_method.h"

void main()
{
	try
	{
		factory_method::test();
		simple_factory::test();
	}
	catch (exception d)
	{
		cout << d.what() << endl;
	}
	system("pause");
}