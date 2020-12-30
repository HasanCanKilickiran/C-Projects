#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 4096
#define MAX_W_COUNT 1000
#define MAX_W_NAME 20

typedef struct wrd {
	char name[MAX_W_COUNT];
	int count;
} wrd_t;

int convert_to_lowercase(char *str)
{
	int ret = 0;

	/*TODO: test this function if it works in each senario */
	for (int i = 0; str[i]; i++)
		str[i] = tolower(str[i]);

	return ret;
}

int remove_nonalpha_chars(char *s)
{
	int ret = 0;

	for (; *s && !isalpha(*s); ++s)
			; // skip leading non-alpha chars
	if (*s == '\0') {
		printf("No alpha chars in the word\n");
		return ret; // there are no alpha characters
	}
	
	char *tail = s + strlen(s);
	for (; !isalpha(*tail); --tail)
			; // skip trailing non-alpha chars
		*++tail = '\0'; // truncate after the last alpha

		return ret;
}

int check_occurance(wrd_t* arr, char* name, int index, int* f_index)
{
	int ret = 0;
	int id = 0;
	*f_index = -1;
	while (id < index) {
		if (!strncmp(arr[id].name, name, MAX_W_NAME)) {
			*f_index = id;
			break;
		}
		id++;
	}

	return ret;
}

int srtcmp(const void* a, const void* b)
{
	wrd_t *wrd_a = (wrd_t*)a;
	wrd_t *wrd_b = (wrd_t*)b;

	return (wrd_b->count - wrd_a->count);
}

int main(void)
{
	FILE* fd;
	FILE* fout = NULL;
	char line[MAX_LINE];
	char *str;
	int ind = -1;
	int i = 0;
	int* f_index = &ind;
	wrd_t* w_arr;

	w_arr = (wrd_t*)malloc(MAX_W_COUNT * sizeof(wrd_t)); /*TODO: or linked list, research? */
	if (w_arr == NULL)
		return -1;

	if ((fd = fopen("source_file.txt", "r")) == NULL) {
		fprintf(stderr, "cannot open file!\n");
		exit(EXIT_FAILURE);
	}

	while (fgets(line, MAX_LINE, fd) != NULL) {
		if ((str = strchr(line, '\n')) != NULL)
			*str = '\0';
		for (str = strtok(line, " "); str != NULL; str = strtok(NULL, " ")) {
			/* remove non_alpha characters */
			if (remove_nonalpha_chars(str))
				printf("Failed to remove non-alpha characters from %s\n", str);

			/* make all the letters lowercase */
			if (convert_to_lowercase(str))
				printf("Failed to convert letters into lowercase\n");

			/* check occurance */
			check_occurance(w_arr, str, i, f_index); /* TODO: add error check */
			if (*f_index < 0) {
				/* if it's a new word add it to the list and set count to 1 */
				strncpy(w_arr[i].name, str, MAX_W_NAME);
				w_arr[i].count = 1;
				i++;
			}
			else {
				/* if it's not increase the count of that word */
				w_arr[*f_index].count += 1;
			}

		}

		/* If the new word is already encountered? So we need to keep each word and the occurence frequency in the memory.
		 * we do not know how long this will be, so dynamic memory allocation would be proper */
	}

	/*Order the array */
	qsort(w_arr, MAX_W_COUNT, sizeof(wrd_t), srtcmp);

	/* Open a file */
	/* TODO: check if the file is already exits, if it is, add a number to the end of the filename, and create */
	if ((fout = fopen("output.txt", "w")) == NULL) {
		fprintf(stderr, "cannot open file!\n");
		goto failure;
	}

	int j = 0;

	while (j < i) {
		fprintf(fout, "%-15s%d\n", w_arr[j].name, w_arr[j].count);
		printf("%-15s%d\n", w_arr[j].name, w_arr[j].count);
		j++;
	}
	fprintf(fout, "Total word count: %d\n", i);
	free(w_arr);
	fclose(fd);
	fclose(fout);

	return 0;
failure:
	return -1;
}

