#include <stdio.h>
#include <limits.h>

int main() {
    int count, src_router, i, j, v, w, min;
    int cost_matrix[100][100], dist[100], last[100], flag[100];

    printf("\nEnter the number of routers: ");
    scanf("%d", &count);

    printf("\nEnter the cost matrix (use -1 for no direct link):\n");
    for (i = 0; i < count; i++) {
        for (j = 0; j < count; j++) {
            scanf("%d", &cost_matrix[i][j]);
            // if (cost_matrix[i][j] == -1) // Handle no direct path
            //     cost_matrix[i][j] = INT_MAX;
        }
    }

    printf("\nEnter the source router (0-based index): ");
    scanf("%d", &src_router);

    // Initialize distance and flag arrays
    for (v = 0; v < count; v++) {
        flag[v] = 0;
        // last[v] = src_router;
        dist[v] = cost_matrix[src_router][v];
    }

    flag[src_router] = 1; // Mark source as visited

    // Dijkstra's algorithm
    for (i = 1; i < count; i++) {
        // Run for (n-1) times
        min = INT_MAX;
        v = -1;

        // Select the unvisited node with the smallest distance
        for (w = 0; w < count; w++) {
            if (!flag[w] && dist[w] < min) {
                min = dist[w];
                v = w;
            }
        }

        if (v == -1) break; // All reachable nodes processed

        flag[v] = 1; // Mark node as visited

        // Update distances of neighbors
        for (w = 0; w < count; w++) {
            if (!flag[w] && cost_matrix[v][w] != INT_MAX) {
                if (dist[v] + cost_matrix[v][w] < dist[w]) {
                    dist[w] = dist[v] + cost_matrix[v][w];
                    last[w] = v;
                }
            }
        }
    }

    // Print shortest path costs
    printf("\nShortest path costs from router %d:\n", src_router);
    for (i = 0; i < count; i++) {
        if (dist[i] == INT_MAX)
            printf("To router %d: No Path\n", i);
        else
            printf("To router %d: Cost = %d\n", i, dist[i]);
    }

    return 0;
}
