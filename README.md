# Custom vector implementation
A custom vector implementation using placement new similar to STL std::vector

 


# Implemented methods
- ## Member functions
        vector();
        vector(std::initializer_list<T> set);
        vector(std::size_t n, const T& default_value = T());
        vector(const vector<T>& temp);
        vector(vector<T>&& temp);
        vector& operator =(const vector<T>& other);
        vector& operator =(vector<T>&& other);
        ~vector();
- ## Element access
        T& front();
        const T& front() const;    
        T& back();
        const T& back() const;
        T& at(int index);
        const T& at(int index) const;
        T* data();
        const T* data() const;
        T& operator [](std::size_t index);
        const T& operator[](std::size_t index) const;
- ## Iterators
        T* begin();
        const T* begin() const;
        T* rbegin();
        const T* rbegin() const;
        T* end();
        const T* end() const;
        T* rend();
        const T* rend() const;
- ## Capacity
        bool empty() const;
        std::size_t size();
        const std::size_t size() const;
        std::size_t max_size();
        const std::size_t max_size() const;
- ## Modifiers
        void resize(std::size_t newSize, T value);
        void resize(std::size_t newSize);
        void emplace_back(Args&&... args);
        void insert(vector<T>::iterator place, T&& argument);
        void insert(vector<T>::iterator place, T& argument);
        void insert(int place, vector<T>::iterator start_of_range, vector<T>::iterator end_of_range);
        void erase(vector<T>::iterator place);
        void erase(vector<T>::iterator start_of_range, vector<T>::iterator end_of_range);
        void push_back(T&& argument);
        void push_back(T& argument);
        void clear();
        void pop_back();
- ## Non-member functions
        bool operator <(const vector<T>& second) const;
        bool operator <=(const vector<T>& second) const;
        bool operator ==(const vector<T>& second) const;
        bool operator !=(const vector<T>& second) const;
        bool operator >(const vector<T>& second) const;
        bool operator >=(const vector<T>& second) const; 
