NAME= philo
CFLAGS= -Wall -Wextra -Werror
CC= gcc
SRC= philolets.c ft_atoi.c

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -Iincludes -o $(NAME)

fclean:
	rm -rf $(NAME)

re: fclean all