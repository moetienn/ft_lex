NAME= lex

SRCS= srcs/main.c \
	   srcs/file_error.c \
	   srcs/ft_lex.c

OBJDIR= obj
OBJS= $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

CC= cc

HDR= includes/lex.h \
	 includes/parser.h 

CFLAGS= -Wall -Wextra -Werror

all: $(NAME)

$(OBJDIR)/%.o: %.c $(HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJDIR)/*.o

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all