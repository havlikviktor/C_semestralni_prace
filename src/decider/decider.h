

#ifndef SEMESTRALNI_PRACE_C_DECIDER_H
#define SEMESTRALNI_PRACE_C_DECIDER_H

#include "../table/hashTable.h"

typedef struct Decider {
    double spam_apr;
    double ham_apr;
    int unique_words;
    int spam_words;
    int ham_words;
} decider;

/**
 * metoda vrátí ukazatel na nove vytvorenou strukturu rozhodovace
 * @param spam_count počet souborů označených jako spam
 * @param ham_count počet souborů označených jako ham
 * @return vraci ukazatel nove vytvocenou strukturu rozhodovace, v pripade chyby vraci null
 */
decider *create_decider(int spam_count,int ham_count,int spam_words,int ham_words,int unique_words);

/**
 * metoda rozhodne o tom zdali se obsah předaného souboru má vyhodnotit jako spam či ham
 * @param file_name ukazatel na celé jméno souboru, jehož obsah se vyhodnocuje
 * @param decider ukazatel na rozhodovač, který má soubor vyhodnotit
 * @param table ukazatel na slovník obsahující naučená slova
 * @return kod HAM_FEED pokud je obsah souboru vyhodnocen jako ham, SPAM_FEED pokud je obsah vyhodnocen jako spam
 */
int make_decision(char *file_full_name,decider *decider,hash_table *table);


/**
 * pro zadanou tabulku se vypočítá pravděpodobnost výskutu jejich prvků ve spam a ham zprávách
 * @param table hash tabulka představující slovník naučených slov
 */
void calculate_P(hash_table *table,decider *decider);

/**
 *
 * @param link je ukazatel na první prvek seznamu, pro jehož každý prvek se má vypočítat pravděpodobnost výskytu
 * @param decider na základě jeho dat se určí pravděpodobnost výskutu slova v každé z tříd
 */
int word_Pi(link *link,decider *decider);

#endif
