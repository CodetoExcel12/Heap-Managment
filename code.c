#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HEAP_SIZE 10000

char heap[HEAP_SIZE];

#include <stdbool.h>

typedef struct meta_block
{
    int free_memory;
    bool free;
    struct meta_block *next;

} meta_data;

meta_data *hdr = (void *)heap;

void Initializie()
{
    hdr->free_memory = HEAP_SIZE - sizeof(struct meta_block);
    hdr->free = true;
    hdr->next = NULL;
}

void *alloc(int req_memory)
{
    meta_data *curr;
    void *result;
    if ((hdr->free_memory) == 0)
    {
        Initializie();
    }
    curr = hdr;
    while (((curr->free_memory < req_memory) || (curr->free == false)) && (curr->next != NULL))
    {
        curr = curr->next;
    }
    if (curr->free_memory == req_memory)
    {
        curr->free = false;
        result = (void *)(++curr);
    }
    else if (curr->free_memory > (req_memory + sizeof(meta_data)))
    {
        meta_data *Next = (void *)(((void *)(curr) + req_memory + sizeof(meta_data)));
        Next->free_memory = curr->free_memory - req_memory - sizeof(meta_data);
        Next->free = true;
        Next->next = curr->next;
        curr->free_memory = req_memory;
        curr->free = false;
        curr->next = Next;
        result = (void *)(++curr);
    }
    else if (curr->free_memory > req_memory)
    {
        curr->free = false;
        result = (void *)(++curr);
    }
    else
    {
        result = NULL;
        printf("Out of memory\n");
    }
    return result;
}

void merge()
{
    meta_data *curr, *Next;
    curr = hdr;
    Next = curr->next;
    while (Next != NULL)
    {
        if (curr->free && Next->free)
        {
            curr->free_memory = curr->free_memory + Next->free_memory + sizeof(meta_data);
            curr->next = Next->next;
        }
        else
        {
            curr = curr->next;
        }
        Next = curr->next;
    }
}

void freee(void *ptr)
{
    if (((void *)heap <= ptr) && (ptr <= (void *)(heap + HEAP_SIZE)))
    {
        meta_data *curr = ptr;
        --curr;
        curr->free = true;
        merge();
    }
    else
    {
        printf("freee called on unallocated memory\n");
    }
}

void showHeap()
{
    meta_data *ptr;
    ptr = hdr;
    while (ptr != NULL)
    {
        if (ptr->free == false)
        {
            printf("%12s\n", "ALLOCATED");
            printf("----------------------\n");
            printf("%10s %7d\n", "Size: ", ptr->free_memory);
            printf("----------------------\n");
        }
        else
        {
            printf("%12s\n", "FREE");
            printf("----------------------\n");
            printf("%10s %7d\n", "Size: ", ptr->free_memory);
            printf("----------------------\n");
        }
        ptr = ptr->next;
    }
    printf("\n\n");
}

int main()
{
    int num_ints, num_floats, num_doubles;

    printf("Enter  size(int) values to allocate: ");
    scanf("%d", &num_ints);

    int *p = alloc(sizeof(int) * num_ints);
    if (p != NULL)
    {
        printf("Enter %d integer values: ", num_ints);
        for (int i = 0; i < num_ints; i++)
        {
            scanf("%d", &p[i]);
        }
    }
    showHeap();

    printf("Enter size(float) values to allocate: ");
    scanf("%d", &num_floats);

    float *q = alloc(sizeof(float) * num_floats);
    if (q != NULL)
    {
        printf("Enter %d float values: ", num_floats);
        for (int i = 0; i < num_floats; i++)
        {
            scanf("%f", &q[i]);
        }
    }
    showHeap();

    freee(p);
    showHeap();

    printf("Enter size(double) values to allocate: ");
    scanf("%d", &num_doubles);

    double *r = alloc(sizeof(double) * num_doubles);
    if (r != NULL)
    {
        printf("Enter %d double values: ", num_doubles);
        for (int i = 0; i < num_doubles; i++)
        {
            scanf("%lf", &r[i]);
        }
    }
    showHeap();

    freee(q);
    showHeap();

    freee(r);
    showHeap();

    printf("Size of metadata block: %d\n", (int)sizeof(meta_data));

    return 0;
}