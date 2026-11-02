
#include <stdlib.h>
#include <math.h>

#include "decider.h"
#include "../reader/reader.h"

/**
 * metoda vrátí ukazatel na nove vytvorenou strukturu rozhodovace
 * @param spam_count počet souborů označených jako spam
 * @param ham_count počet souborů označených jako ham
 * @return vraci ukazatel nove vytvocenou strukturu rozhodovace, v pripade chyby vraci null
 */
decider *create_decider(int spam_count,int ham_count,int spam_words,int ham_words,int unique_words) {
    decider *new_decider;
    if(spam_count < 0 || ham_count < 0)
        return NULL;

    new_decider = malloc(sizeof(decider));
    if(!new_decider)
        return NULL;

    new_decider->ham_apr = (double) ham_count / (ham_count + spam_count); /** implicintni konverze (ham + spam) */
    new_decider->spam_apr =(double) spam_count / (ham_count + spam_count);
    new_decider->unique_words = unique_words;
    new_decider->ham_words = ham_words;
    new_decider->spam_words = spam_words;

    return new_decider;
}

/**
 * metoda rozhodne o tom zdali se obsah předaného souboru má vyhodnotit jako spam či ham
 * @param file_name ukazatel na celé jméno souboru, jehož obsah se vyhodnocuje
 * @param decider ukazatel na rozhodovač, který má soubor vyhodnotit
 * @param table ukazatel na slovník obsahující naučená slova
 * @return kod HAM_FEED pokud je obsah souboru vyhodnocen jako ham, SPAM_FEED pokud je obsah vyhodnocen jako spam
 */
int make_decision(char *file_full_name,decider *decider,hash_table *table) {
    double spam_sum,ham_sum;
    FILE *file;
    link *link;
    int word_hash;
    char word[MAX_WORD_LENGTH];
    ham_sum = spam_sum = 0;

    file = fopen(file_full_name,"r");

    if(!file) {
        printf("cant open file %s %d",file_full_name,decider->ham_words);
        return 0;
    }


    while(read_file(file,word)) {
        word_hash = hash(word);
       link = is_word_in(table->items[word_hash],word);

       if(link) {
           spam_sum += link->spam_p;
           ham_sum += link->ham_p;
       }
    }
    fclose(file);
    /* spam_sum *= decider->spam_apr; */
    /* ham_sum *= decider->ham_apr; */
  /* printf("decided by spam: %.10f ham %.10f\n",spam_sum,ham_sum);*/
    return (spam_sum > ham_sum) ? SPAM_FEED : HAM_FEED;
}


/**
 * pro zadanou tabulku se vypočítá pravděpodobnost výskutu jejich prvků ve spam a ham zprávách
 * @param table hash tabulka představující slovník naučených slov
 */
void calculate_P(hash_table *table,decider *decider) {
    int i;
    if(!table || !decider)
        return;

    for(i = 0;i < TABLE_SIZE;i++) {
        word_Pi(table->items[i],decider);
    }
}

/**
 *
 * @param link je ukazatel na první prvek seznamu, pro jehož každý prvek se má vypočítat pravděpodobnost výskytu
 * @param decider na základě jeho dat se určí pravděpodobnost výskutu slova v každé z tříd
 */
int word_Pi(link *link,decider *decider) {
    double tmp;
    if(!decider)
        return 0;
    while(link) {
    tmp = (double) (link->spam_count + 1) / (decider->spam_words + decider->unique_words);
    link->spam_p = log(tmp);
    tmp =(double) (link->ham_count + 1) / (decider->ham_words + decider->unique_words);
    link->ham_p = log(tmp);
    link = link->next;
    }
    return 1;
}








