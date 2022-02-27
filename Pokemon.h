
#ifndef POKEMON_H_
#define POKEMON_H_

#include "Defs.h"

struct Type {
	char* type_name;
	int amount_of;
	struct Type **effective_against_me;
	int amount_of_against_me;
	struct Type **effective_against_others;
	int amount_of_against_others;
};

struct BiologicalInformation {
	float height;
	float weight;
	int attack;
};

struct Pokemon {
	char* pokemon_name;
	char* pokemon_species;
	BiologicalInformation *pokemon_biological_information;
	Type *pokemon_type;
};

status      TypeInit					 		 	(Type*, char*);
status      BiologicalInformationInit 			    (BiologicalInformation*, float, float, int);
status      PokemonInit				 			    (Pokemon*, char*, char*, BiologicalInformation*, Type*);
status      AddTypeToEffectiveAgainstMe			    (Type*, Type*);
status      AddTypeToEffectiveAgainstOthers		    (Type*, Type*);
status      RemoveTypeFromEffectiveAgainstMe		(Type*, char*);
status      RemoveTypeFromEffectiveAgainstOthers	(Type*, char*);
status      TypePrint							    (Type*);
status      PokemonPrint						    (Pokemon*);
status 		TypeDestroy								(Type*);
status 		PokemonDestroy							(Pokemon*);

#endif /* POKEMON_H_ */
