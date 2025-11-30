#include "mips.h"
#include "symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simulação simples de stack frame
// Variáveis locais começam em -4, -8, etc.
// Temporários (t0, t1) também vão para a stack.

// Estrutura auxiliar para mapear nomes (var ou temp) a offsets
typedef struct VarMap {
    char *name;
    int offset;
    struct VarMap *next;
} VarMap;

VarMap *varmap = NULL;
int stack_offset = 0; // Vai crescer negativamente

// Procura offset de uma variável ou temporário
int get_offset(char *name) {
    // 1. Tenta encontrar nos temporários locais
    VarMap *curr = varmap;
    while(curr) {
        if (strcmp(curr->name, name) == 0) return curr->offset;
        curr = curr->next;
    }

    // 2. Tenta encontrar na SymbolTable (variáveis declaradas)
    Symbol *s = symtab_lookup(name);
    if (s) return s->offset;

    // 3. Se não existe, cria novo espaço na stack (é um temporário novo)
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

    // Cabeçalho
    fprintf(fout, ".data\n");
    fprintf(fout, "newline: .asciiz \"\\n\"\n");
    
    fprintf(fout, ".text\n");
    fprintf(fout, ".globl main\n");
    fprintf(fout, "main:\n");
    
    // Prologo
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
                fprintf(fout, "    .data\n");
                fprintf(fout, "str_%p: .asciiz %s\n", tac->arg1, tac->arg1);
                fprintf(fout, "    .text\n");
                fprintf(fout, "    la $a0, str_%p\n", tac->arg1);
                fprintf(fout, "    li $v0, 4\n");
                fprintf(fout, "    syscall\n");
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