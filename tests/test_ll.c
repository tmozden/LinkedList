#include <stdio.h>
#include <string.h>
#include "ll.h"

static int g_failures = 0;
static int g_current_test_failed = 0;

#define ASSERT_EQ(actual, expected) do { \
    long long _a = (long long)(actual); \
    long long _e = (long long)(expected); \
    if (_a != _e) { \
        printf("    ASSERT_EQ failed at %s:%d: expected %lld, got %lld\n", \
               __FILE__, __LINE__, _e, _a); \
        g_current_test_failed = 1; \
    } \
} while (0)

#define ASSERT_TRUE(x) do { \
    if (!(x)) { \
        printf("    ASSERT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #x); \
        g_current_test_failed = 1; \
    } \
} while (0)

#define ASSERT_NULL(p) do { \
    if ((p) != NULL) { \
        printf("    ASSERT_NULL failed at %s:%d: %s was not NULL\n", \
               __FILE__, __LINE__, #p); \
        g_current_test_failed = 1; \
    } \
} while (0)

#define ASSERT_NOT_NULL(p) do { \
    if ((p) == NULL) { \
        printf("    ASSERT_NOT_NULL failed at %s:%d: %s was NULL\n", \
               __FILE__, __LINE__, #p); \
        g_current_test_failed = 1; \
    } \
} while (0)

#define RUN(test_fn) do { \
    g_current_test_failed = 0; \
    printf("  %s ... ", #test_fn); \
    test_fn(); \
    if (g_current_test_failed) { \
        printf("FAIL\n"); \
        g_failures++; \
    } else { \
        printf("PASS\n"); \
    } \
} while (0)

static int cmp_int(ValueType a, ValueType b) { return a - b; }

static ValueType g_iter_buf[16];
static int g_iter_n;
static void capture(ValueType v) {
    if (g_iter_n < (int)(sizeof(g_iter_buf) / sizeof(g_iter_buf[0]))) {
        g_iter_buf[g_iter_n++] = v;
    }
}
static void reset_capture(void) {
    g_iter_n = 0;
    memset(g_iter_buf, 0, sizeof(g_iter_buf));
}

/* ---------- Lifecycle ---------- */

static void test_create_returns_nonnull_empty_list(void) {
    LinkedListPtr list = llCreateLinkedList();
    ASSERT_NOT_NULL(list);
    ASSERT_EQ(llCount(list), 0);
    llFreeLinkedList(list);
}

static void test_free_empty_list(void) {
    LinkedListPtr list = llCreateLinkedList();
    llFreeLinkedList(list);
    /* ASan would flag any leak or invalid free here. */
}

static void test_free_populated_list(void) {
    LinkedListPtr list = llCreateLinkedList();
    llAppend(list, 1);
    llAppend(list, 2);
    llAppend(list, 3);
    llFreeLinkedList(list);
    /* ASan confirms all three nodes plus the list struct were freed. */
}

/* ---------- Append ---------- */

static void test_append_single_sets_count_one(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *n = llAppend(list, 42);
    ASSERT_NOT_NULL(n);
    ASSERT_EQ(llCount(list), 1);
    llFreeLinkedList(list);
}

static void test_append_multiple_preserves_order(void) {
    LinkedListPtr list = llCreateLinkedList();
    llAppend(list, 1);
    llAppend(list, 2);
    llAppend(list, 3);

    reset_capture();
    llIterate(list, capture);

    ASSERT_EQ(g_iter_n, 3);
    ASSERT_EQ(g_iter_buf[0], 1);
    ASSERT_EQ(g_iter_buf[1], 2);
    ASSERT_EQ(g_iter_buf[2], 3);
    llFreeLinkedList(list);
}

static void test_append_returns_node_holding_value(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *n = llAppend(list, 7);
    ASSERT_EQ(llNodeGetValue(n), 7);
    llFreeLinkedList(list);
}

/* ---------- Remove ---------- */

static void test_remove_head(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *n1 = llAppend(list, 1);
    llAppend(list, 2);
    llAppend(list, 3);

    ValueType removed = llRemove(list, n1);

    ASSERT_EQ(removed, 1);
    ASSERT_EQ(llCount(list), 2);

    reset_capture();
    llIterate(list, capture);
    ASSERT_EQ(g_iter_n, 2);
    ASSERT_EQ(g_iter_buf[0], 2);
    ASSERT_EQ(g_iter_buf[1], 3);
    llFreeLinkedList(list);
}

static void test_remove_tail(void) {
    LinkedListPtr list = llCreateLinkedList();
    llAppend(list, 1);
    llAppend(list, 2);
    Node *n3 = llAppend(list, 3);

    ValueType removed = llRemove(list, n3);

    ASSERT_EQ(removed, 3);
    ASSERT_EQ(llCount(list), 2);

    reset_capture();
    llIterate(list, capture);
    ASSERT_EQ(g_iter_n, 2);
    ASSERT_EQ(g_iter_buf[0], 1);
    ASSERT_EQ(g_iter_buf[1], 2);
    llFreeLinkedList(list);
}

static void test_remove_middle(void) {
    LinkedListPtr list = llCreateLinkedList();
    llAppend(list, 1);
    Node *n2 = llAppend(list, 2);
    llAppend(list, 3);

    ValueType removed = llRemove(list, n2);

    ASSERT_EQ(removed, 2);
    ASSERT_EQ(llCount(list), 2);

    reset_capture();
    llIterate(list, capture);
    ASSERT_EQ(g_iter_n, 2);
    ASSERT_EQ(g_iter_buf[0], 1);
    ASSERT_EQ(g_iter_buf[1], 3);
    llFreeLinkedList(list);
}

static void test_remove_only_node(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *n = llAppend(list, 99);

    ValueType removed = llRemove(list, n);

    ASSERT_EQ(removed, 99);
    ASSERT_EQ(llCount(list), 0);

    reset_capture();
    llIterate(list, capture);
    ASSERT_EQ(g_iter_n, 0);
    llFreeLinkedList(list);
}

static void test_remove_then_append(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *n1 = llAppend(list, 1);
    llRemove(list, n1);
    llAppend(list, 2);
    llAppend(list, 3);

    ASSERT_EQ(llCount(list), 2);

    reset_capture();
    llIterate(list, capture);
    ASSERT_EQ(g_iter_n, 2);
    ASSERT_EQ(g_iter_buf[0], 2);
    ASSERT_EQ(g_iter_buf[1], 3);
    llFreeLinkedList(list);
}

/* ---------- Contains ---------- */

static void test_contains_finds_existing(void) {
    LinkedListPtr list = llCreateLinkedList();
    llAppend(list, 10);
    Node *n2 = llAppend(list, 20);
    llAppend(list, 30);

    Node *found = llContains(list, 20, cmp_int);
    ASSERT_NOT_NULL(found);
    ASSERT_TRUE(found == n2);
    ASSERT_EQ(llNodeGetValue(found), 20);
    llFreeLinkedList(list);
}

static void test_contains_returns_null_when_absent(void) {
    LinkedListPtr list = llCreateLinkedList();
    llAppend(list, 1);
    llAppend(list, 2);
    llAppend(list, 3);

    Node *found = llContains(list, 99, cmp_int);
    ASSERT_NULL(found);
    llFreeLinkedList(list);
}

static void test_contains_on_empty_returns_null(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *found = llContains(list, 1, cmp_int);
    ASSERT_NULL(found);
    llFreeLinkedList(list);
}

static void test_contains_returns_first_match(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *first = llAppend(list, 5);
    llAppend(list, 5);
    llAppend(list, 5);

    Node *found = llContains(list, 5, cmp_int);
    ASSERT_TRUE(found == first);
    llFreeLinkedList(list);
}

/* ---------- Iterate ---------- */

static void test_iterate_empty_list_no_calls(void) {
    LinkedListPtr list = llCreateLinkedList();
    reset_capture();
    llIterate(list, capture);
    ASSERT_EQ(g_iter_n, 0);
    llFreeLinkedList(list);
}

static void test_iterate_visits_all_in_order(void) {
    LinkedListPtr list = llCreateLinkedList();
    llAppend(list, 1);
    llAppend(list, 2);
    llAppend(list, 3);

    reset_capture();
    llIterate(list, capture);
    ASSERT_EQ(g_iter_n, 3);
    ASSERT_EQ(g_iter_buf[0], 1);
    ASSERT_EQ(g_iter_buf[1], 2);
    ASSERT_EQ(g_iter_buf[2], 3);
    llFreeLinkedList(list);
}

/* ---------- Count ---------- */

static void test_count_tracks_appends_and_removes(void) {
    LinkedListPtr list = llCreateLinkedList();
    ASSERT_EQ(llCount(list), 0);

    llAppend(list, 1);
    Node *n2 = llAppend(list, 2);
    llAppend(list, 3);
    ASSERT_EQ(llCount(list), 3);

    llRemove(list, n2);
    ASSERT_EQ(llCount(list), 2);

    llFreeLinkedList(list);
}

/* ---------- Node accessors ---------- */

static void test_node_get_returns_appended_value(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *n = llAppend(list, 1234);
    ASSERT_EQ(llNodeGetValue(n), 1234);
    llFreeLinkedList(list);
}

static void test_node_set_updates_value(void) {
    LinkedListPtr list = llCreateLinkedList();
    Node *n = llAppend(list, 1);

    llNodeSetValue(n, 99);
    ASSERT_EQ(llNodeGetValue(n), 99);

    Node *found = llContains(list, 99, cmp_int);
    ASSERT_TRUE(found == n);
    llFreeLinkedList(list);
}

/* ---------- Runner ---------- */

int main(void) {
    printf("Running ll tests:\n");

    RUN(test_create_returns_nonnull_empty_list);
    RUN(test_free_empty_list);
    RUN(test_free_populated_list);

    RUN(test_append_single_sets_count_one);
    RUN(test_append_multiple_preserves_order);
    RUN(test_append_returns_node_holding_value);

    RUN(test_remove_head);
    RUN(test_remove_tail);
    RUN(test_remove_middle);
    RUN(test_remove_only_node);
    RUN(test_remove_then_append);

    RUN(test_contains_finds_existing);
    RUN(test_contains_returns_null_when_absent);
    RUN(test_contains_on_empty_returns_null);
    RUN(test_contains_returns_first_match);

    RUN(test_iterate_empty_list_no_calls);
    RUN(test_iterate_visits_all_in_order);

    RUN(test_count_tracks_appends_and_removes);

    RUN(test_node_get_returns_appended_value);
    RUN(test_node_set_updates_value);

    printf("\n%d failure(s)\n", g_failures);
    return g_failures != 0;
}
