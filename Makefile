# Main Makefile

# Variables
LEX_DIR = lex
LIBL_DIR = libl

# Targets
.PHONY: all lex libl clean fclean re

all: lex libl

lex:
	@$(MAKE) -C $(LEX_DIR)

libl:
	@$(MAKE) -C $(LIBL_DIR)

clean:
	@$(MAKE) -C $(LEX_DIR) clean
	@$(MAKE) -C $(LIBL_DIR) clean

fclean:
	@$(MAKE) -C $(LEX_DIR) fclean
	@$(MAKE) -C $(LIBL_DIR) fclean

re: fclean all