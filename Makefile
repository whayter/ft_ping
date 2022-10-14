NAME = ft_ping

SRC_DIR = ./src
INC_DIR = ./inc
OBJ_DIR = ./obj

SRC = main.c output.c utilities.c
vpath %.c $(SRC_DIR)
INC = ft_ping.h
vpath %.h $(INC_DIR)
OBJ = $(addprefix  $(OBJ_DIR)/,$(SRC:%.c=%.o))
vpath %.c $(SRC_DIR)

IFLAGS = $(foreach inc, $(INC_DIR),-I$(inc))
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

CC = gcc
RM = rm -rf
MKD = mkdir -p
CLR_ON = \033[1;32m
CLR_OFF = \033[0m

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@($(MKD) $(OBJ_DIR))
	@($(CC) $(CFLAGS) -c $(IFLAGS) $< -o $@)

$(NAME): $(OBJ)
	@(echo "$(CLR_ON)Compiling...$(CLR_OFF)")
	@($(CC) $(CFLAGS) $(IFLAGS) -o $@ $(OBJ))
	@(echo "$(CLR_ON)Done.$(CLR_OFF)")

clean:
	@(echo "$(CLR_ON)Cleaning...$(CLR_OFF)")
	@($(RM) $(OBJ_DIR))
	@(echo "$(CLR_ON)Done.$(CLR_OFF)")

fclean: clean
	@(echo "$(CLR_ON)Fcleaning...$(CLR_OFF)")
	@($(RM) $(NAME))
	@(echo "$(CLR_ON)Done.$(CLR_OFF)")

re: fclean all

.PHONY: all clean fclean re