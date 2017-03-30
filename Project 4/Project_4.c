#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct list_t {
    char *word;
    char docID;
    struct list_t *next;
} list_t;

typedef struct hash_table_t {
    int size;       /* size of the table */
    list_t **table; /* the table elements */
} hash_table_t;


/* shiity substitue for proj3.h */
list_t *search_list(hash_table_t *hashtable, char *word, char docID);
void find_match(hash_table_t *hashtable, char *word, int *score_list);
void score(list_t *pair, int *score_list);

/* Create a new hashtable */
hash_table_t *create_hash_table(int size)
{
    hash_table_t *new_table;
    int i;
    
    if (size<1) return NULL;    /* Invalid hash table size*/
    
    if ((new_table = malloc(sizeof(hash_table_t))) == NULL) return NULL;  /* Attempt to allocate mem for table struct*/
    
    
    if ((new_table->table = malloc(sizeof(list_t *) * size)) == NULL) return NULL; /* Attempt to allocate mem for the table itself */

    for (i=0; i<size; i++) new_table->table[i] = NULL; /* Initialize the elements of the table */
    
    new_table->size = size; /* Set the table's size */
    
    return new_table;
    
}

/* Hash a string for a particular hash table */
int hash_code(hash_table_t *hashtable, char *w)
{
    int S = 0;
 
    while (*w)
        S += *w++;   /* Sum the ASCII values of each char */
    
    return S % hashtable->size; 
}

/* Return a pair (word, docID) to corresponding bucket*/
void *add_to_list(hash_table_t *hashtable, char *word, char docID) 
{
    list_t *newpair;
    int bucket = hash_code(hashtable, word);
    
        /* Attempt to allocate memory for list */
    if ((newpair = malloc(sizeof(list_t))) == NULL) 
        return;

    
    newpair->word = strdup(word);
    
    newpair->docID = docID;
    
    newpair->next = hashtable->table[bucket];
    hashtable->table[bucket] = newpair;
    //printf(" sucessfuly hashed: %d\n", bucket);
    
}

/* Inserts a string word and the associated document docID in the hash table */ 
void hash_table_insert(hash_table_t *hashtable, char *word, char docID) 
{
    list_t *currentpair;
        
    /* Check for duplicate entry */
    currentpair = search_list(hashtable, word, docID);
    
    /* Entry exists, return */
    if (currentpair != NULL) 
    {
        //printf("...fail hash\n");
        return;
    }
    else
    {
    /* Insert into Bucket */
    add_to_list(hashtable, word, docID);
    }
 }

/* Will return list_t word if match found, otherwise return NULL */
list_t *search_list(hash_table_t *hashtable, char *word, char docID)
{
    int bucket = 0;
    list_t *pair;
    //char test[100];  DEBUGG
    //char test2[100]; DEBUGG
        
    bucket = hash_code(hashtable, word);
    //strcpy(test, word);
    //printf(" searching... word: %s bucket: %d doc: %c\n", test, bucket, docID);  DEBUGG
        
    for(pair = hashtable->table[bucket]; pair != NULL; pair = pair->next) 
    {
       // strcpy(test, word);
       // strcpy(test2, pair->word);
            
       // printf("word[%d]: %s | word->doc: %c | pair->word: %s | pair->doc: %c\n", bucket, test, docID, test2, pair->docID);   DEBUGG

        if (strcmp(word, pair->word) == 0 && docID == pair->docID) 
        {

            return pair;
        }
        //memset(test, ' ', 100);
        //memset(test2, ' ', 100);
    }
    
    return NULL;
}

/*  Really really inefficient pre-processing stage */
hash_table_t *training(FILE *D1, FILE *D2, FILE *D3, int size)
{
    /* Create word array to store words from text file */
    char word[20];
    char c;
    int i = 0;
    
    /* create hashtable from create_hash_table */
    hash_table_t *hashtable = create_hash_table(size);
    
    while ((c = fgetc(D1)) != EOF)
    {
        
        if ( ((c>='a') && (c<='z')) || ((c>='A') && (c<='Z')) )
        {
            //add to char array
            word[i++] = c;
            //printf("%c", c);
        }
        else if(c == ' ' || c=='\n')
        {
            word[i] = '\0';
            //hash a bitch
            //printf("\nhashing...\n");
            hash_table_insert(hashtable, word, '1');
            memset(word, ' ', 20);
            i = 0;
        }
    
    }
    word[i] = '\0';
    //printf("\nfinal hash\n");
    hash_table_insert(hashtable, word, '1');
    memset (word, ' ', 20);
    i = 0;
    
    while ((c = fgetc(D2)) != EOF)
    {
        
        if ( ((c>='a') && (c<='z')) || ((c>='A') && (c<='Z')) )
        {
            //add to char array
            word[i++] = c;
            //printf("%c", c);
        }
        else if(c == ' ' || c=='\n')
        {
            word[i] = '\0';            
            //hash a bitch
            //printf("\nhashing...\n");
            hash_table_insert(hashtable, word, '2');
            memset (word, ' ', 20);
            i = 0;
        }
    }
    word[i] = '\0';
    //printf("\nfinal hash\n");
    hash_table_insert(hashtable, word, '2');
    memset (word, ' ', 20);
    i = 0;

    while ((c = fgetc(D3)) != EOF)
    {
        
        if ( ((c>='a') && (c<='z')) || ((c>='A') && (c<='Z')) )
        {
            //add to char array
            word[i++] = c;
            //printf("%c", c);
        }
        else if(c == ' ' || c=='\n')
        {
            //hash a bitch
            word[i] = '\0';
            //printf("\nhashing...\n");
            hash_table_insert(hashtable, word, '3');
            memset (word, ' ', 20);
            i = 0;
        }
    }
    word[i] = '\0';
    //printf("\nfinal hash\n");
    hash_table_insert(hashtable, word, '3');
    memset (word, ' ', 20);
    
    fclose(D1);
    fclose(D2);
    fclose(D3);
    
    return hashtable;
}

/* Read the query file, find words, then call find_match */
void read_query(hash_table_t *hashtable, FILE *query, int *score_list)
{
    char word[200];
    char test[100];
    char c;
    int i = 0;
    list_t *searchword;
    
    fseek(query, 0, SEEK_SET);
        
    i = 0;
    
    while ((c = fgetc(query)) != EOF)
    {
        if ( ((c>='a') && (c<='z')) || ((c>='A') && (c<='Z')) )
        {
            //add to char array
            word[i++] = c;
            //printf("%c", c);
        }
        else if(c == ' ' || c=='\n')
        {
            //search for word
            word[i] = '\0'; 
            find_match(hashtable, word, score_list);

            memset (word, ' ', 20);
            i = 0;
        }
    }
    
    //printf("done w query set... printing scores\n");
}

/* Take keyword, then updates score if found in bucket */
void find_match(hash_table_t *hashtable, char *word, int *score_list)
{
    int bucket = 0;
    list_t *pair;
    char test[100];
    char test2[100];
        
    bucket = hash_code(hashtable, word);
    //strcpy(test, word);
    //printf(" searching... word: %s bucket: %d\n", test, bucket);
        
    for(pair = hashtable->table[bucket]; pair != NULL; pair = pair->next) 
    {
        //strcpy(test, word);
        //strcpy(test2, pair->word);
            
        //printf("word[%d]: %s | pair->word: %s | pair->doc: %c\n", bucket, test, test2, pair->docID);

        if (strcmp(word, pair->word) == 0) 
        {
            //printf("update D%c score...\n", pair->docID);
            score(pair, score_list);
        }
        //memset(test, ' ', 100);
        //memset(test2, ' ', 100);
    }
    
}
 
/* Updates scores in score_list */
void score(list_t *pair, int *score_list)
{
    
    char docID = pair->docID;
    
    if (docID == '1')
    {
        score_list[0]++;
        //printf("\tD1 score: %d\n",  score_list[0]);
    }
    else if (docID == '2')
    {
        score_list[1]++;
        //printf("\tD2 score: %d\n",  score_list[1]);
    }
    else if (docID == '3')
    {
        score_list[2]++;
        //printf("\tD3 score: %d\n",  score_list[2]);
    }

}
/* sorts the score_list of documents in Descending order */
void print_score(int *score_list, int wCount)
{
    
    if (score_list[0] >= score_list[1])          //D1 > D2
    {
        if (score_list[0] >= score_list[2])      //D1 > D3
        {
            printf("D1-%d\n", score_list[0]);
            if (score_list[0] == wCount)
                printf("Perfect Match!\n");
            if (score_list[1] >= score_list[2])  //D2 > D3
            {
                printf("D2-%d\n", score_list[1]);
                printf("D3-%d\n", score_list[2]); //[D1, D2, D3]
                return;
            }
            else
            {
                printf("D3-%d\n", score_list[2]);
                printf("D2-%d\n", score_list[1]); //[D1, D3, D2]
                return;
            }
        }
        else
        {
            printf("D3-%d\n", score_list[2]);
            if (score_list[2] == wCount)
                printf("Perfect Match!\n");
            printf("D1-%d\n", score_list[0]);
            printf("D2-%d\n", score_list[1]);    //[D3, D1, D2]
            return;
        }
    }
    else if (score_list[1] >= score_list[2])    //D2 > D3
    {
        printf("D2-%d\n", score_list[1]);
        if (score_list[1] == wCount)
                printf("Perfect Match!\n");
        if (score_list[0] >= score_list[2])     //D1 > D3
        {
            printf("D1-%d\n", score_list[0]);
            printf("D3-%d\n", score_list[2]);    //[D2, D1, D3]
            return;
        }
        else
        {
            printf("D3-%d\n", score_list[2]);
            printf("D1-%d\n", score_list[0]);    //[D2, D3, D2]
            return;
        }
           
    }
    else
        printf("D3-%d\n", score_list[2]);
        if (score_list[2] == wCount)
                printf("Perfect Match!\n");
        printf("D2-%d\n", score_list[1]);
        printf("D1-%d\n", score_list[0]);        //[D3, D2, D1]
        return;
}

/* Counts number of words in char array */
int countQuery(char *query)
{
    int count = 0;
    while (*query != '\0')
    {
        while (*query != '\0' && isblank(*query))
        {
            query++;
        }
        if (*query != '\0')
        {
            count++;
        }
        
        while (*query != '\0' && !isblank(*query))
        {
            query++;
        }
    }
    return count;
}

int main() 
{
    
    /*ASSUMPTIONS:
    / 
    /Maximum 3 documents to be read
    /Documents and query file all stored in currnet working directory /
    /Maximum query word size capped at 1000 characters length
    /
    /
   */

    FILE *D1 = fopen("D1.txt", "r");
    FILE *D2 = fopen("D2.txt", "r");
    FILE *D3 = fopen("D3.txt", "r");
   
    char qwords[1000];  /* holds query words inputted by user */
    char input[256];    /* holds buffer for size of hashtable */
    int size, i, wCount;
    
    /* Trouble reading any documents, abort */
    if (!D1 || !D2 || !D3)
    {
        printf("Error opening documents. Exiting.\n");
        return 1;
    }
    
    printf("Enter desired hash table size: ");
    fgets(input, 256, stdin);
    size = atoi(input);
    
    /* training phase... constructs hashtable from Files D1 D2 and D3 */
    hash_table_t *hashtable = training(D1, D2, D3, size);
    
    /* int array with scores as following format: [ D1, D2, D3] */
    int score_list[3];
    
    while(1) 
    {
        FILE *query = fopen("query.txt", "w+");
        
        if (!query)
        {
            printf("Error opening query.txt. Exiting.\n");
            return 1;
        }
        
        printf("Enter query words (signal # to abort): ");
        fgets(qwords, 1000, stdin);
        
        /* check for abort symbol */
        if (qwords[0] == '#') return 1;
        
        /* print query words to query.txt */
        fprintf(query, "%s", qwords);
        
        /* clears scores to 0 */
        for (i=0; i<3; i++)
            score_list[i] = 0;
        
        /* Read the query words, search for matches in hashtable, then update score */
        read_query(hashtable, query, score_list);
        
        /* calculate number of words in query */
        wCount = countQuery(qwords);
        
        /* Sort the score_list, and print to console... check for perfect match */
        print_score(score_list, wCount);

        fclose(query);
    }
    
}
