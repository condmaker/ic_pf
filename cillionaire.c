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

// Data struct that contains the question values and the HEAD node
typedef struct questvalue {
    QUESTION * qv;
    QUESTION * head;
} questvalue;

// Function declaration
void print_menu(void);
void credits(void);
void current_status(char *, int, int, int);
questvalue question_read(FILE *);
int new_game(PLAYERINFO *, questvalue);
int toupper(int c);

// The main function of the program
int main(int argc, const char **argv){

    // First arguments
    int seed;
    int commandCheck;
    char input[30];
    int idx = 0;
    PLAYERINFO currentUser;
    questvalue questions;

    // Verification of the opening arguments
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
		
            questions = question_read(fp); 

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
			if (strcmp(argv[3], "-f") == 0){
        
				FILE * fp = fopen(argv[4], "r");

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
			}
      break;
		
		case 1:
			if (strcmp(argv[3],"-s") == 0){
				seed = atoi(argv[4]);
				srand(seed);
			}
      break;
		}
	}
    
    // Prints the starting menu
    print_menu();

    // Loop that will verify which inputs the player has inputted and decide what it will do based on that
    // While 's' and 'r' are working, they do not save the linked list with the questions.
    while(1){

        // Verifies the player input and stores it on the "input"
        printf(">");
        fgets(input, 30, stdin);

        if (input[0] == 'n'){
 
            if (input[1] != '\n'){

                for (idx = 0; input[idx + 2] != '\n'; idx++){
                    currentUser.playerName[idx] = input[idx + 2];  
                }
                
                // In case the previous string was bigger than the new one, it ignores everything beyond (sad for the lost memory doe)
                currentUser.playerName[idx] = '\0';

                // Zeroes the index so it can be used again.
                idx = 0;
            
            }

            else{
                strcpy(currentUser.playerName, "Newbie");
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
                for (idx = 0; input[idx + 2] != '\n'; idx++)
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
                for (idx = 0; input[idx + 2] != '\n'; idx++){
                    filename[idx] = input[idx + 2];
                }

                filename[idx] = '\0';
                idx = 0;


                // Opens/Creates a newfile with the inputted name in write binary mode
                FILE * fp = fopen(filename, "wb");

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
// Need to remove item from list when read, but that requires knowledge of the prior struct of question_read.
// Also need to implement the remaining questions into the save file
int new_game(PLAYERINFO * currentUser, questvalue question_read){

    // Prints the player's status and greets him.
	printf("*** Hi %s, let's get started!\n", currentUser->playerName);
    current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);

    // Main variables
    char optionVect[8] = "ABCD";
    QUESTION * aux;
    char playerInput[30];
    int levels[] = {0, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000};
    int lvliter = 0;
    int erriter = 0;
    int r = rand() % 4;
    int idx = 0;
    int jkrturn;

    for(aux = question_read.head; aux != NULL ; aux = aux ->next){

        // Verifies if user passed a difficulty level and returns the list to the beginning
        if (levels[lvliter - 1] == 1000 || levels[lvliter - 1] == 5000)
            aux = question_read.head;

        // Verifies the player level and checks if the question difficulty type is the same
        if (currentUser->level == 0 || currentUser->level == 500 || currentUser->level == 1000)
            if (aux->type == 0)
                ;
            else
                continue;                
        else if (currentUser->level == 2000 || currentUser->level == 5000)
            if (aux->type == 1)
                ;
            else
                continue;
        else
            if (aux->type == 2)
                ;
            else
                continue;

        // Orders the question based on the random (0-4) variable 'r'
        if (r == 0){
            printf("*** Question: %s", aux->question);
            printf("*** %c: %s", optionVect[0], aux->answers[0]);
            printf("*** %c: %s", optionVect[1], aux->answers[1]);
            printf("*** %c: %s", optionVect[2], aux->answers[2]);
            printf("*** %c: %s", optionVect[3], aux->answers[3]);
        }

        if (r == 1){
            printf("*** Question: %s", aux->question);
            printf("*** %c: %s", optionVect[0], aux->answers[1]);
            printf("*** %c: %s", optionVect[1], aux->answers[0]);
            printf("*** %c: %s", optionVect[2], aux->answers[2]);
            printf("*** %c: %s", optionVect[3], aux->answers[3]);
        }

        if (r == 2){
            printf("*** Question: %s", aux->question);
            printf("*** %c: %s", optionVect[0], aux->answers[1]);
            printf("*** %c: %s", optionVect[1], aux->answers[2]);
            printf("*** %c: %s", optionVect[2], aux->answers[0]);
            printf("*** %c: %s", optionVect[3], aux->answers[3]);
        }

        if (r == 3){
            printf("*** Question: %s", aux->question);
            printf("*** %c: %s", optionVect[0], aux->answers[1]);
            printf("*** %c: %s", optionVect[1], aux->answers[2]);
            printf("*** %c: %s", optionVect[2], aux->answers[3]);
            printf("*** %c: %s", optionVect[3], aux->answers[0]);
        }


        // Gets the player input for the answer or commands
        printf(">");
        fgets(playerInput, 30, stdin);

        // Verifies the input commands
        while (playerInput[0] == 'j' || playerInput[0] == 's' || playerInput[0] == 'q' || playerInput[0] == 'h' || playerInput[0] == 'c'){
          if (strcmp(playerInput, "j 25\n") == 0){

            if (jkrturn == 1){
              puts(MSG_ILLEGAL);
              printf(">");
              fgets(playerInput, 30, stdin);
              continue;
            }

            if (currentUser->j25 == 0){
              puts(MSG_ILLEGAL);
              printf(">");
              fgets(playerInput, 30, stdin);
              continue;
            }

            if (r == 0){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[0]);
              printf("*** %c: %s", optionVect[1], aux->answers[1]);
              printf("*** %c: %s", optionVect[2], aux->answers[2]);
            }

            if (r == 1){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[1]);
              printf("*** %c: %s", optionVect[1], aux->answers[0]);
              printf("*** %c: %s", optionVect[3], aux->answers[3]);
            }

            if (r == 2){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[1]);
              printf("*** %c: %s", optionVect[1], aux->answers[2]);
              printf("*** %c: %s", optionVect[2], aux->answers[0]);
            }

            if (r == 3){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[1]);
              printf("*** %c: %s", optionVect[1], aux->answers[2]);
              printf("*** %c: %s", optionVect[3], aux->answers[0]);
            }
            
            currentUser->j25 = 0;
            printf(">");
            fgets(playerInput, 30, stdin);
            jkrturn = 1;
            r = rand() % 4;
            continue;
          }

          if (strcmp(playerInput, "j 50\n") == 0){

            if (jkrturn == 1){
              puts(MSG_ILLEGAL);
              printf(">");
              fgets(playerInput, 30, stdin);
              continue;
            }

            if (currentUser->j50 == 0){
              puts(MSG_ILLEGAL);
              printf(">");
              fgets(playerInput, 30, stdin);
              continue;
            }

            if (r == 0){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[0]);
              printf("*** %c: %s", optionVect[1], aux->answers[1]);
            }

            if (r == 1){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[1]);
              printf("*** %c: %s", optionVect[1], aux->answers[0]);
            }

            if (r == 2){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[1]);
              printf("*** %c: %s", optionVect[2], aux->answers[0]);
            }

            if (r == 3){
              printf("*** Question: %s", aux->question);
              printf("*** %c: %s", optionVect[0], aux->answers[1]);
              printf("*** %c: %s", optionVect[3], aux->answers[0]);
            }

            currentUser->j50 = 0;
            printf(">");
            fgets(playerInput, 30, stdin);
            jkrturn = 1;
            r = rand() % 4;
            continue;
          }

          if (playerInput[0] == 's'){
            // Initializes the filename that the user inputted
            char filename[MAX];

            if (playerInput[1] != '\0'){

                // Will fill the filename
                for (idx = 0; playerInput[idx + 2] != '\n'; idx++){
                    filename[idx] = playerInput[idx + 2];
                }

                filename[idx] = '\0';
                idx = 0;


                // Opens/Creates a newfile with the inputted name in write binary mode
                FILE * fp = fopen(filename, "wb");

                // Checks if there is any error while opening the file
                if (fp == NULL){
                    fprintf(stderr, "Error opening the file.\n");
                    return 1;
                }

                // Writes the data in PLAYERDATA on that file, then closes it
                fwrite(currentUser->playerName, sizeof(currentUser->playerName), 16, fp);
                fwrite(&currentUser->level, sizeof(currentUser->level), 1, fp);
                fwrite(&currentUser->j50, sizeof(currentUser->j50), 1, fp);
                fwrite(&currentUser->j25, sizeof(currentUser->j25), 1, fp);
                fclose(fp);

                // Shows the "saved game" message and then closes the program.
                puts(MSG_SAVE);
                exit(0);
            }
          }

          if (playerInput[0] == 'q'){
            puts(MSG_QUIT);
            exit(0);
          }

          if (playerInput[0] == 'h'){
            print_menu();
            printf(">");
            fgets(playerInput, 30, stdin);
            continue;
          }

          if (playerInput[0] == 'c'){
            credits();
            printf(">");
            fgets(playerInput, 30, stdin);
            continue;
          }

          break;
        }

        jkrturn = 0;

        switch (r){
        case 0:
            if (playerInput[0] == optionVect[0]){
                puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];
                r = rand() % 4;

                if (erriter == 1)
                    erriter = 0;
                
                if (currentUser->level == levels[8]){
                  current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }
                    
                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %c: %s", optionVect[0], aux->answers[0]);
                erriter += 1;
                if (lvliter != 0)
                  lvliter -= 1;
                currentUser->level = levels[lvliter];
                r = rand() % 4;

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
                
            }
            break;
        
        case 1:
            if (playerInput[0] == optionVect[1]){
              puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];    
                r = rand() % 4;

                if (erriter == 1)
                    erriter = 0;
                
                if (currentUser->level == levels[8]){
                  current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %c: %s", optionVect[1], aux->answers[0]);
                erriter += 1;
                if (lvliter != 0)
                  lvliter -= 1;
                currentUser->level = levels[lvliter];
                r = rand() % 4;

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25); 
            }
            break;

        case 2:
            if (playerInput[0] == optionVect[2]){
            puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];
                r = rand() % 4;

                if (erriter == 1)
                    erriter = 0;
                
                if (currentUser->level == levels[8]){
                  current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }
                
                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %c: %s", optionVect[2], aux->answers[0]);
                erriter += 1;
                if (lvliter != 0)
                  lvliter -= 1;
                currentUser->level = levels[lvliter];
                r = rand() % 4;

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            break;
        
        case 3:
            if (playerInput[0] == optionVect[3]){
              puts("*** Hooray!");
                lvliter += 1;
                currentUser->level = levels[lvliter];
                r = rand() % 4;

                if (erriter == 1)
                    erriter = 0;

                if (currentUser->level == levels[8]){
                  current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
                  puts("*** This is incredible! You have won!");
                  printf("*** Congratulations %s!\n", currentUser->playerName);
                  exit(0);
                }
                    
                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            else{
                puts("*** Woops... That's not correct.");
                printf("*** The correct answer was %c: %s", optionVect[3], aux->answers[0]);
                erriter += 1;
                if (lvliter != 0)
                  lvliter -= 1;
                currentUser->level = levels[lvliter];
                r = rand() % 4;

                if (erriter == 2){
                    puts("*** Sorry, you have lost the game. Bye!");
                    exit(0);
                }

                current_status(currentUser->playerName, currentUser->level, currentUser->j50, currentUser->j25);
            }
            break;
        }

    }

    puts("*** This is embarrassing but we’re out of questions.");
    exit(0);                                                                                                                                                                                                                                                            
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

// Reads the file and appends it to the linked list
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
    printf("*** Name:  %s                           *\n", name);
    printf("*** Level: %0d                               *\n", level);
    printf("*** j50:   %s                             *\n", jokerFiftyMessage);
    printf("*** j25:   %s                             *\n", jokerTwentyMessage);
    puts("********************************************");
}