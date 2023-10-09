#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Pattern_struct {
    char* pat;  //string pattern - exs: "hello", "-ell-", "-----"
    int count;  //frequency of pattern in the current word list
    int changes;//number of occurences of new letter in this pattern
} Pattern;

// The below function takes in a new word and add it to the wordList list based in their word size. If the capacity of the wordList is exceeding then this function doubles the capacity.
// Inputs: word list, number of words, max words, and new Word
// Output: updating the wordlist but no returns in this function.
void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    if(((*numWords)+1) > *maxWords){
        char **newWordArr = (char**)malloc(sizeof(char*)*(*maxWords*2));
        for(int i = 0; i < *maxWords; i++){
            newWordArr[i] = (*words)[i];
        }
        free(*words);
        *words = newWordArr;
        *maxWords *= 2;
    }
    (*words)[(*numWords)] = (char*)malloc(sizeof(char)*strlen(newWord)+1);
    strcpy((*words)[*numWords],newWord);
    (*numWords)++;
}

//The below function compares 2 strings and returns how many characters are different between 2 strings.
// Inputs: 2 strings(str 1, str 2) passed by paramater.
// Outputs: num of characters that are different between 2 strings.
int strNumMods(char* str1, char* str2) {
    int length1 = strlen(str1);
    int length2 = strlen(str2);
    int counter = 0;

    if(length2 > length1){
        counter = length2 - length1;
    }
    else if(length1 > length2){
        counter = length1 - length2;
    }
    while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 != *str2){
            counter++;
        }
        str1++;
        str2++;
    }
    
    return counter;
}

//The below function compares 2 strings and returns the first index where the string differs.
// Inputs: 2 strings(str 1, str 2) passed by paramater.
// Outputs:  index where the string differs.
int strDiffInd(char* str1, char* str2) {
    int index = 0;
    int length = strlen(str1);
    if(strNumMods(str1, str2) == 0){
        return length;
    }
    while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 != *str2){
            return index;
        }
        index++;
        str1++;
        str2++;
    }
    return index;
}

// The below function asks the user for the user input and makes sure the user input is not capital letter, not a number number, and it was not entered previously.
// Inputs: previous guess array which keeps track of all the user inputs.
// Outputs: return the valid userInput.
char valid(char *previousGuess){
    char userInput;
    printf("Guess a letter (# to end game): \n");
    scanf(" %c",&userInput);
    bool isValid = true;
    int ascii = (int)(userInput);
    int index = ascii - 94;

    if((ascii >= 97 && ascii <= 122)){
        for(int p = 0; p < 26; p++){
                if(previousGuess[p] == userInput){
                    printf("Letter previously guessed...\n");
                    return valid(previousGuess);
                    break;
                }
        }
        previousGuess[index] == userInput;
        return userInput;
    }
    else if(userInput == '#'){
        return userInput;
    }
    else{
        printf("Invalid letter...\n");
        return valid(previousGuess);
    }
}

// The below function prints the settings in the starting of the game based on the given user command line arguments.
// Inputs: stats Mode boolean, word list mode boolean, letter list mode boolean, pattern list mode boolean, word size integer, and number of guesses integer
// Outputs: No return but prints the game settings before the user enters anything.
void settingsPrint(bool statsMode,bool wordListMode,bool letterListMode,bool patternListMode,int wordSize,int numGuesses){
    printf("Game Settings:\n");
    printf("  Word Size = %d\n", wordSize);
    printf("  Number of Guesses = %d\n",numGuesses);
    if(statsMode == true){
        printf("  View Stats Mode = ON\n");
    }else{
        printf("  View Stats Mode = OFF\n");
    }
    if(wordListMode == true){
        printf("  View Word List Mode = ON\n");
    }else{
        printf("  View Word List Mode = OFF\n");
    }
    if(letterListMode == true){
        printf("  View Letter List Mode = ON\n");
    }else{
        printf("  View Letter List Mode = OFF\n");
    }
    if(patternListMode == true){
        printf("  View Pattern List Mode = ON\n");
    }else{
        printf("  View Pattern List Mode = OFF\n");
    }
}

// The below function takes in the pattern array and choses the final pattern based on the tie brakers.
// Inputs: number of patterns, final Pattern struct which stores the final pattern to be used, the word size, user Input, and the max Count.
// Outputs: updated final pattern struct with the final pattern.
void choosePattern(int numPatterns,Pattern *finalPattern, Pattern *pattern,int wordSize,char userInput,int maxCount){
    maxCount = 0;
    for(int p = 0; p < numPatterns; p++){
        if(maxCount < pattern[p].count){
            maxCount = pattern[p].count;
            strcpy(finalPattern->pat,pattern[p].pat);
            finalPattern->count  = pattern[p].count;
            finalPattern->changes = pattern[p].changes;
        }
        else if(maxCount == pattern[p].count){
            if(finalPattern->changes > pattern[p].changes){
                strcpy(finalPattern->pat,pattern[p].pat);
                finalPattern->count  = pattern[p].count;
                finalPattern->changes = pattern[p].changes;
            }
            else if(finalPattern->changes == pattern[p].changes){
                for(int w = 0; w < wordSize; w++){
                    if(pattern[p].pat[w] == userInput && finalPattern->pat[w] != userInput){
                        strcpy(finalPattern->pat,pattern[p].pat);
                        finalPattern->count  = pattern[p].count;
                        finalPattern->changes = pattern[p].changes;
                        break;
                    }
                    else if(pattern[p].pat[w] != userInput && finalPattern->pat[w] == userInput){
                        finalPattern->count  = finalPattern->count;
                        finalPattern->changes = finalPattern->changes;
                        break;
                    }
                    else{
                        continue;
                    }
                }
            }
        }
    }
}

// The below function checks if the user Input is present in the final pattern and prints the statements based on that.
// Inputs: final Pattern struct which contains the pattern that is being used in the game, user input character, and int number of guesses passed by parameter.
// Outputs: Doesn't return anything, however prints "Oops, there are no (userInput)'s. You used a guess." if the user input is not present in the final pattern, and "Good guess! The word has at least one (userInput)" if the user input is present in the final pattern.
void printIfCorrect(Pattern finalPattern,char userInput, int *numGuesses){
    bool isDash = true;
    for(int p = 0; p < strlen(finalPattern.pat); p++){
        if(finalPattern.pat[p] != userInput){
            isDash = true;
        }
        else{
            isDash = false;
            break;
        }
    }
    if(isDash == true){
        printf("Oops, there are no %c's. You used a guess.\n", userInput);
        (*numGuesses)--;
    }
    bool hasUserInput = true;
    for(int p = 0; p < strlen(finalPattern.pat); p++){
        if(finalPattern.pat[p] == userInput){
            hasUserInput = true;
            break;
        }
        else{
            hasUserInput = false;
        }
    }
    if(hasUserInput == true){
        printf("Good guess! The word has at least one %c.\n",userInput);
    }
}

// The below function prints the statics if the stats mode is true.
// Inputs: stats Mode boolean, word size integer, number of words in the final as int, character array maxWord which contains the max word contained in the file, int max word length, int number of words in the word list, int word size provided by the user, and int capacity which is the capacity of the word list.
// Outputs: doesn't return anything but prints the stats of the words in the word list.
void printStats(bool statsMode,int fileNumWords,char maxword[40],int maxwordLength,int numWords,int wordSize,int capacity){
    if(statsMode == true){
        printf("The dictionary contains %d words total.\n",fileNumWords);
        printf("The longest word %s has %d chars.\n",maxword,maxwordLength);
        printf("The dictionary contains %d words of length %d.\n",numWords,wordSize);
        printf("Max size of the dynamic words array is %d.\n",capacity);
    }
}

// The below functions sets the settings of the game based on the command line arguments
// Inputs: int argc which is how many arguments there are, char argv array which contains the command line arguments, int number of guesses passed by parameter, bool stats mode passed by parameter, bool word list mode passed by parameter, bool letter list mode passed by parameter, bool pattern list mode passed by parameter.
// Outputs: a bool that returns if the arguments have invalid size, invalid number of guesses or invalid command line arguments.
bool setSettings(int argc,char *argv[],int *wordSize,int *numGuesses,bool *statsMode,bool *wordListMode,bool *letterListMode,bool *patternListMode){
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-n") == 0){
            if(atoi(argv[i + 1]) <= 1 || atoi(argv[i + 1]) > 100){
                printf("Invalid word size.\n");
                printf("Terminating program...\n");
                return true;
            }
            *wordSize = atoi(argv[i + 1]);
            i++;
        }
        else if(strcmp(argv[i],"-g") == 0){
            if(atoi(argv[i + 1]) <= 1 || atoi(argv[i + 1]) > 26){
                printf("Invalid number of guesses.\n");
                printf("Terminating program...\n");
                return true;
            }
            *numGuesses = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i],"-w") == 0){
            *wordListMode = true;
        }
        else if(strcmp(argv[i],"-s") == 0){
            *statsMode = true;
        }
        else if(strcmp(argv[i],"-l") == 0){
            *letterListMode = true;
        }else if(strcmp(argv[i],"-p") == 0){
            *patternListMode = true;
        }
        else if(strcmp(argv[i],"-v") == 0){
            *wordListMode = true;
            *statsMode = true;
            *letterListMode = true;
            *patternListMode = true;
        }
        else{
            printf("Invalid command-line argument.\n");
            printf("Terminating program...\n");
            
            return true;
        }
    }
    return false;
}

// The below prints if the user solved the word.
// Inputs: final pattern which is a type of pattern struct which contains the final pattern.
// Outputs: doesn't return anything, but prints that the user solved the word.
void printsolved(Pattern finalPattern){
    
    printf("You solved the word!\n");
    printf("The word is: %s\n",finalPattern.pat);
    printf("Game over.");
    
}

// The below function prints the word if the user runs out of guesses.
// Inputs: character array wordlist
// Outputs: prints the first word of the word list if the user runs out of guesses.
void printOutOfGuesses(char** wordList){
    printf("You ran out of guesses and did not solve the word.\n");
    printf("The word is: %s\n",wordList[0]);
    printf("Game over.");
}

// The below function checks if the final pattern has any dashes to see if the user solved the word.
// Inputs: bool solved passed by parameter,Pattern struct finalPattern which contains the final pattern used in the game.
// Outputs: no output sets the solved boolean to true or false.
void fixSolved(bool *solved,Pattern finalPattern){
    for(int p = 0; p < strlen(finalPattern.pat); p++){
        if(finalPattern.pat[p] != '-'){
            *solved = true;
        }
        else{
            *solved = false;
            break;
        }
    }
}

// the below function prints the previously guessed letters if the letter list mode is on.
// Inputs: bool letterListMode,char array previousGuess
// Outputs: doesn't output anything but prints the previously guessed array in order.
void trueLetterListMode(bool letterListMode,char *previousGuess){
    if(letterListMode == true){
        printf("Previously guessed letters: ");
        for(int g = 0; g < 26; g++){
            if(previousGuess[g] != '$'){
                printf("%c ",previousGuess[g]);
            }
        }
        printf("\n");
    }
}

// The below function sets adds the pattern if the generated pattern doesn't exist in the pattern array.
// Inputs: int numPatterns passed by parameters,Pattern struct pattern passed by parameters,int patCapacity passed by parameters,char curpat passed by parameters,Pattern struct finalPattern,int wordSize.
// Outputs: doesn't return anything. adds the pattern if the generated pattern doesn't exist in the pattern array and updates the numpattern and pat capacity variables.
void addNewPattern(int *numPatterns,Pattern **pattern,int *patCapacity,char *curpat,Pattern finalPattern,int wordSize){
    if(*numPatterns + 1 > *patCapacity){
        Pattern* newpattern = (Pattern*)malloc(((*patCapacity)*2)*sizeof(Pattern));
        for(int newp = 0; newp < *patCapacity; newp++){
            newpattern[newp] = (*pattern)[newp];
            newpattern[newp].pat = (*pattern)[newp].pat;
        }
        free(*pattern);
        *pattern = newpattern;
        *patCapacity *= 2;
    }
    (*pattern)[*numPatterns].pat = (char*)malloc(sizeof(char)*(wordSize+1));
    strcpy((*pattern)[*numPatterns].pat,curpat);
    (*pattern)[*numPatterns].count = 1;
    (*pattern)[*numPatterns].changes = strNumMods(curpat, finalPattern.pat);
    (*numPatterns)++;
}

// the below function prints the word list if the word list mode is on.
// Inputs: int wordSize,int numWords,char wordList array which contains all the words that word size in length.
// Outputs: Doesn't output anything. prints the word list if the word list mode is on.
void printWordList(int wordSize,int numWords,char **wordList){
    if(numWords == 0){
        printf("Dictionary has no words of length %d.\n",wordSize);
        printf("Terminating program...\n");
    }
    else{
        for(int w = 0; w < numWords; w++){
            printf("  %s\n",wordList[w]);
        }
    }
}

int main(int argc, char* argv[]) {
    printf("Welcome to the (Evil) Word Guessing Game!\n\n");
    bool solved = false;
    int wordSize = 5;
    int numGuesses = 26;
    bool statsMode = false;
    bool wordListMode = false;
    bool letterListMode = false;
    bool patternListMode = false;
    int capacity = 4;
    int fileNumWords = 0;
    int numWords = 0;
    int addedWord = 0;
    char maxword[40];
    int max = 0;
    char** wordList = (char**)malloc(capacity*sizeof(char*)); // contains words that are word size long.
    char word[40]; // contains the max word.

    bool shouldReturn = setSettings(argc,argv,&wordSize,&numGuesses,&statsMode,&wordListMode,&letterListMode,&patternListMode);
    if(shouldReturn == true){
        return 0;
    }
    settingsPrint(statsMode,wordListMode,letterListMode,patternListMode,wordSize,numGuesses);
    // below lines of code reads the file and inputs the word with word size length using the add word function.
    FILE *inputFile = fopen("dictionary.txt","r");
    if(inputFile == NULL){
        printf("The file can't be open.\n");
    }
    while(!feof(inputFile)){
        fscanf(inputFile,"%s",word);
        if(max < strlen(word)){
            max = strlen(word);
            strcpy(maxword,word);
        }
        if(strlen(word) == wordSize){
            addWord(&wordList,&numWords,&capacity,word);
            
        }
        fileNumWords++;
    }
    fclose(inputFile);
    int maxwordLength = strlen(maxword);
    printStats(statsMode,fileNumWords,maxword,maxwordLength,numWords,wordSize,capacity); // prints the stats if the stats mode is on.
    if(wordListMode == true){
        printf("Words of length %d:\n",wordSize);
        printWordList(wordSize,numWords,wordList);
        if(numWords == 0){
            return 0;
        }
    }
    char curpat[wordSize + 1];
    char inputwordpattern[wordSize];
    char previousGuess[26];
    printf("The word pattern is: ");
    // sets curpat array which is builds the pattern for every word and inputwordpattern array which is used to generate pattern every word in the word list to check if a word can go in the word list to all dashes and sets the last character to null character.
    for(int i = 0; i < wordSize; i++){
        curpat[i] = '-';
        printf("%c",curpat[i]);
        inputwordpattern[i] = '-';
    }
    printf("\n\n");
    curpat[wordSize] = '\0';
    inputwordpattern[wordSize] = '\0';
    printf("Number of guesses remaining: %d\n", numGuesses);
    for(int i = 0; i < 26; i++){
        previousGuess[i] = '$';
    }
    if(letterListMode == true){
        printf("Previously guessed letters: \n");
    }
    char userInput = valid(previousGuess);
    if(userInput == '#'){
        printf("Terminating game...\n");  
        return 0;
    }
    // below few lines asks the user for the user input inside the valid function and checks if the user input is valid by checking if the user input is a digit, a uppercase letter, or is previously entered.
    // it takes in the valid user input and subtracts the ascii value of it with 97 which is the ascii value of 'a'. This generates the index which is used to put in the previous guess array. 
    // This makes sure that the user previous guesses are placed in order.
    int ascii = (int)userInput;
    int index = ascii - 97;
    previousGuess[index] = userInput;
    int patCapacity = 4;
    int maxCount = 0;
    int numPatterns = 0;
    Pattern* pattern = (Pattern*)malloc(patCapacity*sizeof(Pattern));
    bool isthere;
    Pattern finalPattern;
    finalPattern.pat = (char*)malloc(sizeof(char)*(wordSize+1));
    strcpy(finalPattern.pat, curpat);
    // The below while loop is where the game play is taken care of.
    while(userInput != '#'){
        isthere = false;
        numPatterns = 0;
        char oldfinal[wordSize+1];
        strcpy(oldfinal,finalPattern.pat);
        // below for loop goes through the all the words in the word list array and generates a pattern which is then added to the pattern array.
        for(int w = 0; w < numWords; w++){
            strcpy(curpat,finalPattern.pat);
            for(int s = 0; s < wordSize; s++){
                if(wordList[w][s] == userInput){
                    curpat[s] = userInput;
                }
            }
            // below for loop checks if the generated pattern is in the pattern array.
            for(int p = 0; p < numPatterns; p++){
                if(strcmp(pattern[p].pat,curpat) == 0) {
                    pattern[p].count++;
                    isthere = true;
                    break;
                }else{
                    isthere = false;
                }
            }
            if(isthere == false){
                addNewPattern(&numPatterns,&pattern,&patCapacity,curpat,finalPattern,wordSize); // adds a new pattern into the pattern array.
            }
        }
        choosePattern(numPatterns,&finalPattern, pattern,wordSize,userInput,maxCount); // chooses the final pattern based in the tie brakers provided in the course description.
        // prints all the patterns if the pattern list mode is turned on.
        if(patternListMode == true){
            printf("All patterns for letter %c: \n",userInput);
            for(int p = 0; p < numPatterns; p++){
                printf("  %s \tcount = %d \tchanges = %d\n",pattern[p].pat,pattern[p].count,pattern[p].changes);
            }
        }
        printIfCorrect(finalPattern,userInput,&numGuesses); // this function prints if the user input is present in the final pattern or not.
        // The below few lines updates the word list by removing the words based on the final pattern.
        char** newWordList = (char**)malloc((finalPattern.count)*sizeof(char*));
        int newWordListInd = 0;
        for(int word= 0; word < numWords; word++){
            strcpy(inputwordpattern,oldfinal);
            for(int s = 0; s < wordSize; s++){
                if(wordList[word][s] == userInput){
                    inputwordpattern[s] = userInput;
                }
            }
            if(strcmp(inputwordpattern,finalPattern.pat) == 0){
                newWordList[newWordListInd++] = wordList[word];
            }
            else{
                free(wordList[word]);
            }
        }
        free(wordList);
        wordList = newWordList;
        numWords = newWordListInd;
        if(statsMode == true){
            printf("Number of possible words remaining: %d\n",numWords);
        }
        solved = true;
        fixSolved(&solved,finalPattern);
        // prints the words if the word list mode is on.
        if(wordListMode == true){
            printf("Possible words are now:\n");
            printWordList(wordSize,numWords,wordList);
        }
        printf("The word pattern is: %s\n\n",finalPattern.pat);
        if(solved == true){
            printsolved(finalPattern);
            for(int p = 0; p < numPatterns; p++){
                free(pattern[p].pat);
            }
            break;
        }
        if(numGuesses == 0){
            printOutOfGuesses(wordList);
            for(int p = 0; p < numPatterns; p++){
                free(pattern[p].pat);
            }
            break;
        }
        printf("Number of guesses remaining: %d\n", numGuesses);
        trueLetterListMode(letterListMode,previousGuess);
        // frees the pattern array to put the patterns again after the user inputs another character.
        for(int p = 0; p < numPatterns; p++){
            free(pattern[p].pat);
        }
        // scans for the user input again and puts it in the previous guess array in order.
        userInput = valid(previousGuess);
        // if the user input is # then the code gets out of the while loop.
        if(userInput == '#'){
            printf("Terminating game...\n");
            break;
        }
        ascii = (int)userInput;
        index = ascii - 97;
        previousGuess[index] = userInput;
        
    }
    // Below lines frees the pattern array and the word list array and the final pattern in the final pattern struct.
    free(pattern);
    for(int i = 0; i < numWords; i++){
        free(wordList[i]);
    }
    free(wordList);
    free(finalPattern.pat);
    return 0;
}