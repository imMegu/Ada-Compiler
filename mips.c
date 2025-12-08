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
    if (!n) {
        perror("malloc VarMap failed"); // Adiciona verificação de erro
        exit(EXIT_FAILURE);
    }

    n->name = strdup(name);
    if (!n->name) {
        perror("strdup VarMap name failed"); // Adiciona verificação de erro
        free(n);
        exit(EXIT_FAILURE);
    }
    n->offset = stack_offset;
    n->offset = stack_offset;
    n->next = varmap; 
    varmap = n;     
    return stack_offset;
}

void mips_generate(TAC *tac, FILE *fout) {
    if (!fout) return;
    
    // Inicialização e Strings (Primeira Leitura do TAC)
    TAC *current_tac = tac;
    while(current_tac) {
        if (current_tac->op == TAC_PRINT_STR) {
            get_string_label(current_tac->arg1);
        }
        current_tac = current_tac->next;
    }

    // Cálculo Rigoroso de Offsets 
    // Recorre o TAC para forçar a chamada a get_offset() para todos os temporários (t%d)
    // e variáveis. Isto preenche a VarMap e o stack_offset GLOBAL.
    current_tac = tac; // Reinicia o ponteiro TAC
    while(current_tac) {
      
        if (current_tac->result) get_offset(current_tac->result);
        
        
        if (current_tac->arg1) {
            int is_str_literal = (current_tac->op == TAC_PRINT_STR);
            int is_num_literal = (current_tac->arg1[0] >= '0' && current_tac->arg1[0] <= '9');

            if (!is_str_literal && !is_num_literal)
                get_offset(current_tac->arg1);
        }
        
        if (current_tac->arg2 && !(current_tac->arg2[0] >= '0' && current_tac->arg2[0] <= '9'))
            get_offset(current_tac->arg2);
            
        current_tac = current_tac->next;
    }

    //GERAÇÃO DA SECÇÃO .data (Usando strings recolhidas no Passo 0)
    fprintf(fout, ".data\n");
    fprintf(fout, "newline: .asciiz \"\\n\"\n");
    dump_string_literals(fout); 

    symtab_assign_offsets(); 
    // AGORA stack_offset TEM O VALOR CORRETO
    int temporary_size = abs(stack_offset); 
    int declared_size = symtab_get_declared_var_size(); 
    int total_stack_size = temporary_size + declared_size; 
   
    fprintf(fout, ".text\n");
    fprintf(fout, ".globl main\n");
    fprintf(fout, "main:\n");
     
    fprintf(fout, "    move $fp, $sp\n");
    // Alocação rigorosa com o tamanho TOTAL calculado:
    fprintf(fout, "    subu $sp, $sp, %d\n", total_stack_size); 
    current_tac = tac; // Reinicia o ponteiro TAC para o loop de geração
    while(current_tac) {
        switch(current_tac->op) {
            case TAC_MOV:
                if (current_tac->arg1[0] >= '0' && current_tac->arg1[0] <= '9') {
                    fprintf(fout, "    li $t0, %s\n", current_tac->arg1);
                } else {
                    fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(current_tac->arg1));
                }
                fprintf(fout, "    sw $t0, %d($fp)\n", get_offset(current_tac->result));
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
            case TAC_GE: {
                
                // Variáveis auxiliares para rastrear o tipo de argumento
                int arg1_is_imm = (current_tac->arg1[0] >= '0' && current_tac->arg1[0] <= '9');
                int arg2_is_imm = (current_tac->arg2[0] >= '0' && current_tac->arg2[0] <= '9');
            
                if (current_tac->op == TAC_ADD && (arg1_is_imm || arg2_is_imm)) {
                    
                    char *reg_arg = arg1_is_imm ? current_tac->arg2 : current_tac->arg1;
                    char *imm_val = arg1_is_imm ? current_tac->arg1 : current_tac->arg2;
                    
                    if (arg1_is_imm && arg2_is_imm) {
                        fprintf(fout, "    li $t0, %s\n", current_tac->arg1);
                        fprintf(fout, "    addi $t2, $t0, %s\n", current_tac->arg2); 
                    } else {
                        fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(reg_arg));
                        fprintf(fout, "    addi $t2, $t0, %s\n", imm_val); 
                    }
                    
                } 
                // OTIMIZAÇÃO RIGOROSA: SUBI (TAC_SUB com Immediato) 
                else if (current_tac->op == TAC_SUB && arg2_is_imm) {
                 
                    if (!arg1_is_imm) {
                        // REG - IMM: Usa ADDI com o imediato negativo
                        int imm_int = atoi(current_tac->arg2); // Necessário para imprimir o negativo
                        
                        fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(current_tac->arg1));
                        fprintf(fout, "    addi $t2, $t0, %d\n", -imm_int); 
                        
                    } else {
                        // IMM - IMM: Não há addi para 16-bit com 2 imediatos, mantém a versão LI/SUB
                        fprintf(fout, "    li $t0, %s\n", current_tac->arg1);
                        fprintf(fout, "    li $t1, %s\n", current_tac->arg2);
                        fprintf(fout, "    sub $t2, $t0, $t1\n");
                    }
                    
                } 
                // CASO PADRÃO (Sem otimização de imediato ou para outras operações)
                else {

                    // Arg1: Load/LI
                    if (arg1_is_imm) 
                        fprintf(fout, "    li $t0, %s\n", current_tac->arg1);
                    else 
                        fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(current_tac->arg1));
                    
                    // Arg2: Load/LI
                    if (arg2_is_imm) 
                        fprintf(fout, "    li $t1, %s\n", current_tac->arg2);
                    else 
                        fprintf(fout, "    lw $t1, %d($fp)\n", get_offset(current_tac->arg2));

                    // Operação
                    if (current_tac->op == TAC_ADD) fprintf(fout, "    add $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_SUB) fprintf(fout, "    sub $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_MUL) fprintf(fout, "    mul $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_DIV) fprintf(fout, "    div $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_MOD) fprintf(fout, "    rem $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_EQ) fprintf(fout, "    seq $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_NE) fprintf(fout, "    sne $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_LT) fprintf(fout, "    slt $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_LE) fprintf(fout, "    sle $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_GT) fprintf(fout, "    sgt $t2, $t0, $t1\n");
                    else if (current_tac->op == TAC_GE) fprintf(fout, "    sge $t2, $t0, $t1\n");
                }

                fprintf(fout, "    sw $t2, %d($fp)\n", get_offset(current_tac->result));
                break;
            }

            case TAC_LABEL:
                fprintf(fout, "%s:\n", current_tac->result);
                break;
            
            case TAC_JMP:
                fprintf(fout, "    j %s\n", current_tac->result);
                break;

            case TAC_IFZ:
                fprintf(fout, "    lw $t0, %d($fp)\n", get_offset(current_tac->arg1));
                fprintf(fout, "    beqz $t0, %s\n", current_tac->result);
                break;

            case TAC_PRINT_INT:
                fprintf(fout, "    lw $a0, %d($fp)\n", get_offset(current_tac->arg1));
                fprintf(fout, "    li $v0, 1\n");
                fprintf(fout, "    syscall\n");
                fprintf(fout, "    li $v0, 4\n");
                fprintf(fout, "    la $a0, newline\n");
                fprintf(fout, "    syscall\n");
                break;

            case TAC_PRINT_STR:

                char *string_label = get_string_label(current_tac->arg1);

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
                fprintf(fout, "    sw $v0, %d($fp)\n", get_offset(current_tac->arg1));
                break;
            
            default: break;
        }
        current_tac = current_tac->next;
    }

    fprintf(fout, "    li $v0, 10\n");
    fprintf(fout, "    syscall\n");
}
