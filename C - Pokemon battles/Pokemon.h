//
// Created by ise on 11/19/21.
//

#include "Defs.h"
#include <stdlib.h>
#include <string.h>

#ifndef POKEMONS_POKEMON_H
#define POKEMONS_POKEMON_H

/// rename the struct type pok_type. will hold the type name, number of Pokemon with the same type
///the struct will store all the other types that effective against him and the types he is effective against them
///the fields against_me_len, against_other_len will update upon insertion or deletion.
typedef struct pok_type {
    char *name;
    int quantity;
    int against_me_len;
    int against_other_len;
    struct pok_type **effective_against_me;
    struct pok_type **effective_against_others;
} pok_type;


///args: height can be decimal, weight can be decimal, power is int and holds the power of the Pokemon
typedef struct Pokemon_details {
    double height;
    double weight;
    int power;
} Pokemon_details;

///rename struct Pokemon to Pokemon will have:
///          name: name of the Pokemon that
///          species: the species of the Pokemon
///          type: will hold the struct type_of_Pokemon
///          details: will hold the struct Pokemon_details
typedef struct Pokemon {
    char *name;
    char *species;
    pok_type *type;
    Pokemon_details *details;
} Pokemon;

///function to create new type_name of Pokemon
pok_type *init_type(char *type_name);

///function to free(delete) type of Pokemon
status free_type(pok_type *type);

///function to create the detail of a Pokemon
Pokemon_details *init_pokemon_details(double height, double weight, int power);

///function to create new pokemon using the type of pokemon and detail of Pokemon
Pokemon *init_Pokemon(char *name, char *species, pok_type *, Pokemon_details *);

/// func to free all pokemon memory located in heap at end of the program
status free_pokemon(element pokemon);

///print pokemon function
status print_pokemon(element pokemon);

///print pokemon type function
status print_type(pok_type *type, int quantity);

///find if specific pokemon type is effective against me
status find_against_me(pok_type *type, pok_type *against_me);

///find if specific pokemon type is effective against other
status find_against_others(pok_type *type, pok_type *against_other);

///check if 2 pokemons are equal
int equalPokemon(element pokemonA, element pokemonB);

///copy pokemon
element DeepCopyPokemon(element pokemon);

///get pokemon type
char *getType(element pokemon);

///calculate the difference between 2 pokemons attack depends on their type
int getAttack(element pokemonA, element pokemonB, int *attackFirst, int *attackSecond);


///add to effective_against_me function, return status success/failure
status add_against_me(pok_type *, pok_type *);

///add to effective_against_others function, return status success/failure
status add_against_others(pok_type *sourceA, pok_type *addB);

///delete from effective_against_me function, return status success/failure
status delete_against_me(pok_type *sourceA, char *deleteB);

//delete from effective_against_me function, return status success/failure
status delete_against_others(pok_type *sourceA, char *deleteB);


#endif //POKEMONS_POKEMON_H
