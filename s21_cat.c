#include "s21_cat.h"

int main(int argc, char **argv) {
  catFlags flags;  // Структура для хранения флагов
  int error_flag = parse_flags(argc, argv, &flags);  // Обрабатываем флаги

  if (error_flag) {
    return 1;
  }

  // Выводим файлы с учётом флагов
  for (int i = optind; i < argc; ++i) {
    FILE *file_name = fopen(argv[i], "r");
    if (file_name == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
    } else {
      print_text(file_name, flags);  // Выводим содержимое файла с флагами
      fclose(file_name);
    }
  }

  return 0;
}

// Функция для обработки флагов
int parse_flags(int argc, char **argv, catFlags *flags) {
  int opt;
  int error_flag = 0;

  // Инициализация флагов в структуре
  flags->flag_e = 0;
  flags->flag_n = 0;
  flags->flag_b = 0;
  flags->flag_s = 0;
  flags->flag_t = 0;
  flags->flag_v = 0;

  // Массив длинных опций
  static struct option long_options[] = {
      {"number", no_argument, 0, 'n'},           // --number
      {"number-nonblank", no_argument, 0, 'b'},  // --number-nonblank
      {"show-ends", no_argument, 0, 'e'},        // --show-ends
      {"squeeze-blank", no_argument, 0, 's'},    // --squeeze-blank
      {"show-tabs", no_argument, 0, 't'},
      {0, 0, 0, 0}  // Конец списка опций
  };

  // Обработка флагов
  while ((opt = getopt_long(argc, argv, "ebnstv", long_options, NULL)) != -1) {
    switch (opt) {
      case 'e':  // Отображение символа конца строки как $
        flags->flag_e = 1;
        flags->flag_v = 1;  // Флаг -e также включает флаг -v
        break;
      case 'b':  // Нумерация непустых строк
        flags->flag_b = 1;
        break;
      case 'n':  // Нумерация всех строк
        flags->flag_n = 1;
        break;
      case 's':  // Сжатие пустых строк
        flags->flag_s = 1;
        break;
      case 't':  // Печать табуляций как ^I
        flags->flag_t = 1;
        flags->flag_v = 1;  // Флаг -t также включает флаг -v
        break;
      case 'v':  // Отображение управляющих символов как ^X
        flags->flag_v = 1;
        break;
      default:  // Обработка ошибок
        fprintf(stderr, "cat: invalid option -- %c\n", opt);
        error_flag = 1;
        break;
    }
  }

  return error_flag;  // Возвращаем флаг ошибки (0 — если ошибок нет)
}

// Функция для вывода файла с учётом флагов
void print_text(FILE *file_name, catFlags flags) {
  int count_str = 1;   // Нумерация строк
  int print_numb = 1;  // Флаг для вывода номера строки
  int count_s = 1;  // Счётчик пустых строк для флага -s
  char c;

  while ((c = fgetc(file_name)) != EOF) {
    if (flags.flag_s && c == '\n')
      count_s++; 
    else
      count_s = 0;
    if ((flags.flag_n && print_numb && !flags.flag_b && count_s < 3) ||
        (flags.flag_b && c != '\n' && print_numb)) {
      printf("%6d\t", count_str);
      count_str++;
      print_numb = 0;
    }
    if (flags.flag_t && c == '\t') {
      putchar('^');
      c = 'I';
    }
    if (flags.flag_v && ((c >= 0 && c <= 8) || (c >= 11 && c <= 31))) {
      putchar('^');
      c += 64;
    }
    if (flags.flag_v && c == 127) {
      putchar('^');
      c -= 64;
    }
    if (count_s < 3) {
      if (flags.flag_e && c == '\n') {
        putchar('$');
      }
      putchar(c);
    }
    if (c == '\n') {
      print_numb = 1;
    }
  }
}

