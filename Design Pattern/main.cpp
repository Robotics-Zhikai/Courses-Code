#include "main.h"

#include "simple_factory.h"

void main()
{
	try
	{
		simple_factory::test();
	}
	catch (exception d)
	{
		cout << d.what() << endl;
	}
	system("pause");
}