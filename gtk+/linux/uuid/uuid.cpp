#include <iostream>
#include <stdio.h>
#include <uuid/uuid.h>

int main()
{
	uuid_t		uu;
	
	uuid_generate( uu );
	std::cout << uu << std::endl;
	
	for(int i = 0; i < 16; i++)
	{
		printf("%X ", ((char *)uu)[i]);
	}
	printf("\n");
	return 0;
}
