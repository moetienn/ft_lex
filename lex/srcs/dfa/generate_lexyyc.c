#include "../../includes/lex.h"

static void write_dfa_alphabet(FILE *file, const char *alphabet, int alphabet_size)
{
    fprintf(file, "const char DFA_ALPHABET[] = {");
    for (int i = 0; i < alphabet_size; ++i)
	{
        if (i > 0)
			fprintf(file, ", ");
        if (alphabet[i] >= 32 && alphabet[i] < 127)
            fprintf(file, "'%c'", alphabet[i]);
        else
            fprintf(file, "%d", (unsigned char)alphabet[i]);
    }
    fprintf(file, "};\n#define DFA_ALPHABET_SIZE %d\n\n", alphabet_size);
}

static void	write_dfa_states(FILE *file, t_lex *lex, const char *alphabet, int alphabet_size)
{
    fprintf(file, "DFA_State DFA_STATES[] = {\n");
    for (int s = 0; s < lex->dfa->state_count; ++s)
    {
        t_dfa_state *state = lex->dfa->states[s];
        fprintf(file, "    { %d, {", state->is_accept ? 1 : 0);
        for (int a = 0; a < alphabet_size; ++a)
        {
            char sym = alphabet[a];
            int dest = -1;
            if (state->transitions) // Check if transitions is not NULL
            {
                for (int t = 0; t < state->transition_count; ++t)
                {
                    if (state->transitions[t] && state->transitions[t]->symbol == sym) // Check each transition
                        dest = state->transitions[t]->next_state->id;
                }
            }
            fprintf(file, "%d", dest);
            if (a < alphabet_size - 1) fprintf(file, ", ");
        }
        printf("state->action_id %d\n", state->action_id);
        fprintf(file, "}, %d }", state->action_id);
        if (s < lex->dfa->state_count - 1)
            fprintf(file, ",\n");
        else
            fprintf(file, "\n");
    }
    fprintf(file, "};\n#define DFA_STATE_COUNT %d\n\n", lex->dfa->state_count);
}

static void	write_dfa_symbol_index_function(FILE *file)
{
    fprintf(file,
        "int dfa_symbol_index(char c) {\n"
        "    for (int i = 0; i < DFA_ALPHABET_SIZE; ++i)\n"
        "        if (DFA_ALPHABET[i] == c) return i;\n"
        "    return -1;\n"
        "}\n\n");
}

static void write_yy_action_function(FILE *file, t_lex *lex)
{
    fprintf(file, "void yy_action(int action_id) {\n");
    fprintf(file, "    switch(action_id) {\n");
    for (size_t i = 0; i < lex->rules_list.count; ++i) {
        fprintf(file, "        case %ld: %s; break;\n", i+1, lex->rules_list.list[i].action);
    }
    fprintf(file, "    }\n}\n\n");
}

static void write_yylex_function(FILE *file)
{
    fprintf(file,
        "int yylex(void) {\n"
        "    int c, idx, state, last_accept, last_accept_pos, i;\n"
        "    char buffer[4096];\n"
        "    int bufpos = 0, input_char;\n\n"
        "    while (1) {\n"
        "        bufpos = 0;\n"
        "        state = 0;\n"
        "        last_accept = -1;\n"
        "        last_accept_pos = -1;\n\n"
        "        while ((input_char = input()) > 0) {\n"
        "            c = input_char;\n"
        "            idx = dfa_symbol_index(c);\n"
        "            if (idx == -1) break;\n"
        "            buffer[bufpos++] = c;\n"
        "            state = DFA_STATES[state].transitions[idx];\n"
        "            if (state == -1) {\n"
        "                unput(c);\n"
        "                break;\n"
        "            }\n"
        "            if (DFA_STATES[state].is_accept) {\n"
        "                last_accept = state;\n"
        "                last_accept_pos = bufpos;\n"
        "            }\n"
        "        }\n"
        "        if (last_accept != -1) {\n"
        "            buffer[last_accept_pos] = '\\0';\n"
        "            yytext = buffer;\n"
        "            yyleng = last_accept_pos;\n"
        "            yy_action(DFA_STATES[last_accept].action_id);\n"
        "            continue; // Continue processing the remaining input\n"
        "        } else {\n"
        "            if (input_char <= 0)\n"
        "                break;\n"
        "        }\n"
        "    }\n"
        "    return 0;\n"
        "}\n\n"
    );
}

void	generate_lexyyc(t_lex *lex, const char *alphabet, int alphabet_size)
{
	if (!lex || !lex->dfa)
	{
		fprintf(stderr, "Error: lex or dfa is NULL\n");
		return;
	}
	FILE *file = fopen("lex.yy.c", "w");
	if (!file)
	{
		perror("Failed to open lex.yy.c for writing");
		return;
	}
	fprintf(file, "#include \"libl/includes/libl.h\"\n");
	if (lex->declaration_code)
		fprintf(file, "%s\n", lex->declaration_code);
	write_dfa_alphabet(file, alphabet, alphabet_size);
	fprintf(file,
		"typedef struct {\n"
		"    int is_accept;\n"
		"    int transitions[DFA_ALPHABET_SIZE];\n"
		"    int action_id; // 0 = pas d'action, n > 0 = numéro d'action\n"
		"} DFA_State;\n\n");
	write_dfa_states(file, lex, alphabet, alphabet_size);
	write_dfa_symbol_index_function(file);
	write_yy_action_function(file, lex);
	write_yylex_function(file);
	if (lex->user_code)
		fprintf(file, "%s\n", lex->user_code);
	fclose(file);
}
