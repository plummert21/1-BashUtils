#define _GNU_SOURCE

#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  struct options opt = {0};
  struct paramOutput param = {0};
  if (!getOptions(argc, argv, &opt)) filesOutput(argc, argv, &opt, &param);
}

int getOptions(int argc, char **argv, struct options *opt) {
  char c;
  char valid_options[] = "beEntTsv"; /* valid options */
  struct option valid_options_long[4] = {
      /* long valid options */
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0}};
  while ((c = getopt_long(argc, argv, valid_options, valid_options_long,
                          NULL)) != -1) {
    if (c == 'b') opt->b = 1;
    if (c == 'e') {
      opt->E = 1;
      opt->v = 1;
    }
    if (c == 'E') opt->E = 1;
    if (c == 'n') opt->n = 1;
    if (c == 't') {
      opt->T = 1;
      opt->v = 1;
    }
    if (c == 'T') opt->T = 1;
    if (c == 's') opt->s = 1;
    if (c == 'v') opt->v = 1;
    if (c == '?') return 1;
  }
  if (opt->b) opt->n = 0;
  return 0;
}

void filesOutput(int argc, char **argv, struct options *opt,
                 struct paramOutput *param) {
  param->indexPrintFile = optind;
  param->countStr = 1;
  param->countNewLine = 1;
  param->NewLine = 1;
  while (param->indexPrintFile < argc) {
    if (access(argv[param->indexPrintFile], R_OK) == -1) {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n",
              argv[param->indexPrintFile]);
    } else {
      printSymbolsFromFile(argv, opt, param);
    }
    param->indexPrintFile++;
  }
}

void printSymbolsFromFile(char **argv, struct options *opt,
                          struct paramOutput *param) {
  FILE *fileStream;
  if ((fileStream = fopen(argv[param->indexPrintFile], "r")) != NULL) {
    char c[1];
    while ((c[0] = fgetc(fileStream)) != EOF) {
      param->countSymbol++;
      if (c[0] == '\n') {
        param->countNewLine++;
      } else {
        param->countNewLine = 0;
      }
      if (opt->s) {
        if (param->countNewLine < 3) printSymbol(c, opt, param);
      } else {
        printSymbol(c, opt, param);
      }
      if (c[0] == '\n') {
        param->NewLine = 1;
      } else {
        param->NewLine = 0;
      }
    }
    fclose(fileStream);
  } else
    fprintf(stderr, "s21_cat: %s: No read file\n", argv[param->indexPrintFile]);
}

void printNumberStr(const char *c, struct options *opt,
                    struct paramOutput *param) {
  if ((param->NewLine) && (opt->b) && (c[0] != '\n')) {
    printf("%6d\t", param->countStr++);
    param->NewLine = 0;
  }
  if ((param->NewLine) && (opt->n)) {
    printf("%6d\t", param->countStr++);
    param->NewLine = 0;
  }
}

void printSymbol(char *c, struct options *opt, struct paramOutput *param) {
  if (param->NewLine) printNumberStr(c, opt, param);
  if ((c[0] == '\t') && (opt->T))
    printf("^I");
  else if ((c[0] == '\n') && (opt->E))
    printf("$\n");
  else if (opt->v)
    v_print(c[0]);
  else
    printf("%c", c[0]);
  if ((c[0] != '\n') && (param->NewLine == 0)) param->NewLine = 1;
}

void v_print(unsigned char c) {
  if (c == 9 || c == 10) {
    printf("%c", c);
  } else if (c >= 32 && c < 127) {
    printf("%c", c);
  } else if (c == 127) {
    printf("^?");
  } else if (c >= 128 + 32) {
    printf("M-");
    (c < 128 + 127) ? printf("%c", c - 128) : printf("^?");
  } else {
    (c > 32) ? printf("M-^%c", c - 128 + 64) : printf("^%c", c + 64);
  }
}