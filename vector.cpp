#include "vector.h"

template <typename T>
my_vector::vector<T>::vector()
: size_(0), buffer_(nullptr) {}

template <typename T>
my_vector::vector<T>::vector(std::initializer_list<T> set)
: size_(set.size()), capacity_(set.size()),
    buffer_(new char[sizeof(T) * set.size()]) {
    std::uninitialized_move(set.begin(), set.end(),
                            reinterpret_cast<T*>(buffer_));
}

template <typename T>
my_vector::vector<T>::vector(std::size_t n, const T& default_value)
: size_(n), capacity_(n), buffer_(new char[sizeof(T) * n]) {
    std::uninitialized_fill_n(reinterpret_cast<T*>(buffer_), n,
                              default_value);
}

template <typename T>
my_vector::vector<T>::vector(const vector<T>& temp)
: size_(temp.size()), capacity_(temp.size()),
    buffer_(new char[sizeof(T) * temp.size()]) {
    std::uninitialized_copy(temp.begin(), temp.end(),
                            reinterpret_cast<T*>(buffer_));
}

template <typename T>
my_vector::vector<T>::vector(vector<T>&& temp)
: size_(temp.size()), capacity_(temp.size()),
    buffer_(temp.buffer_) {
    temp.size_ = 0;
    temp.capacity_ = 0;
    temp.buffer_ = nullptr;
}

template <typename T>
my_vector::vector<T>& my_vector::vector<T>::operator =(const vector<T>& other) {
    std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
    buffer_ = new char[sizeof(T) * other.size()];
    size_ = capacity_ = other.size();
    std::uninitialized_copy(other.begin(), other.end(),
                            reinterpret_cast<T*>(buffer_));
    return *this;
}

template <typename T>
my_vector::vector<T>& my_vector::vector<T>::operator =(vector<T>&& other) {
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

template <typename T>
my_vector::vector<T>::~vector() {
    std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
    delete [] buffer_;
}

template <typename T>
T& my_vector::vector<T>::front() {
    return *std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
const T& my_vector::vector<T>::front() const {
    return *std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
T& my_vector::vector<T>::back() {
    return *std::launder(reinterpret_cast<T*>(buffer_) + size_ - 1);
}

template <typename T>
const T& my_vector::vector<T>::back() const {
    return *std::launder(reinterpret_cast<T*>(buffer_) + size_ - 1);
}

template <typename T>
T& my_vector::vector<T>::at(int index) {
                return *std::launder(reinterpret_cast<T*>(buffer_) + index);
            }

template <typename T>
const T& my_vector::vector<T>::at(int index) const {
    return *std::launder(reinterpret_cast<T*>(buffer_) + index);
}

template <typename T>
void my_vector::vector<T>::pop_back() {
    size_--;
    std::destroy_at(reinterpret_cast<T*>(buffer_) + size_);
}

template <typename T>
T* my_vector::vector<T>::begin() {
    return std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
const T* my_vector::vector<T>::begin() const {
    return std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
T* my_vector::vector<T>::rbegin() {
    return std::launder(reinterpret_cast<T*>(buffer_) + size_);
}

template <typename T>
const T* my_vector::vector<T>::rbegin() const {
    return std::launder(reinterpret_cast<T*>(buffer_) + size_);
}

template <typename T>
T* my_vector::vector<T>::end() {
    return std::launder(reinterpret_cast<T*>(buffer_) + size_);
}

template <typename T>
const T* my_vector::vector<T>::end() const {
    return std::launder(reinterpret_cast<T*>(buffer_) + size_);
}

template <typename T>
T* my_vector::vector<T>::rend() {
    return std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
const T* my_vector::vector<T>::rend() const {
    return std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
std::size_t my_vector::vector<T>::max_size() {
    return capacity_;
}

template <typename T>
const std::size_t my_vector::vector<T>::max_size() const {
    return capacity_;
}

template <typename T>
T* my_vector::vector<T>::data() {
    if (size_ == 0) {
        return nullptr;
    }
    return std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
const T* my_vector::vector<T>::data() const {
    if (size_ == 0) {
        return nullptr;
    }
    return std::launder(reinterpret_cast<T*>(buffer_));
}

template <typename T>
void clear() {
    std::destroy_n(reinterpret_cast<T*>(buffer_), size_);
    size_ = 0;
}

template <typename T>
bool my_vector::vector<T>::empty() const {
    return size_ == 0;
}

template <typename T>
T& my_vector::vector<T>::operator [](std::size_t index) {
    return *std::launder(reinterpret_cast<T*>(buffer_) + index);
}

template <typename T>
const T& my_vector::vector<T>::operator[](std::size_t index) const {
    return *std::launder(reinterpret_cast<T*>(buffer_) + index);
}

template <typename T>
std::size_t my_vector::vector<T>::size() {
    return size_;
}

template <typename T>
const std::size_t my_vector::vector<T>::size() const {
    return size_;
}

template <typename T>
void my_vector::vector<T>::push_back(T&& argument) {
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

template <typename T>
void my_vector::vector<T>::push_back(T& argument) {
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

template <typename T>
template <typename... Args>
void my_vector::vector<T>::emplace_back(Args&&... args) {
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
    new (reinterpret_cast<T*>(buffer_) + size_)
    T(std::forward<Args>(args)...);
    size_++;
}

template <typename T>
void my_vector::vector<T>::insert(vector<T>::iterator place, T&& argument) {
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

template <typename T>
void my_vector::vector<T>::insert(vector<T>::iterator place, T& argument) {
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

template <typename T>
void my_vector::vector<T>::insert(int place, vector<T>::iterator start_of_range,
                                             vector<T>::iterator end_of_range) {
    auto start_index = std::distance(reinterpret_cast<T*>(buffer_), start_of_range);
    auto end_index = std::distance(reinterpret_cast<T*>(buffer_), end_of_range);
    auto length_of_range = end_index - start_index;
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
                    T(*(reinterpret_cast<T*>(buffer_) + size_ +
                        i - length_of_range - 1));
            }
            std::move(reinterpret_cast<T*>(buffer_) + place,
                        reinterpret_cast<T*>(buffer_) + size_ - length_of_range,
                        reinterpret_cast<T*>(buffer_) + place + length_of_range);
        } else {
            auto number_of_elements = place + length_of_range - size_;

        }
    }
    std::move(start_of_range, end_of_range, reinterpret_cast<T*>(buffer_) + place);
    size_ += length_of_range;
}

template <typename T>
void my_vector::vector<T>::erase(vector<T>::iterator place) {
    auto index = std::distance(reinterpret_cast<T*>(buffer_), place);
    std::move(reinterpret_cast<T*>(buffer_) + index + 1,
                reinterpret_cast<T*>(buffer_) + size_,
                reinterpret_cast<T*>(buffer_) + index);
    size_--;
    std::destroy_at(reinterpret_cast<T*>(buffer_) + size_);
}

template <typename T>
void my_vector::vector<T>::erase(vector<T>::iterator start_of_range,
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

template <typename T>
bool my_vector::vector<T>::operator <(const vector<T>& second) const {
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

template <typename T>
bool my_vector::vector<T>::operator <=(const vector<T>& second) const {
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

template <typename T>
bool my_vector::vector<T>::operator ==(const vector<T>& second) const {
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

template <typename T>
bool my_vector::vector<T>::operator !=(const vector<T>& second) const {
    return !(*this == second);
}

template <typename T>
bool my_vector::vector<T>::operator >(const vector<T>& second) const {
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

template <typename T>
bool my_vector::vector<T>::operator >=(const vector<T>& second) const {
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
