#ifndef __PQ_HPP__
#define __PQ_HPP__

#include <queue>

namespace ilrd
{
    template <typename T, typename Container = std::vector<T>, typename Compare = std::less<T>>
    class PQ
    {
    public:
        using pq_t = std::priority_queue<T, Container, Compare>;

        bool empty() const { return m_pq.empty(); }
        size_t size() const { return m_pq.size(); }

        const T& front() const { return m_pq.top(); }

        void push(const T& value) { m_pq.push(value); }
        void pop() { m_pq.pop(); }

    private:
        pq_t m_pq;
    };
}

#endif /* __PQ_HPP__ */
