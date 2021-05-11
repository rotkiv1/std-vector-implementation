#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstring>
#include <initializer_list>
#include <string>
#include <algorithm>
#include <memory>
#include <chrono>

namespace my_vector {

    template <typename T>

    class vector {

        private:

            char* buffer_;
            std::size_t size_;
            int capacity_;
            typedef T* iterator;

        public:

            vector();
            vector(std::initializer_list<T> set);
            vector(std::size_t n, const T& default_value = T());
            vector(const vector<T>& temp);
            vector(vector<T>&& temp);

            vector& operator =(const vector<T>& other);
            vector& operator =(vector<T>&& other);

            ~vector();

            T& front();
            const T& front() const;

            T& back();
            const T& back() const;

            T& at(int index);
            const T& at(int index) const;

            void pop_back();

            T* begin();
            const T* begin() const;
            T* rbegin();
            const T* rbegin() const;

            T* end();
            const T* end() const;
            T* rend();
            const T* rend() const;

            T* data();
            const T* data() const;

            void clear();

            bool empty() const;

            T& operator [](std::size_t index);
            const T& operator[](std::size_t index) const;

            std::size_t size();
            const std::size_t size() const;
            std::size_t maxsize();
            const std::size_t max_size() const;

            void push_back(T&& argument);
            void push_back(T& argument);

            template <typename... Args>
            void emplace_back(Args&&... args);

            void insert(vector<T>::iterator place, T&& argument);
            void insert(vector<T>::iterator place, T& argument);
            void insert(int place, vector<T>::iterator start_of_range,
                                   vector<T>::iterator end_of_range);

            void erase(vector<T>::iterator place);
            void erase(vector<T>::iterator start_of_range,
                       vector<T>::iterator end_of_range);

            bool operator <(const vector<T>& second) const;
            bool operator <=(const vector<T>& second) const;
            bool operator ==(const vector<T>& second) const;
            bool operator !=(const vector<T>& second) const;
            bool operator >(const vector<T>& second) const;
            bool operator >=(const vector<T>& second) const;
    };
}

#endif
