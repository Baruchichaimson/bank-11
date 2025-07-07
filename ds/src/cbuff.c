struct cbuff
{
    size_t front;
    size_t size;
    size_t capacity;
    char buffer[1];
};
