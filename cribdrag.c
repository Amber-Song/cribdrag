#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXTLENGTH 2000

/** 
 * @ciphertext is the cipher text which need to be decoding 
 * @p and @q are the two plaintext which mix together.
*/
char ciphertext[TEXTLENGTH];
char p[TEXTLENGTH];
char q[TEXTLENGTH];
int cipherlen;

int change(int i, char word[]);



/**
 * @brief seek for words in the ciphertext
 * if it makes sense, fit the word in the p and the other one in q.
*/
void drag(char word[]) {

    int i, j, k;
    char ch;

    int wordlen = strlen(word);

    for (i = 0; i < cipherlen - wordlen; i++) {
        for (j = 0; j < wordlen; j++)   {
            ch = ciphertext[i+j] ^ word[j];
            /**
             * if the character get from XOR is 
             * in capital letter, save it
             * in lower-case letter, save it
             * comma or period or space, save it.
             * if the character doesn't make sense,
             * change all the letter it have already changed
            */
            if (ch <= 122 && ch >= 97)  {
                continue;
            } else if (ch <= 65 && ch >= 90)    {
                continue;
            } else if (ch == 44 || ch == 46 || ch == 32)    {
                continue;
            } else   {
                break;
            }
        }

        if (j == wordlen)   {
            k = change(i, word);
            if (k == 0) {
                continue;
            } else  {
                i = i + j -1;
            }
        }
    }
}



int change(int i, char word[])  {
    int j;
    int wordlen = strlen(word);

    for (j = 0; j < wordlen; j++) {
        p[i+j] = word[j];
        q[i+j] = ciphertext[i+j] ^ word[j];
    }
    
    for (; j > 0; j--)
    {
        /**
         * if the word doesn't make sense, undo it
        */
        if ( q[i+j-1] <= 65 && q[i+j-1] >= 90 && q[i+j-2] != 32)  {
            for (j = 0; j < wordlen; j++)   {
                p[i+j] = '\0';
                q[i+j] = '\0';
            }
            return 0;
        }
    }
    return 1;
}



int main() {
    char word[50];
    int i;

    FILE *fp;
    char filenamecp[] = "cp.dms";
    char filenamecq[] = "cq.dms";

    printf("Input the words you want to drag. \n");
    gets(word);


    // Open the file and read into the ciphertext cp.
    fp = fopen(filenamecp, "rb");
    if (fp == NULL) {
        perror("Error: can't open the file.\n");
        exit(0);
    }
    cipherlen = fread(p, 1, TEXTLENGTH, fp);
    fclose(fp);


    // Open the file and read into the ciphertext cq.
    fp = fopen(filenamecq, "rb");
    if (fp == NULL) {
        perror("Error: can't open the file.\n");
        exit(0);
    }
    fread(q, 1, TEXTLENGTH, fp);
    fclose(fp);


    // XOR cq and cp, save in the ciphertext
    for (i = 0; i < cipherlen; i++) {
        ciphertext[i] = p[i] ^ q[i];
    }


    memset(q, 0, TEXTLENGTH);
    memset(p, 0, TEXTLENGTH);


    drag(word);


    // print out the plaintext p and q.
    for (i = 0; i < cipherlen; i++) {
        printf("%c", p[i] ? p[i] : '.');
    }
    printf("\n\n");

    for (i = 0; i < cipherlen; i++) {
        printf("%c", q[i] ? q[i] : '.');
    }
    printf("\n");

    return 0;
}