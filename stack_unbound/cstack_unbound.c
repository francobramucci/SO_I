#include "stack_unbound.h"

typedef struct _CStack{
    struct StackNode* root;
    pthread_mutex_t mutex; 
} *CStack;

CStack CScrear(){
    CStack stack = malloc(sizeof(struct _CStack));
    stack->mutex = PTHREAD_MUTEX_INITIALIZER;
    stack->root = NULL;
}

int CSisEmpty(CStack s){
    phtread_mutex_lock(&s->mutex);
    int flag = !s->root;
    pthread_mutex_unlock(&s->mutex);
    return flag;
}

void CSpush(CStack s, int data){
    phtread_mutex_lock(&s->mutex);
    push(&s->root, data);
    pthread_mutex_unlock(&s->mutex);
}

int CSpop(CStack s){
    phtread_mutex_lock(&s->mutex);
    int flag = pop(&s->root);
    pthread_mutex_unlock(&s->mutex);
    return flag;
}
 
int CStop(CStack s){
    phtread_mutex_lock(&s->mutex);
    int dato = top(s->root);
    pthread_mutex_unlock(&s->mutex);
    return dato;
}

void CSstackFree(CStack s){
    pthread_mutex_lock(&s->mutex);
    stackFree(s->root);
    pthread_mutex_unlock(&s->mutex);
}

