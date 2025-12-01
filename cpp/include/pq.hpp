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

        bool Empty() const { return m_pq.empty(); }
        size_t Size() const { return m_pq.size(); }

        const T& Front() const { return m_pq.top(); }

        void Push(const T& value) { m_pq.push(value); }
        void Pop() { m_pq.pop(); }

    private:
        pq_t m_pq;
    };
}

#endif /* __PQ_HPP__ */
