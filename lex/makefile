NAME= lex

SRCS= srcs/main.c \
	   srcs/ft_lex.c

UTILSSRCS= srcs/utils/utils.c \
			srcs/utils/parser_utils.c \
			srcs/utils/file_error.c

PARSER = srcs/parser/parse_file.c \
			srcs/parser/init_parser.c

DECLARATION = srcs/declaration/first_section.c \
				srcs/declaration/utils_macros.c \
				srcs/declaration/macros.c

RULES = srcs/rules/rules.c \
			srcs/rules/utils_rules.c

OBJDIR= obj
OBJS= $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
UTILSOBJS= $(addprefix $(OBJDIR)/, $(UTILSSRCS:.c=.o))
PARSEROBJS= $(addprefix $(OBJDIR)/, $(PARSER:.c=.o))
DECLARATION_OBJS= $(addprefix $(OBJDIR)/, $(DECLARATION:.c=.o))
RULES_OBJS= $(addprefix $(OBJDIR)/, $(RULES:.c=.o))

CC= cc

HDR= includes/lex.h \
	 includes/parser.h \
	 includes/declaration/macros.h \
	 includes/declaration/first_section.h

CFLAGS= -Wall -Wextra -Werror -g

all: $(NAME)

$(OBJDIR)/%.o: %.c $(HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(UTILSOBJS) $(PARSEROBJS) $(DECLARATION_OBJS) $(RULES_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(UTILSOBJS) $(PARSEROBJS) $(DECLARATION_OBJS) $(RULES_OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJDIR)/*.o

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all