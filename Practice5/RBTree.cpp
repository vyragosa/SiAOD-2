#include "include/RBTree.h"
#include <windows.h>
#include <iostream>
using namespace std;

RBTree::RBTree() {
	root = nullptr;
}

int RBTree::getColor(tNode*& node) {
	if (!node)
		return BLACK;
	return node->color;
}

void RBTree::setColor(tNode*& node, int color) {
	if (!node)
		return;
	node->color = color;
}

tNode* RBTree::insertBST(tNode*& node, tNode*& ptr) {
	if (!node)
		return ptr;

	if (ptr->data < node->data) {
		node->left = insertBST(node->left, ptr);
		node->left->parent = node;
	}
	else if (ptr->data > node->data) {
		node->right = insertBST(node->right, ptr);
		node->right->parent = node;
	}

	return node;
}

void RBTree::insertValue(int n, Patient& patient) {
	tNode* node = new tNode {n, patient};
	root = insertBST(root, node);
	fixInsertRBTree(node);
}

void RBTree::rotateLeft(tNode*& ptr) {
	tNode* right_child = ptr->right;
	ptr->right = right_child->left;

	if (ptr->right != nullptr)
		ptr->right->parent = ptr;

	right_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		root = right_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = right_child;
	else
		ptr->parent->right = right_child;

	right_child->left = ptr;
	ptr->parent = right_child;
}

void RBTree::rotateRight(tNode*& ptr) {
	tNode* left_child = ptr->left;
	ptr->left = left_child->right;

	if (ptr->left != nullptr)
		ptr->left->parent = ptr;

	left_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		root = left_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = left_child;
	else
		ptr->parent->right = left_child;

	left_child->right = ptr;
	ptr->parent = left_child;
}

void RBTree::fixInsertRBTree(tNode*& ptr) {
	while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
		tNode* parent = ptr->parent;
		tNode* grandparent = parent->parent;
		if (parent == grandparent->left) {
			//����� �������� �������
			tNode* uncle = grandparent->right;
			if (getColor(uncle) == RED) {
				// ���� ���� �������, ������ ���� �� ������������ ���� ����� �� �����
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				ptr = grandparent;
			}
			else {
				// ���� ���� ������, ����� ����������� �������� �������� ����� ���� ����
				if (ptr == parent->right) {
					rotateLeft(parent);
					ptr = parent;
					parent = ptr->parent;
				}
				swap(parent->color, grandparent->color);
				rotateRight(grandparent);
				ptr = parent;
			}
		}
		else {
			//������ �������� �������. ������������ ���
			tNode* uncle = grandparent->left;
			if (getColor(uncle) == RED) {
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				ptr = grandparent;
			}
			else {
				if (ptr == parent->left) {
					rotateRight(parent);
					ptr = parent;
					parent = ptr->parent;
				}
				swap(parent->color, grandparent->color);
				rotateLeft(grandparent);
				ptr = parent;
			}
		}
	}
	setColor(root, BLACK);
}

void RBTree::fixDeleteRBTree(tNode*& node) {
	if (!node)
		return;
	tNode* sibling;
	tNode* ptr = node;
	printTree(root);
	while (ptr != root && getColor(ptr) == BLACK) {
		tNode* parent = ptr->parent;
		if (ptr == parent->left) {
			sibling = parent->right;
			if (getColor(sibling) == RED) {
				setColor(sibling, BLACK);
				setColor(parent, RED);
				rotateLeft(parent);
				sibling = parent->right;
			}

			if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
				setColor(sibling, RED);
				
				ptr = parent;

			}
			else {
				if (getColor(sibling->right) == BLACK) {
					setColor(sibling->left, BLACK);
					setColor(sibling, RED);
					rotateRight(sibling);
					sibling = parent->right;
				}
				setColor(sibling, parent->color);
				setColor(parent, BLACK);
				setColor(sibling->right, BLACK);
				rotateLeft(parent);
				ptr = root;
			}

		}
		else {
			sibling = parent->left;
			if (getColor(sibling) == RED) {
				setColor(sibling, BLACK);
				setColor(parent, RED);
				rotateRight(parent);
				sibling = parent->left;
			}

			if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {

				setColor(sibling, RED);
				ptr = parent;
			}
			else {
				if (getColor(sibling->left) == BLACK) {
					setColor(sibling->right, BLACK);
					setColor(sibling, RED);
					rotateLeft(sibling);
					sibling = parent->left;
				}
				setColor(sibling, parent->color);
				setColor(parent, BLACK);
				setColor(sibling->left, BLACK);
				rotateRight(parent);
				break;

			}
		}
		
	}
	setColor(node->parent, BLACK);
	if (node == node->parent->left)
		node->parent->left = nullptr;
	else
		node->parent->right = nullptr;
	delete node;
	setColor(root, BLACK);

}

tNode* RBTree::deleteBST(tNode*& root, int data) {
	if (!root)
		return root;

	if (data < root->data)
		return deleteBST(root->left, data);

	if (data > root->data)
		return deleteBST(root->right, data);

	if (root->left == nullptr || root->right == nullptr)
		return root;

	tNode* temp = minValueNode(root->right);
	root->data = temp->data;
	return deleteBST(root->right, temp->data);
}

void RBTree::printTree(tNode*& node, const std::string& prefix, bool isRight) {
	if (!node)
		return;
	printTree(node->right, prefix + (isRight ? "    " : "|   "), true);
	std::cout << prefix + "|--" << (getColor(node) == RED ? "\033[38;5;9m" : "") << node->data << "\033[0m" << '\n';
	printTree(node->left, prefix + (isRight ? "|   " : "    "), false);
}

void RBTree::deleteValue(int data) {
	tNode* node = deleteBST(root, data);
	fixDeleteRBTree(node);
}

tNode* RBTree::minValueNode(tNode*& node) {
	tNode* ptr = node;
	while (ptr->left != nullptr)
		ptr = ptr->left;
	return ptr;
}

Patient* RBTree::get(tNode*& node, int data) {
	if (node == nullptr)
		return nullptr;
	if (data < node->data)
		return get(node->left, data);
	if (data > node->data)
		return get(node->right, data);
	return &node->ref;
}