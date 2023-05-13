#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_BOOK 0x100
#define MAX_MALLOC_SIZE 0x1000

char *books[MAX_BOOK];
pthread_rwlock_t rwlock_books_and_sizes[MAX_BOOK];

int book_cnt = 0;
pthread_mutex_t mutex_book_cnt;

typedef struct {
    unsigned int title;
    unsigned int content;
} Size;
Size sizes[MAX_BOOK];

int size_cnt = 0;
pthread_mutex_t mutex_size_cnt;

int readint() {
    char buf[8];
    read(0, buf, 8);
    return atoi(buf);
}

void print(const char* str, int size) {
    for (int i = 0; i < size; i++)
        write(1, str + i, 1);
}

void err(const char *message) {
    write(2, "Error: ", 7);
    write(2, message, sizeof(message));
    write(2, "\n", 1);
    exit(-1);
}

void lock(pthread_mutex_t *mutex) {
    if (pthread_mutex_lock(mutex) != 0)
        err("mutex lock");
}

void unlock(pthread_mutex_t *mutex) {
    if (pthread_mutex_unlock(mutex) != 0)
        err("mutex unlock");
}

void rlock(pthread_rwlock_t *rwlock) {
    if (pthread_rwlock_rdlock(rwlock) != 0)
        err("rwlock rdlock");
}

void wlock(pthread_rwlock_t *rwlock) {
    if (pthread_rwlock_wrlock(rwlock) != 0)
        err("rwlock wrlock");
}

void rwunlock(pthread_rwlock_t *rwlock) {
    if (pthread_rwlock_unlock(rwlock) != 0)
        err("rwlock unlock");
}

void menu() {
    print("1. Create book\n", 15);
    print("2. Show book\n", 13);
    print("3. Edit title\n", 14);
    print("4. Edit content\n", 16);
    print(">>> ", 4);
}

typedef struct {
    Size size;
    char *buf;
    int idx;
} Chunk;

void logchar(char c) {
    int fd = open("log", O_RDWR | O_CREAT | O_APPEND);
    if (fd >= 0) {
        write(fd, &c, 1);
        close(fd);
    }
}

void logmsg(const char* msg, int size) {
    for (int i = 0; i < size; i++)
        logchar(msg[i]);
}

int check_str(const char *str1, unsigned int size1, const char *str2, unsigned int size2) {
    int flag = size1 != size2;
    if (!flag)
        for (int j = 0; j < size1; j++) {
            logmsg("check ", 6);
            logmsg(str1 + j, 1);
            logmsg(" and ", 5);
            logmsg(str2 + j, 1);
            if (str1[j] != str2[j])
                return 1;
        }
    return flag;
}

void* check_create(void *argv) {
    Size size = ((Chunk *)argv)->size;
    char *buf = ((Chunk *)argv)->buf;
    int i = 0;
    while (1) {
        lock(&mutex_book_cnt);
        int cnt = book_cnt;
        unlock(&mutex_book_cnt);
        if (i >= cnt)
            break;
        rlock(&rwlock_books_and_sizes[i]);
        int flag1 = check_str(buf, size.title, books[i], sizes[i].title);
        int flag2 = check_str(buf + size.title, size.content,
                              books[i] + sizes[i].title, sizes[i].content);
        rwunlock(&rwlock_books_and_sizes[i]);
        if (!flag1 || !flag2) {
            lock(&mutex_size_cnt);
            size_cnt--;
            unlock(&mutex_size_cnt);
            free(buf);
            free(argv);
            print("The title or content is not unique!\n", 36);
            return NULL;
        }
        i++;
    }
    lock(&mutex_book_cnt);
    int idx = book_cnt++;
    unlock(&mutex_book_cnt);
    wlock(&rwlock_books_and_sizes[idx]);
    books[idx] = buf;
    rwunlock(&rwlock_books_and_sizes[idx]);
    free(argv);
    return NULL;
}

void create() {
    lock(&mutex_book_cnt);
    int cnt = book_cnt;
    unlock(&mutex_book_cnt);
    if (cnt >= MAX_BOOK) {
        print("Bookshelves full!\n", 18);
        return;
    }
    print("Book title size: ", 17);
    unsigned int title_size = readint();
    print("Book content szie: ", 19);
    unsigned int content_size = readint();
    if (title_size > MAX_MALLOC_SIZE ||
        title_size <= 0 ||
        content_size > MAX_MALLOC_SIZE ||
        content_size <= 0 ||
        title_size + content_size > MAX_MALLOC_SIZE) {
        print("Invalid size(s)!\n", 17);
        return;
    }
    lock(&mutex_size_cnt);
    int idx = size_cnt++;
    unlock(&mutex_size_cnt);
    wlock(&rwlock_books_and_sizes[idx]);
    sizes[idx].title = title_size;
    sizes[idx].content = content_size;
    Size size = sizes[idx];
    rwunlock(&rwlock_books_and_sizes[idx]);
    char *buf = malloc(title_size + content_size);
    print("Book title: ", 12);
    read(0, buf, title_size);
    print("Book content: ", 14);
    read(0, buf + title_size, content_size);
    pthread_t check;
    Chunk *c = calloc(sizeof(Chunk), 1);
    c->size = size;
    c->buf = buf;
    if (pthread_create(&check, NULL, check_create, (void *)c) != 0)
        err("pthread create");
}

void show() {
    print("Index: ", 7);
    int idx = readint();
    lock(&mutex_book_cnt);
    int cnt = book_cnt;
    unlock(&mutex_book_cnt);
    if (idx < cnt) {
        rlock(&rwlock_books_and_sizes[idx]);
        print(books[idx], sizes[idx].title);
        print("\n", 1);
        print(books[idx] + sizes[idx].title, sizes[idx].content);
        print("\n", 1);
        rwunlock(&rwlock_books_and_sizes[idx]);
    }
    else
        print("Invalid index!\n", 15);
}

void* check_edit_title(void *argv) {
    Size size = ((Chunk *)argv)->size;
    char *buf = ((Chunk *)argv)->buf;
    int idx = ((Chunk *)argv)->idx;
    int i = 0;
    while (1) {
        if (i != idx) {
            lock(&mutex_book_cnt);
            int cnt = book_cnt;
            unlock(&mutex_book_cnt);
            if (i >= cnt)
                break;
            rlock(&rwlock_books_and_sizes[i]);
            int flag = check_str(buf, size.title, books[i], sizes[i].title);
            rwunlock(&rwlock_books_and_sizes[i]);
            if (!flag) {
                free(argv);
                print("The title is not unique!\n", 25);
                return NULL;
            }
        }
        i++;
    }
    wlock(&rwlock_books_and_sizes[idx]);
    memcpy(books[idx], buf, size.title);
    rwunlock(&rwlock_books_and_sizes[idx]);
    free(argv);
    return NULL;
}

void edit_title() {
    print("Index: ", 7);
    int idx = readint();
    lock(&mutex_book_cnt);
    int cnt = book_cnt;
    unlock(&mutex_book_cnt);
    if (idx < cnt) {
        rlock(&rwlock_books_and_sizes[idx]);
        Size size = sizes[idx];
        rwunlock(&rwlock_books_and_sizes[idx]);
        char *buf = malloc(size.title);
        print("Book title: ", 12);
        read(0, buf, size.title);
        pthread_t check;
        Chunk *c = calloc(sizeof(Chunk), 1);
        c->size = size;
        c->buf = buf;
        c->idx = idx;
        if (pthread_create(&check, NULL, check_edit_title, (void *)c) != 0)
            err("pthread create");
        pthread_join(check, NULL);
        free(buf);
    }
    else
        print("Invalid index!\n", 15);
}

void* check_edit_content(void *argv) {
    Size size = ((Chunk *)argv)->size;
    char *buf = ((Chunk *)argv)->buf;
    int idx = ((Chunk *)argv)->idx;
    int i = 0;
    while (1) {
        if (i != idx) {
            lock(&mutex_book_cnt);
            int cnt = book_cnt;
            unlock(&mutex_book_cnt);
            if (i >= cnt)
                break;
            rlock(&rwlock_books_and_sizes[i]);
            int flag = check_str(buf, size.content,
                              books[i] + sizes[i].title, sizes[i].content);
            rwunlock(&rwlock_books_and_sizes[i]);
            if (!flag) {
                free(argv);
                print("The content is not unique!\n", 27);
                return NULL;
            }
        }
        i++;
    }
    wlock(&rwlock_books_and_sizes[idx]);
    memcpy(books[idx] + size.title, buf, size.content);
    rwunlock(&rwlock_books_and_sizes[idx]);
    free(argv);
    return NULL;
}

void edit_content() {
    print("Index: ", 7);
    int idx = readint();
    lock(&mutex_book_cnt);
    int cnt = book_cnt;
    unlock(&mutex_book_cnt);
    if (idx < cnt) {
        rlock(&rwlock_books_and_sizes[idx]);
        Size size = sizes[idx];
        rwunlock(&rwlock_books_and_sizes[idx]);
        char *buf = malloc(size.content);
        print("Book content: ", 14);
        read(0, buf, size.content);
        pthread_t check;
        Chunk *c = calloc(sizeof(Chunk), 1);
        c->size = size;
        c->buf = buf;
        c->idx = idx;
        if (pthread_create(&check, NULL, check_edit_content, (void *)c) != 0)
            err("pthread create");
        pthread_join(check, NULL);
        free(buf);
    }
    else
        print("Invalid index!\n", 15);
}

void show_log() {
    int fd = open("log", O_RDONLY);
    if (fd < 0)
        err("open log");
    print("Log size: ", 10);
    int size = readint();
    char c;
    for (int i = 0; i < size && read(fd, &c, 1) == 1; i++)
        write(1, &c, 1);
    print("\n", 1);
    close(fd);
}

void init() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    if (pthread_mutex_init(&mutex_book_cnt, NULL) != 0 ||
        pthread_mutex_init(&mutex_size_cnt, NULL) != 0)
        err("mutex init");
    for (int i = 0; i < MAX_BOOK; i++)
        if (pthread_rwlock_init(&rwlock_books_and_sizes[i], NULL) != 0)
            err("mutex init");

    system("rm log -f");
}

int main() {
    init();
    while (1) {
        menu();
        int op = readint();
        switch (op) {
        case 1: create(); break;
        case 2: show(); break;
        case 3: edit_title(); break;
        case 4: edit_content(); break;
        case 5: show_log(); break;
        default: puts("Invalid choice!");
        }
    }
}
