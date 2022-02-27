
#include "Pokemon.h"


static status EffectiveAgainstArrayIncreaseByOne(Type*** effective_against_array,
												 int current_number_of_pointers) {
	//no need to check for null pointers since "effective-against-array" can be empty
	*effective_against_array = (Type **) realloc(*effective_against_array,
												 (current_number_of_pointers + 1) * sizeof(Type *));
	if (*effective_against_array == NULL) {
		return memory_failure;
	}
	return success;
}


static status EffectiveAgainstArrayDecreaseByOne(Type*** effective_against_array,
												 int current_number_of_pointers) {
	if (effective_against_array == NULL) {
		return null_pointer_failure;
	}
	*effective_against_array = (Type **) realloc(*effective_against_array,
												 (current_number_of_pointers - 1) * sizeof(Type *));
	if (current_number_of_pointers > 1 && *effective_against_array == NULL) {
		return memory_failure;
	}
	return success;
}


static Type** TypePointerSearchByPointer(Type** effective_against_array,
										 int current_number_of_pointers,
										 Type* type_pointer) {
	if (type_pointer == NULL) { //null pointer
		return NULL;
	}
	int i;
	for (i = 0; i < current_number_of_pointers; i++){
		if (effective_against_array[i] == type_pointer)
			return (effective_against_array + i);
	}
	return NULL;
}


static Type** TypePointerSearchByName(Type** effective_against_array,
									  int current_number_of_pointers,
									  char* type_pointer_name) {
	if (type_pointer_name == NULL) { //null pointer
		return NULL;
	}
	int i;
	for (i = 0; i < current_number_of_pointers; i++){
		if (strcmp(effective_against_array[i]->type_name, type_pointer_name) == 0)
			return (effective_against_array + i);
	}
	return NULL;
}


static int GetTypePointerIndex(Type** effective_against_array, Type* type) {
	if (type == NULL) { //null pointer
		return -1;
	}
	int i = 0;
	while (effective_against_array[i] != type) {
		i++;
	}
	return i;
}


static status EffectiveAgainstMePrint(Type* type) {
	if (type == NULL) {
		return null_pointer_failure;
	}
	if (type->amount_of_against_me < 0) {
		return print_failure;
	}
	int i;
	for (i = 0; i < type->amount_of_against_me; i++) {
		if (type->effective_against_me[i]->type_name == NULL) {
			return print_failure;
		}
		else {
			if (i == (type->amount_of_against_me - 1)) {
				printf("%s", type->effective_against_me[i]->type_name);
			}
			else {
				printf("%s ,", type->effective_against_me[i]->type_name);
			}
		}
	}
	return success;
}


static status EffectiveAgainstOthersPrint(Type* type) {
	if (type == NULL) {
		return null_pointer_failure;
	}
	if (type->amount_of_against_me < 0) {
		return print_failure;
	}
	int i;
	for (i = 0; i < type->amount_of_against_others; i++) {
		if (type->effective_against_others[i]->type_name == NULL) {
			return print_failure;
		}
		else {
			if (i == (type->amount_of_against_others - 1)) {
				printf("%s", type->effective_against_others[i]->type_name);
			}
			else {
				printf("%s ,", type->effective_against_others[i]->type_name);
			}
		}
	}
	return success;
}


static status BiologicalInformationPrint(BiologicalInformation* b_i) {
	if (b_i == NULL) {
		return null_pointer_failure;
	}
	if (b_i->height < 0  || b_i->weight < 0 || b_i->attack <0) {
		return print_failure;
	}
	printf("Height: %.2f m    Weight: %.2f kg    Attack: %d", b_i->height, b_i->weight, b_i->attack);
	return success;
}


status TypeInit(Type* type_pointer, char* name) {
	if (type_pointer == NULL) {
		return null_pointer_failure;
	}
	type_pointer->type_name = (char *) malloc((strlen(name) + 1) * sizeof(char));
	if (type_pointer->type_name == NULL) {
		return memory_failure;
	}
	strcpy(type_pointer->type_name, name);
	type_pointer->amount_of = 0;
	type_pointer->effective_against_me = (Type**) NULL;
	type_pointer->amount_of_against_me = 0;
	type_pointer->effective_against_others = (Type**) NULL;
	type_pointer->amount_of_against_others = 0;
	return success;
}


status BiologicalInformationInit(BiologicalInformation* biological_information_pointer,
							   float height,
							   float weight,
							   int attack) {
	if (biological_information_pointer == NULL) {
		return null_pointer_failure;
	}
	biological_information_pointer->height = height;
	biological_information_pointer->weight = weight;
	biological_information_pointer->attack = attack;
	return success;
}


status PokemonInit(Pokemon* pokemon_pointer,
				 char* name,
				 char* species,
				 BiologicalInformation* biological_information,
				 Type* type) {
	if (pokemon_pointer == NULL        ||
		name == NULL                   ||
		species == NULL                ||
		biological_information == NULL ||
		type == NULL) {
		return null_pointer_failure;
	}
	pokemon_pointer->pokemon_name = (char *) malloc((strlen(name) + 1) * sizeof(char));
	if (pokemon_pointer->pokemon_name == NULL) {
		return memory_failure;
	}
	strcpy(pokemon_pointer->pokemon_name, name);
	pokemon_pointer->pokemon_species = (char *) malloc((strlen(species) + 1) * sizeof(char));
	if (pokemon_pointer->pokemon_species == NULL)  {
		return memory_failure;
	}
	strcpy(pokemon_pointer->pokemon_species, species);
	pokemon_pointer->pokemon_type = type;
	pokemon_pointer->pokemon_type->amount_of++;
	pokemon_pointer->pokemon_biological_information = biological_information;
	return success;
}



status AddTypeToEffectiveAgainstMe(Type* a, Type* b) {
	if (a == NULL || b == NULL) {
		return null_pointer_failure;
	}
	if (TypePointerSearchByPointer(a->effective_against_me, a->amount_of_against_me, b) != NULL) {
		return add_failure;
	}
	else {
		if (EffectiveAgainstArrayIncreaseByOne(&(a->effective_against_me), a->amount_of_against_me) == memory_failure) {
			return memory_failure;
		}
		a->effective_against_me[a->amount_of_against_me] = b;
		a->amount_of_against_me++;
	}
	return success;
}


status AddTypeToEffectiveAgainstOthers(Type* a, Type* b) {
	if (a == NULL || b == NULL) {
		return null_pointer_failure;
	}
	if (TypePointerSearchByPointer(a->effective_against_others, a->amount_of_against_others, b) != NULL) {
		return add_failure;
	}
	else {
		if (EffectiveAgainstArrayIncreaseByOne(&(a->effective_against_others), a->amount_of_against_others) == memory_failure) {
			return memory_failure;
		}
		a->effective_against_others[a->amount_of_against_others] = b;
		a->amount_of_against_others++;
	}
	return success;
}


status RemoveTypeFromEffectiveAgainstMe(Type* a, char* b_name) {
	if (a == NULL || b_name == NULL) {
		return null_pointer_failure;
	}
	Type** need_to_be_removed = TypePointerSearchByName(a->effective_against_me, a->amount_of_against_me, b_name);
	if (need_to_be_removed == NULL) {
		return remove_failure;
	}
	else {
		int i, need_to_be_removed_index = GetTypePointerIndex(a->effective_against_me, *need_to_be_removed);
		if (need_to_be_removed_index == -1) {
			return null_pointer_failure;
		}
		for (i = need_to_be_removed_index; i < (a->amount_of_against_me - 1); i++) {
			a->effective_against_me[i] = a->effective_against_me[i + 1];
		}
		status condition = EffectiveAgainstArrayDecreaseByOne(&(a->effective_against_me), a->amount_of_against_me);
		if (condition == memory_failure) {
			return memory_failure;
		}
		else if (condition == null_pointer_failure) {
			return null_pointer_failure;
		}
		else {
			a->amount_of_against_me--;
		}
	}
	return success;
}


status RemoveTypeFromEffectiveAgainstOthers(Type* a, char* b_name) {
	if (a == NULL || b_name == NULL) {
		return null_pointer_failure;
	}
	Type** need_to_be_removed = TypePointerSearchByName(a->effective_against_others, a->amount_of_against_others, b_name);
	if (need_to_be_removed == NULL) {
		return remove_failure;
	}
	else {
		int i, need_to_be_removed_index = GetTypePointerIndex(a->effective_against_others, *need_to_be_removed);
		for (i = need_to_be_removed_index; i < (a->amount_of_against_others - 1); i++) {
			a->effective_against_others[i] = a->effective_against_others[i + 1];
		}
		status condition = EffectiveAgainstArrayDecreaseByOne(&(a->effective_against_others), a->amount_of_against_others);
		if (condition == memory_failure) {
			return memory_failure;
		}
		else if (condition == null_pointer_failure) {
			return null_pointer_failure;
		}
		else {
			a->amount_of_against_others--;
		}
	}
	return success;
}


status TypePrint(Type* type) {
	if (type == NULL) {
		return null_pointer_failure;
	}
	if (type->type_name == NULL) {
		return print_failure;
	}
	status condition;
	printf("Type %s -- %d pokemons", type->type_name, type->amount_of);
	if (type->amount_of_against_me > 0) {
		printf("\n\tThese types are super-effective against %s:", type->type_name);
		condition = EffectiveAgainstMePrint(type);
		if (condition == null_pointer_failure) {
			return null_pointer_failure;
		}
		else if (condition == print_failure) {
			return print_failure;
		}
	}
	if (type->amount_of_against_others > 0) {
		printf("\n\t%s moves are super-effective against:", type->type_name);
		condition = EffectiveAgainstOthersPrint(type);
		if (condition == null_pointer_failure) {
			return null_pointer_failure;
		}
		else if (condition == print_failure) {
			return print_failure;
		}
	}
	printf("\n\n");
	return success;
}


status PokemonPrint(Pokemon* pokemon) {
	if (pokemon == NULL) {
		return null_pointer_failure;
	}
	if (pokemon->pokemon_name == NULL || pokemon->pokemon_species == NULL || pokemon->pokemon_type->type_name == NULL) {
		return print_failure;
	}
	printf("%s :\n%s, %s Type.\n", pokemon->pokemon_name, pokemon->pokemon_species, pokemon->pokemon_type->type_name);
	status condition = (BiologicalInformationPrint(pokemon->pokemon_biological_information));
	if (condition == null_pointer_failure) {
		return null_pointer_failure;
	}
	else if (condition == print_failure) {
		return print_failure;
	}
	printf("\n\n");
	return success;
}


status TypeDestroy(Type* t) {
	if (t == NULL) {
		return null_pointer_failure;
	}
	if (t->type_name != NULL) {
		free(t->type_name);
		t->type_name = NULL;
	}
	if (t->effective_against_me != NULL) {
		free(t->effective_against_me);
		t->effective_against_me = NULL;
	}
	if (t->effective_against_others != NULL) {
		free(t->effective_against_others);
		t->effective_against_others = NULL;
	}
	return success;
}


status PokemonDestroy(Pokemon* p) {
	if (p == NULL) {
		return null_pointer_failure;
	}
	if (p->pokemon_name != NULL) {
		free(p->pokemon_name);
		p->pokemon_name = NULL;
	}
	if (p->pokemon_species != NULL) {
		free(p->pokemon_species);
		p->pokemon_species = NULL;
	}
	return success;
}
