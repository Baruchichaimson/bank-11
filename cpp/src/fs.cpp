#include "fs.hpp"

/* ================= FILE ================= */

File::File(const std::string& name)
    : FileSystemItem(name)
{
}

File::File(const File& other)
    : FileSystemItem(other)
{
}

void File::Print(int indent) const
{
    std::cout << std::string(indent, ' ') << "- File: " << m_name << std::endl;
}

FileSystemItem* File::Clone() const
{
    return new File(*this);
}


/* ================= DIRECTORY ================= */

Directory::Directory(const std::string& name)
    : FileSystemItem(name)
{
}

Directory::Directory(const Directory& other)
    : FileSystemItem(other)
{
    // Deep copy — ללא auto
    for (size_t i = 0; i < other.m_children.size(); ++i)
    {
        FileSystemItem* child = other.m_children[i];
        m_children.push_back(child->Clone());
    }
}

Directory::~Directory()
{
    // ללא auto
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        FileSystemItem* item = m_children[i];
        delete item;
    }
}

void Directory::Add(FileSystemItem* item)
{
    m_children.push_back(item);
}

void Directory::Print(int indent) const
{
    std::cout << std::string(indent, ' ') << "+ Directory: " << m_name << std::endl;

    // ללא auto
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        FileSystemItem* item = m_children[i];
        item->Print(indent + 4);
    }
}

FileSystemItem* Directory::Clone() const
{
    return new Directory(*this);
}
