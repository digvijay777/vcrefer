// KMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <stdio.h>

void kmp_init(const char *patn, int len, int *next)
{
	int i, j;

	assert(patn != NULL && len > 0 && next != NULL);
	next[0] = 0;
	for (i = 1, j = 0; i < len; i ++) {
		while (j > 0 && patn[j] != patn[i])
			j = next[j - 1];
		if (patn[j] == patn[i])
			j ++;
		next[i] = j;
	}
}

int kmp_find(const char *text, int text_len, const char *patn,
			 int patn_len, int *next)
{
	int i, j;

	assert(text != NULL && text_len > 0 && patn != NULL && patn_len > 0
		&& next != NULL);
	for (i = 0, j = 0; i < text_len; i ++ ) {
		while (j > 0 && text[i] != patn[j])
			j = next[j - 1];
		if (text[i] == patn[j])
			j ++;
		if (j == patn_len)
			return i + 1 - patn_len;
	}

	return -1;
}

int main(int argc, char *argv[])
{
	int *next;
	int i, pos, len = strlen(argv[2]);

	if (argc < 3) {
		printf("Usage: %s text pattern\n", argv[0]);
		return 1;
	}

	next = (int *)calloc(strlen(argv[2]), sizeof(int));
	kmp_init(argv[2], strlen(argv[2]), next);
	printf("next array:\n");
	for (i = 0; i < len; i ++)
		printf("\t%c", argv[2][i]);
	printf("\n");
	for (i = 0; i < len; i ++)
		printf("\t%d", next[i]);
	printf("\n");

	pos = kmp_find(argv[1], strlen(argv[1]), argv[2], strlen(argv[2]), next);
	printf("find result:\n");
	if (pos < 0) {
		printf("None found!\n");
	} else {
		printf("%s\n", argv[1]);
		for (i = 0; i < pos; i ++)
			printf(" ");
		printf("^\n");
	}

	return 0;
}
