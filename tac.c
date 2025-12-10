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
    t->arg1 = arg1 ? strdup(arg1) : NULL; // <<< ESTA CÓPIA É VITAL
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
            case AND: op = TAC_AND; break;
            case OR: op = TAC_OR; break;
            case XOR: op = TAC_XOR; break;
            case AND_THEN: op = TAC_AND_THEN; break;
            case OR_ELSE: op = TAC_OR_ELSE; break;
            case NOT: op = TAC_NOT; break;
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
        // CORREÇÃO: Passar o ID como o argumento arg1 (terceiro parâmetro)
        code = tac_join(code, tac_create(TAC_READ, NULL, cmd->attr.getLine.id, NULL));
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

// Função tac_dump modificada para escrever num ficheiro
void tac_dump(TAC *tac, const char *filename) {
    // 1. Abrir o ficheiro para escrita ("w" de write)
    FILE *f = fopen(filename, "w");
    
    // 2. Verificar se a abertura do ficheiro foi bem-sucedida
    if (!f) {
        perror("Erro ao abrir ficheiro para dump do TAC");
        return;
    }
    
// 3. Usar fprintf para escrever no ficheiro (f) em vez de printf
    fprintf(f, "\n--- Intermediate Code (TAC) ---\n");
    while(tac) {
        switch(tac->op) {
            // Alterar o operador de atribuição de '=' para ':=', incluindo operadores relacionais e lógicos
            case TAC_ADD: fprintf(f, "%s := %s + %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_SUB: fprintf(f, "%s := %s - %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_MUL: fprintf(f, "%s := %s * %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_DIV: fprintf(f, "%s := %s / %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_MOD: fprintf(f, "%s := %s %% %s\n", tac->result, tac->arg1, tac->arg2); break; 
            case TAC_EQ:  fprintf(f, "%s := %s == %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_NE:  fprintf(f, "%s := %s != %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_LT:  fprintf(f, "%s := %s < %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_GT:  fprintf(f, "%s := %s > %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_LE:  fprintf(f, "%s := %s <= %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_GE:  fprintf(f, "%s := %s >= %s\n", tac->result, tac->arg1, tac->arg2); break;
            
            // Alterar a atribuição simples de '=' para ':='
            case TAC_MOV: fprintf(f, "%s := %s\n", tac->result, tac->arg1); break;
            
            // Alterar rótulo de 'L0:' para 'LABEL L0:'
            case TAC_LABEL: fprintf(f, "LABEL %s:\n", tac->result); break;
            
            // Alterar salto incondicional de 'goto L0' para 'GOTO L0'
            case TAC_JMP: fprintf(f, "GOTO %s\n", tac->result); break;
            
            // Alterar salto condicional de 'ifz' para 'IF ... GOTO'
            // O TAC_IFZ (if zero) salta se a condição (arg1) for 0 (False).
            case TAC_IFZ: fprintf(f, "IF %s = 0 GOTO %s\n", tac->arg1, tac->result); break;
            
            // Funções de I/O
            case TAC_PRINT_INT: fprintf(f, "print_int %s\n", tac->arg1); break;
            case TAC_PRINT_STR: fprintf(f, "print_str %s\n", tac->arg1); break;
            case TAC_READ: fprintf(f, "read %s\n", tac->arg1); break;
            
            // Alterar os operadores lógicos de '=' para ':='
            case TAC_AND: fprintf(f, "%s := %s AND %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_OR: fprintf(f, "%s := %s OR %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_XOR: fprintf(f, "%s := %s XOR %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_NOT: fprintf(f, "%s := NOT %s\n", tac->result, tac->arg1); break;
            case TAC_AND_THEN: fprintf(f, "%s := %s AND_THEN %s\n", tac->result, tac->arg1, tac->arg2); break;
            case TAC_OR_ELSE: fprintf(f, "%s := %s OR_ELSE %s\n", tac->result, tac->arg1, tac->arg2); break;
            default: fprintf(f, "Unknown TAC op\n"); break;
        }
        tac = tac->next;
    }
    fprintf(f, "--- End of TAC ---\n\n");
    
    fclose(f);
}