#pragma once

#define VIRTUAL_DECLARATIONS_VIEW                                                                                       \
    virtual DataViewPtr View() = 0;                                                                                     \
                                                                                                                        \
    virtual DataViewPtr View(size_t fn0) = 0;                                                                           \
    virtual DataViewPtr View(size_t fn0, size_t fn1) = 0;                                                               \
    virtual DataViewPtr View(size_t fn0, size_t fn1, size_t fn2) = 0;                                                   \
                                                                                                                        \
    virtual DataViewPtr View(const std::string_view fn0) = 0;                                                           \
    virtual DataViewPtr View(const std::string_view fn0, const std::string_view fn1) = 0;                               \
    virtual DataViewPtr View(const std::string_view fn0, const std::string_view fn1, const std::string_view fn2) = 0;


#define OVERRIDE_DECLARATIONS_VIEW                                                                                      \
    DataViewPtr View() override;                                                                                        \
                                                                                                                        \
    DataViewPtr View(size_t fn0) override;                                                                              \
    DataViewPtr View(size_t fn0, size_t fn1) override;                                                                  \
    DataViewPtr View(size_t fn0, size_t fn1, size_t fn2) override;                                                      \
                                                                                                                        \
    DataViewPtr View(const std::string_view fn0) override;                                                              \
    DataViewPtr View(const std::string_view fn0, const std::string_view fn1) override;                                  \
    DataViewPtr View(const std::string_view fn0, const std::string_view fn1, const std::string_view fn2) override;


#define DEFINITIONS_VIEW(CONTAINER)                                                                                     \
    DataViewPtr CONTAINER::View() {                                                                                     \
        return DataView::Create(shared_from_this());                                                                    \
    }                                                                                                                   \
    DataViewPtr CONTAINER::View(size_t fn0) {                                                                           \
        return DataView::Create(shared_from_this(), fn0);                                                               \
    }                                                                                                                   \
    DataViewPtr CONTAINER::View(size_t fn0, size_t fn1) {                                                               \
        return DataView::Create(shared_from_this(), fn0, fn1);                                                          \
    }                                                                                                                   \
    DataViewPtr CONTAINER::View(size_t fn0, size_t fn1, size_t fn2) {                                                   \
        return DataView::Create(shared_from_this(), fn0, fn1, fn2);                                                     \
    }                                                                                                                   \
    DataViewPtr CONTAINER::View(const std::string_view fn0) {                                                           \
        return DataView::Create(shared_from_this(), fn0);                                                               \
    }                                                                                                                   \
    DataViewPtr CONTAINER::View(const std::string_view fn0, const std::string_view fn1) {                               \
        return DataView::Create(shared_from_this(), fn0, fn1);                                                          \
    }                                                                                                                   \
    DataViewPtr CONTAINER::View(const std::string_view fn0, const std::string_view fn1, const std::string_view fn2) {   \
        return DataView::Create(shared_from_this(), fn0, fn1, fn2);                                                     \
    }
