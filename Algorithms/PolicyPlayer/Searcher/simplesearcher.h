#ifndef SIMPLESEARCHER_H
#define SIMPLESEARCHER_H

#include "PolicyPlayer/Searcher/searcherwrapper.h"

class SimpleSearcher : public SearcherWrapper
{
    using SearcherWrapper::SearcherWrapper;
public:
    boost::python::numpy::ndarray search(const procon::Field& field) override;
};

#endif // SIMPLESEARCHER_H
