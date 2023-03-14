NAME = philo
CC = gcc
FLAGS = -Wall -Wextra -Werror -pthread
SRC = 	main.c\
		philo_utils.c\
		utils.c\
		des.c\

OBJ = 	$(SRC:.c=.o)


all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f *.o

fclean: clean
	rm -f philo

re: fclean all