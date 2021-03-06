////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     SymbolTable.h (emitters)
//  Authors:  Umesh Madan
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EmitterException.h"

#include <unordered_map>

namespace ell
{
namespace emitters
{
    ///<summary>A table for managing compiler symbols: named values</summary>
    template <class ValueType, ValueType DefaultValue = ValueType(0)>
    class SymbolTable
    {
    public:
        ///<summary>A symbol table contains symbol values</summary>
        using SymbolValue = std::pair<const std::string, ValueType>;

        SymbolTable() = default;

        /// <summary> Construct a symbol table from a list of symbols. </summary>
        ///
        /// <param name="values"> An initializer list of values. </param>
        SymbolTable(std::initializer_list<SymbolValue> values);

        /// <summary> Get a symbol's value from a given name, else return the default. </summary>
        ///
        /// <param name="name"> The name of the symbol. </param>
        ///
        /// <returns> The symbol associated with the name. </returns>
        ValueType Get(const std::string& name) const;

        /// <summary> Adds a new symbol ot the symbol table. </summary>
        ///
        /// <param name="name"> The name of the symbol. </param>
        /// <param name="value"> The value of the symbol. </param>
        void Add(const std::string& name, ValueType value);

        /// <summary> Gets a unique name from a prefix. </summary>
        ///
        /// <param name="namePrefix"> The name prefix. </param>
        ///
        /// <returns> The unique name. </returns>
        std::string GetUniqueName(const std::string& namePrefix) const;

        /// <summary> Returns true if the table contains a symbol with a given name. </summary>
        ///
        /// <param name="name"> The name of the symbol. </param>
        ///
        /// <returns> true if the symbol is in this table, false if not. </returns>
        bool Contains(const std::string& name) const;

        /// <summary> Remove a symbol with a given name from the symbol table. </summary>
        ///
        /// <param name="name"> The symbol name. </param>
        void Remove(const std::string& name);

        /// <summary> Erase all of the entires from the symbol table. </summary>
        void Clear();

    private:
        std::unordered_map<std::string, ValueType> _map;
    };
} // namespace emitters
} // namespace ell

#pragma region implementation

namespace ell
{
namespace emitters
{
    template <typename ValueType, ValueType DefaultValue>
    SymbolTable<ValueType, DefaultValue>::SymbolTable(std::initializer_list<SymbolValue> values) :
        _map(values)
    {
    }

    template <typename ValueType, ValueType DefaultValue>
    ValueType SymbolTable<ValueType, DefaultValue>::Get(const std::string& name) const
    {
        ValueType value = DefaultValue;
        auto search = _map.find(name);
        if (search != _map.end())
        {
            value = search->second;
        }
        return value;
    }

    template <typename ValueType, ValueType DefaultValue>
    void SymbolTable<ValueType, DefaultValue>::Add(const std::string& name, ValueType value)
    {
        if (_map.count(name) > 0)
        {
            throw EmitterException(EmitterError::duplicateSymbol);
        }
        _map[std::move(name)] = std::move(value);
    }

    template <typename ValueType, ValueType DefaultValue>
    std::string SymbolTable<ValueType, DefaultValue>::GetUniqueName(const std::string& namePrefix) const
    {
        // return namePrefix;
        int index = 0;
        while (true)
        {
            auto name = namePrefix + "_" + std::to_string(index);
            if (!Contains(name))
            {
                return name;
            }
            ++index;
        }
    }

    template <typename ValueType, ValueType DefaultValue>
    bool SymbolTable<ValueType, DefaultValue>::Contains(const std::string& name) const
    {
        return (Get(name) != DefaultValue);
    }

    template <typename ValueType, ValueType DefaultValue>
    void SymbolTable<ValueType, DefaultValue>::Remove(const std::string& name)
    {
        auto search = _map.find(name);
        if (search != _map.end())
        {
            _map.erase(search);
        }
    }

    template <typename ValueType, ValueType DefaultValue>
    void SymbolTable<ValueType, DefaultValue>::Clear()
    {
        _map.clear();
    }
} // namespace emitters
} // namespace ell

#pragma endregion implementation
