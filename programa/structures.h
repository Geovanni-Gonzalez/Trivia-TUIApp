#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h> // malloc, free
#include <iostream>

// --- Helper Functions for C-Strings (No string.h allowed) ---
int my_strlen(const char* str) {
    if (!str) return 0;
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void my_strcpy(char* dest, const char* src) {
    if (!dest || !src) return;
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void my_strcat(char* dest, const char* src) {
    if (!dest || !src) return;
    // Find end of dest
    while (*dest != '\0') dest++;
    // Copy src
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

char* my_strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; haystack++) {
        if (*haystack == *needle) {
            const char *h = haystack, *n = needle;
            while (*h && *n && *h == *n) {
                h++;
                n++;
            }
            if (!*n) return (char*)haystack;
        }
    }
    return NULL;
}

char* my_strchr(const char* str, int c) {
    while (*str) {
        if (*str == (char)c) return (char*)str;
        str++;
    }
    if (c == 0) return (char*)str;
    return NULL;
}

// --- MyString Class ---
class MyString {
private:
    char* data;
    int length;

public:
    // Constructor default
    MyString() {
        length = 0;
        data = new char[1];
        data[0] = '\0';
    }

    // Constructor from char*
    MyString(const char* str) {
        if (str) {
            length = my_strlen(str);
            data = new char[length + 1];
            my_strcpy(data, str);
        } else {
            length = 0;
            data = new char[1];
            data[0] = '\0';
        }
    }

    // Copy Constructor
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        my_strcpy(data, other.data);
    }

    // Assignment Operator
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            my_strcpy(data, other.data);
        }
        return *this;
    }

    // Destructor
    ~MyString() {
        delete[] data;
    }

    // Methods
    int len() const { return length; }
    
    const char* c_str() const { return data; }

    bool operator==(const MyString& other) const {
        return my_strcmp(data, other.data) == 0;
    }

    bool operator==(const char* str) const {
        return my_strcmp(data, str) == 0;
    }

    // Concatenation
    MyString operator+(const MyString& other) const {
        int newLen = length + other.length;
        char* newData = new char[newLen + 1];
        my_strcpy(newData, data);
        my_strcat(newData, other.data);
        MyString result(newData);
        delete[] newData;
        return result;
    }
    
    // Output stream overload
    friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
        os << s.data;
        return os;
    }
};

// --- LinkedList Template ---
template <typename T>
struct Node {
    T data;
    Node* next;

    Node(T val) : data(val), next(NULL) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    int size;

public:
    LinkedList() : head(NULL), size(0) {}

    // Copy Constructor (Deep Copy)
    LinkedList(const LinkedList& other) : head(NULL), size(0) {
        Node<T>* curr = other.head;
        while (curr) {
            add(curr->data);
            curr = curr->next;
        }
    }

    // Assignment Operator (Deep Copy)
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* curr = other.head;
            while (curr) {
                add(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    // Destructor
    ~LinkedList() {
        clear();
    }

    void add(T val) {
        Node<T>* newNode = new Node<T>(val);
        if (head == NULL) {
            head = newNode;
        } else {
            Node<T>* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    // Get element at index (0-based)
    T* get(int index) {
        if (index < 0 || index >= size) return NULL;
        Node<T>* temp = head;
        for (int i = 0; i < index; i++) {
            temp = temp->next;
        }
        return &(temp->data);
    }

    int count() const {
        return size;
    }

    void clear() {
        Node<T>* current = head;
        while (current != NULL) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = NULL;
        size = 0;
    }
    
    // For manual iteration if needed
    Node<T>* getHead() const { return head; }
};

#endif // STRUCTURES_H
