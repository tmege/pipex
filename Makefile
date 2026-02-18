NAME    = pipex
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRCS    = $(SRC_DIR)/pipex.c $(SRC_DIR)/utils.c $(SRC_DIR)/find_path.c
OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I $(INC_DIR) -I .
LIBFT   = libft/libft.a

#colors#

GREEN   = \033[0;32m
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
CYAN    = \033[1;36m
RESET   = \033[0m

#rules#

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(BLUE)🚀 Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)
	@echo "$(GREEN)✅ Build successful!$(RESET)"

$(LIBFT):
	@echo "$(CYAN)🔧 Compiling libft...$(RESET)"
	@make -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/pipex.h Makefile
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)📦 Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@make -C libft clean
	@echo "$(CYAN)🧹 Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean
	@echo "$(CYAN)🧹 Binary removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re
