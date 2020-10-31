#include "DataStore.h"

#include <map>
#include <string>
#include <any>

DataStore::DataStore()
{
    pthread_mutex_init(&m_lock, NULL);
}

DataStore::~DataStore()
{
    pthread_mutex_destroy(&m_lock);
}

void DataStore::put(std::string name, std::any obj)
{
    // TODO locking on a per-variable basis - this currently locks all variable names
    pthread_mutex_lock(&m_lock);
    m_data[name] = obj;
    pthread_mutex_unlock(&m_lock);
}
