#include "simplesearcher.h"

boost::python::numpy::ndarray SimpleSearcher::search(const procon::Field& field){
    return evaluator->calcPolicy(field);
}
