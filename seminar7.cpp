#include <iostream>
#include <vector>
#include <queue>

struct IGraph
{
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph
{
public:
    ListGraph(size_t vertices_count)
    {
        vertices.resize(vertices_count);
    }

    ListGraph(const IGraph &graph);

    virtual ~ListGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) override
    {
        vertices[from].push_back(to);
        // prev_vertices[to].push_back(from);
    }

    virtual int VerticesCount() const override
    {
        return vertices.size();
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> result;
        // std::copy(result.begin(), vertices[vertex].begin(), vertices[vertex].end());
        for (int v : vertices[vertex])
        {
            result.push_back(v);
        }
        return result;
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> result;
        // std::copy(result.begin(), prev_vertices[vertex].begin(), prev_vertices[vertex].end());

        for (int from = 0; from < vertices.size(); ++from)
        {
            for (int to : vertices[from])
            {
                if (to == vertex)
                {
                    result.push_back(from);
                }
            }
        }
        return result;
    }

private:
    std::vector<std::vector<int>> vertices;
    std::vector<std::vector<int>> prev_vertices;
};

void dfs_aux(int u, const IGraph &graph, std::vector<bool> &visited, void (*callback)(int v))
{
    visited[u] = true;
    callback(u);

    std::vector<int> children = graph.GetNextVertices(u);

    for (int v : children)
    {
        if (!visited[v])
        {
            dfs_aux(v, graph, visited, callback);
        }
    }
}

int dfs(const IGraph &graph, void (*callback)(int v))
{
    int vertices_count = graph.VerticesCount();
    std::vector<bool> visited;

    visited.assign(vertices_count, false);
    for (int i = 0; i < vertices_count; ++i)
    {
        if (!visited[i])
        {
            dfs_aux(i, graph, visited, callback);
        }
    }
}

void bfs_aux(int u, const IGraph &graph, std::vector<bool> &visited, void (*callback)(int v))
{
    std::queue<int> q;
    q.push(u);
    visited[u] = true;
    while (!q.empty())
    {
        int v = q.front();
        q.pop();

        callback(v);

        std::vector<int> children = graph.GetNextVertices(u);
        for (int w : children)
        {
            if (!visited[w])
            {
                visited[w] = true;
                q.push(w);
            }
        }
    }
}

int bfs(const IGraph &graph, void (*callback)(int v))
{
    int vertices_count = graph.VerticesCount();

    std::vector<bool> visited;

    visited.assign(vertices_count, false);
    for (int i = 0; i < vertices_count; ++i) {
        if (!visited[i]) {
            bfs_aux(i, graph, visited, callback);
        }
    }
}

int main()
{
    ListGraph graph(6);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 0);

    graph.AddEdge(1, 2);
    graph.AddEdge(2, 1);

    graph.AddEdge(2, 3);
    graph.AddEdge(3, 2);

    graph.AddEdge(2, 4);
    graph.AddEdge(4, 2);

    graph.AddEdge(3, 4);
    graph.AddEdge(4, 3);

    dfs(graph, [](int v) {
        std::cout << v << " ";
    });
    std::cout << std::endl;


    bfs(graph, [](int v) {
        std::cout << v << " ";
    });
    std::cout << std::endl;

    std::cout << "OK" << std::endl;
}
