#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>

struct options {
  int b;
  int E;
  int n;
  int T;
  int s;
  int v;
};

struct paramOutput {
  int indexPrintFile;
  int NewLine;
  int countStr;
  int countSymbol;
  int countNewLine;
  int lastNew;
};

int getOptions(int argc, char **argv, struct options *opt);
void filesOutput(int arc, char **argv, struct options *opt,
                 struct paramOutput *param);
void printSymbolsFromFile(char **argv, struct options *opt,
                          struct paramOutput *param);
void printNumberStr(const char *c, struct options *opt,
                    struct paramOutput *param);
void printSymbol(char *c, struct options *opt, struct paramOutput *param);
void v_print(unsigned char c);

#endif