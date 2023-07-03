#ifndef QUEUE_H_
#define QUEUE_H_

#include "main.h"

typedef struct Queue Queue;
struct Queue {
  int front, rear;
  int capacity;
  TreeNode **array;
};

Queue *create_queue(int capacity);
bool is_queue_size_one(Queue *queue);
bool is_queue_empty(Queue *queue);
bool is_queue_full(Queue *queue);
void enqueue(Queue *queue, TreeNode *item);
TreeNode *dequeue(Queue *queue);
void reverse_queue(Queue *queue);
TreeNode *get_queue_front(Queue *queue);
TreeNode *find_queue_min(Queue *firstQueue, Queue *secondQueue);
void destroy_queue(Queue *queue);
void sort_queue(Queue *queue, size_t size, bool order);
TreeNode *make_new_queue_node(char character, int freq);

#if defined(DEBUG)
void display_queue(Queue *queue);
#endif

#endif  // QUEUE_H_
