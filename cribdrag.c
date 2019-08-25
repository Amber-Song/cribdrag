#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TEXTLENGTH 2000

/** 
 * @ciphertext is the input text which is the XOR of the plaintext @p and @q.
 * @cipherlen is the length of the ciphertext.
*/
char ciphertext[TEXTLENGTH];
char p[TEXTLENGTH];
char q[TEXTLENGTH];
int cipherlen;


/**
 * @brief insert function is to insert the searching words and the corresponding string 
 * into plaintext p and q. And print out the corresponding position and string in q.
 * @pos is the position where the words inserted.
 * @word is the one that we are searching.
*/
void insert(int pos, char word[])  {
    int j;
    int wordlen = strlen(word);

    for (j = 0; j < wordlen; j++) {
        p[pos+j] = word[j];
        q[pos+j] = ciphertext[pos+j] ^ word[j];
    }

    printf("%-10d", pos);
    for(j = 0; j < wordlen; j++){
        printf("%c", q[pos+j]);
    }
    printf("\n");
    
}


/**
 * @brief drag function is to find all the possible position in the plaintext for the searching word.
 * If the corresponding string that we get from (the ciphertext XOR word) makes sense, 
 * we will insert them in the plaintext. We will call insert function to implement this.
 * As we assume that all the appearance of the searching words will go into the plaintext p,
 * all the corresponding strings will go into the plaintext q.
*/
void drag(char word[]) {

    int i, j, k;
    char ch;

    int wordlen = strlen(word);

    for (i = 0; i < cipherlen - wordlen; i++) {
        for (j = 0; j < wordlen; j++)   {
            ch = ciphertext[i+j] ^ word[j];
            if (isalpha(ch)) {
                continue;
            } else if (ch == ' ' || ch == '.' || ch == ',')    {
                continue;
            } else {
                break;
            }
        }

        if (j == wordlen)   {
            insert(i, word);
            i = i + j - 1;
        }
    }
}

/**
 * @brief printpq function is to print out the plaintext p and q.
*/
void printpq()  {
    printf("The plaintext p:\n");
    for (int i = 0; i < cipherlen; i++) {
        printf("%c", p[i] ? p[i] : '.');
    }
    printf("\n\n");

    printf("The plaintext q:\n");    
    for (int i = 0; i < cipherlen; i++) {
        printf("%c", q[i] ? q[i] : '.');
    }
    printf("\n");
}

int main() {
    char word[50];
    int i;
    char conch = 'Y';

    /**
     * @brief Read in the two ciphertexts cp and cq.
     * And then XOR them to get the input text (ciphertext).
    */
    FILE *fp;
    char filenamecp[] = "cp.dms";
    char filenamecq[] = "cq.dms";

    printf("Input the words you want to drag. \n");
    printf("*** Recommand you to add spaces to each side of the words. ***\n");
    gets(word);

    fp = fopen(filenamecp, "rb");
    if (fp == NULL) {
        perror("Error: can't open the file.\n");
        exit(0);
    }
    cipherlen = fread(p, 1, TEXTLENGTH, fp);
    fclose(fp);


    fp = fopen(filenamecq, "rb");
    if (fp == NULL) {
        perror("Error: can't open the file.\n");
        exit(0);
    }
    fread(q, 1, TEXTLENGTH, fp);
    fclose(fp);


    for (i = 0; i < cipherlen; i++) {
        ciphertext[i] = p[i] ^ q[i];
    }

    
    /**
     * Initiate the two plaintexts p and q.
     * Start dragging.
    */
    while(conch == 'Y' || conch == 'y'){
        memset(q, 0, TEXTLENGTH);
        memset(p, 0, TEXTLENGTH);

        drag(word);

        printf("If you want to continue to search for other words?(Y/N)\n>");
        scanf("%c", &conch);
    }
    
    /**
     * Print out the plaintext p and q.
    */
    /*
    
    */



    return 0;
}