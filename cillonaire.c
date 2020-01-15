#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Text definition
#define MSG_ARG_ERROR1 "Too many arguments, only -s and -f supported."
#define MSG_ARG_ERROR2 "Non-existent command. Only -s and -f supported."
#define MSG_QUIT "*** Sad to see you go..."
#define MSG_UNK "*** Unknown input."
#define MSG_ILLEGAL "*** Illegal move"
#define MSG_SAVE "*** Ok, your progress has been saved. See you later!"
#define MAX 64
#define ULTMAX 128

// Data struct with player information. 
typedef struct playerinfo {
    char playerName[ULTMAX];
    int level;
    int j50;
    int j25;
} PLAYERINFO;

// Data struct with question information
typedef struct _node {
    char question[ULTMAX];
    char answers[4][ULTMAX];
    enum {easy, medium, hard} type;
    struct _node * next;
} QUESTION;

typedef struct questvalue {
    QUESTION * qv;
    QUESTION * head;
} questvalue;

// Function declaration
void print_menu(void);
void credits(void);
void current_status(char *, int, int, int);
questvalue question_read(FILE *);
void new_game(PLAYERINFO *, questvalue);

// The main function of the program
int main(int argc, const char **argv){

    // First arguments
    int seed;
    int commandCheck;
    char input[30];
    int idx = 0;
    PLAYERINFO currentUser;
    FILE * fp;
    questvalue questions;

    // Verification of the first arguments
    if (argc < 1){
        srand(time(0));
    }
    if (argc > 5){
        puts(MSG_ARG_ERROR1);
        exit(0);
    }
    if (argc > 1){

        if (strcmp(argv[1],"-s") == 0){
            seed = atoi(argv[2]);
            commandCheck = 0;
            srand(seed);
        }

        else if (strcmp(argv[1],"-f") == 0){
            commandCheck = 1;
            FILE * fp = fopen(argv[2], "r");

            if (fp == NULL){
                fprintf(stderr, "Error opening the file.\n");
                return 1;
            }
		
            questions = question_read(fp); //gives segmentation fault for some godforsaken reason

			fclose(fp);
			
			if (questions.qv->next != NULL){
				fprintf(stderr,"Something wrong happened.\n");
				exit(0);
			}

        }

        else{
            puts(MSG_ARG_ERROR2);
            exit(0);
        }
	}

	if (argc > 3){

		switch (commandCheck)
		{
		case 0:
			if (strcmp(argv[3],"-f") == 0){
				FILE * fp = fopen(argv[2], "r");

				if (fp == NULL){
					fprintf(stderr, "Error opening the file.\n");
					return 1;
				}
			
				questions = question_read(fp); 

				fclose(fp);
				
				if (questions.qv->next != NULL){
					fprintf(stderr,"Something wrong happened.\n");
					exit(0);
				}
				
				break;
			}
		
		case 1:
			if (strcmp(argv[3],"-s") == 0){
				seed = atoi(argv[4]);
				srand(seed);
				break;
			}
		}
	}
    
    // Prints the starting menu
    print_menu();

    // Loop that will verify which inputs the player has inputted and decide what it will do based on that
    // n, s and r's basic functions are usable for now (you can input names multiple times, save and read successfully)
    while(1){

        // Verifies the player input and stores it on the "input"
        fgets(input, 30, stdin);

        if (input[0] == 'n'){
 
            if (input[1] != '\n'){

                for (idx; input[idx + 2] != '\n'; idx++){
                    currentUser.playerName[idx] = input[idx + 2];  
                }
                
                // In case the previous string was bigger than the new one, it ignores everything beyond (sad for the lost memory doe)
                currentUser.playerName[idx] = '\0';

                // Zeroes the index so it can be used again.
                idx = 0;
            
            }

            else{
                strcpy(currentUser.playerName, "newbie");
            }
			currentUser.j50 = 1;
			currentUser.j25 = 1;
            new_game(&currentUser, questions);
        }

        else if (input[0] == 'r'){

            // Initializes the filename that the user inputted
            char fileread[MAX];

            if (input[1] != '\0'){

                // Will fill the fileread
                for (idx; input[idx + 2] != '\n'; idx++)
                    fileread[idx] = input[idx + 2];
                }

                fileread[idx] = '\0';
                idx = 0;

                // Opens the file with the inputted name in read binary mode
                FILE * fp = fopen(fileread, "rb");

                // Checks if there is any error while opening the file
                if (fp == NULL){
                    fprintf(stderr, "Error opening the file.\n");
                    return 1;
                }

                // Reads the file and then updates the PLAYERDATA variable
                fread(currentUser.playerName, sizeof(currentUser.playerName), 16, fp);
                fread(&currentUser.level, sizeof(currentUser.level), 1, fp);
                fread(&currentUser.j50, sizeof(currentUser.j50), 1, fp);
                fread(&currentUser.j25, sizeof(currentUser.j25), 1, fp);
                fclose(fp);

                printf("*** Ok %s, where were we? Oh there you go:\n", currentUser.playerName);
                new_game(&currentUser, questions);
            }
        

        else if (input[0] == 's'){

            // Initializes the filename that the user inputted
            char filename[MAX];

            if (input[1] != '\0'){

                // Will fill the filename
                for (idx; input[idx + 2] != '\n'; idx++){
                    filename[idx] = input[idx + 2];
                }

                filename[idx] = '\0';
                idx = 0;


                // Opens/Creates a newfile with the inputted name in write binary mode
                FILE * fp = fopen(filename, "wb");
                printf("%s\n", filename);

                // Checks if there is any error while opening the file
                if (fp == NULL){
                    fprintf(stderr, "Error opening the file.\n");
                    return 1;
                }

                // Writes the data in PLAYERDATA on that file, then closes it
                fwrite(currentUser.playerName, sizeof(currentUser.playerName), 16, fp);
                fwrite(&currentUser.level, sizeof(currentUser.level), 1, fp);
                fwrite(&currentUser.j50, sizeof(currentUser.j50), 1, fp);
                fwrite(&currentUser.j25, sizeof(currentUser.j25), 1, fp);
                fclose(fp);

                // Shows the "saved game" message and then closes the program.
                puts(MSG_SAVE);
                exit(0);
            }
        }

        else if (input[0] == 'h' && input[1] == '\n'){
            print_menu();
            continue;
        }

        else if (input[0] == 'c' && input[1] == '\n'){
            credits();
            continue;
        }

        else if (input[0] == 'q' && input[1] == '\n'){
            puts(MSG_QUIT);
            return 0;

        }

        else{
            puts(MSG_UNK);}
    }

    return 0;

    }

// The main function of the game.
void new_game(PLAYERINFO * currentUser, questvalue question_read){

    // Prints the player's status and greets him.
	  printf("*** Hi %s, let's get started!\n", currentUser->playerName);
    current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);

    // Main variables
    char optionVect[4][2] = {"A", "B", "C", "D"};
    QUESTION * aux;
    int r = rand() % 4;
    char playerInput;
    int levels[] = {0, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000};
    int lvliter = 0;
    int erriter = 0;

    for(aux = question_read.head; aux != NULL ; aux = aux ->next){
        // Verifies the player level and checks if the question difficulty type is the same
        if (currentUser->level == 0 || currentUser->level == 500 || currentUser->level == 1000)
            if (aux->type == 0)
                ;
            else
                continue;                
        else if (currentUser->level == 2000 || currentUser->level == 5000)
            if (aux->type == 2)
                ;
            else
                continue;
        else
            if (aux->type == 3)
                ;
            else
                continue;

        // Orders the question based on the random (0-4) variable 'r'
        if (r == 0){
            printf("*** Question: %s", aux->question);
            printf("*** %s: %s", optionVect[0], aux->answers[0]);
            printf("*** %s: %s", optionVect[1], aux->answers[1]);
            printf("*** %s: %s", optionVect[2], aux->answers[2]);
            printf("*** %s: %s", optionVect[3], aux->answers[3]);
        }

        if (r == 1){
            printf("*** Question: %s", aux->question);
            printf("*** %s: %s", optionVect[0], aux->answers[1]);
            printf("*** %s: %s", optionVect[1], aux->answers[0]);
            printf("*** %s: %s", optionVect[2], aux->answers[2]);
            printf("*** %s: %s", optionVect[3], aux->answers[3]);
        }

        if (r == 2){
            printf("*** Question: %s", aux->question);
            printf("*** %s: %s", optionVect[0], aux->answers[1]);
            printf("*** %s: %s", optionVect[1], aux->answers[2]);
            printf("*** %s: %s", optionVect[2], aux->answers[0]);
            printf("*** %s: %s", optionVect[3], aux->answers[3]);
        }

        if (r == 3){
            printf("*** Question: %s", aux->question);
            printf("*** %s: %s", optionVect[0], aux->answers[1]);
            printf("*** %s: %s", optionVect[1], aux->answers[2]);
            printf("*** %s: %s", optionVect[2], aux->answers[3]);
            printf("*** %s: %s", optionVect[3], aux->answers[0]);
        }

        // Stops the flow of questions for the time being

        fgets(playerInput, 30, stdin);

        switch (r){
        case 0:
            if (strcmp(toupper(playerInput),optionVect[0]) == 0){
                puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];

                if (erriter == 1)
                    erriter = 0;
                
                if (currentUser->level == levels[8]){
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }
                    
                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %s: %s\n", optionVect[0], aux->answers[0]);
                erriter += 1;
                lvliter -= 1;
                currentUser->level = levels[lvliter];

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
                
            }
            break;
        
        case 1:
            if (strcmp(toupper(playerInput),optionVect[1]) == 0){
              puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];

                if (erriter == 1)
                    erriter = 0;
                
                if (currentUser->level == levels[8]){
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %s: %s\n", optionVect[1], aux->answers[0]);
                erriter += 1;
                lvliter -= 1;
                currentUser->level = levels[lvliter];

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25); 
            }
            break;

        case 2:
            if (strcmp(toupper(playerInput),optionVect[2]) == 0){
            puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];

                if (erriter == 1)
                    erriter = 0;
                
                if (currentUser->level == levels[8]){
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }
                
                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %s: %s\n", optionVect[2], aux->answers[0]);
                erriter += 1;
                lvliter -= 1;
                currentUser->level = levels[lvliter];

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            break;
        
        case 3:
            if (strcmp(toupper(playerInput),optionVect[3]) == 0){
              puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];

                if (erriter == 1)
                    erriter = 0;

                if (currentUser->level == levels[8]){
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }
                    
                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %s: %s\n", optionVect[0], aux->answers[0]);
                erriter += 1;
                lvliter -= 1;
                currentUser->level = levels[lvliter];

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            break;
        }

    }                                                                                                                                                                                                                                                             
}

// Prints the main menu.
void print_menu(){
    puts("********************************************");
    puts("***             CILLIONAIRE                *");
    puts("********************************************");
    puts("*** n <name>     - new game                *");
    puts("*** q            - quit                    *");
    puts("*** h            - show this menu          *");
    puts("*** r <filename> - resume game             *");
    puts("*** s <filename> - save progress and quit  *");
    puts("*** j 50         - play 50:50 joker        *");
    puts("*** j 25         - play 25:75 joker        *");
    puts("*** c            - show credits            *");
    puts("********************************************");
}

// Prints the credits of the game
void credits(){
    puts("********************************************");
    puts("*** Created by Marco Domingos              *");
    puts("*** 2020                                   *");
    puts("*** Play Muv-Luv Alternative               *");
    puts("********************************************");
}

questvalue question_read(FILE * fp){

	char * question_vect;
    question_vect = (char *) malloc(10*sizeof(char));
    QUESTION * head;
    QUESTION * tail;
	  head = (QUESTION *) malloc(sizeof(QUESTION));
    tail = (QUESTION *) malloc(sizeof(QUESTION));
    head = NULL;
    tail = NULL;
    QUESTION * new;
    questvalue questionstruct;

    while (fgets(question_vect, ULTMAX, fp) != NULL){
        if (question_vect[0] == ';'){
            continue;
        }
        if (question_vect[0] == 'Q'){
            
            // Inputs the memory address of the QUESTION node in the data struct of the function
            new = (QUESTION *) malloc(sizeof(QUESTION));
            new->next = NULL;

            // Reads lines and copies each of them to their respective places
            strcpy(new->question, question_vect + 9); 
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(new->answers[0], question_vect + 8); 
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(new->answers[1], question_vect + 8);
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(new->answers[2], question_vect + 8);
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(new->answers[3], question_vect + 8);
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp); 
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            
			if (strcmp(question_vect, "DIFFICULTY=easy\n")== 0)
				new->type = easy;
			else if (strcmp(question_vect, "DIFFICULTY=medium\n") == 0)
				new->type = medium;
			else
				new->type = hard;
			strcpy(question_vect, "");

            if (head == NULL){
                head = new;
                tail = new;
            }
            
            else{
              tail->next = new;
              new->next = NULL;
              tail = new;
              //new->next = head;
              //head = new;
            }
        }

    }
    
    free(tail);
    free(question_vect);
    questionstruct.head = head;
    questionstruct.qv = new;
	return questionstruct;

}

// Function that observes and prints the status of the player
void current_status(char * name, int level, int joker50, int joker25){
    char jokerFiftyMessage[4], jokerTwentyMessage[4];

    if (joker50 == 0){
        strcpy(jokerFiftyMessage, "NO");}
    else{
        strcpy(jokerFiftyMessage, "YES");}

    if (joker25 == 0){
        strcpy(jokerTwentyMessage, "NO");}
    else{
        strcpy(jokerTwentyMessage, "YES");}

    puts("********************************************");
    printf("*** Name: %-20s             *\n", name);
    printf("*** Level: %02d                              *\n", level);
    printf("*** j50: %-10s                        *\n", jokerFiftyMessage);
    printf("*** j25: %-10s                        *\n", jokerTwentyMessage);
    puts("********************************************");
}