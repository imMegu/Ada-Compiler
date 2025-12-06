#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "symboltable.h"
#include "tac.h"
#include "mips.h"

extern int yynerrs;
Command *root = NULL;
extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <input_file.ada> [output_file.asm]\n", argv[0]);
        return 1;
    }

    //Abrir ficheiro de entrada
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Erro ao abrir ficheiro de entrada: %s\n", argv[1]);
        return 1;
    }

    char *out_filename = "output.asm"; 
    if (argc >= 3) {
        out_filename = argv[2];
    }

    //Executar Parser
    yyparse();

    //Verifica se houve erros
    if (yynerrs == 0 && root != NULL) {
        
        // GERA TAC 
        TAC *code = generate_code(root);
        
        //IMPRIME NO TERMINAL
        // Lista de instruções no ecrã para verificação
        tac_dump(code); 

        // GERA MIPS PARA FICHEIRO
        FILE *fout = fopen(out_filename, "w");
        if (!fout) {
            printf("Erro ao criar ficheiro de saída: %s\n", out_filename);
            return 1;
        }

        mips_generate(code, fout);
        fclose(fout);
        
        printf("\nSucesso! Código MIPS guardado em '%s'\n", out_filename);

    } else {
        fprintf(stderr, " Erros detetados. Compilação abortada.\n");
        return 1;
    }

    return 0;
}