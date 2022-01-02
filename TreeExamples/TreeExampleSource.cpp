

#include <Tree.h>
#include <TreeTestcases.h>
#include <stack>
#include <algorithm>

bool dfs(Tree<int>* nd, int sum) {
    if (!nd)
        return false;
    sum = sum - nd->value;
    if (sum == 0 && !nd->left && !nd->right)
        return true;
    return (dfs(nd->left, sum) || dfs(nd->right, sum));
}

bool checkTreePath(Tree<int>* t, int s) {
    int sum = s;
    Tree<int>* node = t;

    return dfs(node, sum);
}

#define MAXNUM 1001

void bfs(Tree<int>* t, stack<int>& mystk) {
    if (!t) {
        mystk.push(MAXNUM);
        return;
    }
    if (!t->left && !t->right)
        mystk.push(t->value);
    else {
        bfs(t->left, mystk);
        mystk.push(t->value);
        bfs(t->right, mystk);
    }
}

bool isTreeSymmetric(Tree<int>* t) {
    stack<int> mystk;

    if (!t)
        return true;
    bfs(t, mystk);

    vector<int> vec;
    while (!mystk.empty()) {
        vec.push_back(mystk.top());
        mystk.pop();
    }

    vector<int> rev(vec.rbegin(), vec.rend());
    if (vec == rev)
        return true;
    else
        return false;

}

Tree<int>* buildTree(vector<int> inorder, vector<int> preorder, int inStart, int inEnd, int& pre_ndx) {
    if (inStart > inEnd)
        return NULL;

    int curVal = preorder[pre_ndx];
    cout << "current node" << curVal << " " << inStart << "," << inEnd << " " << pre_ndx << endl;
    pre_ndx++;
    Tree<int>* curNode = new Tree<int>(curVal);

    if (inStart == inEnd)
        return curNode;
    auto it = find(inorder.begin(), inorder.end(), curVal);
    int indx = distance(inorder.begin(), it);

    curNode->left = buildTree(inorder, preorder, inStart, indx - 1, pre_ndx);
    curNode->right = buildTree(inorder, preorder, indx + 1, inEnd, pre_ndx);

    return curNode;

}
#define MAXCHAR 128
class TrieNode
{
public:
    TrieNode* children[MAXCHAR];;
    bool endWord;
    TrieNode()
    {
        for (int i = 0; i < MAXCHAR; i++)
            children[i] = NULL;
        endWord = false;
    }
    void insertSuffixInTrie(string s);
    int searchTrie(string pat, int ind);
};
void TrieNode::insertSuffixInTrie(string s)
{
    if (s.length() > 0) {
        int c = s.at(0) - 'A';
        if (children[c] == NULL)
            children[c] = new TrieNode();
        children[c]->insertSuffixInTrie(s.substr(1));
    }
    else
        endWord = true;
};


int TrieNode::searchTrie(string s, int indx) {

    if (s.length() > 0) {
        int c = s.at(0) - 'A';
        int cindx = -1;
        if (children[c] != NULL) {
            cindx = (children[c])->searchTrie(s.substr(1), indx + 1);
        }
        if (cindx >= 0)
            return cindx;
        else if (endWord)
            return indx;
    }
    if (endWord)
        return indx;
    else
        return -1;

}

vector<string>  searchSubStrings(vector<string> words, vector<string>parts) {
    vector<string> nl;
    vector<TrieNode> roots(words.size());
    sort(parts.begin(), parts.end(), []
    (const string& f1, const string& f2) {return f1.length() > f2.length(); });


    // Insert all the suffixes of the parts into Trie;
    int cnt = 0;
    TrieNode root;
    for (auto x : parts) {
        root.insertSuffixInTrie(x);
        cnt++;
    }

    for (size_t i = 0; i < words.size(); i++) {
        size_t ffirst = words[i].length();
        int maxlength = 0;
        string str;
        str = words[i];
        for (size_t j = 0; j < str.length(); j++) {
            string sub = str.substr(j);
            if (maxlength > sub.length())
                continue;
            //cout << "searching " << sub << endl;
            int inx = root.searchTrie(sub, 0);
            if (inx >= 0) {
                //cout << "found " << sub.substr(0,inx) << " in " << sub << " " << j << endl;
                if (maxlength < inx) {
                    maxlength = inx;
                    ffirst = j;
                }
                else if (maxlength == inx)
                    ffirst = min(ffirst, j);
            }
        }

        if (ffirst < str.length()) {
            str.insert(ffirst, "[");
            str.insert(ffirst + maxlength + 1, "]");
        }
        nl.push_back(str);
    }
    return nl;
}
void swapElems(Tree<int>* t1, Tree<int>* t2) {
    Tree<int>* ltemp, * rtemp;
    int tempNum = t1->value;
    ltemp = t1->left;
    rtemp = t1->right;
    t1->value = t2->value;
    t1->left = t2->left;
    t1->right = t2->right;
    t2->value = tempNum; t2->left = ltemp; t2->right = rtemp;
}
Tree<int>* findElem(Tree<int>* t, Tree<int>* parent, int num) {

    Tree<int>* ret = NULL;
    if (!t)
        ret = NULL;
    else if (t->value > num) {
        ret = findElem(t->left, t, num);
    }
    else if (t->value < num) {
        ret = findElem(t->right, t, num);
    }
    else if (t->value == num) {
        // found the number now remove it.
        if (t->left) {
            // remove the swap rightmost node with t
            Tree<int>* ltree = t->left;
            Tree<int>* ltreepar = t;
            while (ltree->right) {
                ltreepar = ltree;
                ltree = ltree->right;
            }
            if (ltree == t->left) {
                t->value = ltree->value;
                t->left = ltree->left;
            }
            else {
                t->value = ltree->value;
                ltreepar->right = ltree->left;
            }
            ret = t;
        }
        else if (t->right) {
            if (!parent)
                ret = t->right;
            else if (parent->value > t->right->value) {
                parent->left = t->right;
                ret = parent;
            }
            else {
                parent->right = t->right;
                ret = parent;
            }
        }
        else {
            //leaf
            if (parent) {
                if (parent->value > t->value)
                    parent->left = NULL;
                else
                    parent->right = NULL;
            }
            else
                ret = NULL;
        }
    }
    if (parent != NULL)
        return parent;
    else
        return ret;
}



Tree<int>* removeElems(Tree<int>* t, vector<int> queries) {
    Tree<int>* top = t;
    for (auto x : queries)
        top = findElem(top, NULL, x);
    return top;
}


void printTree(Tree<int>* t) {
    if (!t) return;
    cout << t->value << ", ";
    printTree(t->left);
    printTree(t->right);
}
int main()
{

    Tree<int> y1(10, nullptr, nullptr);
    Tree<int> y4(-1, nullptr, nullptr);
    Tree<int> y3(-52, nullptr, nullptr);
    Tree<int> y2(-51, &y3, nullptr);


    Tree<int> y7(-42, &y2, nullptr);
    Tree<int> y8(-53, nullptr, &y7);
    Tree<int> y6(-9, &y8, nullptr);
    Tree<int> y5(5, &y6, &y1);

    Tree<int>* tt = removeElems(&y5, { 1,3,-53,6,7 });
    printTree(tt);

    vector<string> retval = searchSubStrings(words4, parts4);
    //retval = searchSubStrings({"a","b"}, {"b"});

    vector<int> inord = { 3,2,4,1,6,7,5 };
    vector<int> preord = { 1,2,3,4,5,6,7 };
    int prendx = 0;
    Tree<int>* t = buildTree(inord, preord, 0, inord.size() - 1, prendx);
    cout << "preorder ";
    printTree(t);
    cout << endl;
    cout << endl;

    Tree<int> n0(3);
    Tree<int> n1(-2, nullptr, &n0);
    Tree<int> n2(3, nullptr, nullptr);
    Tree<int> n3(4);
    Tree<int> n4(2, &n2, &n3);
    Tree<int> n5(3, nullptr, nullptr);
    Tree<int> n6(4);
    Tree<int> n7(2, &n6, &n5);
    Tree<int> n8(1, &n4, &n7);


    cout << checkTreePath(&n8, 7) << endl;
    cout << isTreeSymmetric(&n8) << endl;
    return 0;
}
