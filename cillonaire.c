#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Text definition
#define MSG_ARG_ERROR1 "Too many arguments, only -s and -f supported."
#define MSG_ARG_ERROR2 "Non-existent command. Only -s and -f supported."
#define MSG_QUIT "*** Sad to see you go..."
#define MSG_UNK "*** Unknown input."
#define MSG_ILLEGAL "*** Illegal move"
#define MSG_SAVE "*** Ok, your progress has been saved. See you later!"
#define MAX 64

// Function declaration
void print_menu(void);
void credits(void);
void current_status(char, int, int, int);

// The main function of the program
int main(int argc, char **argv){

    // Data struct with player information. 
    typedef struct playerinfo {
        char playerName[16];
        int level;
        int j50;
        int j25;
    } PLAYERINFO;

    // First arguments
    int seed;
    int commandCheck;
    char input[30];
    int idx = 0;
    PLAYERINFO currentUser;
    FILE * fp;

    // Verification of the first arguments
    if (argc < 1){
        srand(time(0));
    }
    if (argc > 4){
        puts(MSG_ARG_ERROR1);
        exit(0);
    }
    if (argc > 1){

        if (argv[1] == "-s"){
            seed = atoi(argv[2]);
            commandCheck = 0;
            srand(seed);
        }

        else if (argv[1] == "-f"){
            commandCheck = 1;
            // TODO
        }

        else{
            puts(MSG_ARG_ERROR2);
            exit(0);
        }

        if (argv > 2){
            switch (commandCheck)
            {
            case 0:
                if (argv[3] == "-f")
                    // TODO -f command
                    break;
            
            case 1:
                if (argv[3] == "-s"){
                    seed = atoi(argv[4]);
                    srand(seed);
                    break;
                }
            }
        }
    }
    
    // Prints the starting menu
    print_menu();

    // Loop that will verify which inputs the player has inputted and decide what it will do based on that
    while(1){

        // Verifies the player input and stores it on the "input"
        fgets(input, 30, stdin);

        if (input[0] == 'n'){

            if (input[1] != '\n'){
                
                for (idx; input[idx] != '\n'; idx++){
                    currentUser.playerName[idx] = input[idx + 2];    // Working (i think)
                }

                idx = 0;
            
            }

            else{
                strcpy(currentUser.playerName, "newbie");
                printf("%s\n", currentUser.playerName);
            }

            //new_game();
        }

        else if (input[0] == 'r'){

            // Initializes the filename that the user inputted
            char fileread[MAX];

            if (input[1] != '\0'){

                // Will fill the fileread
                for (int idx; input[idx] != '\0'; idx++)
                    fileread[idx] = input[idx + 2];
                }

                // Opens the file with the inputted name in read binary mode
                FILE * fp = fopen("filename", "rb");

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
                for (int idx; input[idx] != '\0'; idx++){
                    filename[idx] = input[idx + 2];
                }

                // Opens/Creates a newfile with the inputted name in write binary mode
                FILE * fp = fopen("filename", "wb");

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

        else if (input == "h"){
            print_menu();
            continue;
        }

        else if (input == "c"){
            credits();
            continue;
        }

        else if (input == "q"){
            puts(MSG_QUIT);
            return 0;

        }

        else{
            puts(MSG_UNK);}
    }

    return 0;

    }

// The main function of the game.
//void new_game(){
    // INCOMPLETE
//}

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