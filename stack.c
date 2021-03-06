/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2011 Alberto Todde <alberto.tdd@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */


#include "stack.h"
#include <stdlib.h>



struct stack_t {
    Node *top;          // Pointer to the Top of the stack
    size_t max_size;    // Max nodes allowed (0 means INF)
    size_t size;        // Current size of the stack
};


Stack* stack_new(size_t max_size) {
    Stack *stack = malloc(sizeof *stack);
    if (stack != 0) {
        stack->top = 0;
        stack->size = 0;
        stack->max_size = max_size;
    }
    return stack;
}


void stack_delete(Stack *stack) {
    if (stack != 0) {
        Node *ptr = stack->top;
        Node *last;
        while (ptr != 0) {
            last = ptr;
            ptr = node_get_next(ptr);
            node_delete(last);
        }
        free(stack);
    }
}


size_t stack_size(const Stack *stack) {
    return stack != 0 ? stack->size : 0;
}


bool stack_isempty(const Stack *stack) {
    return stack != 0 ? stack->size == 0 : false;
}


bool stack_isfull(const Stack *stack) {
    if (stack != 0) {
        return stack->max_size != INF ? stack->size == stack->max_size : false;
    }
    return true;
}


bool stack_push(Stack *stack, const Article *article) {
    if (stack != 0) {
        if (!stack_isfull(stack)) {
            Node *node = node_new(article);
            if (node != 0) {
                if (stack_isempty(stack)) {
                    stack->top = node;
                } else {
                    node_set_next(node, stack->top);
                    stack->top = node;
                }
                stack->size++;
                return true;
            }
        }
    }
    return false;
}


Article* stack_pop(Stack *stack) {
    if (!stack_isempty(stack)) {
        Article *ptr = node_get_ptr(stack->top);
        Node *old_top = stack->top;
        stack->top = node_get_next(stack->top);
        node_remove(old_top);   // Don't delete article in the node.
        stack->size--;
        return ptr;
    }
    return 0;
}


const Article* stack_peek(const Stack *stack) {
    if (!stack_isempty(stack)) {
        const Article *const_ptr = node_get_const_ptr(stack->top);
        return const_ptr;
    }
    return 0;
}


void stack_iterator_const(const Stack *stack, const_iter_action action) {
    if (stack != 0 && action != 0) {
        Node *ptr = stack->top;
        while (ptr != 0) {
            action(node_get_const_ptr(ptr));
            ptr = node_get_next(ptr);
        }
    }
}


void stack_iterator(Stack *stack, iter_action action) {
    if (stack != 0 && action != 0) {
        Node *ptr = stack->top;
        while (ptr != 0) {
            action(node_get_ptr(ptr));
            ptr = node_get_next(ptr);
        }
    }
}
