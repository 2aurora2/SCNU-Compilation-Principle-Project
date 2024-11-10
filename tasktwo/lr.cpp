#include "lr.h"

LR::LR() : size(0) {}

/*!
    @Function       qHash
    @Description  为 Item 类型提供 qHash 特化版本
    @Parameter
    @Return
    @Attention
*/
uint qHash(const Item &item) {
    return qHash(item.name) + qHash(item.rule) + qHash(item.next) +
           qHash(item.pos);
}

/*!
    @Function       qHash
    @Description  为 State 类型提供 qHash 特化版本
    @Parameter
    @Return
    @Attention
*/
uint qHash(const State &state) { return qHash(state.st); }
