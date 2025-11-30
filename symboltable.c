#include "symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Global Scope Stack Management ---

// The current symbol table/scope. This acts as the head of the scope stack.
static SymbolTable *current_scope = NULL;
// Tracks the current nesting level.
static int current_scope_level = 0;

void symtab_enter_scope() {
  SymbolTable *new_scope = symtab_mk_table();
  // Link the new scope to the current scope (push onto stack)
  new_scope->prev = current_scope;
  current_scope = new_scope;
  current_scope_level++;
  // printf("Entered scope level: %d\n", current_scope_level);
}

void symtab_exit_scope() {
  if (current_scope == NULL) {
    fprintf(stderr,
            "ERROR: Attempted to exit scope when no scope is active.\n");
    return;
  }

  SymbolTable *old_scope = current_scope;
  // Move to the outer scope (pop from stack)
  current_scope = current_scope->prev;
  symtab_free_table(old_scope);
  current_scope_level--;
  // printf("Exited scope level: %d\n", current_scope_level);
}

int symtab_get_current_scope() { return current_scope_level; }

// --- Internal Utilities ---

unsigned int symtab_hash(char *name) {
  unsigned int hash = 5381;
  int c;
  while ((c = *name++))
    hash = ((hash << 5) + hash) + c;
  return hash % SYMBOL_TABLE_SIZE;
}

Symbol *symtab_mk_symbol(char *name, SymbolType type) {
  Symbol *s = (Symbol *)malloc(sizeof(Symbol));
  if (!s) {
    perror("malloc Symbol");
    exit(EXIT_FAILURE);
  }

  // strdup is used to duplicate the string, so we own the memory for the name
  s->name = strdup(name);
  if (!s->name) {
    perror("strdup Symbol name");
    free(s);
    exit(EXIT_FAILURE);
  }
  s->type = type;
  s->scope = current_scope_level;

  return s;
}

SymbolTable *symtab_mk_table() {
  SymbolTable *st = (SymbolTable *)malloc(sizeof(SymbolTable));
  if (!st) {
    perror("malloc SymbolTable");
    exit(EXIT_FAILURE);
  }
  // Initialize all buckets to NULL
  for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
    st->table[i] = NULL;
  }
  st->prev = NULL;
  return st;
}

// --- Core Operations Implementation ---

int symtab_insert(char *name, SymbolType type) {
  if (current_scope == NULL) {
    symtab_enter_scope();
  }

  unsigned int index = symtab_hash(name);
  SymbolTableEntry *current = current_scope->table[index];

  // 1. Check for duplicate in the CURRENT scope (for a C-like language)
  while (current != NULL) {
    if (strcmp(current->symbol->name, name) == 0) {
      // Symbol already exists in the current scope
      return -1;
    }
    current = current->next;
  }

  // 2. Create new symbol and table entry
  Symbol *new_symbol = symtab_mk_symbol(name, type);

  SymbolTableEntry *new_entry =
      (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
  if (!new_entry) {
    perror("malloc SymbolTableEntry");
    // Since strdup was used in mk_symbol, we must free the name
    free(new_symbol->name);
    free(new_symbol);
    exit(EXIT_FAILURE);
  }

  new_entry->symbol = new_symbol;

  // 3. Insert at the head of the linked list (chaining)
  new_entry->next = current_scope->table[index];
  current_scope->table[index] = new_entry;

  return 0; // Success
}

Symbol *symtab_lookup(char *name) {
  SymbolTable *scope_ptr = current_scope;

  // Search through the scope stack from inner to outer
  while (scope_ptr != NULL) {
    unsigned int index = symtab_hash(name);
    SymbolTableEntry *current = scope_ptr->table[index];

    // Traverse the linked list (chain)
    while (current != NULL) {
      if (strcmp(current->symbol->name, name) == 0) {
        // Found the symbol
        return current->symbol;
      }
      current = current->next;
    }

    // Move to the outer scope
    scope_ptr = scope_ptr->prev;
  }

  return NULL; // Not found in any scope
}

// --- Cleanup Implementation ---

void symtab_free_table(SymbolTable *st) {
  if (st == NULL)
    return;

  for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
    SymbolTableEntry *current = st->table[i];
    while (current != NULL) {
      SymbolTableEntry *to_free = current;
      current = current->next;

      // Free the symbol's name and the symbol itself
      free(to_free->symbol->name);
      free(to_free->symbol);
      // Free the list node (table entry)
      free(to_free);
    }
  }

  // Free the table structure itself
  free(st);
}

// Atribui offsets a todas as variáveis do scope global (simplificado)
// No MIPS, vamos guardar as variáveis a partir de $fp (Frame Pointer)
void symtab_assign_offsets() {
    if (!current_scope) return;
    
    int current_offset = 0;
    
    // Percorre todos os buckets da tabela hash
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        SymbolTableEntry *entry = current_scope->table[i];
        while (entry != NULL) {
            // Cada inteiro ocupa 4 bytes
            current_offset -= 4; 
            entry->symbol->offset = current_offset;
            // printf("Var %s offset: %d\n", entry->symbol->name, current_offset);
            entry = entry->next;
        }
    }
}
