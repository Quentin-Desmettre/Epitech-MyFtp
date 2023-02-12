##
## EPITECH PROJECT, 2023
## myftp
## File description:
## Makefile
##

rwildc = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildc,$d/,$2))

SOURCEDIR = src

SRC = $(call rwildc,$(SOURCEDIR),*.c)

CC = gcc

OBJ = $(SRC:.c=.o)

NAME = myftp

CFLAGS = -Wall -Wextra -I ./include -g

all: $(NAME)

$(NAME):   $(OBJ)
	gcc -o $(NAME) $(OBJ) $(CFLAGS)

tests_run:
	cd tests && make && ./tests

clean:
	rm -f $(OBJ)
	find . -name "vgcore.*" -delete
	find . -name "*~" -delete
	find . -name "\#*" -delete

fclean:    clean
	rm -f $(NAME)

re:        fclean all
