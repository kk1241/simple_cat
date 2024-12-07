CC=gcc
CC_FLAGS=-c -Wall -Werror -Wextra -std=c11
BUILD_NAME=s21_cat

# Основная цель для сборки
all: build

# Цель для сборки бинарного файла
build: s21_cat

# Пересборка (очистка и новая сборка)
rebuild: clean build

# Правила для компиляции бинарного файла
s21_cat: s21_cat.o
	$(CC) s21_cat.o -o $(BUILD_NAME)

# Правила для компиляции объектного файла
s21_cat.o: s21_cat.c
	$(CC) $(CC_FLAGS) s21_cat.c -o s21_cat.o

# Очистка
clean:
	rm -rf ./*.o
	rm -rf $(BUILD_NAME)

# Цель для тестов
test: $(BUILD_NAME)
	@mkdir -p tests
	@cp $(BUILD_NAME) tests/
	@cp $(BUILD_NAME).* tests/
	@cp Makefile tests/
	@cd tests && bash test_func_cat.sh
