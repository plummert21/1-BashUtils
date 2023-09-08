#include "s21_grep.h"

int main(int argc, char *argv[]) {
  arg args = {0};
  args.regex = NULL;
  args.currentFile = NULL;

  int opt;
  const struct option long_options[] = {
      {"ignore-case", no_argument, 0, 'i'},
      {"invert-match", no_argument, 0, 'v'},
      {"count", no_argument, 0, 'c'},
      {"files-with-matches", no_argument, 0, 'l'},
      {"line-number", no_argument, 0, 'n'},
      {"no-messages", no_argument, 0, 's'},
      {"regexp", 1, 0, 'e'},
      {"file", 1, 0, 'f'},
      {"only-matching", no_argument, 0, 'o'},
      {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", long_options, NULL)) !=
         -1) {
    if (opt == 'i')
      args.i = 1;
    else if (opt == 'v')
      args.v = 1;
    else if (opt == 'c')
      args.c = 1;
    else if (opt == 'l')
      args.l = 1;
    else if (opt == 'n')
      args.n = 1;
    else if (opt == 'h')
      args.h = 1;
    else if (opt == 's')
      args.s = 1;
    else if (opt == 'e') {
      args.e = 1;
      AddRegex(&args.regex, optarg);
    } else if (opt == 'f') {
      args.f = 1;
      LoadPattern(&args.regex, optarg);
    } else if (opt == 'o')
      args.o = 1;
    else {
      printf("%s: option requires an argument -- 'e'\n", argv[0]);
      printf("Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
      printf("Try '%s --help' for more information.\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  if (args.l) {
    args.c = 0;
    args.o = 0;
  }
  if (args.c) args.o = 0;

  if (args.v && args.o) {
    if (args.regex != NULL) free(args.regex);
    exit(EXIT_FAILURE);
  }

  if (optind < argc) {
    if (!args.e && !args.f) AddRegex(&args.regex, argv[optind++]);
    while (optind < argc && args.regex != NULL) {
      if (optind + 1 < argc) args.fp = 1;
      args.currentFile = argv[optind++];
      if (!access(args.currentFile, 0))
        searchPrint(&args);
      else
        fprintf(stderr, "grep: %s: No such file or directory\n",
                args.currentFile);
    }
  }
  if (args.regex != NULL) free(args.regex);
}

void searchPrint(arg *args) {
  FILE *streamFile = fopen(args->currentFile, "r");
  if (streamFile != NULL) {
    regex_t re;
    if (regcomp(&re, args->regex, args->i ? REG_ICASE : 0 | REG_NEWLINE) != 0)
      exit(1);
    args->lineNubmer = 0;
    args->countFind = 0;
    char *line = NULL;
    size_t buf;
    int res = 0;
    while (getline(&line, &buf, streamFile) != -1) {
      int resultReg;
      args->lineNubmer++;
      regmatch_t pmatch[MAX_GROUPS];
      resultReg = regexec(&re, line, MAX_GROUPS, pmatch, 0);
      char *str = line;
      while (args->o && !resultReg) {
        int strLen = pmatch[0].rm_eo - pmatch[0].rm_so;
        char *tmp = NULL;
        if (strLen > 0) tmp = malloc(strLen + 5);
        sprintf(tmp, "%.*s\n", strLen, str + pmatch[0].rm_so);

        if (!args->c) res = print(args, resultReg, tmp);
        str += pmatch[0].rm_eo;
        free(tmp);
        resultReg = regexec(&re, str, MAX_GROUPS, pmatch, 0);
      }
      if (resultReg == args->v) args->countFind++;
      if (!args->c && !args->o) res = print(args, resultReg, line);
      if (res) break;
    }

    if (args->c)
      (!args->h && args->fp)
          ? printf("%s:%d\n", args->currentFile, args->countFind)
          : printf("%d\n", args->countFind);
    regfree(&re);
    free(line);
    fclose(streamFile);
  }
}
int print(arg *args, int status, char *line) {
  if (status == args->v) {
    if (args->fp && !args->h && !args->l) printf("%s:", args->currentFile);
    if (args->n && !args->l) printf("%d:", args->lineNubmer);
    if (args->l) {
      printf("%s\n", args->currentFile);
      return 1;
    }
    if (!args->c) printf("%s", line);
    AddLine(args, line);
  }
  return 0;
}
void AddLine(arg *args, char *line) {
  if (!args->c && line[strlen(line) - 1] != '\n') printf("\n");
}

void AddRegex(char **regex, char *addStr) {
  char *tmp =
      malloc((*regex ? strlen(*regex) : 0) + (addStr ? strlen(addStr) : 0) + 3);
  sprintf(tmp, "%s%s%s", (*regex ? *regex : ""), (*regex ? "\\|" : ""), addStr);
  *regex = realloc(*regex, strlen(tmp) + 1);
  strcpy(*regex, tmp);
  free(tmp);
}
void LoadPattern(char **regex, char *filePath) {
  FILE *streamFile = fopen(filePath, "r");
  if (streamFile != NULL) {
    char *line = NULL;
    size_t buf;

    while (getline(&line, &buf, streamFile) != -1) {
      if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
      AddRegex(regex, line);
    }
    if (line != NULL) free(line);
  }
}