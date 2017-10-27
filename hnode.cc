#include <iostream>
#include "hnode.h"

HNode::HNode(int size) {
    this->buckets = new FSet[size];
    for(int i = 0; i<size; i++) {
        this->buckets[i] = new FSet();
    }
    this->size = size;
    this->pred = NULL;
}

bool HNode::insert(int key) {return false;}

bool HNode::remove(int key) {return false;}

bool HNode::contains(int key) {return false;}

void HNode::resize(bool grow) {}

bool HNode::apply(OPType type, int key) {return false;}

void HNode::initBucket(HNode t, int hashIndex) {}

int main(void) {return 0;}