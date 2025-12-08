#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// --- Constants ---
#define SYMBOL_TABLE_SIZE 101 // A prime number for better hash distribution

// --- Data Structures ---

typedef struct Symbol {
  char *name;         // The identifier name (e.g., "my_var")
  enum TypeKind type; // The data type of the symbol (e.g., TYPE_INT)
  int scope;          // The nesting level of the symbol's declaration
  int offset;
} Symbol;

// A node in the linked list for hash collisions.
typedef struct SymbolTableEntry {
  Symbol *symbol;
  struct SymbolTableEntry *next;
} SymbolTableEntry;

// The hash table itself, representing one scope.
typedef struct SymbolTable {
  SymbolTableEntry *table[SYMBOL_TABLE_SIZE];
  struct SymbolTable *prev; // Pointer to the outer scope (stack-like linkage)
} SymbolTable;

// --- Function Prototypes ---

// Scope Management
void symtab_enter_scope();
void symtab_exit_scope();
int symtab_get_current_scope();

// Initialization and Hashing
Symbol *symtab_mk_symbol(char *name, enum TypeKind type);
unsigned int symtab_hash(char *name);
SymbolTable *symtab_mk_table();

// Core Operations
// Inserts a new symbol into the current scope's symbol table.
// 0 on success, -1 if the symbol already exists in the current scope.
int symtab_insert(char *name, enum TypeKind type);

Symbol *symtab_lookup(char *name);
void symtab_free_table(SymbolTable *st);
void symtab_assign_offsets();
int symtab_get_declared_var_size();
#endif
