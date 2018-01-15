/* This program is to create strings for Lindenmayer systems*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

enum {WIDTH = 1700, HEIGHT = 950};

//a rule consist of an ancestor (char) and a child (string)
struct Rules{
    char ancestor;
    char *child;
};

//this is the struct to store things in a convenient way
struct System{
//we don't need more than 20 rules
    struct Rules rule[20];
    char *axiom;
    int gen;
};

/*global constants
LIMIT : the maximum length of the calculated string
limit : the maximum number of iterations
err1 : error to indicate invalid input
err2 : error to indicate too long string
*/
enum {LIMIT = 1000000, limit = 100, err1 = -1, err2 = -2};

//string constants to output error
char * const Err1 = "INVALID_INPUT_THE_PROGRAM_DOES_NOTHING";
char * const Err2 = "TOO_LONG_IS_THE_CALCULATED_STRING";

//check validity
bool isValid(int n, char **data){
//we convert data[1] into number and back to string
    char str2[10];
    sprintf(str2, "%d", atoi(data[1]));
//we check the lengths and the first character
    if (atoi(data[1]) <= 0 || strcmp(str2, data[1]) != 0) return false;
//if we have odd many arguments
    if (n % 2 == 1) return false;
//if an ancestor is not a character
    for (int i = 3; i <= n; i += 2){
        if (strlen(data[i]) > 1) return false;
    }
//if the number of the iterations greater than limit
    if (atoi(data[1]) > limit) return false;
//if two ancestors are the same
    for (int i = 3; i <= n - 1; i += 2){
    for (int j = i + 2; j <= n; j += 2){
        if (data[i][0] == data[j][0]) return false;
    }}
    return true;
}

//converting data into struct System
struct System conversion(int n, char **data){
    struct System s;
    s.axiom = malloc(100);
    memset(s.axiom, '\0', 100);
    if (!isValid(n, data)) {
        s.gen   = -1;
        s.axiom = Err1;
    }
    else{
        s.gen   = atoi(data[1]);
        s.axiom = data[2];
        for (int i = 0; i <= n / 2 - 2; i++){
            s.rule[i].child = malloc(100);
            memset(s.rule[i].child, '\0', 100);
            s.rule[i].ancestor = data[2 * i + 3][0];
            s.rule[i].child    = data[2 * i + 4];
        }
    }
    return s;
}

//number of c in str
static int numCharinString(char *str, char c){
    int num = 0;
    for (int i = 0; i < strlen(str); i++){
        if (str[i] == c) num++;
    }
    return num;
}

/* Function to sort an array using insertion sort*/
static void insertionSort(char arr[], int n)
{
   int i, key, j;
   for (i = 1; i < n; i++)
   {
       key = arr[i];
       j = i - 1;

/* Move elements of arr[0..i-1], that are
   greater than key, to one position ahead
   of their current position */
       while (j >= 0 && arr[j] > key)
       {
           arr[j + 1] = arr[j];
           j = j - 1;
       }
       arr[j + 1] = key;
   }
}

//determine the characterset used by the user
char *charset(const struct System sys, int n){
    int numRules = n / 2 - 1;
    int limit = 1000;
    char str[limit];
//we concat things together
    str[0] = '\0';
    strcat(str, sys.axiom);
    for (int i = 0; i < numRules; i++){
        strcat(str, sys.rule[i].child);
    }
//sort the characters
    insertionSort(str, strlen(str));
//we fill another strig with different characters from the concatted string
    char *fill = malloc(limit);
    fill[0] = str[0];
    fill[1] = '\0';
    for (int i = 0; i < strlen(str)-1; i++){
        if (str[i] != str[i + 1]){
            char s[2] = {str[i + 1], '\0'};
            strcat(fill, s);
        }
    }
    return fill;
}

long long int length(const struct System sys, int n, int numIt){
//preparation to the calculation
    if (numIt == -1) return -1;
    int numRules = n / 2 - 1;
    int numChars = strlen(charset(sys, n));
    char cs[numChars];
    memset(cs, '\0', numChars);
    strcpy(cs, charset(sys, n));
    long long int chars[numChars];
    for (int i = 0; i < numChars; i++) chars[i] = 0;
    for (int i = 0; i < numChars; i++){
        chars[i] = numCharinString(sys.axiom, cs[i]);
    }
    long long int oldChars[numChars];
    long long int sum = 0;

//the calculation
    for (int g = 0; g < numIt; g++){//A. loop
      sum = 0;
      for (int i = 0; i < numChars; i++) oldChars[i] = chars[i];
      for (int h = 0; h < numRules; h++){//B. loop
            int saveNum = 0;
            long long int saveValue = 0;
            for (int x = 0; x < numChars; x++){
                if (sys.rule[h].ancestor == cs[x]){
                    saveNum = x;
                    saveValue = oldChars[saveNum];
                }
            }
            for (int i = 0; i < strlen(sys.rule[h].child); i++){
                for (int j = 0; j < numChars; j++){
                    if (sys.rule[h].child[i] == cs[j]) chars[j] += saveValue;
                }
            }
            chars[saveNum] -= saveValue;
        }//B. loop
        int lim = 1000;
        for (int i = 0; i < numChars; i++) sum += chars[i];
        if (sum > LLONG_MAX / lim) return err2;
    }//A. loop

    return sum;
}

//insert b into a after pos
static void insertString(int pos, char *a, char *b){
    char *add = malloc(strlen(a) + strlen(b) + 1);
    memset(add, '\0', strlen(a) + strlen(b) + 1);
    strncpy(add, a, pos);
    add[pos] = '\0';
    strcat(add, b);
    strcat(add, a+pos);
    strcpy(a, add);
    free(add);
    add = NULL;
}

//delete length characters from a after pos
static void deleteString(int pos, int length, char *a){
    char *add = malloc(strlen(a) - length + 1);
    memset(add, '\0', strlen(a) - length + 1);
    strncpy(add, a, pos);
    add[pos] = '\0';
    strcat(add, a+pos+length);
    strcpy(a, add);
    free(add);
    add = NULL;
}

//to determine the result string after the iterations
char *iteration(char **data, int n){
//checking error possible errors
    int numRules = n / 2 - 1;
    if (numRules > 20) return Err1;
    struct System sys = conversion(n, data);
    if (sys.gen == err1) return Err1;
    long long int len = length(sys, n, sys.gen);
    if (sys.gen != err1){
        if (len > LIMIT || len == err2) sys.gen = err2;
    }
    if (sys.gen == err2){
        return Err2;
    }
    char *result = malloc(len + 1);
    memset(result, '\0', len + 1);
    strcat(result, sys.axiom);
    for (int k = 1; k <= sys.gen; k++){
        for (int i = len - 1; i >= 0; i--){
            for (int j = 0; j < numRules; j++){
                if (result[i] == sys.rule[j].ancestor){
//the manipulation in the string
                    deleteString(i, 1, result);
                    insertString(i, result, sys.rule[j].child);
                    break;
                }
            }
        }
    }
   return result;
}

//test Fibonacci words
static void testFibonacci(){
    char **data;
    const int lim = 1000;
    data = (char**)malloc(lim);
    data[1] = "1"; data[2] = "AB"; data[3] = "A";
    data[4] = "AB"; data[5] = "B"; data[6] = "A";
    assert(strcmp(iteration(data, 6), "ABA") == 0);
    data[1] = "2";
    assert(strcmp(iteration(data, 6), "ABAAB") == 0);
    data[1] = "3";
    assert(strcmp(iteration(data, 6), "ABAABABA") == 0);
    data[1] = "4";
    assert(strcmp(iteration(data, 6), "ABAABABAABAAB") == 0);
    data[1] = "5";
    assert(strcmp(iteration(data, 6), "ABAABABAABAABABAABABA") == 0);
    free(data);
}

//test the cantor set
static void testCantor(){
    char **data;
    const int lim = 1000;
    data = (char**)malloc(lim);
    data[1] = "1"; data[2] = "A"; data[3] = "A";
    data[4] ="ABA"; data[5] = "B"; data[6] = "BBB";
    assert(strcmp(iteration(data, 6), "ABA") == 0);
    data[1] = "2";
    assert(strcmp(iteration(data, 6), "ABABBBABA") == 0);
    data[1] = "3";
    assert(strcmp(iteration(data, 6), "ABABBBABABBBBBBBBBABABBBABA") == 0);
    data[1] = "4";
    assert(strcmp(iteration(data, 6),
    "ABABBBABABBBBBBBBBABABBBABABBBBBBBBBBBBBBBBBBBBBBBBBBBABABBBABABBBBBBBBBABABBBABA") == 0);
    free(data);
}

//test Koch's snowflake
static void testKochSnowFlake(){
    char **data;
    const int lim = 1000;
    data = (char**)malloc(lim);
    data[1] = "1"; data[2] = "K"; data[3] = "K";
    data[4] ="K+K--K+K";
    assert(strcmp(iteration(data, 4), "K+K--K+K") == 0);
    data[1] = "2";
    assert(strcmp(iteration(data, 4), "K+K--K+K+K+K--K+K--K+K--K+K+K+K--K+K") == 0);
    data[1] = "3";
    assert(strcmp(iteration(data, 4), "K+K--K+K+K+K--K+K--K+K--K+K+K+K--K+K+K+K--K+K+K+K--K+K--K+K--K+K+K+K--K+K--K+K--K+K+K+K--K+K--K+K--K+K+K+K--K+K+K+K--K+K+K+K--K+K--K+K--K+K+K+K--K+K") == 0);
    free(data);
}

//test Sierpinki's gasket
static void testSierpinskiGasket(){
    char **data;
    const int lim = 1000;
    data = (char**)malloc(lim);
    data[1] = "1"; data[2] = "R"; data[3] = "L";
    data[4] ="R+L+R"; data[5] = "R"; data[6] = "L-R-L";
    assert(strcmp(iteration(data, 6), "L-R-L") == 0);
    data[1] = "2";
    assert(strcmp(iteration(data, 6), "L-R-L+L+R-L-R-L-L-R-L+L+R") == 0);
    data[1] = "3";
    assert(strcmp(iteration(data, 6), "L-R-L+L+R-L-R-L-L-R-L+L+R+L-R-L+L+R+L-R-L-L-R-L+L+R-L-R-L-L-R-L+L+R-L-R-L+L+R-L-R-L-L-R-L+L+R+L-R-L+L+R+L-R-L") == 0);
    free(data);
}

//test the input values
static void testInput(){
    char **data;
    const int lim = 1000;
    data = (char**)malloc(lim);
    data[1] = "0"; data[2] = "R"; data[3] = "L";
    data[4] ="R+L+R"; data[5] = "R"; data[6] = "L-R-L";
    assert(strcmp(iteration(data, 6), Err1) == 0);
    data[1] = "2.2";
    assert(strcmp(iteration(data, 6), Err1) == 0);
    data[1] = "3xd";
    assert(strcmp(iteration(data, 6), Err1) == 0);
    data[1] = "kc17716";
    assert(strcmp(iteration(data, 6), Err1) == 0);
    data[1] = "101";
    assert(strcmp(iteration(data, 6), Err1) == 0);

    data[1] = "100"; data[2] = "A"; data[3] = "A"; data[4] = "AB";
    assert(strcmp(iteration(data, 4), Err1) != 0);
    data[1] = "6"; data[2] = "A"; data[3] = "A"; data[4] = "B";
    data[5] = "A"; data[6] = "C";
    assert(strcmp(iteration(data, 6), Err1) == 0);
    data[1] = "1"; data[2] = "A"; data[3] = "0"; data[4] = "A"; data[5] = "1";
    data[6] = "A"; data[7] = "2"; data[8] = "A"; data[9] = "3"; data[10] = "A";
    data[11] = "4"; data[12] = "A"; data[13] = "5"; data[14] = "A"; data[15] = "6";
    data[16] = "A"; data[17] = "7"; data[18] = "A"; data[19] = "8"; data[20] = "A";
    data[21] = "9"; data[22] = "A"; data[23] = "a"; data[24] = "A"; data[25] = "b";
    data[26] = "A"; data[27] = "c"; data[28] = "A"; data[29] = "d"; data[30] = "A";
    data[31] = "e"; data[32] = "A"; data[33] = "f"; data[34] = "A"; data[35] = "g";
    data[36] = "A"; data[37] = "h"; data[38] = "A"; data[39] = "i"; data[40] = "A";
    data[41] = "j"; data[42] = "A"; data[43] = "k"; data[44] = "A";
    assert(strcmp(iteration(data, 40), Err1) != 0);
    assert(strcmp(iteration(data, 42), Err1) != 0);
    assert(strcmp(iteration(data, 44), Err1) == 0);

    free(data);
}

//test whether the output string is too long
static void testLength(){
    char **data;
    const int lim = 1000;
    data = (char**)malloc(lim);
    data[1] = "100"; data[2] = "R"; data[3] = "L";
    data[4] ="R+L+R"; data[5] = "R"; data[6] = "L-R-L";
    assert(strcmp(iteration(data, 6), Err2) == 0);
    data[1] = "100";
    assert(strcmp(iteration(data, 6), Err2) == 0);
    data[1] = "100";
    assert(strcmp(iteration(data, 6), Err2) == 0);
    data[1] = "100"; data[2] = "A"; data[3] = "A"; data[4] = "AB";
    data[5] = "B"; data[6] = "A";
    assert(strcmp(iteration(data, 6), Err2) == 0);
    data[1] = "100";
    assert(strcmp(iteration(data, 6), Err2) == 0);

    data[1] = "100"; data[2] = "A"; data[3] = "A"; data[4] = "AB";
    data[5] = "B"; data[6] = "A";
    assert(strcmp(iteration(data, 6), Err2) == 0);

    free(data);
}
static void lsystemTest(){
    testFibonacci();
    testCantor();
    testKochSnowFlake();
    testSierpinskiGasket();
    testInput();
    testLength();

    printf("All tests passed!\n");
}

int LsystemMain(int n, char **argv){

    setbuf(stdout,NULL);
//we should decrease n by 1
    n--;
//we need a number, an axiom, and at least one rule so n is at least 4
    if (n > 3){
        char **data;
        data = argv;
        struct System sys = conversion(n, data);
        int sysgenOld = sys.gen;
        printf("Number of iteration: %d, Axiom: %s\n", sys.gen, sys.axiom);
        if (sys.gen != -1){
            for (int i = 0; i <= n/2-2; i++){
                printf("Rule %d: %c -> %s\n", i + 1, sys.rule[i].ancestor, sys.rule[i].child);
        }
        }
            printf("Result: %s\n", iteration(data, n));

        if (sys.gen != -1){
            printf("Characterset you used: %s\n", charset(sys, n));
            printf("Number of characters: %lld\n", length(sys, n, sysgenOld));
        }
//we test
    } else if (n == 0){
          lsystemTest();
    }
    else {
        printf("Type something like\n\t\t./l-system 2 AB A AB B A\n\t\t\tor\n\t\t./l-system\n");
    }

    return 0;
}
