NAME = philo
CC = gcc
FLAGS = -Wall -Wextra -Werror -pthread
SRC = 	main.c\
		philo_utils.c\
		utils.c\

OBJ = 	main.o\
		philo_utils.o\
		utils.o\

all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ)

$(OBJ):
	$(CC) $(FLAGS) -c $(SRC)

clean:
	rm -f *.o

fclean: clean
	rm -f philo

re: fclean all