#include "Source.hpp"

Source::Source(SourceData *sourceData)
{
    if (sourceData != nullptr)
    {
        this->sourceData = sourceData;
    }
}

Source::~Source()
{
    if (this->sourceData != nullptr)
    {
        this->sourceData = nullptr;
    }
}
