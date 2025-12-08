#include "tac.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int temp_count = 0;
int label_count = 0;

// Cria uma variável temporária
char* make_temp() {
    char buffer[20];
    sprintf(buffer, "t%d", temp_count++);
    return strdup(buffer);
}

// Cria label 
char* make_label() {
    char buffer[20];
    sprintf(buffer, "L%d", label_count++);
    return strdup(buffer);
}

TAC* tac_create(TACOp op, char *result, char *arg1, char *arg2) {
    TAC *t = (TAC*)malloc(sizeof(TAC));
    t->op = op;
    t->result = result ? strdup(result) : NULL;
    t->arg1 = arg1 ? strdup(arg1) : NULL;
    t->arg2 = arg2 ? strdup(arg2) : NULL;
    t->next = NULL;
    t->prev = NULL;
    return t;
}

// Junta duas listas de instruções 
TAC* tac_join(TAC *l1, TAC *l2) {
    if (!l1) return l2;
    if (!l2) return l1;
    
    TAC *tmp = l1;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = l2;
    l2->prev = tmp;
    
    return l1;
}

// Gera código para Expressões
char* tac_expr(Expr *e, TAC **code) {
    if (!e) return NULL;

    if (e->kind == E_INTEGER) {
        char val[20];
        sprintf(val, "%d", e->attr.value);
        char *t = make_temp();
        // t = valor
        *code = tac_join(*code, tac_create(TAC_MOV, t, val, NULL));
        return t;
    }
    else if (e->kind == E_BOOL_VALUE) {
        char *t = make_temp();
        char *val = e->attr.value ? "1" : "0";
        *code = tac_join(*code, tac_create(TAC_MOV, t, val, NULL));
        return t;
    }
    else if (e->kind == E_ID) {
        return strdup(e->attr.id);
    }
    else if (e->kind == E_OPERATION || e->kind == E_BOOL) {
        TAC *c1 = NULL;
        TAC *c2 = NULL;
        
        // Gera código para os operandos primeiro
        char *t1 = tac_expr(e->attr.op.left, &c1);
        char *t2 = tac_expr(e->attr.op.right, &c2);
        
        char *res = make_temp();
        TACOp op = TAC_ADD; 
        
        // Mapeia os tokens do parser para operações TAC
        switch(e->attr.op.op) {
            case PLUS: op = TAC_ADD; break;
            case MINUS: op = TAC_SUB; break;
            case MULT: op = TAC_MUL; break;
            case DIV: op = TAC_DIV; break;
            case MOD: op = TAC_MOD; break;
            case EQ: op = TAC_EQ; break;
            case NE: op = TAC_NE; break;
            case LT: op = TAC_LT; break;
            case LE: op = TAC_LE; break;
            case GT: op = TAC_GT; break;
            case GE: op = TAC_GE; break;
            default: break;
        }

        // Junto tudo: Código da esquerda + Código da direita + Instrução atual
        *code = tac_join(*code, c1);
        *code = tac_join(*code, c2);
        *code = tac_join(*code, tac_create(op, res, t1, t2));
        free(t1); 
        free(t2);
        return res;
    }
    return NULL;
}
TAC* tac_cmd(Command *cmd);

// Gera código para Comandos
TAC* tac_cmd(Command *cmd) {
    if (!cmd) return NULL;
    TAC *code = NULL;

    switch(cmd->command) {
        case EXPR_CMD: { 
            // Atribuição: ID := Expr
            if (cmd->attr.value->kind == E_ASSIGNMENT) {
                TAC *expr_code = NULL;
                char *val = tac_expr(cmd->attr.value->attr.assignment.value, &expr_code);
                code = tac_join(expr_code, tac_create(TAC_MOV, cmd->attr.value->attr.assignment.id, val, NULL));
            }
            break;
        }
        case IF_CMD: {
            // IF expr THEN cmd END IF
            TAC *expr_code = NULL;
            char *cond = tac_expr(cmd->attr.ifCmd.expr, &expr_code);
            char *l_end = make_label();

            code = tac_join(code, expr_code);
            code = tac_join(code, tac_create(TAC_IFZ, l_end, cond, NULL));
            code = tac_join(code, tac_cmd(cmd->attr.ifCmd.cmd));
            code = tac_join(code, tac_create(TAC_LABEL, l_end, NULL, NULL));
            break;
        }
        case IF_ELSE_CMD: {
            // IF expr THEN cmd1 ELSE cmd2 END IF
            TAC *expr_code = NULL;
            char *cond = tac_expr(cmd->attr.ifElseCmd.expr, &expr_code);
            char *l_else = make_label();
            char *l_end = make_label();

            code = tac_join(code, expr_code);
            code = tac_join(code, tac_create(TAC_IFZ, l_else, cond, NULL));
            code = tac_join(code, tac_cmd(cmd->attr.ifElseCmd.cmd));
            code = tac_join(code, tac_create(TAC_JMP, l_end, NULL, NULL));
            code = tac_join(code, tac_create(TAC_LABEL, l_else, NULL, NULL));
            code = tac_join(code, tac_cmd(cmd->attr.ifElseCmd.elsecmd));
            code = tac_join(code, tac_create(TAC_LABEL, l_end, NULL, NULL));
            break;
        }
        case WHILE_CMD: {
            // WHILE expr LOOP cmd END LOOP
            char *l_start = make_label();
            char *l_end = make_label();
            TAC *expr_code = NULL;
            char *cond = tac_expr(cmd->attr.whileCmd.expr, &expr_code);

            code = tac_join(code, tac_create(TAC_LABEL, l_start, NULL, NULL));
            code = tac_join(code, expr_code);
            code = tac_join(code, tac_create(TAC_IFZ, l_end, cond, NULL));
            code = tac_join(code, tac_cmd(cmd->attr.whileCmd.cmd));
            code = tac_join(code, tac_create(TAC_JMP, l_start, NULL, NULL));
            code = tac_join(code, tac_create(TAC_LABEL, l_end, NULL, NULL));
            break;
        }
        case COMPOUND_CMD: {
            if (cmd->attr.compound.commands) {
                Node *current = cmd->attr.compound.commands->first;
                while (current) {
                    code = tac_join(code, tac_cmd((Command*)current->value));
                    current = current->next;
                }
            }
            break;
        }
        case PUT_LINE_CMD: {
            if (cmd->attr.putLine.value->kind == E_STRING) {
                code = tac_join(code, tac_create(TAC_PRINT_STR, NULL, cmd->attr.putLine.value->attr.id, NULL));
            } else {
                TAC *expr_code = NULL;
                char *val = tac_expr(cmd->attr.putLine.value, &expr_code);
                code = tac_join(expr_code, tac_create(TAC_PRINT_INT, NULL, val, NULL));
            }
            break;
        }
        case GET_LINE_CMD: {
            code = tac_join(code, tac_create(TAC_READ, cmd->attr.getLine.id, NULL, NULL));
            break;
        }
        default: break;
    }
    return code;
}

// Função principal chamada pelo compiler.c
TAC* generate_code(Command *root) {
    return tac_cmd(root);
}

void tac_dump(TAC *tac) {
    printf("\n--- Intermediate Code (TAC) ---\n");
    while(tac) {
        switch(tac->op) {
            case TAC_ADD: printf("%s = %s + %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_SUB: printf("%s = %s - %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_MUL: printf("%s = %s * %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_DIV: printf("%s = %s / %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_MOV: printf("%s = %s\n", tac->result, tac->arg1); break;
            case TAC_LABEL: printf("%s:\n", tac->result); break;
            case TAC_JMP: printf("goto %s\n", tac->result); break;
            case TAC_IFZ: printf("ifz %s goto %s\n", tac->arg1, tac->result); break;
            case TAC_PRINT_INT: printf("print_int %s\n", tac->arg1); break;
            case TAC_PRINT_STR: printf("print_str %s\n", tac->arg1); break;
            case TAC_READ: printf("read %s\n", tac->arg1); break;
            default: printf("Unknown TAC op\n"); break;
        }
        tac = tac->next;
    }
    printf("--- End of TAC ---\n\n");
}