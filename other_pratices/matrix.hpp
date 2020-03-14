#ifndef EFANNA_MATRIX_H
#define EFANNA_MATRIX_H
#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <iostream>

namespace efanna{

    template <typename T>
    class Matrix {
    private:
        size_t rows_, cols_;
        std::vector<const T*> row_pointers_;

    public:
        Matrix(const size_t rows, const size_t cols, const void*data):
        rows_(rows), cols_(cols)
        {
            size_t align_cols;

#ifdef __GNUC__
#ifdef __AVX__
        align_cols = (cols + 7)/8*8;
#else
#ifdef __SSE2__
        align_cols = (cols + 3)/4*4;

#else
        align_cols = cols;
#endif
#endif
#endif
        for (size_t i = 0; i < rows; i++)
        {
            row_pointers_.push_back(reinterpret_cast<const T*> (data) + (align_cols * i));
        }
        }
    
    size_t get_cols() const
    {
        return cols_;
    }

    size_t get_rows() const
    {
        return rows_;
    }

    const T* get_row(const size_t index) const
    {
        if (index >= rows)
        {
            throw std::runtime_error("index out of range");
        }
        return row_pointers_[index];
    }

    std::vector<std::pair<T, size_t>> brute_force_search
    (size_t idx, size_t k, Distance<T>* distance) const{
            printf("idx: %lu\n", idx);
            std::vector<std::pair<T, size_t>> result;

            for (size_t i = 0; i < rows_; i++)
            {
                result.push_back(std::make_pair(
                    distance->compare(get_row(i), get_row(idx), cols), i));

                ))
            }
            std:partial_sort(result.begin(), result.begin()+k, result.end());
            result.resize(k);
            return result;
        }
    };
}








#endif