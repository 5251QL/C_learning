#include <stdio.h>
#define MESSAGE "HELLO WOELD"

void say_hello(int times) {
	if (times <= 0) {
		printf("error:time must be positive.\n");
		return;
	}
	for (int i = 0; i < times; i++) {
		printf("hello %d\n", i + 1);
	}
}
int main() {
	say_hello(3);
	say_hello(-1);
	say_hello(2);
		return 0;
}