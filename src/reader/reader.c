
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "reader.h"

const char *FILE_SUFFIX = ".txt";
const char *TRAIN_DIRECTORY = "data/";

/**
 *
 * @param file_name šablona jména souboru
 * @param count pořadí generovaného jména
 * @return ukazatel na celé jméno souboru, v případě chyby vrací NULL
 */
char *get_full_name(const char *file_name,int order) {
    char *full_file_name;

    if(!file_name || order < 0)
        return NULL;

    full_file_name = malloc(MAX_FILE_NAME_LENGTH + strlen(FILE_SUFFIX) + sizeof(int));

    if(!full_file_name)
        return NULL;

    sprintf(full_file_name,"%s%.*s%d%s",TRAIN_DIRECTORY,MAX_FILE_NAME_LENGTH,file_name,order,FILE_SUFFIX);

    return full_file_name;
}

/**
 * metoda dostane jako parametr šablonu jmen vzorových souborů, ze kterých se má provádět
 * cvičení. jednotlivá slova
 * @param file_name šablona jména vzorových souborů
 * @param count počet vzorových souborů
 * @param table ukazatel na tabulku, do které se bude provádět zápis unikátních slov nalezených ve vzorových datech
 * @return 1 v případě že se podařilo zpracovat soubor 0 pokud nastala chyba.
 */
 int learn_from_file(const char *file_name,int count,hash_table *table,int feed_type) {
    char *full_name;
    FILE *file;
    char word[MAX_WORD_LENGTH];
    int i;

    if (!file_name || count < 1 || !table)
        return 0;


    for (i = 1; i <= count; ++i) {
        full_name = get_full_name(file_name,i);
        file = fopen(full_name,"r");

        if(!file) {
            printf("cant open file %s ",full_name);
            perror("");
            free(full_name);
            return 0;
        }

        while(read_file(file,word))
            add_to_table(table,word,feed_type);

        free(full_name);
        fclose(file);
    }

    return 1;
}


/**
 *
 * @param file_full_name celé jméno souboru
 * @param buffer ukazatel na pole charů, do kterého se načte další slovo ze souboru
 * @return
 */
char *read_file(FILE *file,char *buffer) {
    if(!file)
        return buffer;

    while(fscanf(file,"%s",buffer) == 1) {
        return buffer;
    }
    return NULL;
}

/**
 * vytvoří nový soubor a otevře pro čtení
 * @param file_full_name celé jméno nového souboru včetně přípony
 * @return
 */
FILE *create_output_file(char *file_full_name) {
    FILE *out_file;
    if(!file_full_name)
        return NULL;

    out_file = fopen(file_full_name,"w");

    if(!out_file)
        return NULL;

    return out_file;
}

/**
 * zapíše předaný výsledek do souboru
 * @param result kod výsledku
 * @param test_name šablona jména testovacího souboru
 * @param i pořadí testovacího souboru
 * @param output_file ukazatel na soubor, do kterého se má provést zápis
 * @return 1 pokud se povedlo výsledek testu zapsat do souboru 0 pokud nastala chyba
 */
int print_result(int result,char *test_name,int i,FILE *output_file) {
    char sign;
    if(!test_name || !output_file)
        return 0;
    if(result == SPAM_FEED)
        sign = 'S';
    else if(result == HAM_FEED)
        sign = 'H';
    else
        sign = 'U';

    fprintf(output_file,"%s%d%s\t%c\n",test_name,i,FILE_SUFFIX,sign);
    return 1;
}




