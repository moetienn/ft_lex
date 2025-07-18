# ft_lex

**ft_lex** is a minimal implementation of a lexical analyzer generator inspired by the classic POSIX `lex` utility.  
This project explores the theory and practice behind lexical analysis, regular expressions, and finite automata, providing a toolchain that transforms human-readable patterns into efficient C code for tokenizing text.

---

## What is `lex`?

`lex` is a program that generates lexical analyzers also known as lexers or scanners. A lexer is a fundamental component of a compiler or interpreter: it reads an input stream and splits it into a sequence of **tokens** (words, numbers, symbols, etc.).  
Tokens are the basic building blocks with which a parser can understand the structure of a programming language or data format.

With `lex`, users write a set of regular-expression rules describing the patterns they want to recognize in the input. The tool then generates C code that, at runtime, efficiently finds and returns tokens according to those rules.

---

## Objective of this Project

The goal of **ft_lex** is to demystify the inner workings of a lexical analyzer generator:

- **Take a `.l` file**: the user writes regular expressions and actions.
- **Tokenize the patterns**: break down each regex into atomic operations.
- **Compile regex to an NFA**: build a Non-deterministic Finite Automaton (NFA) using Thompson's construction, which can recognize the language described by the regex.
- **Convert NFA to DFA**: transform the NFA into a Deterministic Finite Automaton (DFA), an efficient machine for recognizing tokens.
- **Generate C code**: output a C file containing the DFA logic, so the user can tokenize input with high performance.
- **Handle errors gracefully**: provide clear diagnostic messages for malformed expressions or usage errors.

---

## How does it work?

1. **Tokenization**  
    The first step is to parse the user's regular expressions into a sequence of tokens (characters, operators, parentheses, etc.). This makes it easier to analyze and process the regex patterns programmatically.

2. **NFA Construction**  
    Each regular expression is converted into an NFA fragment using Thompson's construction:
    - Each basic symbol forms a tiny NFA.
    - Operators like `*`, `+`, `?`, concatenation, and alternation combine fragments into larger NFAs.
    - The NFA can recognize the language described by the original regex, but it may have "epsilon" (empty) transitions and multiple possible states at once.

3. **DFA Conversion**  
    The NFA is then systematically converted into a DFA:
    - The DFA has exactly one "active" state at any step.
    - The conversion (subset construction) removes ambiguity and epsilon transitions, producing a structure that is faster and simpler to run during tokenization.

4. **Code Generation**  
    The final DFA is turned into readable and efficient C code.
    - The code reads input characters and follows DFA transitions.
    - When the DFA reaches an accepting state, it recognizes a token.

---

## Why is this useful?

Understanding and building a tool like `ft_lex` gives you insight into:
- The foundations of compiler construction
- How regular expressions work under the hood
- The transformation of high-level patterns into low-level, efficient code
- Automata theory (the link between mathematical models and practical software)

## Further Reading

- [Regular Expressions and Finite Automata (Wikipedia)](https://en.wikipedia.org/wiki/Finite_automaton)
- [POSIX `lex` utility specification](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/lex.html)

