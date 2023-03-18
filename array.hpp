#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <exception>
#include <string>

// This Exception is thrown when the array is found to be empty 
class EMPTY_ARRAY_EXCEPTION : public std::exception {
private:
    int length;
    int size;
    std::string error_message;

public:
    EMPTY_ARRAY_EXCEPTION(int length, int size) : length(length), size(size) {
        if (size <= 0) {
            error_message = "ARRAY IS EMPTY. SIZE IS: " + std::to_string(size);
        }
        else if (length <= 0) {
            error_message = "ARRAY IS EMPTY. LENGTH IS: " + std::to_string(length);
        }
        else {
            error_message = "ARRAY IS EMPTY.";
        }
    }

    virtual const char* what() const noexcept {
        return error_message.c_str();
    }
};
 
// This Exception is thrown when some index or as such goes out of range/out of bounds 
class OUT_OF_RANGE_EXCEPTION : public std::exception {
    private:
      int size;
      int index;
      std::string name_of_the_item_that_goes_out_of_range;
      std::string error_message;
      bool special_condition;

    public:
      OUT_OF_RANGE_EXCEPTION(int size, int index,std::string name_of_the_item_that_goes_out_of_range = "INDEX", bool special_condition = 0) 
      : size{size}, index{index}, name_of_the_item_that_goes_out_of_range{name_of_the_item_that_goes_out_of_range}, special_condition{special_condition} {
        if (special_condition == 1) {
           error_message = name_of_the_item_that_goes_out_of_range; 
        }
        else if (index >= size && special_condition == 0) {
           error_message = name_of_the_item_that_goes_out_of_range + " GOES BEYOND THE SIZE OF THE ARRAY. SIZE OF THE ARRAY: " + std::to_string(size) + " " + name_of_the_item_that_goes_out_of_range + ": " + std::to_string(index);
        }
        else if (index < 0 && special_condition == 0) {
           error_message = name_of_the_item_that_goes_out_of_range + " GOES INTO NEGATIVE VALUES. " + name_of_the_item_that_goes_out_of_range + ": " + std::to_string(index);
        }
      }

     virtual const char* what() const noexcept {
        return error_message.c_str();
     }
};

// Array class.
template<class T, size_t Size = 0>
class Array { 
  
  protected:

    int size {}; // Refers to the maximum size of the current array.
    int length {}; // Refers to the number of index's currently filled in the array
    std::unique_ptr<T[]> ptr; // Pointer to the array in the heap.
    
    // template<size_t Size_1,size_t lhs_size>
    // Array<T,Size_1+lhs_size> concat(Array<T,lhs_size> arr) {
    //          int size_copy = this->size;
    //          this->resize(this->size+arr.size, arr[0]);
    //          this->size = size_copy;
    //          Array<T,Size_1+lhs_size> array;
    //          for (int i {}; i < size; i++) {
    //              array[i] = this->at(i);
    //          }
    //          for (int i {this->size}, j {0}; i < this->size+arr.size; i++,j++) {
    //              array[i] = arr[j];
    //          }
    //          return array;
    // }

   public:
    // Returns a Raw pointer pointing to a raw array that is a exact copy to the Array class array
    T* get_array() const {
        T* copy_ptr = new T[size];
        for (int i {}; i < size; i++) {
            copy_ptr[i] = ptr[i];
        }
        return copy_ptr;
    }
    // Returns the size
    constexpr const int get_size() const noexcept { 
        return size;
    }
    // Returns length or you could say returns the number of elements filled in the array
    constexpr const int get_length() const noexcept { 
        return length;
    }
    // Use Square brackets to access elements
    T& operator[] (const int index) const { 
        if (size <= 0) {
           throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        else if (index < 0 || index >= size) {
             throw OUT_OF_RANGE_EXCEPTION(size,index);
        }
        return ptr[index]; 
    }
    
    // template<size_t Size_1>
    // void operator= (const Array<T,Size_1> &rhs) { // Assign a array class object to a different array class object
    //     if (int(Size_1) > this->size) {
    //         this->ptr = std::make_unique<T[]>(rhs.get_size());
    //     }
    //     this->size = rhs.get_size();
    //     for (int i {}; i < size; i++) {
    //         this->at(i) = rhs[i];            
    //     }
    //     this->length = rhs.get_length();
    // }

    // template <size_t Size_1>
    // void operator=(Array<T, Size>&& other) noexcept {
    //     if (this != &other) {
    //         for (size_t i = 0; i < Size; ++i) {
    //             ptr[i] = std::move(other.ptr[i]);
    //         }
    //         other.clear();
    //     }
    // }

    
    template<size_t Size_1>
    bool operator== (const Array<T,Size_1> &rhs) {
        if (this->get_size() != rhs.get_size() || this->get_length() != rhs.get_length()) {
            return false;
        }
        for (int i {}; i < rhs.get_size(); i++) {
            if (this->at(i) != rhs[i]) {
                return false;
            }
        }
        return true;  
}

    template<size_t Size_1>
    bool operator!= (const Array<T,Size_1> &rhs) {
        auto Lambda = [this, &rhs] () {
            if (this->get_size() != rhs.get_size() || this->get_length() != rhs.get_length()) {
                return true;
            }
            for (int i {}; i < rhs.get_size(); i++) {
                if (this->at(i) != rhs[i]) {
                    return true;
                }
            }
            return false;  
        };
        return Lambda();
    }

    // Had planned to make these but ended up getting stuck at a error. You could try to solve it.

    // template<size_t rhs_size>
    // Array<T,rhs_size + (lhs size(Can't access it in return value))> operator+ (const Array<T,rhs_size> &rhs) {
    //          Array<T,lhs_size> array;
    //          for (int i {}; i < size; i++) {
    //              array[i] = this->at(i);
    //          }
    //          for (int i {this->size}, j {0}; j < rhs.get_size(); i++,j++) {
    //              array[i] = rhs[j];
    //          }
    //          return array;
    // };

    //     void operator+= (Array<T> rhs) {
    //          this = this + rhs;
    //     }

    // This functions takes a function as its arguement whose parameter list contains a variable of datatype T called item and another variable whose datatype is int whose job is to point to the index of the array. The function runs a for loop that will run from 0 to size of the array. Inside the loop it passes the item and the current index to the function in the parameter list. If no function is passed it uses a default display method. 
    void display(const std::function<void(T,int)> func = [] (T item, int index) {std::cout << "Index: " << index << " Contents of the index: " << item << "\n";}) const { 
        if (length == 0 || size == 0) {
           throw EMPTY_ARRAY_EXCEPTION(this->size, this->length);
        }
        for (int i {}; i < size; i++) {
            func(ptr[i], i);
        }
        std::cout << "\n";
    }
    // Increases the size and length of the array by one and add's a element to the back of the array.
    void push_back(T item = T{}) noexcept { 
        ++length;
        ++size;
        std::unique_ptr<T[]> temp = std::make_unique<T[]>(size - 1);
        std::copy(ptr.get(), ptr.get() + size - 1, temp.get());
        ptr = std::make_unique<T[]>(size);
        std::copy(temp.get(), temp.get() + size - 1, ptr.get());
        ptr[size-1] = item;   
    }
    // Swap's the item at the index with the item passed in the function.
    void replace(int index, T item) { 
        if (index >= size || index < 0) {
           throw OUT_OF_RANGE_EXCEPTION(size,index);
        } 
        else if (size <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        else {
            ptr[index] = item;
        }
    }
    // Accepts a list of items and replaces the elements in the calling object starting from replace star till replace end with the elements inside the list of items
    virtual void replace(const std::initializer_list<T> item_list, int replace_start, int replace_end) { 
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
            auto item_list_iterator = item_list.begin();
            for (int i {replace_start}; i < replace_end; i++) {
                ptr[i] = *item_list_iterator;
                item_list_iterator++;
            }
        }
    }
    // Increases the size for another element and moves everything starting for the variable "index" to the right to make space for the item and then assigns the item to the index element
    void insert(const int index, const T item) { 
        if (index < 0 || index > size) {
            throw OUT_OF_RANGE_EXCEPTION(size,index);
            return;
        }
        else if (size <= 0) {
             throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        else if (size == length) {
            resize(size + 1);
        }
        for (int i = size - 1; i > index; i--) {
            ptr[i] = ptr[i - 1];
        }
        ptr[index] = item;
        length++;
    }

    // Returns the element at the first index of the array
    T& front() const { 
      if (size <= 0) {
         throw EMPTY_ARRAY_EXCEPTION(size,length);
      }
      return ptr[0];
    }

    // Returns the element at the last index of the array
    T& back() const { 
      if (size <= 0) {
         throw EMPTY_ARRAY_EXCEPTION(size,length);
      }
      return ptr[size-1];
    }

     // Manipulates the size of the array to the liking of the user
    void resize(const int new_size, const T default_value = T{}) {
        if (new_size == size) {
            return; // No need to change the size because its the same
        }
        if (new_size < 0) {
            throw OUT_OF_RANGE_EXCEPTION(size,new_size,"NEW SIZE");
            return;
        }
        if (new_size > size) { // Assign empty elements with default value
            std::unique_ptr<T[]> temp = std::make_unique<T[]>(size);
            std::copy(ptr.get(), ptr.get() + size, temp.get());
            ptr = std::make_unique<T[]>(new_size);
            std::copy(temp.get(), temp.get() + std::min(size, new_size), ptr.get());
            for (int i {size+1}; i < new_size; i++) {
                ptr[i] = default_value;
            }
            size = new_size;
            length = new_size;
        }
        else {
        std::unique_ptr<T[]> temp = std::make_unique<T[]>(new_size);
        std::copy(ptr.get(), ptr.get() + new_size, temp.get());
        ptr = std::make_unique<T[]>(new_size);
        std::copy(temp.get(), temp.get() + std::min(size, new_size), ptr.get());
        size = new_size;
        length = new_size;
        }
    }
    // Searches for the item using simple linear search and returns the index pointing to the item or returns -1 if it is not found
    virtual const int find(const T item) const { 
        if (size <= 0 || length <= 0) {
           throw EMPTY_ARRAY_EXCEPTION(size,length); 
        }
        for (int i {}; i < size; i++) {
            if (ptr[i] == item) {
                return i;
            }
        }
        return -1;
    }
    // if the passed argument is false (0) then it sorts in ascending, if its true (1) then it sorts in descending and by default its ascending
    void sort(const bool &asc_or_desc = 0) { 
        T smallest;
        T largest;
        if (size <= 0 || length <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        else if (size == 1) {
            return;
        }
        if (asc_or_desc == 0) { // Ascending sort
            for (int i {}; i < size; i++) {
                smallest = ptr[i];
                int min_idx = i;
                for (int j {i+1}; j < size; j++) {
                    if (ptr[j] < smallest) {
                        smallest = ptr[j];
                        min_idx = j;
                    }
                }
                ptr[min_idx] = ptr[i];
                ptr[i] = smallest;
            }
        } else { // Descending sort
            for (int i {}; i < size; i++) {
                largest = ptr[i];
                int max_idx = i;
                bool smallest_change {false};
                for (int j {i+1}; j < size; j++) {
                    if (largest < ptr[j]) {
                        largest = ptr[j];
                        max_idx = j;
                    } else {
                        smallest_change = true;
                        smallest = ptr[j];
                    }
                }
                if (smallest_change == false) {
                    smallest = ptr[i];
                }
                ptr[max_idx] = ptr[i];
                ptr[i] = largest;
            }
        }
    }

    // Reverses the array starting from reverse_start to reverse_end. By default if no arguments are passed then it reverses the entire array. If only reverse start is given than reverse end is set to the size of the array. 
    void reverse(const unsigned int reverse_start = 0, unsigned int reverse_end = -1) { 
        if (reverse_end == -1) {
            reverse_end = size;
        }
        else if (reverse_start > size || reverse_start > length || reverse_start > reverse_end) {
            throw OUT_OF_RANGE_EXCEPTION(size,reverse_start,"REVERSE START");
        }
        else if (reverse_end < reverse_start) {
            throw OUT_OF_RANGE_EXCEPTION(size,reverse_end,"REVERSE END");
        }
        if (size == 1 || reverse_end - reverse_start == 0 || reverse_end - reverse_start == 1) {
            return;
        }
        //Odd
        if (((reverse_end+1)-(reverse_start)%2) != 0) {
            for(unsigned int i {reverse_start}, j {reverse_end}; j >= (reverse_end+1)/2;j--) {
                char temp = ptr[j];
                ptr[j] = ptr[i];
                ptr[i] = temp;
                ++i;
            }
        return;
       }

       //Even
       for (unsigned int i {reverse_start}, j {reverse_end}; j >= (reverse_end/2)+1; i++, j--) {
           char temp = ptr[j];
           ptr[j] = ptr[i];
           ptr[i] = temp;
        }
    }
    // Another method for accessing elements if you're used to using vectors.
    T& at(const int &index) const { 
        if (size <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        if (index < 0 || index >= this->size) {
            throw OUT_OF_RANGE_EXCEPTION(size,index);
        }
        return ptr[index];
    }
    
    // Empties the array
    void clear() noexcept { 
        size = 0;
        length = 0;
        ptr = std::make_unique<T[]> (0);
    }

    // Removes the last index
    void pop_back() noexcept {  
        --size;
        --length;
        std::unique_ptr<T[]> temp = std::make_unique<T[]>(size);
        std::copy(ptr.get(), ptr.get() + size, temp.get());
        ptr = std::make_unique<T[]>(size);
        std::copy(temp.get(), temp.get() + size, ptr.get());        
    }

    // Removes specified index
    void remove (const int &index) { 
        if (index < 0 || index > size) {
           throw OUT_OF_RANGE_EXCEPTION(size,index);
        }
        else if (length == 0) {
           throw OUT_OF_RANGE_EXCEPTION(size,length);
        }
        else if (size <= 0) {
           throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        for (int i {index}; i < size; i++) {
           ptr[i] = ptr[i+1];
        }
        resize(size-1);
        length--;
    }
    // Removes index's starting from start index (The first parameter passed) to end index (The last parameter passed)
    void remove(int start_index,int end_index) {
        if (start_index < 0 || start_index > size) {
           throw OUT_OF_RANGE_EXCEPTION(size,start_index);
        }
        if (start_index < 0 || start_index > size) {
           throw OUT_OF_RANGE_EXCEPTION(size,start_index);
        }
        else if (length == 0) {
           throw OUT_OF_RANGE_EXCEPTION(size,length);
        }
        else if (size <= 0) {
           throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        for (int iteration_counter {}; iteration_counter < end_index - start_index;){
            for (int i {start_index}; i < size; i++) {
                ptr[i] = ptr[i+1];
            }
            ++iteration_counter;
            --size;
            --length;
        }
        char copy[size];
        for (int i {}; i < size; i++) {
            copy[i] = ptr[i];
        }
        ptr = std::make_unique<char[]> (size);
        for (int i {}; i < size; i++) {
            ptr[i] = copy[i];
        }  
    }
    // Returns a new Array containing elements starting from start index to end index
    template<size_t Size_1>
    Array<T,Size_1> slice(const int startIndex,const int endIndex) const { 
        if (size <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(length, size);
        } else if (startIndex < 0 || startIndex > size - 1) {
            throw OUT_OF_RANGE_EXCEPTION(size, startIndex, "START INDEX");
        } else if (endIndex < 0 || endIndex > size - 1) {
            throw OUT_OF_RANGE_EXCEPTION(size, endIndex, "END INDEX");
        } else if (startIndex >= endIndex) {
            throw OUT_OF_RANGE_EXCEPTION(size, endIndex, "END INDEX must be greater than START INDEX",1);
        }

    Array<T,Size_1> result_arr;  
    for (int i {startIndex}, j {0}; (endIndex == result_arr.size) ? i < endIndex : i <= endIndex; i++, j++) {
        result_arr[j] = this->ptr[i]; 
    } 
    result_arr.length = Size_1;
    return result_arr;
}
    
    // This function removes all duplicates from the array.
    void unique() { 
        if (size <= 0) {
           throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
         for (int i {}; i < size; i++) {
             for (int j {i+1}; j < size; j++) {
                 if (ptr[i] == ptr[j]) {
                    remove(j);
                 }
             }
         }
    }

    // Counts the number of times the item element appears in the array.
    const int count (const T item) const { 
        if (size <= 0) {
           throw EMPTY_ARRAY_EXCEPTION(size,length); 
        }
        int count {};
         for (int i {}; i < size; i++) {
            if (ptr[i] == item) {
                ++count;
            }
         }
         return count;
    }

    // Returns true if the the array empty. Else returns false
    const bool is_empty() const noexcept { 
         if (size == 0) {
            return true;
         }
         return false;
    }
    
    // Swaps the elements at the index's mentioned by the user
    void swap(const unsigned int index_1, const unsigned int index_2) { 
         if (index_1 < 0 || index_1 > size) {
            throw OUT_OF_RANGE_EXCEPTION(size,index_1,"FIRST INDEX");
            return;
         }
         else if (index_2 < 0 || index_2 > size) {
            throw OUT_OF_RANGE_EXCEPTION(size,index_2,"SECOND INDEX");
            return;
         }
         else if (size <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(size,length);
         }
         T temp = ptr[index_1]; 
         ptr[index_1] = ptr[index_2];
         ptr[index_2] = temp;
    }

    // Returns the greatest element in the array
    const int max() const { 
        T largest = this->at(0);
        if (size <= 0) {
            throw EMPTY_ARRAY_EXCEPTION(size,length);
        }
        for (int i {}; i < size; i++) {
            if (largest < this->at(i)) {
                largest = this->at(i);
            }
        }
        return find(largest);
    } 

    // Fills the array with the specified item starting from fill_start to fill_end
    void fill(const T item, const signed int fill_start = 0, signed int fill_end = 0) { 
        if (length == size) {
            std::cerr << "Array is full\n";
            return;
        }
        if (fill_end == 0) {
            fill_end = size;
        }
        if (size < 0) {
            throw EMPTY_ARRAY_EXCEPTION(size,length);
         }
        if (fill_start > size || fill_start < 0) {
            throw OUT_OF_RANGE_EXCEPTION(size,fill_start, "FILL START");
        }
        if (fill_end > size || fill_end < 0) {
            throw OUT_OF_RANGE_EXCEPTION(size,fill_end,"FILL END");
        }
        if (fill_end - fill_start == 0 ||fill_end - fill_start == 1) {
            return;
        }
         for (int i {fill_start}; i < fill_end; i++) {
             ptr[i] = item;
         }
         length += fill_end - fill_start;
    }
    
    // Takes a function as a parameter, this function will be refered to as func. Func returns a boolean and takes a variable of type T (int, double). The body of func will most likely use conditions to filter out unwanted elements and get a new filtered array all the unwanted elements gone.
    template<size_t Size_1>
    Array<T,Size_1> filter(std::function<bool(T)> func) const { 
             Array<T,Size_1> result;
             int real_size {};
             for (int i {}, j {}; i < size; i++) {
                 if (func(at(i)) == 1) {
                    result[j] = at[i];
                    ++real_size;
                    ++j;
                 }   
             }
             return result;
    }

    // returns the smallest element in the array 
    int min() const {
        T smallest = this->at(0);
        if (size == 1 && length == 1) {
         return at(0);
      }
        else if (size == 1 && length == 0) {
              return -1;
        }
      for (int i {1}; i < size; i++) {
          if (this->at(i) < smallest) {
             smallest = this->at(i);
          }
      }
      return find(smallest);
    }

    // Normal Constructor
    Array(std::initializer_list<T> init_list) 
    : size{Size} {
        ptr = std::make_unique<T[]> (size);
        if (init_list.size() > size) {
            throw OUT_OF_RANGE_EXCEPTION(size,init_list.size(), "INITIALIZER LIST SIZE");
        }
        std::copy(init_list.begin(), init_list.end(), ptr.get());
        if (init_list.size() < size) {
            length = init_list.size();
            for (long unsigned int i {init_list.size()-1}; i < size; i++) {
                ptr[i] = T{};
            }
        }
        else if (init_list.size() == size) {
             length = size;
        }
    }

    // Copy constructor
    Array(const Array& source) 
    : size{source.size}, length{source.length} {
        ptr = std::make_unique<T[]>(size);
        std::copy(source.ptr.get(), source.ptr.get() + size, ptr.get());
    }

    // Move constructor
    template<size_t Size_1>
    Array(Array<T, Size_1>&& rhs) noexcept
    {
        ptr = std::move(rhs.ptr);
        size = rhs.size;
        length = rhs.length;
        rhs.size = 0;
        rhs.length = 0;
    }

    // Empty array
    Array() 
    : size{int(Size)}, length{0}{
      if (size > 0) {
        ptr = std::make_unique<T[]> (size);
      }
      else {
        ptr = nullptr;
      }
    }       
};

