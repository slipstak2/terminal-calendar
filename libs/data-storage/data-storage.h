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
#include "data-view.hpp"

        template<size_t... Indexes2>
        static auto CreateDataView() {
            return DataView<Indexes2...>(DataStorage());
        }

        template<typename... Values2>
        static auto CreateDataStorage() {
            return DataStorage<Values2...>()
        }

        template<typename DataViewT = decltype(CreateDataView())>
        class DataSet {
        public:
            DataSet(DataViewT dataView) : dv(dataView)
            {}

            auto Get(size_t idx) {
                return dv.Get(idx);
            }

        protected:
            DataViewT dv;
        };
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

        template<size_t... Indexes>
        auto View() {
            return DataView<Indexes...>(*this);
        }

        template<size_t... Indexes>
        auto Set() {
            return DataSet(View<Indexes...>());
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

} // exp1