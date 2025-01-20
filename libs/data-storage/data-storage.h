#pragma once

#pragma once
#include <tuple>
#include <vector>

#include "subtuple.h"



namespace exp1 {

    template<typename... Values>
    class DataStorage {
    public:

        template<size_t... Indeces>
        class DataView {
        public:
            template<size_t... SubIndexes>
            class DataSubView {
            public:
                DataSubView(DataView& dv) : dataView(dv) {
                    indexes.resize(dv.Size());
                    for (size_t i = 0; i < indexes.size(); ++i) {
                        indexes[i] = i;
                    }
                }
                auto Get(size_t num) {
                    return subtuple<SubIndexes...>(dataView.Get(indexes[num]));
                }

            protected:
                std::vector<size_t> indexes;
                DataView& dataView;
            };
        public:
            DataView(DataStorage& ds) : dataStorage(ds) {
                indexes.resize(ds.Size());
                for (size_t i = 0; i < indexes.size(); ++i) {
                    indexes[i] = i;
                }
            }
            template<size_t... SubIndeces>
            auto SubView() {
                return DataSubView<SubIndeces...>(*this);
            }

            size_t Size() {
                return indexes.size();
            }

            auto Get(size_t num) {
                return subtuple<Indeces...>(dataStorage.Get(indexes[num]));
            }
            void Print() {
                for (int num = 0; num < indexes.size(); ++num) {
                    print_tuple_pretty(Get(num));
                }

            }


        protected:
            std::vector<size_t> indexes;
            DataStorage& dataStorage;
        };

    public:
        void Add(std::tuple<Values... >&& row) {
            rows.emplace_back(row);
        }
        size_t Size() {
            return rows.size();
        }

        const std::tuple<Values... >& Get(size_t idx) {
            return rows[idx];
        }

        void Print() {
            for (size_t idx = 0; idx < rows.size(); ++idx) {
                //print_tuple(Get(idx));
                print_tuple_pretty(Get(idx));
            }
        }

        template<size_t... Indexes>
        auto View() {
            return DataView<Indexes...>(*this);
        }

    protected:
        std::vector<std::tuple<Values... >> rows;
    };

} // exp1