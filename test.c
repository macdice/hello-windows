#include <locale.h>
#include <stdio.h>

int
main()
{
	_locale_t loc1 = _create_locale(LC_ALL, "C");
	_locale_t loc2 = _create_locale(LC_ALL, "C");
	printf("%p %p\n", loc1, loc2);
}
