#ifndef SCANNER_H
#define SCANNER_H

typedef enum {
  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_WHILE,
  TOKEN_ASSIGN,        // :=
  TOKEN_PLUS,          // +
  TOKEN_MINUS,         // -
  TOKEN_MULTIPLY,      // *
  TOKEN_DIVIDE,        // /
  TOKEN_EQUAL,         // =
  TOKEN_LESS,          // <
  TOKEN_GREATER,       // >
  TOKEN_LESS_EQUAL,    // <=
  TOKEN_GREATER_EQUAL, // >=
  TOKEN_NOT,           // not
  TOKEN_AND,           // and
  TOKEN_OR,            // or
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_BOOLEAN,
  TOKEN_PUT_LINE,
  TOKEN_GET_LINE,
  TOKEN_PROCEDURE,
  TOKEN_MAIN,
  TOKEN_BEGIN,     // begin
  TOKEN_END,       // end
  TOKEN_IS,        // is
  TOKEN_SEMICOLON, // ;
  TOKEN_COMMA,     // ,
  TOKEN_LPAREN,    // (
  TOKEN_RPAREN,    // )
  TOKEN_COMMENT,
  TOKEN_EOF
} TokenType;

typedef struct {
  TokenType type;
  char *lexeme; // representa o texto do token
  int line;     // linha para msgs de erro
} Token;

Token makeToken(TokenType type);

#endif
