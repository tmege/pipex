NAME    = pipex
SRCS    = pipex.c utils.c find_path.c
OBJS    = $(SRCS:.c=.o)
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
LIBFT   = libft/libft.a

#colors#

GREEN   = \033[0;32m
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
CYAN    = \033[1;36m
RESET   = \033[0m

#rules#

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)🔧 Compilation de la libft...$(RESET)"
	@make -C libft
	@echo "$(BLUE)🚀 Linking de $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)
	@echo "$(GREEN)✅ Compilation réussie !$(RESET)"

%.o: %.c Makefile pipex.h libft/includes/libft.h libft/includes/ft_printf.h libft/Makefile libft/srcs/ft_printf/Makefile
	@echo "$(YELLOW)📦 Compilation de $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make -C libft clean
	@echo "$(CYAN)🧹 Fichiers objets nettoyés$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean
	@echo "$(CYAN)🧹 Binaire supprimé$(RESET)"
	@echo "$(BLUE)🚀 Bibliothèque libft nettoyée$(RESET)"

re: fclean all

.PHONY: all clean fclean re

