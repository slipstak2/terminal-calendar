
template<size_t... Indexes>
class DataView {
public:
    DataView(DataStorage& ds) : dataStorage(ds) {
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
    DataStorage& dataStorage;
};
