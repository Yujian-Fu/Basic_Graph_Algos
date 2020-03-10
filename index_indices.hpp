#ifndef EFANNA_INIT_INDICES_H_
#define EFANNA_INIT_INDICES_H_
#include "base_index.hpp"
#include "kdtreeub_index.hpp"
#include "hashing_index.hpp"

namespace efanna{

    template <template<typename> class Index, typename DataType>
    inline InitIndex<DataType> *
    create_index_by_type(const init_algorithm index_type,
    const Matrix<DataType>& dataset, const IndexParams& params,
    const Distance<DataType> * d)
    {
        
    }
}