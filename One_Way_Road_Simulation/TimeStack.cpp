#include "TimeStack.h"

// ���������� �������� � ����
void TimeStack::push(const DWORD value) {
    internalStack.push(value);
}

// �������� �������� �������� �� �����
void TimeStack::pop() {
    if (!internalStack.empty()) {
        internalStack.pop();
    }
}

// ��������� �������� �������� �����
DWORD TimeStack::top() const {
    if (!internalStack.empty()) {
        return internalStack.top();
    }
    else {
        return -1;
    }
}

// ��������, ���� �� ����
bool TimeStack::isEmpty() const {
    return internalStack.empty();
}

// ��������� ������� �����
size_t TimeStack::size() const {
    return internalStack.size();
}