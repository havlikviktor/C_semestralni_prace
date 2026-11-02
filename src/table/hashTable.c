
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashTable.h"

void init_table(hash_table *new_table) {
    int i;
    if(!new_table)
        return;

    new_table->unique_words = 0;
    new_table->ham_words = 0;
    new_table->spam_words = 0;
    for(i = 0; i < TABLE_SIZE;i++) {
        new_table->items[i] = NULL;
    }
}
/**
 *  Funkce pro vytvoření nové hash tabulky.
 * @return ukazatel na nově dinamicky alokovanou hash tabulku
 */
hash_table *create_table(void) {
    hash_table *new_table = malloc(sizeof(hash_table));

    if(!new_table)
        return NULL;

    new_table->items = malloc(sizeof(link) * TABLE_SIZE);

    if(!new_table->items) {
        free(new_table);
        return NULL;
    }

    init_table(new_table);
    return new_table;
}

/**
 * Funkce smaže předaný link, popřípadě všechny další sřetězené linky
 * @param link ukazatel na link, který má být odstraněn
 * @return 1 pokud se podarilo link smazat 0 pokud link neexistuje
 */
int destroy_link(link **destroy_link) {
    link *parent,*child;
    if(!*destroy_link)
        return 0;

    parent = *destroy_link;
    while(parent) {
         child = parent->next;
        free(parent->word);
        free(parent);
        parent = child;
    }

    *destroy_link = NULL;
    return 1;
}
/**
 * Funkce smaže předanou tabulku
 * @param table ukazatel na tabulku, která má být smazána
 * @return 1 pokud se smazání porařilo 0 pokud předaná tabulky neexistuje
 */
int destroy_table(hash_table **table) {
    int i;
    if(!table || !*table)
        return 0;

    for(i = 0; i < TABLE_SIZE;i++) {
        destroy_link(&((*table)->items[i]));
    }

    free((*table)->items);
    free(*table);
    return 1;
}



/**
 *
 * @param hash_word ukazatel na pole znaků, pro které se má vytvořit hash kod
 * @return hash klíč do hashTabulky
 */
int hash(const char *hash_word) {
    int i;
    int length = strlen(hash_word);
    int new_hash = hash_word[0] - FIRST_CHAR_OFFSET;
    char actual_char;

    if(hash_word == NULL)
        return 0;

    for(i = 1; i < length;i++) {
        actual_char = hash_word[i] - FIRST_CHAR_OFFSET;              /* ascii kod a = 97 odectenim 96 je hodnotu znaku a = 1*/
        new_hash = ((new_hash * CHARSET_SIZE) + actual_char) % TABLE_SIZE;
    }

    return new_hash;
}

/**
 *
 * @param table hashovaci tabulka, do které se má přidat nové slovo
 * @param add_word slovo urcene k pridani do tabulky, nemusí být zaručeno že se toto slovo v tabulce jestě nenachází
 * @return 1 pokud se podařilo přidání slova do tabulky 0 v případě chyby.
 */
int add_to_table(hash_table *table,char *add_word,int feed_type) {
    int word_hash = hash(add_word);
    link *existing_link;

    if(!table || !add_word)
        return -1;

    existing_link = is_word_in(table->items[word_hash],add_word);

    if(!existing_link) {
        existing_link = create_link(add_word);

        existing_link->next = table->items[word_hash];
        table->items[word_hash] = existing_link;
        table->unique_words++;
    }

    if(feed_type == HAM_FEED) {
        existing_link->ham_count++;
        table->ham_words++;
    }
    else if(feed_type == SPAM_FEED) {
        existing_link->spam_count++;
        table->spam_words++;
    }

    return 1;
}

/**
 * inicializuje hodnoty linku na výchozí hodnoty
 * @param link link lež má být inicializován
 */
void link_init(link *link) {
    if(!link)
        return;

    link->ham_count = 0;
    link->spam_count = 0;
    link->ham_p = 0;
    link->spam_p = 0;
    link->next = NULL;
}
/**
 * vytvoří nový prvek seznamu a naplní ho předaným slovem
 * @param word ukazatel na slovo jež má být obsahem nově vytvořeného linku
 * @return ukazatel na nově vytovřený link nebo NULL pokud stala při vytváření linku chyba
 */
link *create_link(char *word) {
    link *new_link = malloc(sizeof(link));

    if(!new_link)
        return NULL;

    new_link->word = malloc(strlen(word) + 2);       /* \0 */
    if(!new_link->word) {
        free(new_link);
        return NULL;
    }

    new_link->word = memcpy(new_link->word,word, strlen(word) + 2);   /*2 -> '\0' počet kopírovaných byjtů zvýšen o 2 aby došlo k překopířování znaku pro konec řetězce*/
    link_init(new_link);
    return new_link;
}

/**
 * metoda zjisti zdali v seznamu linku nachazi link obsahujici jako hodnotu pozadovane slovo
 * @param link prvni link seznamu
 * @param word hledane slovo
 * @return ukazatel na link obsahujici dane slovo nebo NULL pokud se dane slovo v seznamu nevyskytuje
 */
link *is_word_in(link *link,char *word) {
    if(!word)
        return NULL;

    while(link != NULL) {
        if(!strcmp(link->word,word))
            return link;
        link = link->next;
    }

    return NULL;
}


/**
 * (testovací metoda) ukazuje rozložení prvku v tabulce a vytížení jednotlivých indexů
 * @param table ukazatel na tabulku, o které chceme zístat informace
 */
void show_table_distribution(hash_table *table) {
    int i,max,actual;

    if(!table)
        return;

    max = 0;

    for(i = 0;i < TABLE_SIZE;i++) {
        link *link = table->items[i];
        actual = 0;

        while(link != NULL) {
            ++actual;
            link = link->next;
        }
        if(actual > max)
            max = actual;
    }

    printf("max index load is %d, unique words in table %d\n",max,table->unique_words);
}
