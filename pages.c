#include "page.h"
#include <stddef.h>

// Array representing all physical pages
#define NUM_PAGES 128  // 128 pages of 2MB each, covering 256MB of memory
#define PAGE_SIZE (2 * 1024 * 1024)  // 2MB per page
struct ppage physical_page_array[NUM_PAGES];

// Head of the free physical pages list, defined in page.h
struct ppage *release_physical_pages = NULL;


/**
 * Initial Free List (free_physical_pages)
    At the start, all 128 physical pages are in a linked list.
    The init_pfa_list() function will initialize this list by iterating over physical_page_array
    and linking each page into the free list.
 */

void init_pfa_list(void) {
    release_physical_pages = &physical_page_array[0];

    for (int i = 0; i < NUM_PAGES; i++) {
        // Finding the phyiscal address of the page
        physical_page_array[i].physical_addr = (void *)((unsigned long)i * PAGE_SIZE);


        // Setting the first pointer and next pointer
        if (i == 0) {
            physical_page_array[i].prev = NULL;
        } else {
            physical_page_array[i].prev = &physical_page_array[i - 1];
        }

        if (i == NUM_PAGES - 1) {
            physical_page_array[i].next = NULL;
        } else {
            physical_page_array[i].next = &physical_page_array[i + 1];
        }
    }
}

/**
 * Allocating Page Frames
 * Allocates a contiguous list of npages physical pages from the free list.
 * If there are not enough pages available, returns NULL.
 * The function removes the first npages from free_physical_pages and returns
 * a pointer to the head of the newly allocated list.
 */
struct ppage *allocate_physical_pages(unsigned int npages) {
    if (npages == 0) {
        return NULL;
    }

    // Check if there are at least npages free.
    struct ppage *curr = free_physical_pages;
    unsigned int count = 0;
    while (curr && count < npages) {
        curr = curr->next;
        count++;
    }
    if (count < npages) {
        // Not enough free pages available, terminates the function.
        return NULL;
    }

    // Remove the first npages from the free list.
    struct ppage *allocd_list = free_physical_pages;
    struct ppage *tail_alloc = allocd_list;

    // Finding the tail of the allocated list.
    for (unsigned int i = 1; i < npages; i++) {
        tail_alloc = tail_alloc->next;
    }

    // Updating the free list to know how many pages are left.
    release_physical_pages = tail_alloc->next;
    if (release_physical_pages != NULL) {
        release_physical_pages->prev = NULL;
    }

    // Detach the allocated block from the free list.
    tail_alloc->next = NULL;

    return allocd_list;
}

/**
 * Freeing Page Frames
 * Walks through the list of pages to be freed
 * and appends them to the tail of the free list.
 * The function updates the prev and next pointers of the free list accordingly,
 * this is done recursively by finding the tail of the list to be freed.
 */
void free_physical_pages(struct ppage *ppage_list) {
    if (ppage_list == NULL) {
        return;
    }

    // Find the tail of the list to be freed.
    struct ppage *tail = ppage_list;
    while (tail->next != NULL) {
        tail = tail->next;
    }

    // Finding the next pointer to be part of the free list
    tail->next = release_physical_pages;
    if (release_physical_pages != NULL) {
        release_physical_pages->prev = tail;
    }

    // The freed list now becomes the head of the free list.
    release_physical_pages = ppage_list;
}
