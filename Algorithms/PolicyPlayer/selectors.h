#ifndef SELECTORS_H
#define SELECTORS_H

#include "field.h"
#include <functional>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace procon::selector{

    namespace np = boost::python::numpy;
    using func_type = std::function<std::vector<int>(const Field&, const np::ndarray&)>;

    func_type argmax_selector = [](const Field&, const np::ndarray& arr){
        int agent_count = arr.get_shape()[0];
        std::vector<int> ret_vec(agent_count);
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            float max_policy_value = 0;
            int max_policy_index = 0;
            for(int move_index = 0; move_index < 8; ++move_index){
                float policy_value = boost::python::extract<float>(arr[agent_index][move_index]);
                if(max_policy_value < policy_value){
                    max_policy_value = policy_value;
                    max_policy_index = move_index;
                }
            }
            ret_vec[agent_index] = max_policy_index;
        }
        return ret_vec;
    };
}


#endif // SELECTORS_H
