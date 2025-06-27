LIBNAME= libl.a

CC= cc
CFLAGS= -Wall -Wextra -Werror

AR= ar rcs

SRCS= srcs/libl.c
OBJS= $(SRCS:.c=.o)
INCLUDES= -Iincludes

all : $(LIBNAME)
$(LIBNAME): $(OBJS) includes/libl.h
	$(AR) $(LIBNAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(LIBNAME)

re: fclean all