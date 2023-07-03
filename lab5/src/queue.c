#include "queue.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"
#include "sort.h"
#include "tools.h"

Queue *create_queue(int capacity)
{
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  check_null_pointer(queue);

  queue->front = queue->rear = -1;
  queue->capacity = capacity;
  queue->array = (TreeNode **)malloc(queue->capacity * sizeof(TreeNode *));

  return queue;
}

bool is_queue_size_one(Queue *queue)
{
  return queue->front == queue->rear && queue->front != -1;
}

bool is_queue_empty(Queue *queue)
{
  return queue->front == -1;
}

bool is_queue_full(Queue *queue)
{
  return queue->rear == queue->capacity - 1;
}

void enqueue(Queue *queue, TreeNode *item)
{
  assert(!is_queue_full(queue));

  queue->array[++queue->rear] = item;
  if (queue->front == -1) {
    ++queue->front;
  }
}

TreeNode *dequeue(Queue *queue)
{
  if (is_queue_empty(queue)) {
    return NULL;
  }

  TreeNode *temp = queue->array[queue->front];
  if (queue->front == queue->rear) {
    queue->front = queue->rear = -1;
  } else {
    ++queue->front;
  }

  return temp;
}

void reverse_queue(Queue *queue)
{
  if (queue->front == -1) {
    return;
  }

  TreeNode *item = dequeue(queue);
  reverse_queue(queue);
  enqueue(queue, item);
}

TreeNode *get_queue_front(Queue *queue)
{
  if (is_queue_empty(queue)) {
    return NULL;
  }

  return queue->array[queue->front];
}

TreeNode *find_queue_min(Queue *firstQueue, Queue *secondQueue)
{
  if (is_queue_empty(firstQueue)) {
    return dequeue(secondQueue);
  }
  if (is_queue_empty(secondQueue)) {
    return dequeue(firstQueue);
  }

  if (get_queue_front(firstQueue)->freq < get_queue_front(secondQueue)->freq) {
    return dequeue(firstQueue);
  }
  if (get_queue_front(firstQueue)->freq ==
      get_queue_front(secondQueue)->freq) {
    return dequeue(firstQueue);
  }
  return dequeue(secondQueue);
}

#ifdef DEBUG
void display_queue(Queue *queue)
{
  if (queue->rear == -1) {
    puts("Queue is empty");
    return;
  }

  for (int i = queue->front; i <= queue->rear; i++) {
    printf("%d ", queue->array[i]->character);
  }

  printf("\n");
}
#endif

void destroy_queue(Queue *queue)
{
  free(queue->array);
  free(queue);
}

void sort_queue(Queue *queue, size_t size, bool order)
{
  radix_sort_nodes(queue->array, size, order);
}

TreeNode *make_new_queue_node(char character, int freq)
{
  TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
  check_null_pointer(temp);

  temp->left = temp->right = NULL;
  temp->character = character;
  temp->freq = freq;

  return temp;
}
