#ifndef LDL_H
#define LDL_H
#include <iostream>
#include <stdexcept>
#include <memory>

using namespace std;

template <typename T>
class LDL{
    public:
        LDL (){
            listSize = 0;
            listFront = nullptr;
            listBack = nullptr;
        }

        ~LDL (){
            clear ();
        }

        //Comportamientos de la lista.
        bool empty () const;
        size_t size () const;
        void push_front (const T &element);
        void push_back (const T &element);
        const T& front () const;
        const T& back () const;
        void pop_front ();
        void pop_back ();
        void insert (size_t position, const T &element);
        void erase (size_t position);
        void clear ();
        void remove (const T &value);
        T& operator [] (size_t position);

    private:
        struct NodeLDL{
            T data;
            shared_ptr<NodeLDL> prev;
            shared_ptr<NodeLDL> next;

            NodeLDL (const T &element, shared_ptr<NodeLDL> p = nullptr, shared_ptr<NodeLDL> n = nullptr) : 
            data (element), prev (p), next (n) {};
        };

        size_t listSize;
        shared_ptr<NodeLDL> listFront;
        shared_ptr<NodeLDL> listBack;
};

template <typename T>
bool LDL<T>::empty () const{
    return listSize == 0;
}

template <typename T>
size_t LDL<T>::size () const{
    return listSize;
}

template <typename T>
void LDL<T>::push_front (const T &element){
    if (empty ()){
        //La lista estaba vacia, por lo que este elemento es el primero. Tanto listFront como listBack apuntaran al mismo lugar.
        listFront = make_shared<NodeLDL> (element);
        listBack = listFront;
    }
    else{
        //Cuando la lista no esta vacia antes de agregar cada elemento, se deben indicar otros parametros.
        //Para agregar algo al frente, agregamos un nuevo nodo que guardara el elemento, y hacia adelante apunta a lo que antes era el frente
        //de la lista y hacia atras apunte al nullptr.
        shared_ptr<NodeLDL> newNode = make_shared<NodeLDL> (element, nullptr, listFront);
        //Ahora, el listFront actual debe tener al newNode como su elemento previo. De esa forma, agregamos algo al frente de la lista.
        listFront->prev = newNode;
        listFront = newNode;
    }

    listSize++;
}

template <typename T>
void LDL<T>::push_back (const T &element){
    if (empty ()){
        listFront = make_shared<NodeLDL> (element);
        listBack = listFront;
    }
    else{
        //Al agregar un element atras de la lista, el elemento anterior debe ser el ultimo elemento de la lista actual, y el frente debera ser nullptr.
        shared_ptr<NodeLDL> newNode = make_shared<NodeLDL> (element, listBack, nullptr);
        //listBack actual debe tener a newNode como su elemento siguiente. Asi hemos añadido un elemento a la lista desde atras.
        listBack->next = newNode;
        listBack = newNode;
    }

    listSize++;
}

template <typename T>
const T& LDL<T>::front () const{
    if (empty ()){
        throw runtime_error ("Trying front() from an empty list.");
    }

    return listFront->data;
}

template <typename T>
const T& LDL<T>::back () const{
    if (empty ()){
        throw runtime_error ("Trying back() from an empty list.");
    }

    return listBack->data;
}

template <typename T>
void LDL<T>::pop_front (){
    if (empty ()){
        throw runtime_error ("Trying pop_front() from an empty list.");
    }

    if (size () == 1){
        //Si solo hay un elemento, basta con hacer que tanto el frente como el fin sean nulos.
        listFront = nullptr;
        listBack = nullptr;
    }
    else{
        //Cuando hay mas de un elemento, debemos hacer otro procedimiento.
        //listFront debe ser igual al elemento guardado en next.
        listFront = listFront->next;
        //El campo que apunta al elemento anterior, debe ser igual a nulo.
        listFront->prev = nullptr;
    }

    listSize--;
}

template <typename T>
void LDL<T>::pop_back (){
    if (empty ()){
        throw runtime_error ("Trying pop_back() from an empty list.");
    }

    if (size () == 1){
        listFront = nullptr;
        listBack = nullptr;
    }
    else{
        //Para eliminar el ultimo elemento, solo debemos hacer que el elemento anterior al ultimo sea ahora listBack y apuntar su parte next a nullptr.
        listBack = listBack->prev;
        listBack->next = nullptr;
    }

    listSize--;
}

template <typename T>
void LDL<T>::insert (size_t position, const T &element){
    if (empty ()){
        throw runtime_error ("Trying insert(size_t position, const T &element) in an empty list.");
    }

    if (position > size()){
        throw runtime_error ("Trying insert(size_t position, const T &element) in non valid position.");
    }

    if (position == 0){
        push_front (element);
    }
    else if (position == size()){
        push_back (element);
    }
    else{
        shared_ptr<NodeLDL> temp = listFront;

        for (size_t i = 0; i < position - 1; i++){
            temp = temp->next;
        }

        shared_ptr<NodeLDL> newNode = make_shared<NodeLDL> (element, temp, temp->next);

        temp->next = newNode;
        newNode->next->prev = newNode;

        listSize++;
    }
}

template <typename T>
void LDL<T>::erase (size_t position){
    if (empty()){
        throw runtime_error ("Trying erase() from an empty list.");
    }
    if (position >= size()){
        throw runtime_error ("Trying erase() in non valid position.");
    }

    if (position == 0){
        pop_front ();
    }
    else if (position == size ()-1){
        pop_back ();
    }
    else{
        shared_ptr<NodeLDL> temp = listFront;

        for (size_t i = 0; i < position - 1; i++){
            temp = temp->next;
        }

        temp->next->next->prev = temp;
        temp->next = temp->next->next;

        listSize--;
    }
}

template <typename T>
void LDL<T>::clear (){
    while (!empty()){
        pop_front ();
    }
}

template <typename T>
void LDL<T>::remove (const T &value){
    if (empty ()){
        throw runtime_error ("Trying remove () from an empty list.");
    }

    T data;
    shared_ptr<NodeLDL> temp = listFront;
    size_t i = 0;

    while (temp != nullptr){
        data = temp->data;

        temp = temp->next;

        if (data == value){
            erase (i);
            i--;
        }

        i++;
    }
}

template <typename T>
T& LDL<T>::operator [] (size_t position){
    if (empty ()){
        throw runtime_error ("Trying [] in an empty list.");
    }
    if (position >= size ()){
        throw runtime_error ("Trying [] in non valid position.");
    }

    shared_ptr<NodeLDL> temp = listFront;

    for (size_t i = 0; i < position; i++){
        temp = temp->next;
    }

    return temp->data;
}

#endif