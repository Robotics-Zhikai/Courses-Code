#include "main.h"

#include "simple_factory.h"
#include "factory_method.h"
#include "abstract_factory.h"
#include "singleton_pattern.h"
void main()
{
	try
	{
		singleton_pattern::test();
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