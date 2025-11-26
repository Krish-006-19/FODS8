#include <iostream>
#include <string>
#include <sstream>
using namespace std;

const int MAX_STACK_SIZE = 100;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    int height;

    TreeNode(int v) {
        val = v;
        left = right = nullptr;
        height = 1;
    }
};

class Stack {
private:
    TreeNode* arr[MAX_STACK_SIZE];
    int topIndex;

public:
    Stack() { topIndex = -1; }

    bool isEmpty() {
        return topIndex == -1;
    }

    bool isFull() {
        return topIndex == MAX_STACK_SIZE - 1;
    }

    void push(TreeNode* node) {
        if (isFull()) {
            cout << "Stack overflow! Increase MAX_STACK_SIZE.\n";
            return;
        }
        arr[++topIndex] = node;
    }

    TreeNode* pop() {
        if (isEmpty()) {
            cout << "Stack underflow!\n";
            return nullptr;
        }
        return arr[topIndex--];
    }

    TreeNode* top() {
        if (isEmpty()) return nullptr;
        return arr[topIndex];
    }

    void clear() {
        topIndex = -1;
    }
};

class AVL{
public:
    void inorder(TreeNode* root) {
        Stack st;
        TreeNode* curr = root;
        while (!st.isEmpty() || curr) {
            while (curr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.pop();
            cout << curr->val << " ";
            curr = curr->right;
        }
        cout << "\n";
    }

    void preorder(TreeNode* root) {
        if (!root) return;
        Stack st;
        st.push(root);
        while (!st.isEmpty()) {
            TreeNode* node = st.pop();
            cout << node->val << " ";
            if (node->right) st.push(node->right);
            if (node->left) st.push(node->left);
        }
        cout << "\n";
    }

    int height(TreeNode* n) { return n ? n->height : 0; }

    int getBalance(TreeNode* n) {
        return n ? height(n->left) - height(n->right) : 0;
    }

    TreeNode* rotateRight(TreeNode* unbalancedNode) {
        TreeNode* leftChild = unbalancedNode->left;
        TreeNode* leftChildRightSubtree = leftChild->right;
    
        leftChild->right = unbalancedNode;
        unbalancedNode->left = leftChildRightSubtree;
    
        unbalancedNode->height = 1 + max(height(unbalancedNode->left), height(unbalancedNode->right));
        leftChild->height = 1 + max(height(leftChild->left), height(leftChild->right));
        return leftChild;
    }
    
    TreeNode* rotateLeft(TreeNode* unbalancedNode) {
        TreeNode* rightChild = unbalancedNode->right;
        TreeNode* rightChildLeftSubtree = rightChild->left;
    
        rightChild->left = unbalancedNode;
        unbalancedNode->right = rightChildLeftSubtree;
    
        unbalancedNode->height = 1 + max(height(unbalancedNode->left), height(unbalancedNode->right));
        rightChild->height = 1 + max(height(rightChild->left), height(rightChild->right));
        return rightChild;
    }

    TreeNode* addNode(TreeNode* root, int key){
        if(!root) return new TreeNode(key);

        Stack st;
        TreeNode* curr = root;
        TreeNode* parent = NULL;
        bool inserted = false;

        while(!inserted){
            st.push(curr);
            if(key < curr->val){
                if(!curr->left){
                    curr->left = new TreeNode(key);
                    st.push(curr->left);
                    inserted = true;
                }
                else curr = curr->left;
            } 
            else if(key > curr->val){
                if(!curr->right){
                    curr->right = new TreeNode(key);
                    st.push(curr->right);
                    inserted = true;
                } 
                else curr = curr->right;
            } 
            else{
                cout << "Duplicate value " << key << " not inserted.\n";
                return root;
            }
        }

        while(!st.isEmpty()){
            TreeNode* node = st.pop();
            node->height = 1 + max(height(node->left), height(node->right));
            int balance = getBalance(node);

            if(balance > 1){
                if (key < node->left->val)
                    node = rotateRight(node);
                else {
                    node->left = rotateLeft(node->left);
                    node = rotateRight(node);
                }
            } 
            else if (balance < -1){
                if (key > node->right->val)
                    node = rotateLeft(node);
                else {
                    node->right = rotateRight(node->right);
                    node = rotateLeft(node);
                }
            }

            if(!st.isEmpty()){
                TreeNode* parent = st.top();
                if (node->val < parent->val) parent->left = node;
                else parent->right = node;
            } 
            else root = node;
        }

        return root;
    }

    TreeNode* searchNode(TreeNode* root, int key){
        TreeNode* curr = root;
        while(curr){
            if (key == curr->val) return curr;
            curr = (key < curr->val) ? curr->left : curr->right;
        }
        return NULL;
    }

    bool updateNode(TreeNode*& root, int oldVal, int newVal) {
        TreeNode* tempRoot = deleteNode(root, oldVal);
        if (tempRoot == root) return false; 
        root = tempRoot;
        root = addNode(root, newVal);
        return true;
    }

    TreeNode* deleteNode(TreeNode* root, int key){
        if(!root) return NULL;

        Stack st;
        TreeNode* curr = root;
        TreeNode* parent = NULL;

        while(curr && curr->val != key){
            st.push(curr);
            parent = curr;
            curr = (key < curr->val) ? curr->left : curr->right;
        }
        if(!curr) return root;

        if(curr->left && curr->right){
            TreeNode* succParent = curr;
            TreeNode* succ = curr->right;
            st.push(curr);
            while(succ->left){
                st.push(succ);
                succParent = succ;
                succ = succ->left;
            }
            curr->val = succ->val;
            curr = succ;
            parent = succParent;
        }

        TreeNode* child = curr->left ? curr->left : curr->right;

        if(!parent) root = child;
        else if(curr == parent->left) parent->left = child;
        else parent->right = child;

        delete curr;

        while(!st.isEmpty()) {
            TreeNode* node = st.pop();
            node->height = max(height(node->left), height(node->right)) + 1;
            int balance = getBalance(node);

            if (balance > 1){
                if (getBalance(node->left) >= 0)
                    node = rotateRight(node);
                else{
                    node->left = rotateLeft(node->left);
                    node = rotateRight(node);
                }
            }
            else if(balance < -1){
                if(getBalance(node->right) <= 0)
                    node = rotateLeft(node);
                else{
                    node->right = rotateRight(node->right);
                    node = rotateLeft(node);
                }
            }

            if(!st.isEmpty()){
                TreeNode* parent2 = st.top();
                if (node->val < parent2->val) parent2->left = node;
                else parent2->right = node;
            } 
            else root = node;
        }
        return root;
    }
};

int valueChecker() {
    int choice;
    string line;
    while (true) {
        cout << "Enter Choice: ";
        getline(cin, line);
        stringstream ss(line);
        if (ss >> choice && ss.eof() && choice >= 1 && choice <= 7) break;
        else cout << "Invalid choice! Please enter a number between 1 and 7.\n";
    }
    return choice;
}

int valueCheckerBT() {
    int val;
    string line;
    while (true) {
        getline(cin, line);
        stringstream ss(line);
        if (ss >> val && ss.eof() && val >= 0 && val <= 10000000) break;
        else cout << "Invalid! Enter value between 0 and 10000000: ";
    }
    return val;
}

int main() {
    AVL tree;
    TreeNode* root = nullptr;
    int val, oldVal, newVal, choice;

    do {
        cout << "\nAVL Tree\n";
        cout << "1. Insert Node\n";
        cout << "2. Update Node\n";
        cout << "3. Delete Node\n";
        cout << "4. Search Node\n";
        cout << "5. Inorder Traversal\n";
        cout << "6. Preorder Traversal\n";
        cout << "7. Exit\n";

        choice = valueChecker();

        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                val = valueCheckerBT();
                root = tree.addNode(root, val);
                cout << "Inserted.\n";
                break;

            case 2:
                cout << "Enter old value: ";
                oldVal = valueCheckerBT();
                cout << "Enter new value: ";
                newVal = valueCheckerBT();
                if (tree.updateNode(root, oldVal, newVal))
                    cout << "Updated.\n";
                else
                    cout << "Value not found.\n";
                break;

            case 3:
                cout << "Enter value to delete: ";
                val = valueCheckerBT();
                root = tree.deleteNode(root, val);
                cout << "Deleted (if existed).\n";
                break;

            case 4:
                cout << "Enter value to search: ";
                val = valueCheckerBT();
                if (tree.searchNode(root, val))
                    cout << "Node found.\n";
                else
                    cout << "Node not found.\n";
                break;

            case 5:
                cout << "Inorder: ";
                tree.inorder(root);
                break;

            case 6:
                cout << "Preorder: ";
                tree.preorder(root);
                break;

            case 7:
                cout << "Exiting.\n";
                break;
        }

    } while (choice != 7);

    return 0;
}
