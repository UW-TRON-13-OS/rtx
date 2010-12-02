#include "utils.h"

int main()
{
	cout<<"Hello \n\n";

	char buffer[1024];
	int i = 2540;
	char j = 'r';
	char k[] = "Hello\0";

	void * params[] = { &i, &j, &k, NULL }; // Null terminated array of void poitners

	rtx_spritf(buffer, "Integer is %9i. \n The charatcer is %5c. \n And random location is %e. \n The string is %s. \n", params);

	cout<<"\n\n\n The buffer result is: \n\n"<<buffer;


	char t[50];
	rtx_strcpy(t, "THIS IS A TEST FOOL! \n\n\0");

	cout<<"\n\n Test two!!! \n\n"<<t;

	system("PAUSE");
	return 0;
}