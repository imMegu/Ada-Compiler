#include <stdlib.h>
#include "scanner.h"

static int current = 0;
static int line = 1;
static const char *source;

void initScanner(const char *src) {
  source = src;
  current = 0;
  line = 1;
}

static char advance() { return source[current++]; }

static char peek() { return source[current]; }

static int isAtEnd() { return source[current] == '\0'; }

Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.lexeme = NULL; // pode guardar lexema se quiser
  token.line = line;
  return token;
}

Token getNextToken() {
  while (!isAtEnd()) {
    char c = advance();

    // Ignorar espaços e tabs
    if (c == ' ' || c == '\r' || c == '\t') {
      continue;
    }

    if (c == '\n') {
      line++;
      continue;
    }

    if (c == '+')
      return makeToken(TOKEN_PLUS);
    if (c == '-')
      return makeToken(TOKEN_MINUS);
    if (c == '*')
      return makeToken(TOKEN_MULTIPLY);

    // Exemplo simples para identificar :=
    if (c == ':') {
      if (peek() == '=') {
        advance();
        return makeToken(TOKEN_ASSIGN);
      }
    }

    // Implementar reconhecimento para identificadores, palavras-chave e
    // números...

    // Se não reconheceu, pode reportar erro ou ignorar por enquanto
  }

  return makeToken(TOKEN_EOF);
}
