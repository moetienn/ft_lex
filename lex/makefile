NAME= lex

SRCS= srcs/main.c \
	   srcs/ft_lex.c

UTILSSRCS= srcs/utils/utils.c \
			srcs/utils/parser_utils.c \
			srcs/utils/file_error.c

PARSER = srcs/parser/parse_file.c \
			srcs/parser/init_parser.c

FIRST_SECTION = srcs/first_section/first_section.c \
				srcs/first_section/macros.c

OBJDIR= obj
OBJS= $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
UTILSOBJS= $(addprefix $(OBJDIR)/, $(UTILSSRCS:.c=.o))
PARSEROBJS= $(addprefix $(OBJDIR)/, $(PARSER:.c=.o))
FIRST_SECTION_OBJS= $(addprefix $(OBJDIR)/, $(FIRST_SECTION:.c=.o))

CC= cc

HDR= includes/lex.h \
	 includes/parser.h \
	 includes/first_section/macros.h \
	 includes/first_section/first_section.h

CFLAGS= -Wall -Wextra -Werror -g

all: $(NAME)

$(OBJDIR)/%.o: %.c $(HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(UTILSOBJS) $(PARSEROBJS) $(FIRST_SECTION_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(UTILSOBJS) $(PARSEROBJS) $(FIRST_SECTION_OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJDIR)/*.o

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all