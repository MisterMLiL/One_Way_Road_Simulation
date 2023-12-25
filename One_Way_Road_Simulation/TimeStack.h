#pragma once

#include <stack>
#include <windows.h>

class TimeStack {
private:
    std::stack<DWORD> internalStack;

public:
    // Добавление элемента в стек
    void push(const DWORD value);

    // Удаление верхнего элемента из стека
    void pop();

    // Получение верхнего элемента стека
    DWORD top() const;

    // Проверка, пуст ли стек
    bool isEmpty() const;

    // Получение размера стека
    size_t size() const;
};