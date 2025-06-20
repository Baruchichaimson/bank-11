#include <stddef.h>
#include <stdint.h>

void *MyMemset(void *dest, int c, size_t n)
{
    unsigned char *byte_ptr = dest;
    size_t i = 0;

    while (((uintptr_t)(byte_ptr) % sizeof(size_t)) != 0 && n > 0) /* if its not aligned and n > 0 and convert the pointer to unsigned long */
    {
        *byte_ptr++ = (unsigned char)c;
        --n;
    }

    size_t word_value = 0;
    for (i = 0; i < sizeof(size_t); ++i) /* build a words */ 
    {
        word_value <<= 8;
        word_value |= (unsigned char)c;
    }

    size_t *word_ptr = (size_t *)byte_ptr; /* set the words in the address*/ 
    while (n >= sizeof(size_t))
    {
        *word_ptr++ = word_value;
        n -= sizeof(size_t);
    }

    byte_ptr = (unsigned char *)word_ptr; /* set the byte that remaining */
    while (n--)
    {
        *byte_ptr++ = (unsigned char)c;
    }

    return dest;
}


void *MyMemcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;       /* creat a pointers to one byte*/
    const unsigned char *s = src;

	/* if dest is not aligned or src is not aligned and n > 0 and convert the pointer to unsigned long */
    while (((uintptr_t)d % sizeof(size_t) != 0 || (uintptr_t)s % sizeof(size_t) != 0) && n > 0)
    {
        *d++ = *s++;
        --n;
    }

    size_t *dw = (size_t *)d;				/* convert the pointers to size_t (words) {32 bits | 8 byte} */
    const size_t *sw = (const size_t *)s;

    while (n >= sizeof(size_t))    /* still have a place to words*/
    {
        *dw++ = *sw++;
        n -= sizeof(size_t);
    }

    d = (unsigned char *)dw;         /* convert again the pointers to char (bytes) {4 bits | 1 byte} */
    s = (const unsigned char *)sw;

    while (n--)    /* copy the byte that remaining */
    {
        *d++ = *s++;
    }

    return dest;
}

void *MyMemmove(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;        /* creat a pointers to one byte*/
    const unsigned char *s = src;

    if (d < s || d >= s + n)   /* if the dest is before the src we dont have overlap because the pointer dont meet in the same char ("abcd"-"bcdd" & 3 || "abcd"-"abab" & 2) */
    {
        return MyMemcpy(dest, src, n);  /* we are copy with memcpy */
    }
    else
    {
        d += n;        /* we take the pointers to the end because now we have a overlap */
        s += n;

		/* set the words in the address and convert the pointer to unsigned long */
        while (n >= sizeof(size_t) &&
               ((uintptr_t)(d - sizeof(size_t)) % sizeof(size_t) == 0) &&
               ((uintptr_t)(s - sizeof(size_t)) % sizeof(size_t) == 0))
        {
            d -= sizeof(size_t);
            s -= sizeof(size_t);
            n -= sizeof(size_t);
            *(size_t *)d = *(const size_t *)s;
        }

        while (n--)   /* copy the byte that remaining */
        {
            *(--d) = *(--s);
        }

        return dest;
    }
}

