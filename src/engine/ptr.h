#pragma once

#include <iostream>
#include <memory>

#define GENERATE_PTR(clazz) public: \
                                using Ptr = std::shared_ptr<clazz>; \
                                template<class... Args> \
                                inline static Ptr New(Args&&... args) { \
                                    return std::make_shared<clazz>(std::forward<Args>(args)...); \
                                }\

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}