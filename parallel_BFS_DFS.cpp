#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V; // number of vertices
    vector<vector<int>> adj; // adjacency list

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS: ";

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << current << " ";

            // Parallelize the loop over neighbors
            #pragma omp parallel for
            for (int i = 0; i < adj[current].size(); i++) {
                int neighbor = adj[current][i];

                // Use critical section to avoid race condition
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
        cout << endl;
    }

    void parallelDFSUtil(int node, vector<bool>& visited) {
        stack<int> s;
        s.push(node);

        cout << "Parallel DFS: ";

        while (!s.empty()) {
            int current = s.top();
            s.pop();

            if (!visited[current]) {
                visited[current] = true;
                cout << current << " ";

                // Parallelize neighbor push
                #pragma omp parallel for
                for (int i = 0; i < adj[current].size(); i++) {
                    int neighbor = adj[current][i];

                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            s.push(neighbor);
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        parallelDFSUtil(start, visited);
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;

    Graph g(V);

    cout << "Enter number of edges: ";
    cin >> E;

    cout << "Enter " << E << " edges (u v):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting node for traversal: ";
    cin >> start;

    cout << "Graph Traversals using OpenMP:" << endl;
    g.parallelBFS(start);
    g.parallelDFS(start);

    return 0;
}


// Time Complexity: O(V+E)