#include <iostream>
#include <vector>
using namespace std;

class trie
{
public:
    trie()
    {
        this->left = NULL;
        this->right = NULL;
    }

    void addnum(int num)
    {
        trie *node = this;
        for (int i = 30; i >= 0; i--)
        {
            if (((num >> i) & 1) == 0)
            {
                if (!node->left)
                {
                    node->left = new trie();
                }
                node = node->left;
            }
            else
            {
                if (!node->right)
                {
                    node->right = new trie();
                }
                node = node->right;
            }
        }
    }

    int find(int num)
    {
        int ans = 0;
        trie *node = this;
        for (int i = 30; i >= 0; i--)
        {
            if (((num >> i) & 1) == 0)
            {
                if (node->right)
                {
                    ans = ans * 2 + 1;
                    node = node->right;
                }
                else
                {
                    ans = ans * 2;
                    node = node->left;
                }
            }
            else
            {
                if (node->left)
                {
                    ans = ans * 2 + 1;
                    node = node->left;
                }
                else
                {
                    ans = ans * 2;
                    node = node->right;
                }
            }
        }
        return ans;
    }

private:
    trie *left;
    trie *right;
};

int findMaximumXOR(vector<int> &nums)
{
    int res = INT32_MIN;
    trie *trie_tree = new trie();
    trie_tree->addnum(nums[0]);

    for (int i = 1; i < nums.size(); i++)
    {
        int tmp = trie_tree->find(nums[i]);
        res = tmp > res ? tmp : res;
        trie_tree->addnum(nums[i]);
    }
    return res;
}

int main()
{
    vector<int> test = {3, 10, 5, 25, 2, 8};
    int res = findMaximumXOR(test);
    cout << res << endl;
}