NAME = philo

CFLAGS = -g -Wall -Wextra -Werror -pthread

RM = rm -rf

SRCS = 	main.c utils.c

$(NAME) :
	gcc $(CFLAGS) $(SRCS) -o $(NAME)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

clean :
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re