//
// Created by ise on 11/19/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pokemon.h"
#include "Defs.h"


status find_against_me(pok_type *type, pok_type *against_me) {
    if (type == NULL || against_me == NULL) {
        return failure;
    }
    for (int i = 0; i < type->against_me_len; i++) {
        if (strcmp(against_me->name, type->effective_against_me[i]->name) == 0)
            return success;
    }
    return failure;
}

status find_against_others(pok_type *type, pok_type *against_other) {
    if (type == NULL || against_other == NULL) {
        return failure;
    }
    for (int i = 0; i < type->against_other_len; i++) {
        if (strcmp(against_other->name, type->effective_against_others[i]->name) == 0)
            return success;
    }
    return failure;
}

pok_type *init_type(char *type_name) {
    if (type_name == NULL)
        return NULL;
    pok_type *new_type = (pok_type *) malloc(sizeof(pok_type));
    if (new_type == NULL) {
        return NULL;
    }
    // +1 for the \0
    new_type->name = (char *) malloc(sizeof(char) * strlen(type_name) + 1);
    if (new_type->name == NULL) {
        return NULL;
    }
    strcpy(new_type->name, type_name);

    //will update upon insert to battle
    new_type->quantity = 0;

    //will initialize the dynamic list on add_against_others func
    new_type->effective_against_others = NULL;
    new_type->against_me_len = 0;

    //will initialize the dynamic list on add_against_others func
    new_type->effective_against_me = NULL;
    new_type->against_other_len = 0;
    return new_type;
}

status free_type(pok_type *type) {
    if (type->effective_against_others != NULL)
        free(type->effective_against_others);
    if (type->effective_against_me != NULL)
        free(type->effective_against_me);
    free(type->name);
    free(type);
    type = NULL;
    return success;
}

Pokemon_details *init_pokemon_details(double height, double weight, int power) {
    Pokemon_details *bio = (Pokemon_details *) malloc(sizeof(Pokemon_details));
    if (bio == NULL) {
        return NULL;
    }
    bio->height = height;
    bio->weight = weight;
    bio->power = power;
    return bio;
}

Pokemon *init_Pokemon(char *name, char *species, pok_type *type_n, Pokemon_details *bio) {

    Pokemon *new_pokemon = (Pokemon *) malloc(sizeof(Pokemon));
    if (new_pokemon == NULL || bio == NULL || type_n == NULL) {
        return NULL;
    }

    new_pokemon->name = (char *) malloc(sizeof(char) * strlen(name) + 1);
    if (new_pokemon->name == NULL) {
        return NULL;
    }
    strcpy(new_pokemon->name, name);

    new_pokemon->species = (char *) malloc(sizeof(char) * strlen(species) + 1);
    if (new_pokemon->species == NULL) {
        return NULL;
    }
    strcpy(new_pokemon->species, species);

    //assigning pointers to Pokemon_details and pok_type structs
    new_pokemon->type = type_n;
    new_pokemon->details = bio;
    return new_pokemon;
}

status free_pokemon(element pokemon) {
    Pokemon *pok = (Pokemon *) pokemon;
    free(pok->details);
    free(pok->name);
    free(pok->species);
    free(pok);
    return success;
}

status add_against_me(pok_type *source_A, pok_type *add_B) {
    if (source_A == NULL || add_B == NULL) {
        return failure;
    }
    if (source_A->against_me_len == 0) {
        source_A->effective_against_me = malloc(sizeof(pok_type *));
        if (source_A->effective_against_me == NULL) {
            return failure;
        }
        source_A->effective_against_me[0] = add_B;
        source_A->against_me_len += 1;
        return success;
    } else {
        for (int i = 0; i < source_A->against_me_len; i++) {
            if (strcmp(add_B->name, source_A->effective_against_me[i]->name) == 0)
                return failure;
        }
        source_A->effective_against_me =
                realloc(source_A->effective_against_me, sizeof(pok_type *) * (source_A->against_me_len + 1));
        if (source_A->effective_against_me == NULL) {
            return failure;
        }
        source_A->effective_against_me[source_A->against_me_len] = add_B;
        source_A->against_me_len += 1;
        return success;
    }
}

status add_against_others(pok_type *sourceA, pok_type *addB) {
    if (sourceA == NULL || addB == NULL) {
        return failure;
    }
    if (sourceA->against_other_len == 0) {
        sourceA->effective_against_others = malloc(sizeof(pok_type *));
        if (sourceA->effective_against_others == NULL) {
            return failure;
        }
        sourceA->effective_against_others[0] = addB;
        sourceA->against_other_len += 1;
        return success;
    } else {
        for (int i = 0; i < sourceA->against_other_len; i++) {
            if (strcmp(addB->name, sourceA->effective_against_others[i]->name) == 0)
                return failure;
        }
        sourceA->effective_against_others =
                realloc(sourceA->effective_against_others, sizeof(pok_type *) * (sourceA->against_other_len + 1));
        if (sourceA->effective_against_others == NULL) {
            return failure;
        }
        sourceA->effective_against_others[sourceA->against_other_len] = addB;
        sourceA->against_other_len += 1;
        return success;
    }
}

status print_pokemon(element pokemon) {
    Pokemon *pok = (Pokemon *) pokemon;
    if (!pok || !pok->type|| !pok->species || !pok->details)
        return failure;
    printf("%s :\n", pok->name);
    printf("%s, %s Type.\n", pok->species, pok->type->name);
    printf("Height: %.2f m    Weight: %.2f kg    Attack: %d\n\n",
           pok->details->height, pok->details->weight, pok->details->power);
    return success;
}

status print_type(pok_type *type, int quantity) {
    if (type == NULL) {
        return failure;
    }
    int x;
    if (type->against_me_len == 0 && type->against_other_len == 0) { x = 1; }
    else if (type->against_me_len > 0 && type->against_other_len == 0) { x = 2; }
    else if (type->against_me_len == 0 && type->against_other_len > 0) { x = 3; }
    else { x = 4; }
    switch (x) {
        case 1:
            printf("Type %s -- %d pokemons\n\n", type->name, quantity);
            return success;
        case 2:
            printf("Type %s -- %d pokemons\n", type->name, quantity);
            printf("\tThese types are super-effective against %s:", type->name);
            for (int i = 0; i < type->against_me_len - 1; i++) {
                printf("%s ,", type->effective_against_me[i]->name);
            }
            printf("%s\n\n", type->effective_against_me[type->against_me_len - 1]->name);
            return success;
        case 3:
            printf("Type %s -- %d pokemons\n", type->name, quantity);
            printf("\t%s moves are super-effective against:", type->name);
            for (int i = 0; i < type->against_other_len - 1; i++) {
                printf("%s ,", type->effective_against_others[i]->name);
            }
            printf("%s\n\n", type->effective_against_others[type->against_other_len - 1]->name);
            return success;
        case 4:
            printf("Type %s -- %d pokemons\n", type->name, quantity);
            printf("\tThese types are super-effective against %s:", type->name);
            for (int i = 0; i < type->against_me_len - 1; i++) {
                printf("%s ,", type->effective_against_me[i]->name);
            }
            printf("%s\n", type->effective_against_me[type->against_me_len - 1]->name);
            printf("\t%s moves are super-effective against:", type->name);
            for (int i = 0; i < type->against_other_len - 1; i++) {
                printf("%s ,", type->effective_against_others[i]->name);
            }
            printf("%s\n\n", type->effective_against_others[type->against_other_len - 1]->name);
            return success;
        default:
            return failure;
    }
}


///equal the Attack field of 2 pokemons///
int equalPokemon(element pokemonA, element pokemonB) {
    if (!pokemonA || !pokemonB) { return -10; }
    Pokemon *pokA = (Pokemon *) pokemonA;
    Pokemon *pokB = (Pokemon *) pokemonB;
    int a_attack = pokA->details->power;
    int b_attack = pokB->details->power;
    if (a_attack > b_attack) { return 1; }
    if (a_attack < b_attack) { return -1; }
    else return 0;
}

element DeepCopyPokemon(element pokemon) {
    if (!pokemon) { return NULL; }
    Pokemon *sourcePokemon = (Pokemon *) pokemon;
    Pokemon *pokemon_copy = init_Pokemon(sourcePokemon->name, sourcePokemon->species,
                                         sourcePokemon->type, sourcePokemon->details);
    if (!pokemon_copy) { return NULL; }
    return pokemon_copy;
}

char *getType(element pokemon) {
    if (!pokemon) { return NULL; }
    Pokemon *pok = (Pokemon *) pokemon;
    return pok->type->name;
}

int getAttack(element pokemonA, element pokemonB, int *attackFirst, int *attackSecond) {
    Pokemon *pokA = (Pokemon *) pokemonA;
    Pokemon *pokB = (Pokemon *) pokemonB;
    int result;
    *attackFirst = pokA->details->power;
    *attackSecond = pokB->details->power;
    /// if pok B is in affective against me list of pokemon A, or pok A is
    /// in effective against other list of pok B then reduce 10 points from A attack.
    if (find_against_me(pokA->type, pokB->type) == success || find_against_others(pokB->type, pokA->type) == success) {
        *attackFirst -= 10;
    }
    /// if pok A is in affective against me list of pokemon B, or pok B is
    /// in effective against other list of pok A then reduce 10 points from B attack.
    if (find_against_me(pokB->type, pokA->type) == success || find_against_others(pokA->type, pokB->type) == success) {
        *attackSecond -= 10;
    }
    result = *attackFirst - *attackSecond;
    return result;
}

