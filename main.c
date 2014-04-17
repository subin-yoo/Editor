#include <stdio.h>

int main(int argc, char* argv[])
{
	fputs("\033[5;20H",stdout);
	fputs("\033[5;20HHello\n",stdout);

return 0;
}

