#pragma once

#include <iostream>
#include <exception>
#include <memory>
#include "array.hpp"
#include <stdexcept>

class String : protected Array<char,0> {
    public:

    void fill(const char item, const signed int fill_start = 0, signed int fill_end = 0) = delete;
    template<typename T,size_t Size_1>
    Array<T,Size_1> slice(const int startIndex,const int endIndex) const  = delete;
    template <typename T>
    void display(const std::function<void(T,int)> func = [] (T item, int index) {std::cout << "Index: " << index << " Contents of the index: " << item << "\n";}) const  = delete;
    template <typename T>
    T* get_array() const = delete;
    template<typename T>
    void insert(const int index, const T item) = delete;
    
    String operator+(const String &rhs) const {
        if (rhs.size == 0 || (rhs.size == 0 && this->size == 0)) {
            return *this;
        }
        else if (rhs.size == 1){
             String copy {*this};
             copy.push_back(rhs.front());
             return copy;
        }
        else if (this->size == 1) {
            String copy {*this};
            copy.push_back(rhs.front());
            return copy;
        }
        String result;
        result.length = this->length+rhs.length;
        result.size = result.length;
        result.ptr = std::make_unique<char[]> (this->length+rhs.length);
        for (int i {}; i < this->length; i++) {
            result[i] = this->at(i);
        }
        for (int i {this->length}, j {}; j < rhs.length; i++,j++) {
            result[i] = rhs[j];
        }
        return result;
    }

    void operator+=(const String &rhs) {
        *this = *this + rhs;
    }

    void operator=(const String &&rhs) {
        this->size = rhs.size;
        this->length = rhs.length;
        this->ptr = std::make_unique<char[]> (size);
        for (int i {}; i < rhs.length; i++) {
            ptr[i] = rhs.ptr[i];
        }
    }

    void operator=(const String &rhs) {
        this->size = rhs.size;
        this->length = rhs.length;
        this->ptr = std::make_unique<char[]> (size);
        for (int i {}; i < rhs.length; i++) {
            ptr[i] = rhs.ptr[i];
        }
    }

    const bool operator== (const String &rhs) const {
        if (this->size != rhs.size || this->length != rhs.length) {
            return false;
        }
        for (int i {}; i < size; i++) {
            if (this->ptr[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    } 

    const bool operator!= (const String &rhs) const {
        if (this->size != rhs.size || this->length != rhs.length) {
            return true;
        }
        for (int i {}; i < size; i++) {
            if (this->ptr[i] != rhs[i]) {
                return true;
            }
        }
        return false;
    }

    friend std::ostream &operator<< (std::ostream &os, const String &str);

    String(const char string[]) {
        int i;
        for (i = 0; string[i] != '\0'; i++) {}
        length = i;
        size = length;
        Array::length = length;
        ptr = std::make_unique<char[]> (length);
        for (int j {}; j < length; j++) {
            ptr[j] = string[j];
        }
    }

    String(std::string string) {
        length = string.size();
        size = length;
        ptr = std::make_unique<char[]> (length);
        for (int j {}; j < length; j++) {
            ptr[j] = string[j];
        }
    }

    String(const String &source) {
        size = source.size;
        length = source.length;
        ptr.reset(new char[length]); // To fix a segmentation fault.
        for (int i {}; i < length; i++) {
            ptr[i] = source.ptr[i];
        }
    }

    String() {
        size = 0;
        Array::length = 0;
        ptr = std::make_unique<char[]> (0);
    }

    void change_uppercase(const int start_index = 0, int end_index = -1) {
        if (end_index == -1) {
            end_index = size;
        }
        for (int i {start_index}; i <= end_index; i++) {
            if (ptr[i] > 96 && ptr[i] < 123) {
                ptr[i] -= 32;
            }
        }
    }

    void change_lowercase() {
        for (int i {}; i < length; i++) {
            if (ptr[i] > 64 && ptr[i] < 91) {
                ptr[i] += 32;
            }
        }
    }

    void push_back(const char &character) {
        ++length;
        ++size;
        std::unique_ptr<char[]> temp = std::make_unique<char[]>(size - 1);
        std::copy(ptr.get(), ptr.get() + size - 1, temp.get());
        ptr = std::make_unique<char[]>(size);
        std::copy(temp.get(), temp.get() + size - 1, ptr.get());
        ptr[size-1] = character;  
    }

    void push_back(const char* str) {
        int i {};
        for (; str[i] != '\0'; ++i) {}
        const int og_size_copy {size};
        length += i;
        size = length;
        std::unique_ptr<char[]> temp = std::make_unique<char[]>(size - 1);
        std::copy(ptr.get(), ptr.get() + size - 1, temp.get());
        ptr = std::make_unique<char[]>(size);
        std::copy(temp.get(), temp.get() + size - 1, ptr.get());
        std::copy(str,str+i, ptr.get() + og_size_copy);  
    }

    void push_front(const char &character) {
        ++length;
        ++size;
        std::unique_ptr<char[]> temp = std::make_unique<char[]>(size - 1);
        std::copy(ptr.get(), ptr.get() + size - 1, temp.get());
        ptr = std::make_unique<char[]>(size);
        std::copy(temp.get(), temp.get() + size - 1, ptr.get());
        this->insert(0,character);
    }

    void reserve(const int &indexes) {
        size += indexes;
        length += indexes;
        char *buffer = new char[size];
        for (int i {}; i < size; i++) {
            buffer[i] = ptr[i];
        }
        ptr = std::make_unique<char[]> (size+indexes);
        for (int i {}; i < size; i++) {
            ptr[i] = buffer[i];
        }
        return;
    }

    const char* c_str() const {
        char *return_value = new char[size];
        for (int i {}; i < size; i++) {
            return_value[i] = ptr[i];
        }
        return return_value;
    } 

    String substr(const int &startIndex,const int &endIndex) const { // Returns a new Array containing elements starting from start index to end index
        if (size <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(length, size);
        } else if (startIndex < 0 || startIndex > size - 1) {
            throw OUT_OF_RANGE_EXCEPTION(size, startIndex, "START INDEX");
        } else if (endIndex < 0 || endIndex > size - 1) {
            throw OUT_OF_RANGE_EXCEPTION(size, endIndex, "END INDEX");
        } else if (startIndex >= endIndex) {
            throw OUT_OF_RANGE_EXCEPTION(size, endIndex, "END INDEX must be greater than START INDEX",1);
        }

        String result_str;
        result_str.size = endIndex - startIndex;
        result_str.length = result_str.size;
        result_str.ptr = std::make_unique<char[]> (result_str.size); 
        for (int i {startIndex}, j {0}; (endIndex == result_str.size) ? i < endIndex : i <= endIndex; i++, j++) {
            result_str[j] = this->ptr[i]; 
        } 
        return result_str;
    }

    void insert(int index,const char &character) {
        if (index < 0 || index > size) {
            throw OUT_OF_RANGE_EXCEPTION(size,index);
        }
        else if (size == 0 && index > 0) {
            throw OUT_OF_RANGE_EXCEPTION(size,index);
        }
        ++size;
        ++length;
        char buffer[size-1];
        std::copy(ptr.get(),ptr.get()+size-1,buffer);
        ptr = std::make_unique<char[]> (size);
        std::copy(buffer,buffer+size-1, ptr.get());
        for (int i {size-1}; i > 0; --i) {
            ptr[i] = ptr[i-1]; 
        }
        ptr[0] = character;

    }
    
    void insert(int index,const char *str) {
        if (index < 0 || index > size) {
            throw OUT_OF_RANGE_EXCEPTION(size,index);
        }
        else if (size == 0 && index > 0) {
            throw OUT_OF_RANGE_EXCEPTION(size,index);
        }
        int size_of_str {};
        for (; str[size_of_str] != '\0'; size_of_str++) {} 
        if (this->size > 0) {
            char buffer[size];
            for (int i {}; i < size; i++) {
                buffer[i] = ptr[i];
            }
            ptr = std::make_unique<char[]> (size_of_str+size);
            for (int i {}; buffer[i] != '\0'; i++) {
                ptr[i] = buffer[i]; 
            }
        }
        else {
            ptr = std::make_unique<char[]> (size_of_str);
        }        
        for (int j {}; j < size_of_str; j++) {
            for (int i {size}; i > index; i--) {
                ptr[i] = ptr[i - 1];
            }
            ptr[index] = str[j];
            index++;
            length++;
            size++;
        }
    }
    
    void replace(const char* str, int replace_start = 0, int replace_end = 0)  {
        if (replace_start >= size || replace_start < 0) {
           throw OUT_OF_RANGE_EXCEPTION(size,replace_start,"REPLACE START");
        } 
        else if (replace_end >= size || replace_end < 0) {
            throw OUT_OF_RANGE_EXCEPTION(size,replace_end, "REPLACE END");
        }
        else if (size <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        else if (replace_start >= replace_end) {
            throw OUT_OF_RANGE_EXCEPTION(size,0,"REPLACE START GREATER THAN REPLACE END",1);
        }
        else {
            for (int i {replace_start},j{}; i < replace_end; i++,j++) {
                ptr[i] = str[j];
            }
        }
    }

    bool is_capital() {
        for (int i {}; i < size; i++) {
            if (ptr[i] < 65 || ptr[i] > 91) {
                return false;
            }
        }
        return true;
    }

    bool is_alphabetical() {
        for (int i {}; i < size; i++) {
            if (ptr[i] > 64 && ptr[i] < 91 || ptr[i] > 96 && ptr[i] < 123) {
                 return true;
            }
        }
        return false;
    }
    
    bool is_numerical() {
        for (int i {}; i < size; i++) {
            if (ptr[i] < 47 || ptr[i] > 57) {
                return false;
            }
        }
        return true;
    }
};

std::ostream &operator<< (std::ostream &os, const String &str) {
     for (int i {}; i < str.length; i++) {
         os << str.ptr[i];
     }
     return os;
}
