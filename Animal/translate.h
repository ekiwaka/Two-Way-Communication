#pragma once
char* translate(char str[])
{
	static char reversed[1024];
	memset(reversed, 0, sizeof(reversed));

	for (int i = strlen(str) - 2; i >= 0; i--)
	{
		reversed[strlen(str) - 2 - i] = str[i]; // -2 since last character in the buffer is the return key
	}
	return reversed;
};