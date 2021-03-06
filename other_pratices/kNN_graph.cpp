#pragma once

#include <vector>
#include <limits>
#include <random>
#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <thread>
#include <array>
#include <omp.h>


namespace nng{

    template <class DataT, class SimT>
    
    struct NNDescent
    {
    public:

        typedef DataT data_type;
        typedef SimT sim_type;
        typedef typename sim_type::value_type sim_value_type;

        struct node_type
        {
            node_type(const data_type& data) : data(data) {}
            node_type():data(){}
            data_type data;
        };

        struct  nbd_elem_type
        {
            nbd_elem_type(): node_id(), similarity(std::numeric_limits<sim_value_type>::min()){}
            nbd_elem_type(const std::size_t node_id, const sim_value_type similarity): 
                node_id(node_id), similarity(similarity) {}
            std::size_t node_id;
            sim_value_type similarity;
        };
        
        struct comp_similartiy
        {
            bool operator()(const nbd_elem_type& e1, const nbd_elem_type& e2) const
            {return e1.similarity > e2.similarity}
        };

        typedef std::<nbd_elem_type> nbd_type;

        struct nearer 
        {
            bool operator()(const nbd_elem_type& elem1, const nbd_elem_type& elem2) const
            {return elem1.similarity > elem2.similarity}
        }

        typedef std::mt19937 rand_type;

        public:

        NNDescent(
            const uint32_t K,
            const double rho = 0.5,
            const double perturb_rate = 10,
            const std::size_t num_random_join = 10,
            const rand_type& rand = rand_type(0),
            const sim_type& sim = sim_type()
            )
        : K(K), num_random_join(num_random_join),
        rho(rho), perturb_rate(perturb_rate),
        nodes(), nbds(), rand(rand), sim(sim),
        checked() {}

        NNDescent(
            const std::size_t K,
            const std::vector<data_type>& datas,
            const double rho = 0.5,
            const double perturb_rate = 0,
            const rand_type& rand = rand_type(0),
            const sim_type& sim = sim_type()
        )
        : K(K), nodes(), num_random_join(num_random_join),
        rho(rho), perturb_rate(perturb_rate), nbds(), rand(rand),
        sim(sim), checked()
        {init_graph(datas)}

        public:
        void init_graph(const std::vector<data_type)& datas)
        {
            if (datas.size() == 0)
            return ;

            this->clear();

            for (const data_type& data: datas)
            {
                this->nodes.push_back(node_type(data));
            }

            this->init_random_nbds();
        }

        std::size_t update()
        {
            if(this->nodes.size() < 2 || this->K == 0)
                return 0;
            
            const std::size_t N = this->nodes.size();

            std::vector<nbd_type> old_nbds(N), new_nbds(N), old_rnbds(N), new_rnbds(N);

            for (std::size_t i = 0; i < N; i++)
            {
                this->prep_nbd(i, new_nbds[i], old_nbds[i]);
            }

            for(std::size_t i = 0; i < N; i++)
            {
                const auto & new_nbd(new_nbds[i]), &old_nbd(old_nbds[i]);
                for (const nbd_elem_type& elem: old_nbd)
                {
                    assert(elem.node_id != i);
                    old_rnbds[elem.node_id]
                }
            }


        }

        

        
    };
};