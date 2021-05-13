NAME =		minishell
SRCS =		
LIBFT =		libft
LIBS = 		./libft/libft.a 
OBJS =		$(SRCS:.c=.o)
HEADER = 	./srcs/minishell.h
%.o:		%.c $(HEADER)
			$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) 


CC =		gcc
CFLAGS =	-Wall -Wextra -Werror 
RM =		rm -f
all :		$(NAME) 

$(NAME) :	$(OBJS) $(LIBFT)
			$(MAKE) -C $(LIBFT) all
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -I$(HEADER)
			@echo "minishell is ready"

$(LIBFT) :  libft/*.c libft/*.h
			$(MAKE) -C $(LIBFT) all
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -I$(HEADER)


clean :		
			$(MAKE) -C $(LIBFT) clean
			$(RM) $(OBJS)

fclean 	:	clean
			$(MAKE) -C $(LIBFT) fclean
			$(RM) $(NAME)

re :		fclean all

norm:
			norminette srcs/*.c srcs/minishell.h libft/*.c libft/libft.h

.PHONY :	all clean fclean re norm
