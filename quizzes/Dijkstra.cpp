#include <bits/stdc++.h>
using namespace std;

/* ---- Node for priority queue ---- */
struct Node {
    int x;
    int y;
    int cost;

    /* ---- min-heap ---- */
    bool operator>(const Node& other) const 
    {
        return cost > other.cost;
    }
};

/* ---- Dijkstra on weighted grid ---- */
int minPathCost(const vector<vector<int>>& grid)
{
    int n = grid.size();
    int m = grid[0].size();

    const int INF = INT_MAX;

    vector<vector<int>> dist(n, vector<int>(m, INF));

    priority_queue<Node, vector<Node>, greater<Node>> pq;

    /* ---- start cell ---- */
    dist[0][0] = grid[0][0];
    pq.push({0, 0, grid[0][0]});

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();

        int x = curr.x;
        int y = curr.y;
        int cost = curr.cost;

        /* ---- outdated entry ---- */
        if (cost > dist[x][y])
            continue;

        /* ---- destination reached ---- */
        if (x == n - 1 && y == m - 1)
            return cost;

        /* ---- relax neighbors ---- */
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                int newCost = cost + grid[nx][ny];

                if (newCost < dist[nx][ny]) {
                    dist[nx][ny] = newCost;
                    pq.push({nx, ny, newCost});
                }
            }
        }
    }

    return -1; // should not happen (grid is fully connected)
}

int main()
{
    vector<vector<int>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };

    cout << minPathCost(grid) << endl; // Output: 7
    return 0;
}
