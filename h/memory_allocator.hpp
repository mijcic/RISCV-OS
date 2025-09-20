//
// Created by os on 7/20/24.
//

#ifndef PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
public:
    struct Node {
        size_t size;
        Node* next = nullptr;
    };

    static void initMemoryAllocator();
    static void* mem_alloc(size_t size);
    static int mem_free(void*);
    static int tryToJoin(Node* cur);
private:
    MemoryAllocator() {}
    static Node* fmem_head;
};

#endif //PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_HPP
