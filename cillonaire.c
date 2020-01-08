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
typedef struct {
    char question[ULTMAX];
    char answers[4][ULTMAX];
    enum category;
    enum diff {easy, medium, hard} type;
} QUESTION;

typedef struct _node * link;
head = NULL;

typedef struct _node {
    QUESTION qust;
    link next;
} node;

// Function declaration
void print_menu(void);
void credits(void);
void current_status(char, int, int, int);
void question_read(FILE *, node);
void new_game(PLAYERINFO *);
int push(link *, QUESTION);
QUESTION pop(link *);
int isEmpty(link);

// The main function of the program
int main(int argc, const char **argv){

    // First arguments
    int seed;
    int commandCheck;
    char input[30];
    int idx = 0;
    PLAYERINFO currentUser;
    FILE * fp;
    node questions;

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

            question_read(fp, questions); //gives segmentation fault for some godforsaken reason
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
				FILE * fp = fopen(argv[4], "r");

				if (fp == NULL){
					fprintf(stderr, "Error opening the file.\n");
					return 1;
				}

				question_read(fp, questions);
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
            printf("%s, %s\n",questions.qust.question, questions.next);
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
    
    // INCOMPLETE
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

void question_read(FILE * fp, node questions){

	char * question_vect;
    question_vect = (char *) malloc(10*sizeof(char));
	int dog;

    while (fgets(question_vect, ULTMAX, fp) != NULL){
        if (question_vect[0] == ';'){
            continue;
        }
        if (question_vect[0] == 'Q'){

            // Coloca o endereço de memória do node da questão a estrutura de dados da função
            push(head, questions.qust); // deixa o ponteiro null, tem que ver oq colocar ao inves de and questions

            // Passa linhas e copia cada uma das coisas pra seus respectivos lugares
            strcpy(questions.qust.question, question_vect + 9); 
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(questions.qust.answers[0], question_vect + 8); 
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(questions.qust.answers[1], question_vect + 8);
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(questions.qust.answers[2], question_vect + 8);
            strcpy(question_vect, "");
            fgets(question_vect, ULTMAX, fp);
            strcpy(questions.qust.answers[3], question_vect + 8);
            strcpy(question_vect, "");
			printf("%s", questions.qust.question); 
			printf("%s", questions.qust.answers[0]); 
			printf("%s", questions.qust.answers[1]); 
			printf("%s", questions.qust.answers[2]);
			printf("%s", questions.next);
            //fgets(question_vect, ULTMAX, fp);
            //falta enum types 
        }
    }

    free(question_vect);

}

int push(link * head, QUESTION q){
    link x = malloc(sizeof(node));
    if (x == NULL){
        fprintf(stderr, "Out of Memory!!\n");
        return 0;
    }

    x-> qust = q;
    x-> next = *head;

    *head = x;
	free(x);
    return 1;
}

QUESTION pop(link * head){
    QUESTION x;
    link aux = *head;

    if (isEmpty(*head)){
        fprintf(stderr, "Stack is empty!!\n");
        exit(1);
    }

    x = aux -> qust;
    *head = aux -> next;

    free(aux);
}

int isEmpty(link l){
    return l = NULL;
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