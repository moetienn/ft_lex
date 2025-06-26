NAME= lex

SRCS= main.c \

OBJS= $(SRCS:.c=.o) 

CC= cc

HDR= includes/lex.h

CFLAGS= -Wall -Wextra -Werror

all: $(NAME)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

clean: 
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all