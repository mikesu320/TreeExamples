#pragma once
#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>

using namespace std;
//
// Binary trees are already defined with this interface:
template<typename T>
struct Tree {
	Tree(const T& v) : value(v), left(nullptr), right(nullptr) {}
	Tree(const T& v, Tree* l, Tree* r) : value(v), left(l), right(r) {}
	~Tree() { left = nullptr; right = nullptr; };
	T value;
	Tree* left;
	Tree* right;
};



#endif // TREE_H_INCLUDED
