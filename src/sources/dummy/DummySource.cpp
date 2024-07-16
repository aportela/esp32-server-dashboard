#include "DummySource.hpp"

DummySource::DummySource(void)
{
}

DummySource::~DummySource()
{
}

SourceDataCPU DummySource::getCurrentCPU(void)
{
}

SourceDataMemory DummySource::getCurrentMemory(void)
{
}

SourceDataNetwork DummySource::getCurrentNetwork(void)
{
}

SourceDataTemperature DummySource::getCurrentTemperature(void)
{
}

SourceData DummySource::getCurrent(SourceDataType entity)
{
    switch (entity)
    {
    case SDT_CPU:
        return (this->getCurrentCPU());
        break;
    case SDT_MEMORY:
        return (this->getCurrentMemory());
        break;
    case SDT_NETWORK:
        return (this->getCurrentNetwork());
        break;
    case SDT_TEMPERATURE:
        return (this->getCurrentTemperature());
        break;
    }
}
