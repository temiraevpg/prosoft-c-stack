#include "cstack.h"
#include <stddef.h>

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define DSTACK  20  /* Количество обслуживаемых стеков */

struct node {
    struct node* pred;
    unsigned int size;
    char data[0];
};

struct node* d_stack[DSTACK] = { NULL };

hstack_t stack_new(void)
{
    for (int i = 0; i < DSTACK; i++) {
        if (d_stack[i] == NULL) {
            struct node* p = malloc(sizeof(struct node));
            if(p == NULL) return -1;
            p->pred = NULL;
            d_stack[i] = p; 
            return i;
        }
    }
    return -1;
}

/**
* @brief Удалить стек, если соответствующий хэндлеру стек существует.
* @param[in] hstack хэндлер стека
*/
void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack)) return;
    struct node* p = d_stack[hstack];
    while (p->pred != NULL) {
        d_stack[hstack] = p->pred;
        free(p);
        p = d_stack[hstack];
    }
    free(p);
    d_stack[hstack] = NULL;
}

/**
* @brief Проверить хэндлер
* @param[in] hstack хэндлер стека
* @returns 0 - соответствующий хэндлеру стек существует, 1 - нет.
*/
int stack_valid_handler(const hstack_t hstack)
{
    if(d_stack[hstack] != NULL) return 0;
    else return 1;
}

/**
* @brief Получить количество элементов в стеке, если соответствующий хэндлеру стек существует.
* @param[in] hstack  хэндлер стека.
* @return количество элементов в стеке, если соответствующий хэндлеру стек существует, или 0 в противном случае.
*/
unsigned int stack_size(const hstack_t hstack)
{
    unsigned int size = 0;
    if (stack_valid_handler(hstack)) return size;
    struct node* p = d_stack[hstack];
    while (p->pred != NULL) {
        size++;
        p = p->pred;
    }
    return size;
}

/**
* @brief Добавить элемент данных из буфера в стек, если соответствующий хэндлеру стек существует.
* @param[in] hstack  хэндлер стека
* @param[in] data_in указатель на буфер с данными
* @param[in] size размер буфера с данными
*/
void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(stack_valid_handler(hstack)) return;
    if(data_in == NULL) return;
    if(size == 0) return;

    struct node* p = malloc((sizeof(struct node) + sizeof(char)*size));
    if(p == NULL) return;

    p->pred = d_stack[hstack];
    d_stack[hstack] = p;
    p->size = size;
    memcpy(&p->data, data_in, size);
}

/**
* @brief Извлечь элемент из стека и записать данные этого элемента в буфер, если соответствующий хэндлеру стек существует.
* @param[in] stack хэндлер стека
* @param[out] data_out указатель на буфер для записи данных
* @param[in] size размер буфера
* @return размер записанных данных в байтах, если соответствующий хэндлеру стек существует, или 0 в противном случае
*/
unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    unsigned int len = 0;
    if (stack_valid_handler(hstack)) return len;
    if (data_out == NULL) return len;
    if (size == 0) return len;

    struct node* p = d_stack[hstack];
    if (p->pred == NULL) return len;
    if(p->size != size) return len;
    len = p->size;

    d_stack[hstack] = p->pred;
    /* изначально было так, можно запросить из стека или сколько надо или только сколько есть */
    //len = p->size >= size ? size : p->size;
    memcpy(data_out, &p->data, len);
    free(p);
    return len;
}
