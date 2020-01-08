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
    char playerName[16];
    int level;
    int j50;
    int j25;
} PLAYERINFO;

// Data struct with question information
typedef struct _node {
    char question[ULTMAX];
    char answers[4][ULTMAX];
    enum diff {easy, medium, hard} type;
    struct _node * next;
} QUESTION;

// Function declaration
void print_menu(void);
void credits(void);
void current_status(char, int, int, int);
QUESTION * question_read(FILE *);
void new_game(PLAYERINFO *);

// The main function of the program
int main(int argc, const char **argv){

    // First arguments
    int seed;
    int commandCheck;
    char input[30];
    int idx = 0;
    PLAYERINFO currentUser;
    FILE * fp;
    QUESTION * questions;

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
			
			if (questions->next == NULL){
				fprintf(stderr,"Something wrong happened.\n");
				exit(0);
			}

			printf("%d\n", questions->type);
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
			
				questions = question_read(fp); //gives segmentation fault for some godforsaken reason

				fclose(fp);
				
				if (questions->next == NULL){
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

            //new_game();
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

                printf("*** Ok %s, where were we? Oh there you go:", currentUser.playerName);
                //new_game();
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
void new_game(PLAYERINFO *currentUser){
    
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

QUESTION * question_read(FILE * fp){

	char * question_vect;
    question_vect = (char *) malloc(10*sizeof(char));
    QUESTION * head = NULL;
	head = (QUESTION *) malloc(sizeof(QUESTION));
    QUESTION * new;

    while (fgets(question_vect, ULTMAX, fp) != NULL){
        if (question_vect[0] == ';'){
            continue;
        }
        if (question_vect[0] == 'Q'){
            
            // Coloca o endereço de memória do node da questão a estrutura de dados da função
            new = (QUESTION *) malloc(sizeof(QUESTION));
            new->next = NULL;

            // Passa linhas e copia cada uma das coisas pra seus respectivos lugares
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
			printf("%s", new->question); 
			printf("%s", new->answers[0]); 
			printf("%s", new->answers[1]); 
			printf("%s", new->answers[2]);
			printf("%s", new->answers[3]);
            fgets(question_vect, ULTMAX, fp);  // pra nao zuar as coisas
			strcpy(new->type, question_vect + 11);
			
            //falta enum types 

            if (head == NULL){
                head = new;
            }
            
			else{
				new->next = head;
				head = new;
			}
        }

    }

    free(question_vect);
	return new;

}


void current_status(char name, int level, int joker50, int joker25){
    char jokerFiftyMessage[3], jokerTwentyMessage[3];

    if (joker50 == 0){
        strcpy(jokerFiftyMessage, "NO");}
    else{
        strcpy(jokerFiftyMessage, "YES");}
    if (joker25 == 0){
        strcpy(jokerTwentyMessage, "NO");}
    else{
        strcpy(jokerTwentyMessage, "YES");}

    puts("********************************************");
    printf("*** Name: %02s                             *\n", name);
    printf("*** Level: %02d                            *\n", level);
    printf("*** j50: %02s                              *\n", jokerFiftyMessage);
    printf("*** j25: %02s                              *\n", jokerTwentyMessage);
    puts("********************************************");
}