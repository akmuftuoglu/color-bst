/*
PIC 10C Homework 5, ColorBST.h
Purpose: Implement a Color Binary Search Tree
Author: Aral Muftuoglu
Date: 01/01/2021
*/

#ifndef ColorBST_h
#define ColorBST_h

#include <iostream>
#include <string>
#include <vector>

template <typename T>
class TreeNode;

template <typename T>
class ColorBST
{
    public:
        ColorBST();
        void insert(T data, char c = 'r');
        void ascending_print() const; 
        void descending_print() const;
        T largest() const;
        bool BlackRoot();
        bool NoDoubleRed();
        bool BlackDepth();
    
        // helper functions
        void ascendHelper(TreeNode<T>* current) const;
        void descendHelper(TreeNode<T>* current) const;
        void noDoubleRedHelper(TreeNode<T>* current);
        void blackDepthHelper(TreeNode<T>* current);
        
    
    private:
        TreeNode<T>* root;
        bool noDoubleRed;
        bool validBlackDepth;
        std::vector<T> dataHistory;
        template <typename U>
        friend class TreeNode;
};

template <typename T>
class TreeNode
{
    public:
        TreeNode();
        void insert_node(TreeNode* new_node);
    private:
        TreeNode* left;
        TreeNode* right;
        T data;
        std::string color;
        template <typename U>
        friend class ColorBST;
};

template <typename T>
ColorBST<T>::ColorBST()
{
    root = nullptr;
    noDoubleRed = true;
    validBlackDepth = true;
}

template <typename T>
void ColorBST<T>::insert(T data, char c)
{
    auto ptr = std::find(dataHistory.begin(), dataHistory.end(), data);
    
    if (ptr == dataHistory.end())
    {
        dataHistory.push_back(data);
    }
    else
    {
        return;
    }
    TreeNode<T>* new_node = new TreeNode<T>();
    new_node->data = data;
    new_node->left = nullptr;
    new_node->right = nullptr;
    if (root == nullptr) {root = new_node;}
    else {root->insert_node(new_node);}
    
    if (c == 'b')
    {
        new_node->color = "black";
    }
    else
    {
        new_node->color = "red";
        
    }
}

template <typename T>
void ColorBST<T>::ascending_print() const
{
    ascendHelper(root);
    std::cout << std::endl;
}

template <typename T>
void ColorBST<T>::descending_print() const
{
    descendHelper(root);
    std::cout << std::endl;
}

template <typename T>
T ColorBST<T>::largest() const
{
    TreeNode<T>* current = root;
    
    while (current->right != nullptr)
    {
        current = current->right;
    }
    
    return current->data;
}


template <typename T>
bool ColorBST<T>::BlackRoot()
{
    if (root->color == "black")
    {
        return true;
    }
    else
    {
        return false;
    }
}


template <typename T>
bool ColorBST<T>::NoDoubleRed()
{
    noDoubleRedHelper(root);
    return noDoubleRed;
}

template <typename T>
bool ColorBST<T>::BlackDepth()
{
    blackDepthHelper(root);
    
    int blackLeftOfRoot = 0;
    int blackRightOfRoot = 0;
    
    TreeNode<T>* current = root;
    
    // if after the helper function validBlackDepth still holds true, then check for the root node for black depth
    // check the "sides" of the tree for equal number black nodes, if they are not the same and validBlackDepth is true, it shouldn't be
    
    // edge case: say if you have 3 black nodes on the left side of the tree, and you have two on the right but the last black node  on the right side  has a right child of nullptr and a left child of a black node
    // then, validBlackDepth would have been false after the helper function since there is a case already that checks for that
    while (current != nullptr)
    {
        if (current->color == "black")
        {
            blackLeftOfRoot++;
        }
        
        current = current->left;
    }
    
    current = root;
    
    while (current != nullptr)
    {
        if (current->color == "black")
        {
            blackRightOfRoot++;
        }
        
        current = current->right;
    }
    
    
    if (blackLeftOfRoot != blackRightOfRoot)
    {
        validBlackDepth = false;
    }
    
    
    return validBlackDepth;
}

template <typename T>
void ColorBST<T>::ascendHelper(TreeNode<T>* current) const
{
    if (current->left != nullptr)
    {
        ascendHelper(current->left);
    }
    
    // acts like a recursion stack popper
    std::cout << current->data << " ";
    
    if (current->right != nullptr)
    {
        ascendHelper(current->right);
    }
    
}

template <typename T>
void ColorBST<T>::descendHelper(TreeNode<T>* current) const
{

    if (current->right != nullptr)
    {
        descendHelper(current->right);
    }
    
    // acts like a recursion stack popper
    std::cout << current->data << " ";
    
    if (current->left != nullptr)
    {
        descendHelper(current->left);
    }
}

template <typename T>
void ColorBST<T>::noDoubleRedHelper(TreeNode<T> *current)
{
    // searches through each node
    
    if (current->left != nullptr)
    {
        if (current->color == "red" && current->left->color == "red")
        {
            noDoubleRed = false;
        }
        noDoubleRedHelper(current->left);
    }
    
    if (current->right != nullptr)
    {
        if (current->color == "red" && current->right->color == "red")
        {
            noDoubleRed = false;
        }
        
        noDoubleRedHelper(current->right);
    }
       
    
}


template <typename T>
void ColorBST<T>::blackDepthHelper(TreeNode<T> *current)
{
    // we are searching from greatest to least node, even though it doesnt really matter which way we are searching
    // our goal is to find nodes that violate their black depth property
    // if a node doesnt have any of these properties, then that node should follow the black depth property
    // we will check a case for the root node in the other function
    
    if (current->right != nullptr)
    {
        blackDepthHelper(current->right);
    }
    
    // if the right child of the current node is black and the left child is a nullptr, this would violate black depth
    if (current->right != nullptr && current->left == nullptr)
    {
        if (current->right->color == "black")
        {
            validBlackDepth = false;
        }
    }
    
    // if the left child of the current node is black and the right child is a nullptr, this would violate black depth
    if (current->right == nullptr && current->left != nullptr)
    {
        if (current->left->color == "black")
        {
            validBlackDepth = false;
        }
    }

    
    if (current->right != nullptr && current->left != nullptr)
    {
        if (current->right->color == "red" && current->left->color == "black")
        {
            // if the right child is red and the left child is black then if the both the red and black child have nullptr as children, that would violate black depth
            if ((current->right->right == nullptr || current->right->left == nullptr) && (current->left->right == nullptr || current->left->left == nullptr))
            {
                validBlackDepth = false;
            }
            
            // check the right most descendant, it must be black otherwise black depth is violated
            // edge case: if rightmost has a left node that's black, it already violated a different test eariler.
            if (current->left->right == nullptr || current->left->left == nullptr)
            {
                TreeNode<T> *curr = current;
                
                curr = curr->right;
                
                while (curr->right != nullptr)
                {
                    if (curr->color == "black")
                    {
                        break;
                    }
                    curr = curr->right;
                }
                
               if (curr->right == nullptr && curr->color != "black")
               {
                   validBlackDepth = false;
               }
            }
        }
        
        if (current->left->color == "red" && current->right->color == "black")
        {
            // if the left child is red and the right child is black then if the both the red and black child have nullptr as children, that would violate black depth
            if ((current->right->right == nullptr || current->right->left == nullptr) && (current->left->right == nullptr || current->left->left == nullptr))
            {
                validBlackDepth = false;
            }
            
            // check the leftmost descendant, it must be black otherwise black depth is violated
            // edge case: if leftmost has a right node that's black, it already violated a different test eariler.
            if (current->right->right == nullptr || current->right->left == nullptr)
            {
                TreeNode<T> *curr = current;
                
                curr = curr->left;
                
                while (curr->left != nullptr)
                {
                    if (curr->color == "black")
                    {
                        break;
                    }
                    curr = curr->left;
                }
                
               if (curr->left == nullptr && curr->color != "black")
               {
                   validBlackDepth = false;
               }

            }
        }
        
        
        // check if a node has 2 black nodes as children and one has child more no more children that the other child does not have a red node as a child
        if (current->right->color == "black" && current->left->color == "black")
        {
            if ((current->left->right == nullptr || current->left->left == nullptr) && (current->right->right != nullptr || current->right->left != nullptr))
            {
                if (current->right->right->color == "black" || current->right->left->color == "black" )
                {
                    validBlackDepth = false;
                }
            }
            
            if ((current->right->right == nullptr || current->right->left == nullptr) && (current->left->right != nullptr || current->left->left != nullptr))
            {
                if (current->left->right->color == "black" || current->left->left->color == "black" )
                {
                    validBlackDepth = false;
                }
            }
        }
    }
    
    if (current->left != nullptr)
    {
        blackDepthHelper(current->left);
    }
}


// START OF TreeNode IMPLEMENTATIONS //

template <typename T>
TreeNode<T>::TreeNode()
{
    right = nullptr;
    left = nullptr;
}

template <typename T>
void TreeNode<T>::insert_node(TreeNode* new_node)
{
    if (new_node->data < data)
    {
        if (left == nullptr) { left = new_node; }
        else { left->insert_node(new_node); }
    }
    else if (data < new_node->data)
    {
        if (right == nullptr) { right = new_node; }
        else { right->insert_node(new_node); }
    }
}


#endif /* ColorBST_h */
