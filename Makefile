# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/30 19:18:26 by tkashi            #+#    #+#              #
#    Updated: 2024/05/13 18:38:10 by tkashi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := philo
BONUS_NAME :=
OBJECT_DIR := obj
INCLUDE_DIR := includes
SRC_DIR := srcs
BONUS_DIR :=
FILES := main.c \
		init_philo.c \
		str_utils.c \
		utils.c \
		supervisor_utils.c \
		thread_mutex_helper.c \
		worker_routines.c \
		eating_routine.c \
		exit_utils.c \
		utils_extra.c
SRCS := $(addprefix $(SRC_DIR)/, $(FILES))
BONUS_FILES :=
CFLAGS := -Wall -Wextra -Werror -g3
# DEBUG=1 make re to compile with debug flags
ifdef DEBUG
    CFLAGS += -DDEBUG=1 -g
endif
IFLAGS := -I$(INCLUDE_DIR)
LFLAGS :=
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o))
BONUS_OBJ :=  $(addprefix $(BONUS_DIR)/,$(BONUS_FILES:.c=.o))
CC := gcc
DATA_RACE := valgrind --tool=drd
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	LEAK_TOOL := leaks -atExit --
else
	LEAK_TOOL := valgrind --leak-check=full
	CFLAGS += -pthread
	LFLAGS += -lpthread
endif
ARGS ?= 5 800 200 200
END := "\033[0m"
WHITE := "\033[1;37m"
NC := "\033[0m"
PINK := "\033[1;35m"
GREEN := "\033[32m"
BOLD := "\033[1m"
L_PURPLE := "\033[38;5;55m"

.PHONY: all
all: $(NAME) display_ascii

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(NAME)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


.PHONY: bonus
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(NAME)
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(OBJS_WITHOUT_MAIN) $(LFLAGS) -o $(BONUS_NAME)

.PHONY: re
re: fclean all

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -f $(BONUS_OBJ)

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

# Run the program with the arguments specified in the ARGS variable
# Example: make leak ARGS="5 800 200 200 3"
.PHONY: leak
leak: $(NAME)
	$(LEAK_TOOL) ./$(NAME) $(ARGS)

.PHONY: data_race
data_race: $(NAME)
	$(DATA_RACE) ./$(NAME) $(ARGS)

.PHONY: display_ascii
display_ascii:
	@echo $(BOLD)$(PINK); cat srcs/ascii_art.txt; echo $(END)
	@echo $(BOLD)$(L_PURPLE) ✨$(NAME)✨ $(GREEN)is ready 🎉 $(END)
