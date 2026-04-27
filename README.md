# ll

A small doubly-linked list in C, distributed as source you drop into your project.

## Using it in your project

1. Copy `src/ll.h` and `src/ll.c` into your project (or add this repo as a git submodule and point your build at `src/`).
2. Include the header where you need a list:
   ```c
   #include "ll.h"
   ```
3. Compile `ll.c` alongside your other sources.

There is no build system, no static/shared library to link, and no install step — the two files are the library.

## Configuring the value type

By default the list stores `int`. To store a different type, edit the `ValueType` typedef in `ll.h`:

```c
typedef int ValueType;   // change this line
```

That's the only line you should need to change. If you later pull in a newer version of the library from upstream, you'll need to re-apply this edit (or resolve a one-line merge conflict).

A single build can only hold one kind of list at a time. If you need lists of multiple types in the same program, this library isn't the right fit.

## Example

```c
#include <stdio.h>
#include "ll.h"

static int compareInt(ValueType a, ValueType b) { return a - b; }
static void printInt(ValueType v) { printf("%d\n", v); }

int main(void) {
    LinkedListPtr list = llCreateLinkedList();

    llAppend(list, 1);
    llAppend(list, 2);
    llAppend(list, 3);

    llIterate(list, printInt);

    Node *found = llContains(list, 2, compareInt);
    if (found) llRemove(list, found);

    printf("count: %u\n", llCount(list));

    llFreeLinkedList(list);
    return 0;
}
```

## API

See `ll.h` for the full set of functions and their preconditions. Briefly:

- `llCreateLinkedList` / `llFreeLinkedList` — lifecycle.
- `llAppend` / `llRemove` — add and remove nodes.
- `llContains` — find a node by value, using a caller-supplied compare function.
- `llIterate` — call a function for each value in order.
- `llCount` — number of values in the list.
- `llNodeGetValue` / `llNodeSetValue` — read or update the value held by a node.

All functions assume non-NULL list and node pointers, and that any node passed alongside a list belongs to that list. Passing invalid pointers is undefined behavior.
