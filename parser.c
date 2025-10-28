#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"

static Token currentToken;

static void advance();
static void expect(TokenType type);
static ASTNode* createNode(ASTNodeType type, Token token);

// Inicializa parser e vai ler o primeiro token
void parserInit(const char* source) {
    initScanner(source);
    advance();
}
// Avança para o próximo token
static void advance() {
    int t = getNextToken();
    currentToken.type = (TokenType)t;
}
// verificamos se o token é aquele esperado, 
static void expect(TokenType type) {
    if (currentToken.type == type) {
        advance();
    } else {
        printf("Erro de sintaxe na linha %d: esperado token %d\n", currentToken.line, type);
        exit(1);
    }
}
// Cria um novo nó AST
static ASTNode* createNode(ASTNodeType type, Token token) {
    ASTNode* node = (ASTNode*) malloc(sizeof(ASTNode));
    node->type = type;
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}
ASTNode* parseProgram(void) {
    expect(TOKEN_PROCEDURE);
    expect(TOKEN_MAIN);
    expect(TOKEN_IS);
    expect(TOKEN_BEGIN);

    // estou a assumir que o programa tem apenas uma statement para já, aumentar depois
    ASTNode* statement = parseStatement();
    expect(TOKEN_END);
    ASTNode* programNode = createNode(AST_PROGRAM, currentToken);
    programNode->left = statement;
    return programNode;
}
ASTNode* parseStatement(void) {
    if (currentToken.type == TOKEN_IDENTIFIER) {
        return parseAssignment();
    } else if (currentToken.type == TOKEN_IF) {
        return parseIf();
    } else if (currentToken.type == TOKEN_WHILE) {
        return parseWhile();
    } else if (currentToken.type == TOKEN_PUT_LINE) {
        return parsePutLine();
    } else if (currentToken.type == TOKEN_GET_LINE) {
        return parseGetLine();
    } else {
        printf("Erro de sintaxe: comando inesperado linha %d\n", currentToken.line);
        exit(1);
    }
    return NULL; 
}

// Parsing de um comando de atribuição simples: id := expr;
ASTNode* parseAssignment(void) {
    Token id = currentToken;
    expect(TOKEN_IDENTIFIER);
    expect(TOKEN_ASSIGN);
    ASTNode* expr = parseExpression();
    expect(TOKEN_SEMICOLON);
    ASTNode* assignNode = createNode(AST_ASSIGNMENT, id);
    assignNode->left = createNode(AST_IDENTIFIER, id);
    assignNode->right = expr;

    return assignNode;
}

// Parsing muito simplificado de expressions,vamos precisar aumentar isto
ASTNode* parseExpression(void) {
    if (currentToken.type == TOKEN_NUMBER) {
        ASTNode* numberNode = createNode(AST_NUMBER, currentToken);
        advance();
        return numberNode;
    } else if (currentToken.type == TOKEN_IDENTIFIER) {
        ASTNode* idNode = createNode(AST_IDENTIFIER, currentToken);
        advance();
        return idNode;
    } else {
        printf("Erro de sintaxe: expressão esperada linha %d\n", currentToken.line);
        exit(1);
    }
    return NULL;
}
// isto foi o copilot
// Funções stub para if, while, put_line, get_line, temos de implementar mais conforme a gramática
ASTNode* parseIf(void) {
    printf("Parsing de if ainda não implementado\n");
    exit(1);
}

ASTNode* parseWhile(void) {
    printf("Parsing de while ainda não implementado\n");
    exit(1);
}

ASTNode* parsePutLine(void) {
    printf("Parsing de put_line ainda não implementado\n");
    exit(1);
}

ASTNode* parseGetLine(void) {
    printf("Parsing de get_line ainda não implementado\n");
    exit(1);
}

// criei para libertar memória da AST
void freeAST(ASTNode* root) {
    if (!root) return;
    freeAST(root->left);
    freeAST(root->right);
    free(root);
}
