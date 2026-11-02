
#ifndef SEMESTRALNI_PRACE_C_READER_H
#define SEMESTRALNI_PRACE_C_READER_H

#include <stdio.h>
#include "../table/hashTable.h"

#define MAX_FILE_NAME_LENGTH 30
#define MAX_WORD_LENGTH 100

/**
 * ze šablony jména souboru a jeho pořadí vytovří celé jméno tohoto soubor včetně cesty
 * @param file_name šablona jména souboru
 * @param count pořadí generovaného jména
 * @return ukazatel na celé jméno souboru, v případě chyby vrací NULL
 */
char *get_full_name(const char *file_name,int order);

/**
 * metoda dostane jako parametr šablonu jmen vzorových souborů, ze kterých se má provádět
 * cvičení. jednotlivá slova
 * @param file_name šablona jména vzorových souborů
 * @param count počet vzorových souborů
 * @param table ukazatel na tabulku, do které se bude provádět zápis unikátních slov nalezených ve vzorových datech
 * @return
 */
int learn_from_file(const char *file_name,int count,hash_table *table,int feed_type);

/**
 *
 * @param file ukazatel na soubor z kterého se má číst
 * @param buffer ukazatel na pole charů, do kterého se načte další slovo ze souboru
 * @return ukazatel na rezezec predravujici dalsi nactene slovo ze souboru nebo null v pripade že neexistuje dalsi slovo
 */
char *read_file(FILE *file,char *buffer);

/**
 *
 * @param file_full_name cele jmeno souboru včetně přípony
 * @return vrací ukazatel na nově vytvořený soubor nebo NULL v případě chyby
 */
FILE *create_output_file(char *file_full_name);

/**
 * zapíše předaný výsledek do souboru
 * @param result kod vysledku, ktery se ma vytisknout, urcuje zdali se je soubor spam 'S' ham 'H' nebo dosloli k neocekavane chybe 'U'
 * @param test_name sablona jmena testovaneho souboru
 * @param i poradu v testovanych souborech
 * @param output_file ukazatel na soubor, do kterého se má provést zápis
 * @return 1 pokud se povedlo výsledek testu zapsat do souboru 0 pokud nastala chyba
 */
int print_result(int result,char *test_name,int i,FILE *output_file);


#endif
