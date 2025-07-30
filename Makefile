LEX_DIR = lex
LIBL_DIR = libl
OUTPUT_BINARY = scanner
LIBL_BINARY = $(LIBL_DIR)/libl.a
LEX_YY_C = lex.yy.c

.PHONY: all lex libl clean fclean re run

all: $(OUTPUT_BINARY)

$(OUTPUT_BINARY): $(LEX_YY_C) libl
	cc -o $(OUTPUT_BINARY) $(LEX_YY_C) $(LIBL_BINARY)

$(LEX_YY_C): lex
	@$(LEX_DIR)/lex easy.l

lex:
	@$(MAKE) -C $(LEX_DIR)

libl:
	@$(MAKE) -C $(LIBL_DIR)

run: all
	@./$(OUTPUT_BINARY) easy.l

clean:
	@$(MAKE) -C $(LEX_DIR) clean
	@$(MAKE) -C $(LIBL_DIR) clean
	@rm -f $(LEX_YY_C)

fclean:
	@$(MAKE) -C $(LEX_DIR) fclean
	@$(MAKE) -C $(LIBL_DIR) fclean
	@rm -f $(OUTPUT_BINARY) $(LEX_YY_C)

re: fclean all