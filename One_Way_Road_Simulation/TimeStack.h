#pragma once

#include <stack>
#include <windows.h>

class TimeStack {
private:
    std::stack<DWORD> internalStack;

public:
    // ���������� �������� � ����
    void push(const DWORD value);

    // �������� �������� �������� �� �����
    void pop();

    // ��������� �������� �������� �����
    DWORD top() const;

    // ��������, ���� �� ����
    bool isEmpty() const;

    // ��������� ������� �����
    size_t size() const;
};