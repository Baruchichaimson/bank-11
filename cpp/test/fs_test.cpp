#include <iostream>

#include "fs.hpp"

int main()
{
    Directory* root = new Directory("root");

    Directory* home = new Directory("home");
    Directory* user = new Directory("user");
    Directory* docs = new Directory("documents");

    File* a = new File("notes.txt");
    File* b = new File("todo.txt");
    File* c = new File("pic.png");
    File* d = new File("resume.pdf");

    docs->Add(d);
    user->Add(a);
    user->Add(b);
    user->Add(docs);

    home->Add(user);
    root->Add(home);
    root->Add(c);

    std::cout << "=== ORIGINAL ===\n";
    root->Print();

    /* =============== CLONE =============== */
    Directory* cloned = static_cast<Directory*>(root->Clone());

    std::cout << "\n=== CLONE ===\n";
    cloned->Print();

    delete root;
    delete cloned;

    return 0;
}
