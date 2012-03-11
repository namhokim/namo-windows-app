/*
 모든 길은 인다이렉션(indirection, 간접 접근)으로 통한다

 코드에 있는 값을 바로 사용하지 말고 그 값을 가리키는 포인터를 사용하라
 */
#include <cstdio>
#include <cstring>

int main(int argc, const char *argv[])
{
	if (argc == 1) {
		printf("You need to provide a file name");
		return 1;
	}

	FILE *wordFile = fopen(argv[1], "r");
	char word[100];

	while (fgets(word, 100, wordFile)) {
		// strip off the trailing \n
		word[strlen(word) - 1] = '\0';

		printf("%s is %d characters long\n", word, strlen(word));
	}
	fclose(wordFile);

	return 0;
}