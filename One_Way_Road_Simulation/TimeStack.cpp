#include "TimeStack.h"

// Добавление элемента в стек
void TimeStack::push(const DWORD value) {
    internalStack.push(value);
}

// Удаление верхнего элемента из стека
void TimeStack::pop() {
    if (!internalStack.empty()) {
        internalStack.pop();
    }
}

// Получение верхнего элемента стека
DWORD TimeStack::top() const {
    if (!internalStack.empty()) {
        return internalStack.top();
    }
    else {
        return -1;
    }
}

// Проверка, пуст ли стек
bool TimeStack::isEmpty() const {
    return internalStack.empty();
}

// Получение размера стека
size_t TimeStack::size() const {
    return internalStack.size();
}