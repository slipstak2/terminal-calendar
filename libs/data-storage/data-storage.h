#pragma once

#pragma once
#include <tuple>
#include <vector>

#include "subtuple.h"
#include "printer.h"


namespace exp1 {

    template<typename... Values>
    class DataStorage {

    public:
//#include "data-view.hpp"

    public:
        void Add(std::tuple<Values... >&& row) {
            rows.emplace_back(std::move(row));
        }

        size_t Size() {
            return rows.size();
        }

        const std::tuple<Values... >& Get(size_t idx) {
            return rows[idx];
        }

        void Print(std::ostream& stream = std::cout, const std::string& separator = "\n") {
            if (rows.size()) {
                print_tuple_pretty(Get(0), stream);
            }
            for (size_t idx = 1; idx < rows.size(); ++idx) {
                stream << separator;
                print_tuple_pretty(Get(idx), stream);
            }
        }

    protected:
        std::vector<std::tuple<Values... >> rows;
    };

    template<typename DataStorageT, size_t... Indexes>
    class DataView {
    public:
        DataView(DataStorageT& ds) : dataStorage(ds) {
            indexes.resize(ds.Size());
            for (size_t i = 0; i < indexes.size(); ++i) {
                indexes[i] = i;
            }
        }

        size_t Size() {
            return indexes.size();
        }

        auto Get(size_t num) {
            return subtuple<Indexes...>(dataStorage.Get(indexes[num]));
        }

        void Print(std::ostream& stream = std::cout, const std::string& separator = "\n") {
            if (indexes.size()) {
                print_tuple_pretty(Get(0), stream);
            }
            for (size_t num = 1; num < indexes.size(); ++num) {
                stream << separator;
                print_tuple_pretty(Get(num), stream);
            }
        }

    protected:
        std::vector<size_t> indexes;
        DataStorageT& dataStorage;
    };

    template<typename DataViewT, typename DataStorageT>
    class DataSet {
    public:
        DataSet(DataViewT dataView) : dv(dataView)
        {}

        auto Get(size_t idx) {
            return dv.Get(idx);
        }

    protected:
        DataViewT dv;
        DataStorageT storage;
    };
} // exp1