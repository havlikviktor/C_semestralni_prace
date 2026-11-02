#include <stdio.h>
#include <stdlib.h>

#include "table/hashTable.h"
#include "reader/reader.h"
#include "decider/decider.h"

#define VALID_ARG_COUNT 7

/**
 * zkontoluje zda byl program spuštěn se správným počtem argumentů
 * @param arguments_count počet předaných argumentů programu (+1 název)
 * @return 1 pokud počet předaných argumentů odpovídá zadání 0 pokud nikoliv
 */
int check_argc(int arguments_count) {

    if((arguments_count - 1)  != VALID_ARG_COUNT) {
        printf("arguments given: %d needed: %d\n",arguments_count - 1, VALID_ARG_COUNT);
        printf("arguments must be in format: <spam_files_name> <spam_count> <ham_files_name> <ham_count> <test_files_name> <test_count> <output_file_name>\n");
        printf("note: file names must be without suffix (.txt)");
        return 0;
    }
    return 1;
}

/**
 * funkce naplni predany slovnik daty z testovacich souboru
 * @param spam_name sablona jmen spam souboru
 * @param ham_name sablona jmen ham souboru
 * @param spam_count pocet spam souboru
 * @param ham_count pocet ham souboru
 * @param dictionary slovnik k naplneni
 * @return 1 pokud se povedlo slovnik naplnit daty ze souboru 0 v pripade chyby
 */
int fill_dictionary(char *spam_name,char *ham_name,int spam_count,int ham_count,hash_table *dictionary) {

    if(!learn_from_file(spam_name,spam_count,dictionary,SPAM_FEED)) {
        printf("cant learn from spam files");
        return 0;
    }
    if(!learn_from_file(ham_name,ham_count,dictionary,HAM_FEED)) {
        printf("cant learn from ham files");
        return 0;
    }
    return 1;
}

/**
 * inicializuje pointery na struktury, potřebné ke spuštění testování
 * @param decider ukazatel na rozhodovač v této chvíli se nebude provádět jeho incializace, pouze se nastaví na hodnotu NULL
 * @param new_table ukazatel na hashovací tabulku jež má být vytvořna
 * @param new_file celé jméno výstupního souboru jež se má otevřít pro zápis případně ještě vytvořit
 * @param new_file_name poiter na jméno, které má nést nově vytvořený výstupní soubor
 * @return 1 pokud vytovření proběhlo v pořádku 0 pokud nastala chyba
 */
int prepare_structs(decider **decider,hash_table **new_table,FILE **new_file,char *new_file_name) {
    *new_table = create_table();
    if(!*new_table) {
        perror("failed to create hash table");
        return 0;
    }
    *new_file = create_output_file(new_file_name);
    if (!*new_file) {
        destroy_table(new_table);
        perror("failed to create output file");
        return 0;
    }

    *decider = NULL;
    return 1;
}

/**
 * připraví struktury na daný test
 * @param spam_name šablona jmen pro učební ham soubory
 * @param ham_name počet ham souborů
 * @param spam_count šablona jmen pro učební spam soubory
 * @param ham_count  počet spam souborů
 * @param dictionary ukazatel na hashovací tabulku představující slovník, do kterého se budou kládat slova z učebních souborů
 * @param decider ukazatel na rozhodovač jež má být zinicializován po naplnění slovníku daty z učebních souborů
 * @return 1 pokud proběhla připrava v pořádku 0 pokud nastala chyba
 */
int test_prepare(char *spam_name,char *ham_name,int spam_count,int ham_count,hash_table *dictionary,decider **decider) {

    if(!fill_dictionary(spam_name,ham_name,spam_count,ham_count,dictionary))
        return 0;

    *decider = create_decider(spam_count,ham_count,dictionary->spam_words,dictionary->ham_words,dictionary->unique_words);
    if(!*decider) {
        perror("cant create decider structure");
        return 0;
    }

    calculate_P(dictionary, *decider);
    return 1;
}

/**
 * spusti beh testu, ve kterem se provadi vyhodnocovani testovanych souboru a jejich zarazeni do prislosne tridy ham | spam
 * vyhodnoceni jednotlivych souboru je ukladano do vystupniho souboru
 * @param test_name sablona jmen testovanych souboru
 * @param test_count pocet souboru k otestovani
 * @param table slovnik obsahujici slova ze vzorovych souboru s informaci kolikrat se vyskytovaly v jednotlivych tridach
 * @param out_file cele jmeno souboru, do ktereho se ma ukladat vystup testu
 * @return 1 pokud test probehl uspesne 0 pokud nastala chyba
 */
int run_test(char *test_name,int test_count,hash_table *table,decider *decider,FILE *output_file) {
    char *file_full_name;
    int i,ham,spam,result;
    ham = spam = 0;

    if(!test_name || !table || !output_file)
        return 0;

    for(i = 1;i <= test_count;i++) {
        file_full_name = get_full_name(test_name,i);
        result = make_decision(file_full_name,decider,table);

        if(result == HAM_FEED)
            ham++;
        else if(result == SPAM_FEED)
            spam++;
        else {
            free(file_full_name);
             return 0;
            }
        print_result(result,test_name,i,output_file);
        free(file_full_name);
    }
    printf("hams detected: %d | spams detected: %d",ham,spam);

    return 1;
}

/**
 * ukončovací fáze testu, funkce se stará o uvolnění praměti využívané stukturami potřebné při běhu programu
 * @param table ukazatel na ukazatel hashovací tabulky jež má být uvolněna
 * @param decider ukazatel na ukazatel jež má být uvolněn
 * @param file ukazatel na soubor, do kterého se prováděl záznam o běhu testu a jež má být zavřen
 */
void end_test(hash_table **table,decider **decider,FILE *file) {
    destroy_table(table);
    free(*decider);
    fclose(file);
}

/**
 * vstupní bod programu
 * @param argc počet předaných argumentů
 * @param argv pole ukazatelů na vstupní argumenty
 * @return 0 pokud program úspěsně proběhl 1 pokud nastala za běhu programu chyba
 */
int main(int argc, char *argv[]) {
    hash_table *dictionary;
    decider *decider;
    FILE *output_file;
    int spam_count,ham_count,test_count;

    if(!check_argc(argc)) return 1;
    printf("given arguments: ok\n");

    spam_count = atoi(argv[2]);
    ham_count = atoi(argv[4]);
    test_count = atoi(argv[6]);

    if(!prepare_structs(&decider,&dictionary,&output_file,argv[7])) goto fail;
    printf("created structures: ok\n");

    if(!test_prepare(argv[1],argv[3],spam_count,ham_count,dictionary,&decider)) goto fail;
    printf("structures prepared for test: ok\n\n");

    printf("starting test for %s(1-%d).txt \n",argv[5],test_count);

    if(run_test(argv[5],test_count,dictionary,decider,output_file)) {
        end_test(&dictionary,&decider, output_file);
        return EXIT_SUCCESS;
    }
    else {
        goto fail;
    }

    fail:
    end_test(&dictionary,&decider, output_file);
    return EXIT_FAILURE;
}




