#include <numbers>
#include <iostream>
#include <queue>
#include <random>
#include "Tree.h"
#include "graphics.h"
#include "GlobalState.h"

void Tree::draw() {
	startingNode.draw();
}


void Tree::generateTree() {
    startingNode.removeChildren();
    int currentNodeAmount = 1;
    Node::maxDepth = 0;

    static std::mt19937 rng{ std::random_device{}() };

    std::uniform_real_distribution<float> angleDist(
        -std::numbers::pi_v<float> / 6.,
        std::numbers::pi_v<float> / 6.
    );

    std::uniform_real_distribution<float> lengthDist(
        0.1,
        1.9
    );

    std::uniform_int_distribution<int> branchDist(
        GlobalState::instance.minBranchesPerNode,
        GlobalState::instance.maxBranchesPerNode
    );

    std::queue<Node*> q;
    q.push(&startingNode);

    while (!q.empty()) {
        Node* parent = q.front();
        q.pop();

        // Generate a random amount of nodes to spawn
        int nodesToGenerate = branchDist(rng);
        // Clamp to max nodes
		nodesToGenerate = std::min(nodesToGenerate, GlobalState::instance.nodes - currentNodeAmount);
        for (int i = 0; i < nodesToGenerate; i++) {
            q.push(new Node(angleDist(rng), lengthDist(rng), parent));
            currentNodeAmount++;
        }
    }
}