#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#define MAX_BUCKETS 100
#define MULTIPLIER 31

#define LENGTH 30
typedef struct User_login{
   char name[LENGTH];  
   int totalcount;
}ELementType;                 //链表的节点

typedef struct SListNode
{
 ELementType data;
 struct SListNode* next;
}Node, *PNode, *List;              //封装链表节点和next指针


static PNode table[MAX_BUCKETS];

static unsigned long hashstring(const char *str);
static void cleartable();
static PNode walloc(const char *str);
static PNode lookup(const char *str);
static PNode find(PNode wp , const char *str);

/*创建一个节点*/
static PNode walloc(const char *str)
{
    PNode p = (PNode)malloc(sizeof(Node));
    if (p != NULL) {
        strcpy(p->data.name, str);
        p->data.totalcount = 0;
        p->next = NULL;
    }
    return p;
}

/*计算哈希值*/
static unsigned long hashstring(const char *str)
{
    unsigned long hash = 0;
    // Using the polynomial rolling hash algorithm: h(s) = (s[0]*B^(n-1) + ... + s[n-1]) % M
    // This can be computed iteratively as: h = (h * B + c) % M
    for (; *str; str++) {
        hash = hash * MULTIPLIER + *str;
    }
    return hash % MAX_BUCKETS;
}

/*在一个链表中查找人名，找到返回指针，找不到返回NULL*/
static PNode find(PNode wp , const char *str)
{
    PNode p = wp;
    while (p != NULL) {
        if (strcmp(p->data.name, str) == 0) {
            return p; // Found the node
        }
        p = p->next;
    }
    return NULL; // Node not found in the list
}

/*将在散列表中查找相应节点，并进行相应操作，找到返回指针，没找到则创建节点并加入散列表,并返回指针*/
static PNode lookup(const char *str)
{
    unsigned long hash_val = hashstring(str);
    PNode wp = find(table[hash_val], str);

    if (wp == NULL) { // If not found, create a new node
        wp = walloc(str);
        if (wp != NULL) {
            // Insert the new node at the beginning of the list (bucket)
            wp->next = table[hash_val];
            table[hash_val] = wp;
        }
    }
    return wp; // Return the found or newly created node
}

/*删除散列表*/
static void cleartable()
{
    PNode wp = NULL, p = NULL;
    int i = 0;

    for (i = 0; i < MAX_BUCKETS; i++) {
        wp = table[i];
        while (wp) {
            p = wp;
            wp = wp->next;
            free(p);
        }
        table[i] = NULL; // Ensure the bucket pointer is cleared
    }
}

/*读取文件，创建散列表*/
void file_read_ht()
{
    FILE *fp = fopen("user_login.txt", "r");
    if (fp == NULL) {
        perror("Error opening user_login.txt");
        return;
    }
    char word[1024];
    char *name;
    PNode wp = NULL;

    memset(table, 0, sizeof(table));

    int count = 0;
    while (1) {
        if (fscanf(fp, "%s", word) != 1)
            break;
        name = strtok(word, ",");
        if (name == NULL) continue; // Skip if strtok fails
        ////begin
        //加入散列表，2条语句实现
        wp = lookup(name);
        if (wp) wp->data.totalcount++;
        ////end
 
        count++;
    }
    printf("Total records processed: %d \n", count);
    fclose(fp);
}

/*将散列表写入文件*/
void file_write_ht()
{
    FILE *fp;
    int count = 0;

    if((fp=fopen("output.txt","wt")) == NULL ) {
        printf("Fail to open file!\n");    
        exit(0);  
    }

    ////begin
    PNode wp;
    int i;
    for (i = 0; i < MAX_BUCKETS; i++) {
        wp = table[i];
        while (wp != NULL) {
            fprintf(fp, "%s,%d\n", wp->data.name, wp->data.totalcount);
            count++;
            wp = wp->next;
        }
    }
    fclose(fp);
    ////end
    printf("Total unique users written to file: %d\n", count);   
}

/*搜索功能*/
void search_ht()
{
    char name[LENGTH]; 
    printf("Enter name, 'q' to exit:\n");
    scanf("%s", name);    
     
    while (strcmp(name, "q") != 0) {
        unsigned long hash = hashstring(name);
        PNode wp = table[hash];
        PNode curr = NULL;

        ////begin
        curr = find(wp, name);
        if (curr != NULL) {
            printf("Found! Name: %s, Login Count: %d\n", curr->data.name, curr->data.totalcount);
        } else {
            printf("User '%s' not found.\n", name);
        }
        ////end

        printf("\nEnter name, 'q' to exit:\n");
        scanf("%s", name);
    }    

    cleartable();     
}