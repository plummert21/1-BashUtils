#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE
#define MAX_GROUPS 1
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int fp;  // multiFiles
  int countFind;
  int lineNubmer;
  char *regex;
  char *currentFile;
} arg;

void AddRegex(char **, char *);
void LoadPattern(char **, char *);
void searchPrint(arg *);
void AddLine(arg *, char *);
int print(arg *, int, char *);

#endif