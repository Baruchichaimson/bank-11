#include <string>
#include <iostream>
#include <stack>


class TreeNode
{
public:
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : val(val), left(nullptr), right(nullptr) {}
};

std::string serialize(TreeNode* root)
{
    std::string result;
    std::stack<TreeNode*> st;

    st.push(root);

    while (!st.empty())
    {
        TreeNode* curr = st.top();
        st.pop();

        if (!curr)
        {
            result += "null,";
            continue;
        }

        result += std::to_string(curr->val);
        result += ",";

        st.push(curr->right);
        st.push(curr->left);
    }

    return result;
}

std::string NextToken(const std::string& data, size_t& pos)
{
    size_t start = pos;
    while (pos < data.size() && data[pos] != ',')
        ++pos;

    std::string token = data.substr(start, pos - start);
    ++pos;
    return token;
}

TreeNode* DeserializeHelper(const std::string& data, size_t& pos)
{
    std::string token = NextToken(data, pos);

    if (token == "null")
        return nullptr;

    TreeNode* node = new TreeNode(std::stoi(token));
    node->left = DeserializeHelper(data, pos);
    node->right = DeserializeHelper(data, pos);

    return node;
}

TreeNode* deserialize(const std::string& data)
{
    size_t pos = 0;
    return DeserializeHelper(data, pos);
}

int main()
{
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->right = new TreeNode(5);
    root->right->left = new TreeNode(4);

    std::string serialized = serialize(root);
    std::cout << "Serialized tree:\n" << serialized << std::endl;

    TreeNode* restored = deserialize(serialized);

    std::string serializedAgain = serialize(restored);
    std::cout << "\nSerialized again after deserialization:\n" << serializedAgain << std::endl;
    return 0;
}