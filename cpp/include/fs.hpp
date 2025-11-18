#include <iostream>
#include <vector>
#include <string>

class FileSystemItem
{
public:
    FileSystemItem(const std::string& name) : m_name(name) {}
    virtual ~FileSystemItem() {}

    virtual void Print(int indent = 0) const = 0;
    virtual FileSystemItem* Clone() const = 0;   

protected:
    FileSystemItem(const FileSystemItem& other)   
        : m_name(other.m_name) {}

    std::string m_name;
};

class File : public FileSystemItem
{
public:
    File(const std::string& name);
    void Print(int indent = 0) const ;
    FileSystemItem* Clone() const ;

protected:
    File(const File& other); 

class Directory : public FileSystemItem
{
public:
    Directory(const std::string& name);
    ~Directory();

    void Add(FileSystemItem* item);
    void Print(int indent = 0) const ;
    FileSystemItem* Clone() const ;

protected:
    Directory(const Directory& other); // Protected copy ctor

private:
    std::vector<FileSystemItem*> m_children;
};
