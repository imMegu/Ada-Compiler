#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

// enum para tipos de nós AST, são apenas alguns, está imcompleto
typedef enum {
    AST_PROGRAM,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_ASSIGNMENT,
    AST_IF,
    AST_WHILE,
    AST_PUT_LINE,
    AST_GET_LINE,
    AST_IDENTIFIER,
    AST_NUMBER,
    AST_BOOLEAN,
    AST_UNKNOWN
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    struct ASTNode* left;
    struct ASTNode* right;
    Token token;
} ASTNode;

// Protótipos das funções do parser

// Inicializa o parser e vai ler o primeiro token
void parserInit(const char* source);

// Função principal para iniciar o parse do programa
ASTNode* parseProgram(void);

// Funções de parsing para expressões, comandos, etc.
ASTNode* parseStatement(void);
ASTNode* parseExpression(void);
ASTNode* parseAssignment(void);
ASTNode* parseIf(void);
ASTNode* parseWhile(void);
ASTNode* parsePutLine(void);
ASTNode* parseGetLine(void);

// Função para liberar memória da AST
void freeAST(ASTNode* root);

#endif
