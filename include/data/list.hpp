#ifndef LIST_H
#define LIST_H
#include <system.hpp>

template <typename T>
class List {
    private:
        /// @brief The allocated data array.
        T* data = NULL;

        /// @brief Re-allocates the data array and copies old elements to the new capacity.
        /// @param newCapacity The new size of the data array.
        void Reallocate(int newCapacity);
    
    public:
        /// @brief The capacity of the data array.
        int capacity;

        /// @brief The number of items in the data array.
        int count;

        /// @brief Constructs an empty list with an initial capacity of 8
        List();

        /// @brief Frees the list.
        ~List();

        /// @brief Constructs an empty list with the given initial capacity.
        /// @param initialCapacity The allocated size of the array storing the data.
        List(int initialCapacity);

        /// @brief Adds the item to the list
        /// @param item The item to add.
        void Add(T item);

        /// @brief Gets the item at the given index
        /// @param index The index of the item.
        /// @return The item by the given index.
        T Get(int index);

        /// @brief Removes the item from the list
        /// @param index The index of the item to remove
        void Remove(int index);

        /// @brief Clears the list
        void Clear();
};

template <typename T>
void List<T>::Reallocate(int newCapacity) {
    if (this->data == NULL) {
        this->data = (T*) malloc(newCapacity * sizeof(T));
        this->capacity = newCapacity;
        this->count = 0;
    } else {
        T *oldData = this->data;
        this->data = (T*) malloc(newCapacity * sizeof(T));

        if (newCapacity > this->capacity) {
            memcpy((void*)this->data, oldData, this->count * sizeof(T));
        } else {
            crash("List decreased in size (incorrect call to Reallocate)");
        }
        
        this->capacity = newCapacity;
        free(oldData);
    }
}

template <typename T>
List<T>::~List() {
    free(this->data);
}

template <typename T>
List<T>::List() {
    this->Reallocate(8);
}

template <typename T>
List<T>::List(int initialCapacity) {
    this->Reallocate(initialCapacity);
}

template <typename T>
void List<T>::Add(T item) {
    if (this->count >= this->capacity) {
        // Double list in size and add four
        this->Reallocate(this->capacity * 2 + 4);
    }

    this->data[this->count] = item;
    this->count++;
}

template <typename T>
T List<T>::Get(int index) {
    if (index < 0 || index >= this->count) {
        crash("List index out of range.");
    }
    return this->data[index];
}

template <typename T>
void List<T>::Remove(int index) {
    crash("List Remove not yet implemented");
}

template <typename T>
void List<T>::Clear() {
    free(this->data);
    this->data = NULL;
    this->Reallocate(8);
}

#endif

