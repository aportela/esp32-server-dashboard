#include "Source.hpp"

Source::Source(SourceData *sourceData) : sourceData(sourceData)
{
}

Source::~Source()
{
    this->sourceData = nullptr;
}
