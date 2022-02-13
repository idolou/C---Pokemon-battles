//
// Created by ise on 12/16/21.
//
#include <stdio.h>
#include "Pokemon.h"
#include "Defs.h"
#include <stdlib.h>
#include <string.h>
#include "BattleByCategory.h"

///pokemon functions
///*************************HELP FUNCTION*******************/
pok_type *find_type(pok_type **types, char *type, int length) {
    if (types == NULL || type == NULL) {
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        if (types[i] == NULL) { return NULL; }
        if (strcmp(types[i]->name, type) == 0)
            return types[i];
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    ///Program arguments
    int numberOfTypes = strtol(argv[1], NULL, 10);
    int capacityInType = strtol(argv[2], NULL, 10);
    char *configurationFile = (argv[3]);



    ///Program util variables
    bool err = false;
    int option = 0;
    bool file_null = false;  //if file is empty
    char line[buff];     //will help separate the txt file
    char line2[buff];    //will help separate the txt file
    char *word = NULL;           //will help save certain words from txt file
    char* end;
    char type_input[buff];       //to store data from user

    pok_type *sourceA = NULL;  //for type insert and deletion
    pok_type *addB = NULL;    //for type insert and deletion
    Pokemon *pokemon_to_remove = NULL; //for menu functionality
    Pokemon_details *details = NULL;  //for menu functionality
    Pokemon *pokemon_toAdd = NULL;  //for menu functionality


    ///initial array of pointers in the size of number of pokemon Types that the program gets in configuration



///////////////////////*** loading the txt file into the program***//////////////////////////////
    FILE *file = NULL;
    file = fopen(configurationFile, "r");
    if (file == NULL) {
        file_null = true;
        goto menu;
    }


    fscanf(file, "%s", line);   //get rid of first line (Types)
    fscanf(file, "%s", line);  // load next line - Types

    if (strcmp(line, "Pokemons") == 0) {   //if there is no types at all
        goto File_close;
    }


    ///***create new battle, will store all the Data***///
    Battle battle = createBattleByCategory(capacityInType, numberOfTypes, line, equalPokemon,
                                           DeepCopyPokemon, free_pokemon,
                                           getType, getAttack, print_pokemon);
    if (battle == NULL) {
        printf("Memory Problem\n");
        err = true;
        goto exit;
    }


    pok_type **Types = (pok_type **) malloc(sizeof(pok_type *) * numberOfTypes);
    if (Types == NULL) {
        destroyBattleByCategory(battle);
        printf("Memory Problem\n");
        err = true;
        goto exit;
    }
    for (int i = 0; i < numberOfTypes; i++) {
        Types[i] = NULL;
    }

    int i = 0;
    word = strtok(line, ",");
    while (word != NULL) {

        Types[i] = init_type(word);     //add each Types to Types which is Dynamic array of all Types in program
        if (Types[i] == NULL) {
            printf("Memory Problem\n");
            err = true;
            goto exit;
        }

        word = strtok(NULL, ",");  //get the next word(type) after comma(',')
        i++;
    }


    ///load to each type - against me/ others Types///
    fscanf(file, "%s" "%s", line, line2);

    char *me = "effective-against-me";
    while (strcmp(line, "Pokemons") != 0) {
        word = strtok(line2, ":");
        if (strcmp(word, me) == 0) {                   //case 1 the line contains Types effective against me
            word = strtok(NULL, ",");
            while (word != NULL) {
                sourceA = find_type(Types, line, numberOfTypes);
                addB = find_type(Types, word,
                                 numberOfTypes);  //use find type func to return the type of pokemon if found
                if (add_against_me(sourceA, addB) == failure) {
                    printf("Memory Problem\n");
                    err = true;
                    goto exit;
                }
                word = strtok(NULL, ",");   // line =  the source type, word = type to add, Types is the dynamic array
            }
        } else {                                         //case 2 the line contains Types effective against me
            word = strtok(NULL, ",");
            while (word != NULL) {
                sourceA = find_type(Types, line, numberOfTypes);
                addB = find_type(Types, word, numberOfTypes);
                if (add_against_others(sourceA, addB) == failure) {
                    printf("Memory Problem\n");
                    err = true;
                    goto exit;
                }
                word = strtok(NULL, ",");
            }
        }

        if (fscanf(file, "%s", line) == EOF) {
            goto File_close;
        }
        if (fscanf(file, "%s", line2) == EOF) {
            goto File_close;
        }


        sourceA = NULL;
        addB = NULL;
    }

    ///***insert all the Pokemons by their type***///
    double height;
    double weight;
    int power;
    char name[buff];
    char species[buff];


    word = strtok(line2, ",");
    while (word != NULL) {
        while (word != NULL) {
            strcpy(name, word);
            word = strtok(NULL, ",");
            strcpy(species, word);
            word = strtok(NULL, ",");
            height = (double) atof(word);
            word = strtok(NULL, ",");
            weight = (double) atof(word);
            word = strtok(NULL, ",");
            power = (int) atoi(word);
            word = strtok(NULL, ",");

            sourceA = find_type(Types, word, numberOfTypes);
            details = init_pokemon_details(height, weight, power);
            pokemon_toAdd = init_Pokemon(name, species, sourceA,
                                         details);
            if (insertObject(battle, pokemon_toAdd) == failure) {
                printf("Memory Problem\n");
                err = true;
                free_pokemon(pokemon_toAdd);
                goto exit;
            }
            word = strtok(NULL, ",");
            sourceA = NULL;
            details = NULL;
        }
        if (fscanf(file, "%s", line2) == EOF) { break; }      // scan the next line - next pokemon
        word = strtok(line2, ",");
    }
    File_close:
    word = NULL;
    fclose(file);  //close the configurationFile after all data inserted



///Main menu///
    while (option != 6) {
        menu:
        printf("Please choose one of the following numbers:\n");
        printf("1 : Print all Pokemons by types\n");
        printf("2 : Print all Pokemons types\n");
        printf("3 : Insert Pokemon to battles training camp\n");
        printf("4 : Remove strongest Pokemon by type\n");
        printf("5 : Fight\n");
        printf("6 : Exit\n");
        scanf(" %s", line);
        option = strtol(line, NULL, 10);
        switch (option) {
            case 1:
                if (numberOfTypes < 1 || file_null) { break; }
                displayObjectsByCategories(battle);
                break;
            case 2:
                if (numberOfTypes < 1 || file_null) { break; }
                for (int s = 0; s < numberOfTypes; s++) {
                    print_type(Types[s], getNumberOfObjectsInCategory(battle, Types[s]->name));
                }
                break;
            case 3:
                printf("Please enter Pokemon type name:\n");
                scanf("%s", type_input);
                sourceA = find_type(Types, type_input, numberOfTypes);
                if (sourceA == NULL) {
                    printf("Type name doesn't exist.\n");
                    break;
                }
                if (getNumberOfObjectsInCategory(battle, sourceA->name) >= capacityInType) {
                    printf("Type at full capacity.\n");
                    break;
                }
                printf("Please enter Pokemon name:\n");
                scanf("%s", name);
                printf("Please enter Pokemon species:\n");
                scanf("%s", species);
                printf("Please enter Pokemon height:\n");
                scanf("%s", line);
                height = strtod(line, &word);
                printf("Please enter Pokemon weight:\n");
                scanf("%s", line);
                weight = strtod(line, &word);
                printf("Please enter Pokemon attack:\n");
                scanf("%s", word);
                power = strtol(word, &end, 10);
                if(word==end || weight == 0 || height == 0){
                    printf("Please enter valid numbers.\n");
                    break;
                }
                details = init_pokemon_details(height, weight, power);
                pokemon_toAdd = init_Pokemon(name, species, sourceA,
                                             details);
                if (insertObject(battle, pokemon_toAdd) == failure) {
                    printf("Memory Problem\n");
                    err = true;
                    goto exit;
                }
                printf("The Pokemon was successfully added.\n");
                print_pokemon(pokemon_toAdd);
                word = NULL;
                end = NULL;
                break;
            case 4:
                printf("Please enter type name:\n");
                scanf("%s", type_input);
                sourceA = find_type(Types, type_input, numberOfTypes);
                if (sourceA == NULL) {
                    printf("Type name doesn't exist.\n");
                    break;
                } else if (getNumberOfObjectsInCategory(battle, sourceA->name) < 1) {
                    printf("There is no Pokemon to remove.\n");
                    break;
                } else {
                    pokemon_to_remove = removeMaxByCategory(battle, sourceA->name);
                    if (pokemon_to_remove == NULL) {
                        printf("Memory problem.\n");
                        goto exit;
                    }
                    printf("The strongest Pokemon was removed:\n");
                    print_pokemon(pokemon_to_remove);
                    free_pokemon(pokemon_to_remove);
                }
                break;
            case 5:
                printf("Please enter Pokemon type name:\n");
                scanf("%s", type_input);
                sourceA = find_type(Types, type_input, numberOfTypes);
                if (sourceA == NULL) {
                    printf("Type name doesn't exist.\n");
                    break;
                }
                printf("Please enter Pokemon name:\n");
                scanf("%s", name);
                printf("Please enter Pokemon species:\n");
                scanf("%s", species);
                printf("Please enter Pokemon height:\n");
                scanf("%s", line);
                height = strtod(line, &word);
                printf("Please enter Pokemon weight:\n");
                scanf("%s", line);
                weight = strtod(line, &word);
                printf("Please enter Pokemon attack:\n");
                scanf("%s", word);
                power = strtol(word, &end, 10);
                if(end == word || weight == 0 || height == 0){
                    printf("Please enter valid numbers.\n");
                    break;
                }

                pokemon_toAdd = init_Pokemon(name, species, sourceA,
                                             init_pokemon_details(height, weight, power));
                if (!pokemon_toAdd) {
                    printf("Type name doesn't exist.\n");
                    err = true;
                    goto exit;
                }
                printf("You choose to fight with:\n");
                print_pokemon(pokemon_toAdd);
                fight(battle, pokemon_toAdd);
                free_pokemon(pokemon_toAdd);
                word = NULL;
                end = NULL;
                break;

            case 6:
                goto exit;
            default:
                printf("Please choose a valid number.\n");
        }
    }


    exit:
    if (file_null == false) {
        for (int j = 0; j < numberOfTypes; j++) {
            if (Types[j]) { free_type(Types[j]); }
        }
    }
    if (Types)
        free(Types);
    if (battle)
        destroyBattleByCategory(battle);
    printf("All the memory cleaned and the program is safely closed.\n");
    if(err == true){
        exit(1);
    }
    exit(0);

}





