# Variables
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = philosophers.c
OBJS = $(SRCS:.c=.o)
NAME = philosophers
HEADER = philosophers.h

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# Phony targets
.PHONY: all clean fclean re
