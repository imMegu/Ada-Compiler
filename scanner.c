#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scanner.h"
// retirei mutas cenas de um codigo do stackoverflow, tenho de retificar alguns pontos ainda
static int current = 0;
static int line = 1;
static const char* source;

// Função para criar uma cópia do lexema
static char* copyLexeme(const char* start, int length) {
    char* lexeme = (char*)malloc(length + 1);
    strncpy(lexeme, start, length);
    lexeme[length] = '\0';
    return lexeme;
}

void initScanner(const char* src) {
    source = src;
    current = 0;
    line = 1;
}

static char advance() {
    return source[current++];
}

static char peek() {
    return source[current];
}

static char peekNext() {
    if (source[current] == '\0') return '\0';
    return source[current + 1];
}

static int isAtEnd() {
    return source[current] == '\0';
}

Token makeToken(TokenType type, const char* lexeme, int line) {
    Token token;
    token.type = type;
    token.lexeme = copyLexeme(lexeme, strlen(lexeme));
    token.line = line;
    return token;
}

Token errorToken(const char* message) {
    return makeToken(TOKEN_EOF, message, line);
}

static int isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Função para criar token para identificadores ou palavras-chave
static Token identifier() {
    int start = current - 1;
    while (isAlpha(peek()) || isDigit(peek())) advance();
    int length = current - start;
    char* text = copyLexeme(source + start, length);

    // Verificar se é palavra-chave
    if (strcmp(text, "if") == 0) {
        free(text);
        return makeToken(TOKEN_IF, "if", line);
    }
    if (strcmp(text, "then") == 0) {
        free(text);
        return makeToken(TOKEN_THEN, "then", line);
    }
    if (strcmp(text, "else") == 0) {
        free(text);
        return makeToken(TOKEN_ELSE, "else", line);
    }
    if (strcmp(text, "while") == 0) {
        free(text);
        return makeToken(TOKEN_WHILE, "while", line);
    }
    if (strcmp(text, "not") == 0) {
        free(text);
        return makeToken(TOKEN_NOT, "not", line);
    }
    if (strcmp(text, "and") == 0) {
        free(text);
        return makeToken(TOKEN_AND, "and", line);
    }
    if (strcmp(text, "or") == 0) {
        free(text);
        return makeToken(TOKEN_OR, "or", line);
    }
    if (strcmp(text, "procedure") == 0) {
        free(text);
        return makeToken(TOKEN_PROCEDURE, "procedure", line);
    }
    if (strcmp(text, "main") == 0) {
        free(text);
        return makeToken(TOKEN_MAIN, "main", line);
    }
    if (strcmp(text, "begin") == 0) {
        free(text);
        return makeToken(TOKEN_BEGIN, "begin", line);
    }
    if (strcmp(text, "end") == 0) {
        free(text);
        return makeToken(TOKEN_END, "end", line);
    }
    if (strcmp(text, "is") == 0) {
        free(text);
        return makeToken(TOKEN_IS, "is", line);
    }
    if (strcmp(text, "put_line") == 0) {
        free(text);
        return makeToken(TOKEN_PUT_LINE, "put_line", line);
    }
    if (strcmp(text, "get_line") == 0) {
        free(text);
        return makeToken(TOKEN_GET_LINE, "get_line", line);
    }
    if (strcmp(text, "true") == 0 || strcmp(text, "false") == 0) {
        free(text);
        return makeToken(TOKEN_BOOLEAN, text, line);
    }

    return makeToken(TOKEN_IDENTIFIER, text, line);
}

static Token number() {
    int start = current - 1;
    while (isDigit(peek())) advance();

    int length = current - start;
    char* text = copyLexeme(source + start, length);
    Token token = makeToken(TOKEN_NUMBER, text, line);
    free(text);
    return token;
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') { 
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token getNextToken() {
    skipWhitespace();

    if (isAtEnd()) return makeToken(TOKEN_EOF, "", line);

    char c = advance();

    if (isAlpha(c))
        return identifier();
    if (isDigit(c))
        return number();

    switch (c) {
        case '+': return makeToken(TOKEN_PLUS, "+", line);
        case '-': return makeToken(TOKEN_MINUS, "-", line);
        case '*': return makeToken(TOKEN_MULTIPLY, "*", line);
        case ';': return makeToken(TOKEN_SEMICOLON, ";", line);
        case ',': return makeToken(TOKEN_COMMA, ",", line);
        case '(': return makeToken(TOKEN_LPAREN, "(", line);
        case ')': return makeToken(TOKEN_RPAREN, ")", line);
        case '=': return makeToken(TOKEN_EQUAL, "=", line);
        case '<':
            if (peek() == '=') {
                advance();
                return makeToken(TOKEN_LESS_EQUAL, "<=", line);
            }
            return makeToken(TOKEN_LESS, "<", line);
        case '>':
            if (peek() == '=') {
                advance();
                return makeToken(TOKEN_GREATER_EQUAL, ">=", line);
            }
            return makeToken(TOKEN_GREATER, ">", line);
        case ':':
            if (peek() == '=') {
                advance();
                return makeToken(TOKEN_ASSIGN, ":=", line);
            }
            break;
    }

    // Caso caractere não reconhecido
    char errLexeme[2] = { c, '\0' };
    return errorToken(errLexeme);
}
