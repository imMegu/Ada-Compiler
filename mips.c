#include "mips.h"
#include "symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct VarMap {
    char *name;
    int offset;
    struct VarMap *next;
} VarMap;

VarMap *varmap = NULL;
int stack_offset = 0; 

typedef struct StringLiteral {
    char *value;
    char *label; 
    struct StringLiteral *next;
} StringLiteral;

StringLiteral *string_literals = NULL;
int str_label_count = 0;

// Função para procurar/criar o rótulo da string
char* get_string_label(char *value) {
    StringLiteral *curr = string_literals;
    while(curr) {
        if (strcmp(curr->value, value) == 0) return curr->label;
        curr = curr->next;
    }

    //Criar novo rótulo e nó
    char buffer[20];
    sprintf(buffer, "str_%d", str_label_count++);
    char *new_label = strdup(buffer);

    StringLiteral *new_lit = malloc(sizeof(StringLiteral));
    new_lit->value = strdup(value);
    new_lit->label = new_label;
    
    new_lit->next = string_literals;
    string_literals = new_lit;

    return new_label;
}

// Função para despejar todas as strings na secção .data
void dump_string_literals(FILE *fout) {
    StringLiteral *curr = string_literals;
    // O curr agora contém a lista de trás para a frente. 
    // Para imprimir na ordem correta, a abordagem mais simples é iterar e imprimir.
    // Vamos assumir que a ordem de impressão não é crítica para o assembler.
    while(curr) {        
        fprintf(fout, "%s: .asciiz \"%s\"\n", curr->label, curr->value); 

        curr = curr->next;
    }
}



// Procura offset de uma variável ou temporário
int get_offset(char *name) {
    // Tenta encontrar nos temporários locais
    VarMap *curr = varmap;
    while(curr) {
        if (strcmp(curr->name, name) == 0) return curr->offset;
        curr = curr->next;
    }

    //Tenta encontrar na SymbolTable (variáveis declaradas)
    Symbol *s = symtab_lookup(name);
    if (s) return s->offset;

    // Cria novo espaço na stack (é um temporário novo)
    stack_offset -= 4;
    VarMap *n = malloc(sizeof(VarMap));
    n->name = strdup(name);
    n->offset = stack_offset;
    n->next = varmap;
    varmap = n;
    return stack_offset;
}

void mips_generate(TAC *tac, FILE *fout) {
    if (!fout) return;
    TAC *current_tac = tac;
    while(current_tac) {
        if (current_tac->op == TAC_PRINT_STR) {
            get_string_label(current_tac->arg1);
        }
        current_tac = current_tac->next;
    }

    fprintf(fout, ".data\n");
    fprintf(fout, "newline: .asciiz \"\\n\"\n");
    
    dump_string_literals(fout);
    
    fprintf(fout, ".text\n");
    fprintf(fout, ".globl main\n");
    fprintf(fout, "main:\n");
     
    fprintf(fout, "    move $fp, $sp\n");
    fprintf(fout, "    subu $sp, $sp, 1000\n"); 

    symtab_assign_offsets(); 
    
    while(tac) {
        switch(tac->op) {
            case TAC_MOV:
                if (tac->arg1[0] >= '0' && tac->arg1[0] <= '9') {
                    fprintf(fout, "    li $t0, %s\n", tac->arg1);
                } else {
                    fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(tac->arg1));
                }
                fprintf(fout, "    sw $t0, %d($fp)\n", get_offset(tac->result));
                break;

            case TAC_ADD:
            case TAC_SUB:
            case TAC_MUL:
            case TAC_DIV:
            case TAC_MOD:
            case TAC_EQ:
            case TAC_NE:
            case TAC_LT:
            case TAC_LE:
            case TAC_GT:
            case TAC_GE:
                // Arg1
                if (tac->arg1[0] >= '0' && tac->arg1[0] <= '9') 
                    fprintf(fout, "    li $t0, %s\n", tac->arg1);
                else 
                    fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(tac->arg1));
                
                // Arg2
                if (tac->arg2[0] >= '0' && tac->arg2[0] <= '9') 
                    fprintf(fout, "    li $t1, %s\n", tac->arg2);
                else 
                    fprintf(fout, "    lw $t1, %d($fp)\n", get_offset(tac->arg2));

                // Operação
                if (tac->op == TAC_ADD) fprintf(fout, "    add $t2, $t0, $t1\n");
                else if (tac->op == TAC_SUB) fprintf(fout, "    sub $t2, $t0, $t1\n");
                else if (tac->op == TAC_MUL) fprintf(fout, "    mul $t2, $t0, $t1\n");
                else if (tac->op == TAC_DIV) fprintf(fout, "    div $t2, $t0, $t1\n");
                else if (tac->op == TAC_MOD) fprintf(fout, "    rem $t2, $t0, $t1\n");
                else if (tac->op == TAC_EQ) fprintf(fout, "    seq $t2, $t0, $t1\n");
                else if (tac->op == TAC_NE) fprintf(fout, "    sne $t2, $t0, $t1\n");
                else if (tac->op == TAC_LT) fprintf(fout, "    slt $t2, $t0, $t1\n");
                else if (tac->op == TAC_LE) fprintf(fout, "    sle $t2, $t0, $t1\n");
                else if (tac->op == TAC_GT) fprintf(fout, "    sgt $t2, $t0, $t1\n");
                else if (tac->op == TAC_GE) fprintf(fout, "    sge $t2, $t0, $t1\n");

                fprintf(fout, "    sw $t2, %d($fp)\n", get_offset(tac->result));
                break;

            case TAC_LABEL:
                fprintf(fout, "%s:\n", tac->result);
                break;
            
            case TAC_JMP:
                fprintf(fout, "    j %s\n", tac->result);
                break;

            case TAC_IFZ:
                fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(tac->arg1));
                fprintf(fout, "    beqz $t0, %s\n", tac->result);
                break;

            case TAC_PRINT_INT:
                fprintf(fout, "    lw $a0, %d($fp)\n", get_offset(tac->arg1));
                fprintf(fout, "    li $v0, 1\n");
                fprintf(fout, "    syscall\n");
                fprintf(fout, "    li $v0, 4\n");
                fprintf(fout, "    la $a0, newline\n");
                fprintf(fout, "    syscall\n");
                break;

            case TAC_PRINT_STR:

                char *string_label = get_string_label(tac->arg1);

                fprintf(fout, "    la $a0, %s\n", string_label); 
                fprintf(fout, "    li $v0, 4\n");                
                fprintf(fout, "    syscall\n");
                
                // Imprimir newline
                fprintf(fout, "    li $v0, 4\n");
                fprintf(fout, "    la $a0, newline\n");
                fprintf(fout, "    syscall\n");
                break;

            case TAC_READ:
                fprintf(fout, "    li $v0, 5\n");
                fprintf(fout, "    syscall\n");
                fprintf(fout, "    sw $v0, %d($fp)\n", get_offset(tac->arg1));
                break;
            
            default: break;
        }
        tac = tac->next;
    }

    fprintf(fout, "    li $v0, 10\n");
    fprintf(fout, "    syscall\n");
}