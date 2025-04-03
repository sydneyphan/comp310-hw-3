#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>

// Structure to represent a physical page
struct ppage {
    struct ppage *next;  // Pointer to next page in the list
    struct ppage *prev;  // Previous Page
    void *physical_addr; // Physical address of this page
};

// Global pointer to the free physical pages list
// name change to avoid confusion from free_physical_pages
extern struct ppage *release_physical_pages;


void init_pfa_list(void); // Initialize the free physical pages list
struct ppage *allocate_physical_pages(unsigned int npages); // Allocate a number of physical pages
extern void free_physical_pages(struct ppage *ppage_list); // Free a list of physical pages


#endif // PAGE_H
