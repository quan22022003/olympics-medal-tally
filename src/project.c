#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareBronze(const void* a, const void* b) {
    int retVal;
    Nation* nat1 = (Nation*)a;
    Nation* nat2 = (Nation*)b;

    retVal = nat2->bronze - nat1->bronze;

    return retVal;
}

int compareSilver(const void* a, const void* b) {
    int retVal;
    Nation* nat1 = (Nation*)a;
    Nation* nat2 = (Nation*)b;

    retVal = nat2->silver - nat1->silver;
    if (!retVal) {
        retVal = compareBronze(a, b);
    }

    return retVal;
}

int compareGold(const void* a, const void* b) {
    int retVal;
    Nation* nat1 = (Nation*)a;
    Nation* nat2 = (Nation*)b;

    retVal = nat2->gold - nat1->gold;

    //If both nations have the same number of gold medals, compare their number of silver medals
    if (!retVal) {
        retVal = compareSilver(a, b);
    }

    return retVal;
}

int compareName(const void* a, const void* b) {
    int retVal;
    Nation* nat1 = (Nation*)a;
    Nation* nat2 = (Nation*)b;

    retVal = strcmp(nat1->name, nat2->name);

    return retVal;
}

int line_count(const char* filename) {
    FILE* f;

    f = fopen(filename, "r");
    if (!f) return -1;

    char string[1001];
    char* ret;
    int count = 0;

    ret = fgets(string, sizeof(string), f);

    //Read a new line until meeting an empty line
    while (ret) {
        count++;
        ret = fgets(string, sizeof(string), f);
    }
    
    fclose(f);
    return count;
}

int countNations(Nation* database) {
    int num_of_nations = 0;

    //Count the number of nations in the database have non-empty names.
    while(database[num_of_nations].name[0]) {
        num_of_nations++;
    }
    
    return num_of_nations;
}

Nation* handleA(Nation* database, char* entry) {
    int num_of_nations;
    Nation new;
    int retVal;
    char name[900];

    //Extract the number of nations in the database
    num_of_nations = countNations(database); 

    //Parse the entry
    retVal = sscanf(entry, "%*s %s\n", name);

    //Check if the arguments are valid
    if (retVal != 1) {
        printf("A should be followed by exactly 1 argument.\n");
        return NULL;
    }

    //Create a search key for the binary search
    strncpy(new.name, name, sizeof(new.name));
    new.gold = 0;
    new.silver = 0;
    new.bronze = 0;

    qsort(database, num_of_nations, sizeof(Nation), compareName);

    //Check if the nation is already in the database
    if (bsearch(&new, database, num_of_nations, sizeof(Nation), compareName)) {
        printf("Nation \"%s\" is already in the database.\n", name);
        return NULL;
    }

    //Create a new dynamically allocated database
    Nation* new_database = (Nation*)calloc((num_of_nations + 2), sizeof(Nation));
    
    //Reallocate existed nations from old database to the new database
    for (int i = 0; i < num_of_nations; i ++) {
        strncpy(new_database[i].name, database[i].name, sizeof(new_database[i].name));
        new_database[i].gold = database[i].gold;
        new_database[i].silver = database[i].silver;
        new_database[i].bronze = database[i].bronze;
    }
    
    //Add the nation to the database
    new_database[num_of_nations] = new;

    printf("SUCCESS\n");
   
    return new_database;
}

void handleM(Nation* database, char* entry) {
    int num_of_nations;
    Nation search_key;
    Nation* search_nation;
    int gold, silver, bronze;
    int retVal;
    char name[1001];

    //Extract the number of nations in the database
    num_of_nations = countNations(database);

    retVal = sscanf(entry, "%*s %s %i %i %i\n", name, &gold, &silver, &bronze);
    //Check if the arguments are valid
    if (retVal != 4) {
        printf("M should be followed by exactly 4 arguments.\n");
        return;
    }

    strcpy(search_key.name, name);

    qsort(database, num_of_nations, sizeof(Nation), compareName);
    search_nation = (Nation*)bsearch(&search_key, database, num_of_nations, sizeof(Nation), compareName);
    
    //Check if the nation is already in the database
    if (!search_nation) {
        printf("Nation \"%s\" is not in the database.\n", name  );
    } else {
        search_nation->gold += gold;        //Change the number of gold medal of the nation by a number gold.
        search_nation->silver += silver;    //Change the number of silver medal of the country by a number silver.
        search_nation->bronze += bronze;    //Change the number of bronze medal of the country by a number bronze.
        printf("SUCCESS\n");
    }
}

void commonPrint(Nation* database, FILE* f) {
    int num_of_nations;
    int i;
    Nation curr;

    //if the FILE pointer is NULL, print an error message
    if (!f) {
        printf("An error has occured.\n");
        return;
    }  

    //Extract the number of nations in the database
    num_of_nations = countNations(database);
    //Sort the nations by the number of gold medals
    qsort(database, num_of_nations, sizeof(Nation), compareGold);
    
    //Print the info
    for (i = 0; i < num_of_nations; i++){
        curr = database[i];
        fprintf(f, "%s %i %i %i\n", curr.name, curr.gold, curr.silver, curr.bronze);
        if (feof(f)) {
            printf("An error has occured.\n");
            return;
        }
    }
    printf("SUCCESS\n");
}

void handleL(Nation* database) {
    commonPrint(database, stdout);
}

char* extractFileName(char* entry) {
    int retVal;
    char* filename;
    int nameLength;

    nameLength = strlen(entry) - 3;
    filename = (char*)malloc(nameLength + 1);

    retVal = sscanf(entry, "%*s %s\n", filename);

    //Check if the arguments are valid
    if (retVal != 1) {
        return NULL;
    } else return filename;
}

char* extractNationName(char* line) {
    int retVal;
    char* nationName;
    int nameLength;

    nameLength = strlen(line) - 7;
    nationName = (char*)malloc(nameLength + 1);

    retVal = sscanf(line, "%s %*s\n", nationName);
    //Check if the arguments are valid
    if (retVal != 1) {
        return NULL;
    } else return nationName;
}

void handleW(Nation* database, char* entry) {
    char* filename;
    filename = extractFileName(entry);
    FILE* f;

    //Check if the pointer to the file name is not NULL
    if (!filename){
        free(filename);
        printf("The command arguments are not valid.\n");
        return;
    }

    f = fopen(filename, "w+");

    //Check if the file is opened to write
    if (!f) {
        free(filename);
        printf("An error has occured.\n");
        return;
    } else {
        commonPrint(database, f);
        free(filename);
    }

    fclose(f);
}

Nation* handleO(char* entry) {
    char* filename;
    int num_of_nations;
    char string[1001];
    FILE* f;
    char* ret;
    int i;
    char* nationName;

    filename = extractFileName(entry);

    //Check if the pointer to filename is NULL
    if (!filename){
        printf("The command arguments are not valid.\n");
        free(filename);
        return NULL;
    }

    //Extract the number of lines in the text file, i.e. the number of nations in the text file
    num_of_nations = line_count(filename);
    f = fopen(filename, "r");

    //Check if the file can be opened.
    if (!f) {
        printf("Cannot open file %s for reading.\n", filename);
        free(filename);
        return NULL;
    }

    //Create an empty database to add nations
    Nation* database = (Nation*)calloc((num_of_nations + 1), sizeof(Nation));
    ret = fgets(string, sizeof(string), f);
    for (i = 0; i < num_of_nations; i++) {
        nationName = extractNationName(ret);
        strcpy(database[i].name, nationName);
        sscanf(ret, "%*s %i %i %i\n", &database[i].gold, &database[i].silver, &database[i].bronze);
        ret = fgets(string, sizeof(string), f);
        free(nationName);
    }

    printf("SUCCESS\n");

    fclose(f);
    free(filename);
    return database;
}

int main(void) {
    Nation* database;
    Nation* new_database;
    char entry[1001];
    char* ret;


    database = (Nation*)calloc(1, sizeof(Nation));          //Create an empty database

    while (1) {
        ret = fgets(entry, sizeof(entry), stdin);

        if (ret[0] == 'Q') {                                //If the command is "Q"
            printf("SUCCESS\n");
            free(database);                                 //free the database
            break;
        }
        

        switch(ret[0]) {
            case 'A':                                       //If the command is "A"
                new_database = handleA(database, ret);
                if (new_database) {
                    free(database);
                    database = new_database;                //Old database is freed and replaced by new database
                }
                break;
            case 'L':                                       //If the command is "L"
                handleL(database);
                break;
            case 'M':                                       //If the command is "M"
                handleM(database, ret);
                break;
            case 'W':                                       //If the command is "W"
                handleW(database, ret);
                break;
            case 'O':                                       //If the command is "O"
                new_database = handleO(ret);
                if (new_database) {
                    free(database);
                    database = new_database;
                }
                break;
            default:                                        //If the command is other than what above, i.e. is an invalid command
                printf("Invalid command %s", ret);
                break;
        }
    }    
    return 1;
}