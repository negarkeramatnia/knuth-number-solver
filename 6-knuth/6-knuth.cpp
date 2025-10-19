#include <iostream>
#include <cmath>
#include <unordered_set>
#include <chrono>
#include <queue>
using namespace std;

struct Node {
    double number;
    Node* parent;
    char action;
    int path;
    Node* next;
};
Node* allocated_head = nullptr;
unordered_set<double> visited;
double epsilon = 1e-9;

bool isEqual(double a, double b) {
    return fabs(a - b) <= epsilon;
}

bool addVisited_hash(double num) {
    if (visited.count(num)) return false;
    visited.insert(num);
    return true;
}

Node* createNode(double number, Node* parent, char action, int path) {
    Node* newNode = new Node{ number, parent, action, path };
    allocated_head = newNode;
    return newNode;
}

void printSolutionPath(Node* goalNode) {
    if (goalNode == nullptr || goalNode->action == 'I') {
        return;
    }
    printSolutionPath(goalNode->parent);
    double previous_number = goalNode->parent->number;
    switch (goalNode->action) {
    case 'm':
        cout << previous_number << " *2 = " << goalNode->number << "\n";
        break;
    case 'd':
        cout << previous_number << " -1 = " << goalNode->number << "\n";
        break;
    case 's':
        cout << "sqrt(" << previous_number << ") = " << goalNode->number << "\n";
        break;
    case 'f':
        cout << "floor(" << previous_number << ") = " << goalNode->number << "\n";
        break;
    }
}

void solve(double start, double target) {
    queue<Node*> frontier;
    Node* startNode = createNode(start, nullptr, 'I', 0);
    frontier.push(startNode);

    Node* goalNode = nullptr;
    char actions[4] = { 'm', 'd', 's', 'f' };

    while (!frontier.empty()) {
        Node* currentNode = frontier.front();
        frontier.pop();

        if (isEqual(currentNode->number, target)) {
            goalNode = currentNode;
            break;
        }
        if (currentNode->number < 0) continue;

        double newValue;
        for (int i = 0; i < 4; i++) {
            switch (actions[i]) {
            case 'm': newValue = currentNode->number * 2; break;
            case 'd': newValue = currentNode->number - 1; break;
            case 's':
                if (currentNode->number < 0) continue;
                newValue = sqrt(currentNode->number);
                break;
            case 'f':
                newValue = floor(currentNode->number);
                if (isEqual(newValue, currentNode->number)) continue;
                break;
            }
            if (addVisited_hash(newValue)) {
                frontier.push(createNode(newValue, currentNode, actions[i], currentNode->path + 1));
            }
        }
    }
    if (goalNode) {
        cout << "Solution found in " << goalNode->path << " steps!\n";
        printSolutionPath(goalNode);
    }
    else {
        cout << "No solution found.\n";
    }
    Node* curr = allocated_head;
    while (curr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

int main() {
    double target;
    cout << "Enter your target number: ";
    cin >> target;
    auto startTime = chrono::high_resolution_clock::now();

    solve(4.0, target);

    auto endTime = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(endTime - startTime).count();
    cout << "Execution time: " << duration << " seconds\n";
    return 0;
}