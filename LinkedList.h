#pragma once
#include <stdexcept>

//Inspired by my code from data structures class
//And also here: https://www.codesdope.com/blog/article/c-linked-lists-in-c-singly-linked-list/

template <typename T>
class Node {

private:

    template <class U> friend class LinkedList;

    T data;
    Node<T>* previous;
    Node<T>* next;

public:

    Node() = default;
    Node(const T& val, Node<T>* prev=nullptr): data(val), previous(prev), next(nullptr) {}

};

template <class T>
class LinkedList {

private:

    Node<T>* head;
    Node<T>* tail;
    size_t _size;

public:

    LinkedList(): head(nullptr), tail(nullptr), _size(0) {}
    ~LinkedList();

    //List iterator wrapper
    class ListIter
    {
    private:
        Node<T>* iter;
        template <class U> friend class LinkedList;
    public:
        ListIter() = default;
        ListIter(Node<T>* n): iter(n) {}
        ListIter operator++(int)
        {
            iter = iter->next;
            return *this;
        }
        ListIter operator--(int)
        {
            iter = iter->previous;
            return *this;
        }
        T& operator*()
        {
            return iter->data;
        }
        bool isEnd() const
        {
            return iter == nullptr;
        }
        bool isTail() const
        {
            return iter->next == nullptr;
        }
    };

    void push_front(const T& element);
    void pop_front();
    void push_back(const T& element);
    void pop_back();
    void insert(size_t index, const T& element);
    void insert(ListIter& iter, const T& element);
    void erase(ListIter& position);
    size_t size() const;

    T& operator[](size_t index);

    //Iterator functions
    ListIter begin();
    ListIter end();


};

template <class T>
void LinkedList<T>::push_back(const T& element)
{

    Node<T>* elem = new Node<T>(element);
    if (_size == 0)
    {
        head = elem;
        tail = elem;
    }
    else
    {
        elem->previous = tail;
        tail->next = elem;
        tail = elem;
    }

    _size++;

}

template <class T>
void LinkedList<T>::pop_back()
{
    if (_size == 1)
        delete head;
    else if (_size > 1)
    {
        Node<T>* temp = tail->previous;
        delete tail;
        tail = temp;
        tail->next = nullptr;
    }
    else
        throw std::out_of_range("Cannot pop from empty list");

    _size--;
}

template <class T>
void LinkedList<T>::insert(size_t index, const T& element)
{
    //Error handling
    if (index > (_size - 1) && _size != 0)
        throw std::out_of_range("Index is out of range");

    Node<T>* elem = new Node<T>(element);
    if (_size == 0) //special case - insert into empty list
    {
        head = elem;
        tail = elem;
    }
    else if (index == 0) //special case - insert at beginning of list
    {
        head->previous = elem;
        elem->next = head;
        head = elem;
    }
    else //normal case
    {
        Node<T>* iter = head;
        for (size_t i = 0; i < index; i++) //advance iter to index
            iter = iter->next;
        
        elem->previous = iter->previous;
        elem->previous->next = elem;
        elem->next = iter;
        iter->previous = elem;
    }

    _size++;
}

template <class T>
void LinkedList<T>::insert(LinkedList<T>::ListIter& iter, const T& element)
{
    //Error handling
    if (iter.iter == nullptr && _size != 0)
        throw std::out_of_range("Index is out of range");

    Node<T>* elem = new Node<T>(element);
    if (_size == 0) //special case - insert into empty list
    {
        head = elem;
        tail = elem;
    }
    else if (iter.iter->previous == nullptr) //special case - insert at beginning of list
    {
        head->previous = elem;
        elem->next = head;
        head = elem;
    }
    else //normal case
    {   
        elem->previous = iter.iter->previous;
        elem->previous->next = elem;
        elem->next = iter.iter;
        iter.iter->previous = elem;
    }

    _size++;
}



template <class T>
size_t LinkedList<T>::size() const
{
    return _size;
}

template <class T>
LinkedList<T>::~LinkedList()
{
    if (_size == 1)
        delete head;
    else if (_size > 1) //no action needed for _size == 0
    {
        Node<T>* iter = head;
        while (iter != nullptr)
        {
            iter = head->next;
            delete head;
            head = iter;
        }

    }
    
}

template <class T>
typename LinkedList<T>::ListIter LinkedList<T>::begin()
{
    ListIter iter(head);
    return iter;
}

template <class T>
typename LinkedList<T>::ListIter LinkedList<T>::end()
{
    ListIter iter(tail);
    return iter;
}

template <class T>
T& LinkedList<T>::operator[](size_t index)
{
    if (index == 0)
        return head->data;
    else if (index == _size - 1)
        return tail->data;

    Node<T>* iter = head;
    for (size_t i = 0; i < index; i++)
        iter = iter->next;
    return iter->data;
}

template <class T>
void LinkedList<T>::push_front(const T& element)
{
    Node<T>* n = new Node<T>(element);
    if (_size == 0)
    {
        head = n;
        tail = n;
    }
    else
    {
        head->previous = n;
        n->next = head;
        head = n;
    }
    _size++;
}

template <class T>
void LinkedList<T>::pop_front()
{
    if (_size == 1)
        delete head;
    else
    {
        Node<T>* temp = head;
        head = head->next;
        head->previous = nullptr;
        delete temp;
    }
    _size--;
}

template <class T>
void LinkedList<T>::erase(LinkedList<T>::ListIter& position)
{
    //Removes the element referenced by the "position" iterator
    if (position.iter == head)
    {
        position.iter->next->previous = nullptr;
        head = position.iter->next;
        delete position.iter;
    }
    else if (position.iter == tail)
    {
        position.iter->previous->next = nullptr;
        tail = position.iter->previous;
        delete position.iter;
    }
    else
    {
        position.iter->previous->next = position.iter->next;
        position.iter->next->previous = position.iter->previous;
        delete position.iter;
    }
    _size--;
}