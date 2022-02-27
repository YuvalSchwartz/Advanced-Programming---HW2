
#include "Pokemon.h"


status TypeArrayDestroy(Type* t_array_to_destroy, int n_of_types) {
	if (t_array_to_destroy == NULL) {
		return null_pointer_failure;
	}
	int i;
	for (i = 0; i < n_of_types; i++) {
		if (TypeDestroy(t_array_to_destroy + i) == null_pointer_failure) {
			return null_pointer_failure;
		}
	}
	return success;
}

status PokemonArrayDestroy(Pokemon* p_array_to_destroy, int n_of_pokemons) {
	if (p_array_to_destroy == NULL) {
		return null_pointer_failure;
	}
	int i;
	for (i = 0; i < n_of_pokemons; i++) {
		if (PokemonDestroy(p_array_to_destroy + i) == null_pointer_failure) {
			return null_pointer_failure;
		}
	}
	return success;
}

status Destroy(Type* type_array_to_destroy,
			 Pokemon* pokemon_array_to_destroy,
			 int number_of_types,
			 int number_of_pokemons) {
	if (type_array_to_destroy == NULL || pokemon_array_to_destroy == NULL) {
		return null_pointer_failure;
	}
	if (TypeArrayDestroy(type_array_to_destroy, number_of_types) == null_pointer_failure
		||
		PokemonArrayDestroy(pokemon_array_to_destroy, number_of_pokemons) == null_pointer_failure) {
		return null_pointer_failure;
	}
	return success;
}

Type* ArrayTypeSearchByName(Type* types_array_pointer, int num_of_types, char* type_name) {
	if (types_array_pointer == NULL || type_name == NULL) {
		return NULL;
	}
	int i;
	for (i = 0; i < num_of_types; i++){
		if (strcmp((types_array_pointer + i)->type_name, type_name) == 0) {
			return ((types_array_pointer + i));
		}
	}
	return NULL;
}

Pokemon* ArrayPokemonSearchByName(Pokemon* pokemons_array_pointer, int num_of_pokemons, char* pokemons_name) {
	if (pokemons_array_pointer == NULL || pokemons_name == NULL) {
		return NULL;
	}
	int i;
	for (i = 0; i < num_of_pokemons; i++){
		if (strcmp((pokemons_array_pointer + i)->pokemon_name, pokemons_name) == 0) {
			return ((pokemons_array_pointer + i));
		}
	}
	return NULL;
}


int main(int argc, char* argv[])
{
	int types_size = strtol(argv[1], NULL, 10), pokemons_size = strtol(argv[2], NULL, 10);
	Type types_array[types_size];
	Pokemon pokemons_array[pokemons_size];
	BiologicalInformation pokemons_biological_information_array[pokemons_size];

	status condition;
	FILE *file_pointer;
	char buffer[MAX_INPUT_SIZE];
	char types_title[] = "Types"; //string that first line should be if there are any types
	char pokemons_title[] = "Pokemons"; //string that indicates end of types messing and start of pokemons messing
	char effective_against_me_title[] = "effective-against-me";
	char *token;
	const char comma[2] = ",";
	const char colon[2] = ":";
	int i, j;

	//temp-pokemon attributes declaration for pokemon's initialization:
	int pokemon_line_indexes[6]; //indexes of string details in pokemons line reading (name,species,height,weight,attack,type)
	pokemon_line_indexes[0] = 0; //index of name is always at the beggining of pokemons line


	file_pointer = fopen(argv[3], "r"); //opens the file

	fscanf(file_pointer, "%s", buffer); //reads the first word of the file
	if (strcmp(types_title, buffer) == 0) { //checks if word is "Types"
		fscanf(file_pointer, "%s", buffer); //reads 'name of types' line
		token = strtok(buffer, comma);
		for (i = 0; i < types_size; i++) {
			condition = TypeInit(types_array + i, token); //initialize each type
			if (condition == memory_failure) {
				printf("Memory Problem\n");
				Destroy(types_array, pokemons_array, types_size, pokemons_size);
				fclose(file_pointer);
				return 1;
			}
			else if (condition == null_pointer_failure) {
				printf("Null Pointer Problem\n");
				Destroy(types_array, pokemons_array, types_size, pokemons_size);
				fclose(file_pointer);
				return 1;
			}
			token = strtok(NULL, comma);
		}
		fscanf(file_pointer, "%s", buffer); //reads the next word
		while (strcmp(pokemons_title, buffer) != 0) { //checks if word is not "Pokemons"
			Type* enter_to = ArrayTypeSearchByName(types_array, types_size, buffer);
			which_to_enter against;
			//reads the rest of the line ("me" or "others" and names of types):
			fscanf(file_pointer, "%s", buffer);
			token = strtok(buffer, colon); //equals "effective-against-me" or "effective-against-others"
			if (strcmp(effective_against_me_title, token) == 0) {
				against = me;
			}
			else {
				against = others;
			}
			token = strtok(NULL, comma); //continues to names of types
			while (token != NULL) {
				Type* entered = ArrayTypeSearchByName(types_array, types_size, token);
				if (against == me) {
					condition = AddTypeToEffectiveAgainstMe(enter_to, entered);
				}
				else {
					condition = AddTypeToEffectiveAgainstOthers(enter_to, entered);
				}
				if (condition == add_failure) {
					printf("This type already exist in the list.");
				}
				else if (condition == memory_failure) {
					printf("Memory Problem\n");
					Destroy(types_array, pokemons_array, types_size, pokemons_size);
					fclose(file_pointer);
					return 1;
				}
				else if (condition == null_pointer_failure) {
					printf("Null Pointer Problem\n");
					Destroy(types_array, pokemons_array, types_size, pokemons_size);
					fclose(file_pointer);
					return 1;
				}
				token = strtok(NULL, comma);
			}
			fscanf(file_pointer, "%s", buffer); //reads another line of "effective-against" or "Pokemons"
		}
	}
	if (strcmp(pokemons_title, buffer) == 0) { //checks if word is "Pokemons"
		for (i = 0; i < pokemons_size; i++) {
			fscanf(file_pointer, "%s", buffer); //reads line of a pokemon
			token = strtok(buffer, comma);
			for (j = 1; j < 6; j++) {
				pokemon_line_indexes[j] = pokemon_line_indexes[j - 1] + strlen(token) + 1;
				token = strtok(NULL, comma);
			}
			condition = BiologicalInformationInit(&(pokemons_biological_information_array[i]),
												  strtof(buffer + pokemon_line_indexes[2], NULL),
												  strtof(buffer + pokemon_line_indexes[3], NULL),
												  strtol(buffer + pokemon_line_indexes[4], NULL, 10));
			if (condition == null_pointer_failure) {
				printf("Null Pointer Problem\n");
				Destroy(types_array, pokemons_array, types_size, pokemons_size);
				fclose(file_pointer);
				return 1;
			}
			condition = PokemonInit(&(pokemons_array[i]),
									buffer,
									buffer + pokemon_line_indexes[1],
									&(pokemons_biological_information_array[i]),
									ArrayTypeSearchByName(types_array, types_size, buffer + pokemon_line_indexes[5]));
			if (condition == null_pointer_failure) {
				printf("Null Pointer Problem\n");
				Destroy(types_array, pokemons_array, types_size, pokemons_size);
				fclose(file_pointer);
				return 1;
			}
		}
	}

	fclose(file_pointer);


	Type *users_type_to_enlrage, *users_type;
	Pokemon *users_pokemon;
	char users_choice[2] = "";
    while(strcmp(users_choice, "9") != 0) {
	    printf("Please choose one of the following numbers:\n");
	    printf("1 : Print all Pokemons\n");
	    printf("2 : Print all Pokemons types\n");
	    printf("3 : Add type to effective against me list\n");
	    printf("4 : Add type to effective against others list\n");
	    printf("5 : Remove type from effective against me list\n");
	    printf("6 : Remove type from effective against others list\n");
	    printf("7 : Print Pokemon by name\n");
	    printf("8 : Print Pokemons by type\n");
	    printf("9 : Exit\n");
	    scanf("%2s", users_choice);

        if (strcmp(users_choice, "1") == 0) {
    		for (i = 0; i < pokemons_size; i++) {
    			condition = PokemonPrint(&(pokemons_array[i]));
    			if (condition == print_failure) {
    				printf("Print Failure\n");
    			}
    			else if (condition == null_pointer_failure) {
    				printf("Null Pointer Problem\n");
    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
    				return 1;
    			}
    		}
    		continue;
        }

        else if (strcmp(users_choice, "2") == 0) {
    		for (i = 0; i < types_size; i++) {
    			condition = TypePrint(&(types_array[i]));
    			if (condition == print_failure) {
    				printf("Print Failure\n");
    		    }
    			else if (condition == null_pointer_failure) {
    				printf("Null Pointer Problem\n");
    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
    				return 1;
    			}
    		}
    		continue;
        }

        else if (strcmp(users_choice, "3") == 0) {
    		printf("Please enter type name:\n");
    		scanf("%s", buffer);
    		users_type_to_enlrage = ArrayTypeSearchByName(types_array, types_size, buffer);
    		if (users_type_to_enlrage != NULL) {
    			char user_type_to_add_input[MAX_INPUT_SIZE];
    			printf("Please enter type name to add to %s effective against me list:\n", buffer);
    			scanf("%s", user_type_to_add_input);
				Type* type_to_add = ArrayTypeSearchByName(types_array, types_size, user_type_to_add_input);
				if (type_to_add != NULL) {
					condition = AddTypeToEffectiveAgainstMe(users_type_to_enlrage, type_to_add);
					if (condition == add_failure) {
						printf("This type already exist in the list.\n");
					}
					else if (condition == memory_failure) {
						printf("Memory Problem\n");
						Destroy(types_array, pokemons_array, types_size, pokemons_size);
						return 1;
					}
	    			else if (condition == null_pointer_failure) {
	    				printf("Null Pointer Problem\n");
	    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
	    				return 1;
	    			}
					else {
						if (TypePrint(users_type_to_enlrage) == print_failure) {
							printf("Print Failure\n");
						}
					}
				}
				else {
					printf("Type name doesn't exist.\n");
				}
    		}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
    		continue;
        }

        else if (strcmp(users_choice, "4") == 0) {
    		printf("Please enter type name:\n");
    		scanf("%s", buffer);
    		users_type_to_enlrage = ArrayTypeSearchByName(types_array, types_size, buffer);
    		if (users_type_to_enlrage != NULL) {
    			char user_type_to_add_input[MAX_INPUT_SIZE];
    			printf("Please enter type name to add to %s effective against others list:\n", buffer);
    			scanf("%s", user_type_to_add_input);
				Type* type_to_add = ArrayTypeSearchByName(types_array, types_size, user_type_to_add_input);
				if (type_to_add != NULL) {
					condition = AddTypeToEffectiveAgainstOthers(users_type_to_enlrage, type_to_add);
					if (condition == add_failure) {
						printf("This type already exist in the list.\n");
					}
					else if (condition == memory_failure) {
						printf("Memory Problem\n");
						Destroy(types_array, pokemons_array, types_size, pokemons_size);
						return 1;
					}
	    			else if (condition == null_pointer_failure) {
	    				printf("Null Pointer Problem\n");
	    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
	    				return 1;
	    			}
					else {
						if (TypePrint(users_type_to_enlrage) == print_failure) {
							printf("Print Failure\n");
						}
					}
				}
				else {
					printf("Type name doesn't exist.\n");
				}
    		}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
    	    continue;
        }

        else if (strcmp(users_choice, "5") == 0) {
    		printf("Please enter type name:\n");
    		scanf("%s", buffer);
    		users_type = ArrayTypeSearchByName(types_array, types_size, buffer);
    		if (users_type != NULL) {
    			char user_type_to_remove_input[MAX_INPUT_SIZE];
    			printf("Please enter type name to remove from %s effective against me list:\n", buffer);
    			scanf("%s", user_type_to_remove_input);
    			condition = RemoveTypeFromEffectiveAgainstMe(users_type, user_type_to_remove_input);
    			if (condition == remove_failure) {
    				printf("Type name doesn't exist in the list.\n");
    			}
    			else if (condition == memory_failure) {
    				printf("Memory Problem\n");
    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
    				return 1;
    			}
    			else if (condition == null_pointer_failure) {
    				printf("Null Pointer Problem\n");
    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
    				return 1;
    			}
    			else {
    				if (TypePrint(users_type) == print_failure) {
    				printf("Print Failure\n");
    				}
    			}
    		}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
    		continue;
        }

        else if (strcmp(users_choice, "6") == 0) {
    		printf("Please enter type name:\n");
    		scanf("%s", buffer);
    		users_type = ArrayTypeSearchByName(types_array, types_size, buffer);
    		if (users_type != NULL) {
    			char user_type_to_remove_input[MAX_INPUT_SIZE];
    			printf("Please enter type name to remove from %s effective against others list:\n", buffer);
    			scanf("%s", user_type_to_remove_input);
    			condition = RemoveTypeFromEffectiveAgainstOthers(users_type, user_type_to_remove_input);
    			if (condition == remove_failure) {
    				printf("Type name doesn't exist in the list.\n");
    			}
    			else if (condition == memory_failure) {
    				printf("Memory Problem\n");
    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
    				return 1;
    			}
    			else if (condition == null_pointer_failure) {
    				printf("Null Pointer Problem\n");
    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
    				return 1;
    			}
    			else {
    				if (TypePrint(users_type) == print_failure) {
    				printf("Print Failure\n");
    				}
    			}
    		}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
    		continue;
        }

        else if (strcmp(users_choice, "7") == 0) {
        	printf("Please enter Pokemon name:\n");
        	scanf("%s", buffer);
        	users_pokemon = ArrayPokemonSearchByName(pokemons_array, pokemons_size, buffer);
    		if (users_pokemon != NULL) {
    			condition = PokemonPrint(users_pokemon);
    			if (condition == print_failure) {
    			printf("Print Failure\n");
    			}
    			else if (condition == null_pointer_failure) {
    				printf("Null Pointer Problem\n");
    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
    				return 1;
    			}
    		}
    		else {
    			printf("The Pokemon doesn't exist.\n");
    		}
    		continue;
        }

        else if (strcmp(users_choice, "8") == 0) {
        	printf("Please enter type name:\n");
        	scanf("%s", buffer);
        	users_type = ArrayTypeSearchByName(types_array, types_size, buffer);
    		if (users_type != NULL) {
    			if (users_type->amount_of > 0) {
        			printf("There are %d Pokemons with this type:\n", users_type->amount_of);
        			for (i = 0; i < pokemons_size; i++) {
        				if (strcmp(pokemons_array[i].pokemon_type->type_name, buffer) == 0) {
        					condition = PokemonPrint(pokemons_array + i);
        					if (condition == print_failure) {
        					    printf("Print Failure\n");
        					}
        	    			else if (condition == null_pointer_failure) {
        	    				printf("Null Pointer Problem\n");
        	    				Destroy(types_array, pokemons_array, types_size, pokemons_size);
        	    				return 1;
        	    			}
        				}
        			}
    			}
    			else {
    				printf("There are no Pokemons with this type.\n");
    			}
    		}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
    		continue;
        }

        else if (strcmp(users_choice, "9") == 0) {
    		Destroy(types_array, pokemons_array, types_size, pokemons_size);
    		printf("All the memory cleaned and the program is safely closed.\n");
    		break;
        }

        else {
        	printf("Please choose a valid number.\n");
        	continue;
        }

    }
    return 0;
}

