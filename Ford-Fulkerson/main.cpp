#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <algorithm>
#include <vector>

using namespace std;

class Graph
{
public:
    static const int maxSize = 30;

protected:
    int rGraph[maxSize][maxSize];
    int capacity[maxSize][maxSize];
    bool visited[maxSize];
    vector<int> path;
    int maxFlow, size;

public:
    Graph(int v){
        maxFlow = 0;
        size = v;
        for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; ++j){
                rGraph[i][j] = 0;
                capacity[i][j] = 0;
            }
            visited[i] = false;
            path.clear();
        }
    }

    int getMaxFlow(){
        return maxFlow;
    }

    void readEdges(int n){
        int n1, n2, w;
        for(int i = 0; i < n; ++i){
            scanf("%d%d%d", &n1, &n2, &w);
            capacity[n1][n2] = w;
        }
    }

    void unVisitAll(){
        for(int i = 0; i < size; ++i){
            visited[i] = false;
        }
    }

    void fillPath(stack<int> st){
        path.clear();
        while(!st.empty()){
            path.push_back(st.top());
            st.pop();
        }

        reverse(path.begin(), path.end());
    }

    void setResidualGraph(){
        for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; ++j){
                rGraph[i][j] = capacity[i][j];
            }
        }
    }

    int findBottleNeck(int t){
        int bottleNeck = INT_MAX;

        for(int i = 0; i < path.size(); ++i){
            int n1 = path[i];
            int n2 = path[i+1];
            bottleNeck = min(bottleNeck, rGraph[n1][n2]);

            if(n2 == t){
                return bottleNeck;
            }
        }
    }

    void setFlowsInResidualNetwork(int t, int flow){

        for(int i = 0; i < path.size(); ++i){
            int n1 = path[i];
            int n2 = path[i+1];
            rGraph[n1][n2] -= flow;
            rGraph[n2][n1] += flow;

            if(n2 == t){
                break;
            }
        }
    }

    bool DFS(int s, int t)
    {
        bool backtrack = false;
        stack<int> st;
        int node, i;
        unVisitAll();

        st.push(s);
        visited[s] = true;

        while(!st.empty())
        {
            backtrack = false;
            node = st.top();
            if(node == t){
                fillPath(st);
                return true;
            }

            for(i = 0; i < size; ++i)
            {
                if(visited[i]){
                    continue;
                }
                else{
                    if(rGraph[node][i] > 0){
                        st.push(i);
                        visited[i] = true;
                        backtrack = true;
                        break;
                    }
                }
            }

            if(!backtrack){
                st.pop();
            }
        }

        return false;
    }

    void FordFulkerson(int s, int t)
    {
        setResidualGraph();

        while(DFS(s, t))
        {
            int flow = findBottleNeck(t);
            setFlowsInResidualNetwork(t, flow);

            maxFlow += flow;
        }
    }

};

int main()
{
    int s, t, v, n;
    printf("Unesite graf protoka!\n");
    printf("Unesite broj cvorova i grana u grafu: ");
    scanf("%d%d", &v, &n);
    Graph g(v);

    printf("\nUnesite grane grafa i njihove tezine: \n");
    g.readEdges(n);

    printf("\nUnesite pocetni i krajnji cvor: \n");
    scanf("%d%d", &s, &t);

    g.FordFulkerson(s, t);
    printf("\nMaksimalni protok u grafu je: %d", g.getMaxFlow());

}
/*
6 9

0 1 10
0 2 10
2 1 2
1 3 9
2 4 4
2 3 8
3 4 6
3 5 10
4 5 10

0 5
*/
