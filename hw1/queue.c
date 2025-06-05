/* 
 * C 编程技能测试实验代码
 * 为课程预检验
 */

/*
 * 程序实现一个同时支持 FIFO 和 LIFO 的队列
 *
 * 使用单链表表示队列元素
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  创建空队列
  如果没有分配空间则返回 NULL
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    /* 如果 malloc 返回 NULL 呢? */
    if (q == NULL) {
        return NULL; // 分配失败，返回 NULL
    }
    /* 如果 malloc 返回 NULL 呢? */
    q->head = NULL;
    q->tail = NULL; // 初始化队尾指针
    q->size = 0;    // 初始化队列大小
    return q;
}

/*
  释放队列所用到的所有空间
  如果 q 为 NULL 则没有效果
*/
void q_free(queue_t *q)
{
    if (q == NULL) {
        return; // q 为 NULL，直接返回
    }

    /* 如何释放链表元素和字符串 */
    list_ele_t *current = q->head;
    list_ele_t *next_node;
    while (current != NULL) {
        next_node = current->next; // 保存下一个节点的指针
        if (current->value != NULL) {
            free(current->value); // 释放字符串占用的空间
        }
        free(current); // 释放链表元素本身
        current = next_node;
    }

    /* 释放队列结构 */
    free(q);
}

/*
  在队首插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false 
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* 如果 q 为 NULL你该怎么办? */
    if (q == NULL) {
        return false;
    }

    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false; // 节点空间分配失败
    }

    /* 别忘了为字符串分配空间并拷贝它 */
    // 使用 strdup 来分配并拷贝字符串，它会处理 malloc 和 strcpy
    // strdup 返回 NULL 表示分配失败
    size_t s_len = strlen(s);
    newh->value = (char *)malloc(s_len + 1); // 为字符串分配内存
    if (newh->value == NULL) {
        free(newh); // 释放之前为节点分配的空间
        return false; // 字符串空间分配或拷贝失败
    }
    strcpy(newh->value, s); // 拷贝字符串

    newh->next = q->head;
    q->head = newh;

    // 如果这是插入的第一个元素，队尾也指向它
    if (q->tail == NULL) {
        q->tail = newh;
    }
    q->size++; // 增加队列大小
    return true;
}


/*
  在队尾插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false 
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* 你需要为该函数编写完整的代码 */
    /* 记住: 函数时间复杂度为 O(1) */
    if (q == NULL) {
        return false;
    }

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (newt == NULL) {
        return false; // 节点空间分配失败
    }

    // 为字符串分配空间并拷贝
    size_t s_len = strlen(s);
    newt->value = (char *)malloc(s_len + 1); // +1 for null terminator
    if (newt->value == NULL) {
        free(newt); // 释放节点空间
        return false; // 字符串空间分配失败
    }
    strcpy(newt->value, s); // 拷贝字符串
    
    newt->next = NULL; // 队尾元素的 next 总是 NULL

    if (q->head == NULL) { // 队列为空
        q->head = newt;
        q->tail = newt;
    } else { // 队列非空
        q->tail->next = newt;
        q->tail = newt;
    }
    q->size++; // 增加队列大小
    return true;
}


/*
  移除队首元素Attempt to remove element from head of queue.
  成功则返回 true
  队列为 NULL 或者为 空 则返回 false 
  如果 sp 非空 并移除了一个元素，则将移除的字符串拷贝给 *sp
  (至多 bufsize-1 个字符, 加上1个 null 结束符（'\0',0）)
  元素和字符串使用的空间需要释放
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* 你需要修复这个代码 */
    if (q == NULL || q->head == NULL) { // 队列为 NULL 或为空
        return false;
    }

    list_ele_t *oldh = q->head; // 指向要移除的头部元素

    if (sp != NULL && bufsize > 0) {
        // 拷贝字符串到 sp
        strncpy(sp, oldh->value, bufsize - 1);
        sp[bufsize - 1] = '\0'; // 确保 null 结尾
    }

    q->head = oldh->next; // 更新队首指针

    // 如果移除后队列为空，也要更新队尾指针
    if (q->head == NULL) {
        q->tail = NULL;
    }

    // 释放被移除元素的字符串和元素本身
    if (oldh->value != NULL) {
        free(oldh->value);
    }
    free(oldh);
    
    q->size--; // 减小队列大小
    return true;
}

/*
  返回队列的元素数量
  如果队列为 NULL 或者为 空 则返回 0
 */
int q_size(queue_t *q)
{
    /* 你需要为这个函数编写代码 */
    /* 记住: 函数时间复杂度为 O(1) */
    if (q == NULL) {
        return 0;
    }
    return q->size;
}

/*
  反转队列元素
  如果 q 为 NULL 则没有效果
  该函数不能直接或者间接调用其他函数为队列元素分配任何额外的空间
  (例如, 不能调用 q_insert_head, q_insert_tail, 或者 q_remove_head).
  应该是通过重排现有元素实现
 */
void q_reverse(queue_t *q)
{
    /* 你需要为这个函数编写代码 */
    if (q == NULL || q->head == NULL || q->head->next == NULL) {
        // 队列为 NULL，或为空，或只有一个元素，则无需反转
        return;
    }

    list_ele_t *prev = NULL;
    list_ele_t *current = q->head;
    list_ele_t *next_node = NULL;
    
    // 原来的头结点将成为新的尾结点
    q->tail = q->head; 

    while (current != NULL) {
        next_node = current->next; // 保存下一个节点
        current->next = prev;      // 反转当前节点的指针
        prev = current;            // prev 前进
        current = next_node;       // current 前进
    }

    // prev 现在是新的头结点
    q->head = prev;
    
    // 确保新的尾结点的 next 指针为 NULL (虽然在循环中它已经被设为 NULL 了，但明确一下更好)
    // q->tail->next 应该已经是 NULL 了，因为它是原来的头结点，在反转过程中，
    // 当 current 指向它时，current->next 会被设为 prev (此时 prev 是 NULL)
    // 或者当它是最后一个被处理的节点时，它的 next 会指向它之前的节点。
    // 实际上，q->tail->next = NULL; 这一句是必须的，因为原来的头结点在反转后成为尾结点，
    // 它的 next 应该指向 NULL。
    if (q->tail != NULL) { // 确保尾部不是NULL (虽然前面检查了 q->head->next != NULL)
        q->tail->next = NULL;
    }
}

