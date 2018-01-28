//
//  main.c
//  hw1
//
//  Created by borute on 2018/1/26.

//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, const char * argv[]) {
    //argv[1] = directory
    //argv[2] = the integer
    DIR *dir;
    dir = opendir(argv[1]);
    int count = 0;
    struct dirent* dirn;
    if (dir == NULL) {
        printf("ERROR: This is not a directory.\n");
        return 1;
    }
    //long** file_sizes;
    //create an array of regular file names
    char** file_names;
    int index = 0;
    while ((dirn = readdir(dir)) != NULL) {
        printf("found %s", dirn->d_name);
        struct stat a;
        count += 1;
        //temporarily used to save the file path
        char* path = malloc(1 + strlen(argv[1]) + 1 + strlen(dirn->d_name));
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirn->d_name);
        lstat(path, &a);
        //file mode
        printf("File mode %d\n", a.st_mode);
        printf("is regular: %d \n", S_ISREG(a.st_mode));
        //mark the regular files
        if(S_ISREG(a.st_mode)){
            file_names = realloc(file_names, (sizeof(file_names) + sizeof(char*)));
            file_names[index] = malloc(1+strlen(dirn->d_name));
            //file_sizes = realloc(file_sizes, (sizeof(file_sizes) + sizeof(long*)));
            //file_sizes[index] = malloc(sizeof(long));
            //*file_sizes[index] = (long)a.st_size;
            strcpy(file_names[index], dirn->d_name);
            index += 1;
        }
        //free
        free(path);
    }
    /*
    if(close == 0){
        printf("%d files found.\n", count);
        fflush(stdout);
    }*/
    //printf("%s\n", file_names[2]);
    //printf("yay\n");
    int size = 32;
    char** words = calloc(size, sizeof(char*));
    int** counts = calloc(size, sizeof(int*));
    printf("Allocated initial parallel arrays of size %d.\n", size);
    int word_count = 0;
    int total_word_count = 0;
    char* word_temp = malloc(80);
    
    //for(int a = 0; a < index; a++){
        //open file
        //FILE* text = fopen(file_names[a],"r");
        FILE* text = fopen("text.txt","r");
        //char* full_text = malloc(*file_sizes[3]+1);
       // printf("yay1\n");
        if(text != NULL){
            //read the original text
            /*
            rewind(text);
            fread(full_text, 1, ftell(text),text);
            //deal with non-alphanumeric characters
            for(long gg = 0; gg < strlen(full_text); gg++){
                if(isalnum(full_text[gg]) == 0){
                    full_text
                }
            }
             */
            rewind(text);
            //printf("yay2\n");
            _Bool new = 1;
            while(fscanf(text,"%s", word_temp) != EOF){
                //printf("%s\n", word_temp);
                //printf("words[0]: %s\n", words[0]);
                //non-alphanumeric characters?
                
                _Bool nonal = 0;
                for(int gg = 0; gg < strlen(word_temp); gg++){
                    if(isalnum(word_temp[gg]) == 0){
                        word_temp[gg] = ' ';
                        nonal = 1;
                    }
                }
                if(nonal){
                    char* sub_words;
                    char** words_temp;
                    sub_words = strtok(word_temp, " ");
                    int counter = 0;
                    while(sub_words != NULL){
                        //printf("%s\n", sub_words);
                        counter += 1;
                        words_temp = realloc(words_temp, counter * sizeof(char*));
                        words_temp[counter-1] = malloc(1 + strlen(sub_words));
                        strcpy(words_temp[counter-1], sub_words);
                        sub_words = strtok(NULL, " ");
                    }
                    for(int gg = 0; gg < counter; gg++){
                        if(strlen(words_temp[gg]) > 1){
                            for(int b = 0; b < word_count; b++){
                                if(strcmp(words_temp[gg], words[b]) == 0){
                                    *counts[b] += 1;
                                    new = 0;
                                    total_word_count += 1;
                                }
                            }
                            //if a new word is found
                            if(new){
                                words[word_count] = malloc(1+strlen(words_temp[gg]));
                                counts[word_count] = malloc(sizeof(int));
                                strcpy(words[word_count], words_temp[gg]);
                                *counts[word_count] = 1;
                                word_count += 1;
                                total_word_count += 1;
                                //printf("Word count plus 1\n");
                            }
                        }
                    }
                    free(sub_words);
                    //free(words_temp);
                }
                else if(strlen(word_temp) > 1){
                    for(int b = 0; b < word_count; b++){
                        if(strcmp(word_temp, words[b]) == 0){
                            *counts[b] += 1;
                            new = 0;
                            total_word_count += 1;
                        }
                    }
                    //if a new word is found
                    if(new){
                        words[word_count] = malloc(1+strlen(word_temp));
                        counts[word_count] = malloc(sizeof(int));
                        strcpy(words[word_count], word_temp);
                        *counts[word_count] = 1;
                        word_count += 1;
                        total_word_count += 1;
                        //printf("Word count plus 1\n");
                    }
                }
                new = 1;
                
                //extend array
                if(word_count == size){
                    size += 32;
                    
                    printf("Re-allocated parallel arrays to be size %d.\n", size);
                    words = realloc(words, size * sizeof(char*));
                    counts = realloc(counts, size * sizeof(int*));
                    
                }
            }
        }
    //}
    //free(text);
    printf("All done (successfully read %d words; %d unique words).\n", total_word_count, word_count);
    printf("All words (and corresponding counts) are:\n");
    _Bool format_all = 1;
    int argument = 0;
    if(argc == 3){
        argument = atoi(argv[2]);
    }
    if(argc == 3){
        format_all = 0;
        //printf("The second argument is %d\n", something);
        if((argument >= total_word_count) || (argument*2 >= total_word_count)){
            format_all = 1;
        }
    }
    if(format_all){
        for(int gg = 0; gg < word_count; gg++){
            printf("%s -- %d\n", words[gg], *counts[gg]);
        }
    }
    else{
        printf("First %d words (and corresponding counts) are:\n", argument);
        for(int gg = 0; gg < argument; gg++){
            printf("%s -- %d\n", words[gg], *counts[gg]);
        }
        printf("Last %d words (and corresponding counts) are:\n", argument);
        for(int gg = word_count - argument; gg < word_count; gg++){
            printf("%s -- %d\n", words[gg], *counts[gg]);
        }
    }
    for(int gg = 0; gg < word_count; gg++){
        free(words[gg]);
    }
    free(words);
    for(int gg = 0; gg < word_count; gg++){
        free(counts[gg]);
    }
    free(counts);
    free(word_temp);
    free(dirn);
    free(dir);
   // }
        //word
    //}
        //if(fscanf(text, "%s", words[word_count])!=EOF){
            
    //}
    //printf("yay\n");
    
    /*
    fscanf(text,"%s",str);
    printf("%s\n", str);
    */
     //free the array that saves file names
    /*
    for(int a = 0; a < index - 2; a++){
        free(file_names[a]);
    }
    */
    free(file_names);
    
    
}

