#ifndef DATASTORE_H
#define DATASTORE_H

#include <map>
#include <string>
#include <any>
#include <stdexcept>

class DataStore
{
private:
    /**
     * Data storage.
     * Maps a name to any data type
     */
    std::map<std::string, std::any> m_data;

    /**
     * Lock for data
     */
    pthread_mutex_t m_lock;

public:
    DataStore();
    ~DataStore();

    /**
     * Puts the object in the data store
     */
    void put(std::string name, std::any obj);

    /**
     * Returns object from data store.
     * Returns NULL if not present
     */
    template <class T>
    T get(std::string name);
};

/* *********** Template declarations ************* */
template <class T>
T DataStore::get(std::string name)
{
    std::map<std::string, std::any>::iterator it;

    pthread_mutex_lock(&m_lock);
    it = m_data.find(name);

    if (it == m_data.end())
    {
        pthread_mutex_unlock(&m_lock);
        // not in map
        throw std::out_of_range("variable name " + name + " not in DataStore");
    }
    else
    {
        // fetch data and attach a type
        std::any data = m_data[name];
        pthread_mutex_unlock(&m_lock);

        T data_type = std::any_cast<T>(data);
        return data_type;
    }
    
}

#endif