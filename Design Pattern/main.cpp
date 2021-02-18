#include "main.h"

#include "simple_factory.h"
#include "factory_method.h"
#include "abstract_factory.h"

void main()
{
	try
	{
		abstract_factory::test();
		factory_method::test();
		simple_factory::test();
	}
	catch (exception d)
	{
		cout << d.what() << endl;
	}
	system("pause");
}