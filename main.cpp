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

            vector() : size_(0), buffer_(nullptr) {}

            vector(std::initializer_list<T> set)
            : size_(set.size()), capacity_(set.size()),
              buffer_(new char[sizeof(T) * set.size()]) {
                std::uninitialized_move(set.begin(), set.end(),
                                        reinterpret_cast<T*>(buffer_));
            }

            vector(std::size_t n, const T& default_value = T())
            : size_(n), capacity_(n), buffer_(new char[sizeof(T) * n]) {
                std::uninitialized_fill_n(reinterpret_cast<T*>(buffer_),
                                          n, default_value);
            }

            vector(const vector<T>& temp)
            : size_(temp.size()), capacity_(temp.size()),
              buffer_(new char[sizeof(T) * temp.size()]) {
                std::uninitialized_copy(temp.begin(), temp.end(),
                                        reinterpret_cast<T*>(buffer_));
            }

            vector(vector<T>&& temp)
            : size_(temp.size()), capacity_(temp.size()),
              buffer_(temp.buffer_) {
                temp.size_ = 0;
                temp.capacity_ = 0;
                temp.buffer_ = nullptr;
            }

            vector& operator =(const vector<T>& other) {
                std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                buffer_ = new char[sizeof(T) * other.size()];
                size_ = capacity_ = other.size();
                std::uninitialized_copy(other.begin(), other.end(),
                                        reinterpret_cast<T*>(buffer_));
                return *this;
            }

            vector& operator =(vector<T>&& other) {
                std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                buffer_ = new char[sizeof(T) * other.size()];
                std::uninitialized_copy(other.begin(), other.end(),
                                        reinterpret_cast<T*>(buffer_));
                size_ = capacity_ = other.size();
                other.size = 0;
                other.capacity_ = 0;
                other.buffer_ = nullptr;
                return *this;
            }

            ~vector() {
                std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                delete [] buffer_;
            }

            T& front() {
                return *std::launder(reinterpret_cast<T*>(buffer_));
            }

            const T& front() const {
                return *std::launder(reinterpret_cast<T*>(buffer_));
            }

            T& back() {
                return *std::launder(reinterpret_cast<T*>(buffer_) + size_ - 1);
            }

            const T& back() const {
                return *std::launder(reinterpret_cast<T*>(buffer_) + size_ - 1);
            }

            T& at(int index) {
                return *std::launder(reinterpret_cast<T*>(buffer_) + index);
            }

            const T& at(int index) const {
                return *std::launder(reinterpret_cast<T*>(buffer_) + index);
            }

            void pop_back() {
                size_--;
                std::destroy_at(reinterpret_cast<T*>(buffer_) + size_);
            }

            T* begin() {
                return std::launder(reinterpret_cast<T*>(buffer_));
            }

            const T* begin() const {
                return std::launder(reinterpret_cast<T*>(buffer_));
            }

            T* rbegin() {
                return std::launder(reinterpret_cast<T*>(buffer_) + size_);
            }

            const T* rbegin() const {
                return std::launder(reinterpret_cast<T*>(buffer_) + size_);
            }

            T* end() {
                return std::launder(reinterpret_cast<T*>(buffer_) + size_);
            }

            const T* end() const {
                return std::launder(reinterpret_cast<T*>(buffer_) + size_);
            }

            T* rend() {
                return std::launder(reinterpret_cast<T*>(buffer_));
            }

            const T* rend() const {
                return std::launder(reinterpret_cast<T*>(buffer_));
            }

            T* data() {
                if (size_ == 0) {
                    return nullptr;
                }
                return std::launder(reinterpret_cast<T*>(buffer_));
            }

            void clear() {
                std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                size_ = 0;
            }

            bool empty() const {
                return size_ == 0;
            }

            T& operator [](std::size_t index) {
                return *std::launder(reinterpret_cast<T*>(buffer_) + index);
            }

            const T& operator[](std::size_t index) const {
                return *std::launder(reinterpret_cast<T*>(buffer_) + index);
            }

            std::size_t size() {
                return size_;
            }

            const std::size_t size() const {
                return size_;
            }

            void push_back(T&& argument) {
                if (size_ == capacity_) {
                    char* new_buffer_ = new char[size_ * sizeof(T) * 2];
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_),
                                            reinterpret_cast<T*>(buffer_) + size_,
                                            reinterpret_cast<T*>(new_buffer_));
                    std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                    delete [] buffer_;
                    buffer_ = new_buffer_;
                    capacity_ = size_ * 2;
                }
                new (reinterpret_cast<T*>(buffer_) + size_)
                T(std::forward<T>(argument));
                size_++;
            }

            void push_back(T& argument) {
                if (size_ == capacity_) {
                    char* new_buffer_ = new char[size_* sizeof(T) * 2];
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_),
                                            reinterpret_cast<T*>(buffer_) + size_,
                                            reinterpret_cast<T*>(new_buffer_));
                    std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                    delete [] buffer_;
                    buffer_ = new_buffer_;
                    capacity_ = size_ * 2;
                }
                new (reinterpret_cast<T*>(buffer_) + size_) T(argument);
                size_++;
            }

            void insert(vector<T>::iterator place, T&& argument) {
                auto index = std::distance(reinterpret_cast<T*>(buffer_), place);
                if (size_ == capacity_) {
                    char* new_buffer_ = new char[size_ * sizeof(T) * 2];
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_),
                                            reinterpret_cast<T*>(buffer_) + index,
                                            reinterpret_cast<T*>(new_buffer_));
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_) + index,
                                            reinterpret_cast<T*>(buffer_) + size_,
                                            reinterpret_cast<T*>(new_buffer_) + index + 1);
                    new (reinterpret_cast<T*>(new_buffer_) + index)
                    T(std::forward<T>(argument));
                    std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                    delete [] buffer_;
                    buffer_ = new_buffer_;
                    capacity_ = size_ * 2;
                } else {
                    new (reinterpret_cast<T*>(buffer_) + size_)
                        T(*(reinterpret_cast<T*>(buffer_) + size_ - 1));
                    std::move(reinterpret_cast<T*>(buffer_) + index,
                              reinterpret_cast<T*>(buffer_) + size_ - 1,
                              reinterpret_cast<T*>(buffer_) + index + 1);
                    *(reinterpret_cast<T*>(buffer_) + index) =
                    std::forward<T>(argument);
                }
                size_++;
            }

            void insert(vector<T>::iterator place, T& argument) {
                auto index = std::distance(reinterpret_cast<T*>(buffer_), place);
                if (size_ == capacity_) {
                    char* new_buffer_ = new char[size_ * sizeof(T) * 2];
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_),
                                            reinterpret_cast<T*>(buffer_) + index,
                                            reinterpret_cast<T*>(new_buffer_));
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_) + index,
                                            reinterpret_cast<T*>(buffer_) + size_,
                                            reinterpret_cast<T*>(new_buffer_) + index + 1);
                    new (reinterpret_cast<T*>(new_buffer_) + index) T(argument);
                    std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
                    delete [] buffer_;
                    buffer_ = new_buffer_;
                    capacity_ = size_ * 2;
                } else {
                    new (reinterpret_cast<T*>(buffer_) + size_)
                        T(*(reinterpret_cast<T*>(buffer_) + size_ - 1));
                    std::move(reinterpret_cast<T*>(buffer_) + index,
                              reinterpret_cast<T*>(buffer_) + size_ - 1,
                              reinterpret_cast<T*>(buffer_) + index + 1);
                    *(reinterpret_cast<T*>(buffer_) + index) = argument;
                }
                size_++;
            }

            /* to do */
            void insert(int place, vector<T>::iterator start_of_range,
                                   vector<T>::iterator end_of_range) {
                int start_index = std::distance(reinterpret_cast<T*>(buffer_), start_of_range);
                int end_index = std::distance(reinterpret_cast<T*>(buffer_), end_of_range);
                int length_of_range = end_index - start_index;
                if (size_ == capacity_) {
                    char* new_buffer_ = new char[sizeof(T) * size_ * 2];
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_),
                                            reinterpret_cast<T*>(buffer_) + place,
                                            reinterpret_cast<T*>(new_buffer_));
                    std::uninitialized_move(reinterpret_cast<T*>(buffer_) + place,
                                            reinterpret_cast<T*>(buffer_) + size_,
                                            reinterpret_cast<T*>(new_buffer_) + place +
                                            length_of_range);
                    std::destroy(reinterpret_cast<T*>(buffer_), reinterpret_cast<T*>(buffer_) +
                                 size_);
                    delete [] buffer_;
                    buffer_ = new_buffer_;
                    capacity_ = size_ * 2;
                } else {
                    if (size_ >= place + length_of_range) {
                        for (int i = length_of_range; i > 0; i--) {
                            new (reinterpret_cast<T*>(buffer_) + size_ + i - 1)
                                T(*(reinterpret_cast<T*>(buffer_) + size_ + i - length_of_range - 1));
                        }
                        std::move(reinterpret_cast<T*>(buffer_) + place,
                                  reinterpret_cast<T*>(buffer_) + size_ - length_of_range,
                                  reinterpret_cast<T*>(buffer_) + place + length_of_range);
                    } else {
                        int number_of_elements = place + length_of_range - size_;

                    }
                }
                std::move(start_of_range, end_of_range, reinterpret_cast<T*>(buffer_) + place);
                size_ += length_of_range;
            }

            void erase(vector<T>::iterator place) {
                int index = std::distance(reinterpret_cast<T*>(buffer_), place);
                std::move(reinterpret_cast<T*>(buffer_) + index + 1,
                          reinterpret_cast<T*>(buffer_) + size_,
                          reinterpret_cast<T*>(buffer_) + index);
                size_--;
                std::destroy_at(reinterpret_cast<T*>(buffer_) + size_);
            }

            void erase(vector<T>::iterator start_of_range,
                       vector<T>::iterator end_of_range) {
                auto start_index = std::distance(reinterpret_cast<T*>(buffer_),
                                                 start_of_range);
                auto end_index = std::distance(reinterpret_cast<T*>(buffer_),
                                               end_of_range);
                auto length_of_range = end_index - start_index;
                std::move(reinterpret_cast<T*>(buffer_) + end_index,
                          reinterpret_cast<T*>(buffer_) + size_,
                          reinterpret_cast<T*>(buffer_) + start_index);
                std::destroy(reinterpret_cast<T*>(buffer_) + size_ - length_of_range,
                             reinterpret_cast<T*>(buffer_) + size_);
                size_ -= length_of_range;
            }

            bool operator <(const vector<T>& second) const {
                auto less_size = std::min(size_, second.size_);
                for (int i = 0; i < less_size; i++) {
                    if (*(reinterpret_cast<T*>(buffer_) + i) !=
                        *(reinterpret_cast<T*>(second.buffer_) + i)) {
                        return *(reinterpret_cast<T*>(buffer_) + i) <
                               *(reinterpret_cast<T*>(second.buffer_) + i);
                    }
                }
                return size_ < second.size_;
            }

            bool operator <=(const vector<T>& second) const {
                auto less_size = std::min(size_, second.size_);
                for (int i = 0; i < less_size; i++) {
                    if (*(reinterpret_cast<T*>(buffer_) + i) !=
                        *(reinterpret_cast<T*>(second.buffer_) + i)) {
                        return *(reinterpret_cast<T*>(buffer_) + i) <
                               *(reinterpret_cast<T*>(second.buffer_) + i);
                    }
                }
                return size_ <= second.size_;
            }

            bool operator ==(const vector<T>& second) const {
                if (size_ != second.size_) {
                    return false;
                } else {
                    for (int i = 0; i < size_; i++) {
                        if (*(reinterpret_cast<T*>(buffer_) + i) !=
                            *(reinterpret_cast<T*>(second.buffer_) + i)) {
                            return false;
                        }
                    }
                    return true;
                }
            }

            bool operator !=(const vector<T>& second) const {
                return !(*this == second);
            }

            bool operator >(const vector<T>& second) const {
                auto less_size = std::min(size_, second.size_);
                for (int i = 0; i < less_size; i++) {
                    if (*(reinterpret_cast<T*>(buffer_) + i) !=
                        *(reinterpret_cast<T*>(second.buffer_) + i)) {
                        return *(reinterpret_cast<T*>(buffer_) + i) >
                               *(reinterpret_cast<T*>(second.buffer_) + i);
                    }
                }
                return size_ > second.size_;
            }

            bool operator >=(const vector<T>& second) const {
                auto less_size = std::min(size_, second.size_);
                for (int i = 0; i < less_size; i++) {
                    if (*(reinterpret_cast<T*>(buffer_) + i) !=
                        *(reinterpret_cast<T*>(second.buffer_) + i)) {
                        return *(reinterpret_cast<T*>(buffer_) + i) >
                               *(reinterpret_cast<T*>(second.buffer_) + i);
                    }
                }
                return size_ >= second.size_;
            }
    };
}

int main() {
    my_vector::vector<int> a{1, 2, 3};
    auto p = a.rbegin();
    p--;
    std::cout << *p;
}
