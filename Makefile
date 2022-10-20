NAME = ft_ping

SRC_DIR = ./src
INC_DIR = ./inc
OBJ_DIR = ./obj

SRC = main.c parsing.c ping.c pong.c output.c utilities.c
vpath %.c $(SRC_DIR)
INC = ft_ping.h
vpath %.h $(INC_DIR)
OBJ = $(addprefix  $(OBJ_DIR)/,$(SRC:%.c=%.o))
vpath %.c $(SRC_DIR)

IFLAGS = $(foreach inc, $(INC_DIR),-I $(inc))
CFLAGS = -Wall -Wextra -Werror -g#-fsanitize=address

CC = gcc
RM = rm -rf
MKD = mkdir -p
CLR_ON = \033[1;32m
CLR_OFF = \033[0m

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@$(MKD) $(OBJ_DIR)
	@echo -n "* "
	$(CC) $(CFLAGS) -c $(IFLAGS) $< -o $@

$(NAME): $(OBJ)
	@echo -n "* "
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(OBJ) -lm
	@echo "$(CLR_ON)Compilation completed.$(CLR_OFF)"

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(CLR_ON)Cleaning completed.$(CLR_OFF)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(CLR_ON)Fcleaning completed.$(CLR_OFF)"

re: fclean all

.PHONY: all clean fclean re