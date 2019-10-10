#include <stdio.h>

int main(void) {
	char buffer[5];
	int count = 13;
	int result;
	result = sprintf(buffer, "%02d\n\r", count++);
	printf("%s", buffer);
	return 1;
}
