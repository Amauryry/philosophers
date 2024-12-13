# Variables
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCDIR = src
OBJDIR = obj
SRCS = $(SRCDIR)/philosophers.c $(SRCDIR)/subsidiary_f.c $(SRCDIR)/sub_f_2.c $(SRCDIR)/activities.c $(SRCDIR)/setup_clean.c $(SRCDIR)/parsing.c $(SRCDIR)/activities_2.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
NAME = philo
HEADER = philosophers.h

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADER)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@rmdir $(OBJDIR) 2>/dev/null || true

fclean: clean
	rm -f $(NAME)

re: fclean all

# Phony targets
.PHONY: all clean fclean re