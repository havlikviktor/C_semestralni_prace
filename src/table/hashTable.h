
#ifndef SEMESTRALNI_PRACE_C_HASHTABLE_H
#define SEMESTRALNI_PRACE_C_HASHTABLE_H

#include <stddef.h>
/**
 * velikost tabulky
 */
#define TABLE_SIZE 1019
/**
 * počet písmen abecedy, ze kterých lze sestavit název souboru
 */
#define CHARSET_SIZE 26
/**
 * odchylka prvního znaku abecedy od nuly v zápis ascii
 */
#define FIRST_CHAR_OFFSET 96
/**
 * kod definující že se jedná o ham
 */
#define HAM_FEED 69
/**
 * kod definující že se jedná o spam
 */
#define SPAM_FEED 420

typedef struct Link {
    char *word;                /* uložené slovo*/
    int ham_count;             /* četnost daného slova v ham zprávách*/
    int spam_count;            /* četnost daného slova ve spam zprávách*/
    double ham_p;              /* pravdepodobnost vyskytu slova ve tride ham*/
    double spam_p;             /* pravdepodobnost vyskytu slova ve tride spam*/
    struct Link *next;         /*další link seznamu*/
} link;

typedef struct Hash_table {
    int unique_words;         /* počet unikátní slov v tabulce*/
    int spam_words;           /* celkový počet slov ve tride spam vcetne opakujicich se*/
    int ham_words;           /* celkový počet slov ve tride ham vcetne opakujicich se*/
    link **items;            /* pole seznamů slov v tabulce*/
} hash_table;

/**
 *
 * @param hash_word ukazatel na pole znaků, pro které se má vytvořit hash kod
 * @return hash clíč do hashTabulky
 */
int hash(const char *hash_word);

/**
 *  Funkce pro vytvoření nové hash tabulky.
 * @return ukazatel na nově dinamicky alokovanou hash tabulku
 */
hash_table *create_table(void);

/**
 * Funkce smaže předanou tabulku
 * @param table ukazatel na tabulku, která má být smazána
 * @return 1 pokud se smazání porařilo 0 pokud nastala chyba
 */
int destroy_table(hash_table **table);

/**
 *
 * @param table hashovaci tabolka, do které se má přidat nové slovo
 * @param add_word slovo urcene k pridani do tabulky, nemusí být zaručeno že se toto slovo v tabulce jestě nenachází
 * @return 1 pokud se podařilo přidání slova do tabulky 0 v případě chyby.
 */
int add_to_table(hash_table *table,char *add_word,int feed_type);

/**
 *  Funkce pro vytvoření nové hash tabulky.
 * @return ukazatel na nově dinamicky alokovanou hash tabulku
 */
link *create_link(char *word);

/**
 * inicializuje hodnoty linku na výchozí hodnoty
 * @param link link lež má být inicializován
 */
void link_init(link *link);

/**
 * metoda zjisti zdali v seznamu linku nachazi link obsahujici jako hodnotu pozadovane slovo
 * @param link prvni link seznamu
 * @param word hledane slovo
 * @return ukazatel na link obsahujici dane slovo nebo NULL pokud se dane slovo v seznamu nevyskytuje
 */
link *is_word_in(link *link,char *add_word);

/**
 * Funkce smaže předaný link, popřípadě všechny další sřetězené linky
 * @param link ukazatel na link, který má být odstraněn
 * @return 1 pokud se podarilo link smazat 0 pokud link neexistuje
 */
int destroy_link(link **destroy_link);

/**
 * (testovací metoda) ukazuje rozložení prvku v tabulce a vytížení jednotlivých indexů
 * @param table ukazatel na tabulku, o které chceme zístat informace
 */
void show_table_distribution(hash_table *table);
#endif
