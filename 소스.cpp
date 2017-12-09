#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<errno.h>
#include<ctype.h>
#include<io.h>
#include<iostream>

struct file
{
	int save;
	char title[150];
};

char* timeToString(struct tm *t);

void file(char voca[150]);

void main()
{
	char search[150];
	printf("찾고 싶은 단어는? : ");
	scanf("%s", search);
	file(search);
	system("pause");
}

void file(char voca[150])
{
	struct _finddata_t fd;
	struct file *title, temp;
	struct _stat buf;
	long handle;
	int i = 0, k, num = 0;
	int result = 1;
	char *st;
	char str[150] = { 0 };
	char str1[150] = { 0 };

	handle = _findfirst("C:\*.txt", &fd);
	if (handle == -1)
		return;
	
	while (result != -1)
	{
		result = _findnext(handle, &fd);
		num++;
	}

	title = (struct file *)calloc(num, sizeof(struct file));
	    result = 1;

		handle = _findfirst("C:\*.txt", &fd);

		while (result != -1)
		{
			strcpy(title[i].title, fd.name);
			result = _findnext(handle, &fd);
			i++;
		}

		_findclose(handle);

		strcpy(str1, voca);

		for (i = 0; i < num; i++)
		{
			FILE * fp = fopen(title[i].title, "rt");

			while (!feof(fp))
			{
				fscanf(fp, "%s", str);

				st = &str[0];

				if (isalpha(str[0]) == 0)
				{
					st++;
				}

				if (_strnicmp(st, str1, strlen(str1)) == 0)
				{
					title[i].save++;
				}
			}
			fclose(fp);
		}

		for (i = 0; i < num; i++)
		{
			for (k = 0; k < num - 1; k++)
			{
				if (title[k].save < title[k + 1].save)
				{
					temp = title[k];
					title[k] = title[k + 1];
					title[k + 1] = temp;
				}
			}
		}
		
		for(i = 0; i < num; i++)
		{
			if (_stat(title[i].title, &buf) != 0)
			{
				switch (errno)
				{
				case ENOENT:
					fprintf(stderr, "File %s not found.\n", title[i].title); break;
				case EINVAL:
					fprintf(stderr, "Invalid parameter to _stat.\n"); break;
				default:
					fprintf(stderr, "Unexpected error in _stat.\n");
				}
			}

			else
			{
				printf("파일 이름 : %s, 단어 개수 : %d, 수정된 날짜 : %s 누적 개수 : %d\n", title[i].title, title[i].save, timeToString(localtime(&buf.st_mtime)), i + 1);
			}
		}
		free(title);
	}

char* timeToString(struct tm *t)
{
	static char s[20];

	sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
		        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		        t->tm_hour, t->tm_min, t->tm_sec
	);

	return s;
}