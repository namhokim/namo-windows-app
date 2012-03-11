/*
 ��� ���� �δ��̷���(indirection, ���� ����)���� ���Ѵ�

 �ڵ忡 �ִ� ���� �ٷ� ������� ���� �� ���� ����Ű�� �����͸� ����϶�
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