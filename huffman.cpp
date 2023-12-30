#include <vector>
#include <queue>
#include <stdio.h>
#include "huffman.h"

void print_huffman_code(huffman_node_t * p, int level) {
    if (p->tag == HUFFMAN_LEAF) {
        for (int i = 0; i < level; i++) { putchar(' '); }
        printf("(%d)->%d\n", p->weight, p->value.leaf);
    }
    else {
        for (int i = 0; i < level; i++) { putchar(' '); }
        printf("left (%d):\n", p->weight);
        print_huffman_code(p->value.pair.left, level+1);
        for (int i = 0; i < level; i++) { putchar(' '); }
        printf("right (%d):\n", p->weight);
        print_huffman_code(p->value.pair.right, level+1);
    }
}

huffman_node * make_huffman_code(std::vector<int> weights) {
                                 //std::vector<token_id> leaves) {
    std::vector<huffman_node*> nodes;
    huffman_node * p;
    nodes.reserve(weights.size()+1);
    for (int i = 0; i < weights.size(); i++) {
        p = new huffman_node;
        p->tag = HUFFMAN_LEAF;
        p->weight = weights[i];
        p->value.leaf = i;
        nodes.push_back(p);
    }

    for (int i = 0; i < (weights.size() - 1); i++) {
        huffman_node *n1, *n2, *npair;
        
        int i_min = 0;
        int v_min = nodes[0]->weight;
        for (int j = 1; j < nodes.size(); j++) {
            if (nodes[j]->weight < v_min) {
                i_min = j;
                v_min = nodes[j]->weight;
            }
        }
        
        n1 = nodes[i_min];
        nodes.erase(nodes.begin() + i_min);

        i_min = 0;
        v_min = nodes[0]->weight;
        for (int j = 1; j < nodes.size(); j++) {
            if (nodes[j]->weight < v_min) {
                i_min = j;
                v_min = nodes[j]->weight;
            }
        }

        n2 = nodes[i_min];
        nodes.erase(nodes.begin() + i_min);

        npair = new huffman_node;
        npair->tag = HUFFMAN_PAIR;
        npair->weight = n1->weight + n2->weight;
        npair->value.pair.left = n1;
        npair->value.pair.right = n2;

        nodes.push_back(npair);
    }
    /*for (int i = 0; i < nodes.size(); i++) {
        print_huffman_code(nodes[i], 0);
        }*/
    return nodes[0];
}

void free_huffman_code(huffman_node_t * code) {
    if (code->tag == HUFFMAN_PAIR) {
        free_huffman_code(code->value.pair.left);
        free_huffman_code(code->value.pair.right);
    }
    delete code;
}

int huffman_encode(huffman_node_t * code, std::vector<bool> bit_source,
                   int * p_source, leaf_t * p_decode) {
    bool b;
    while (1) {
        if (code->tag == HUFFMAN_LEAF) {
            *p_decode = code->value.leaf;
            //*p_end = start;
            return 0;
        }
        if (*p_source >= bit_source.size()) { // hit the end of the vector
            return -1;
        }

        b = bit_source[*p_source];
        (*p_source)++;

        if (b) {
            code = code->value.pair.right;
        }
        else {
            code = code->value.pair.left;
        }
    }
}

std::vector<bool> huffman_decode(huffman_node_t * code, leaf_t id, int * status) {
    int lstatus;
    std::vector<bool> result = {};
    if (code->tag == HUFFMAN_LEAF) {
        if (id == code->value.leaf) {
            *status = 1;
            return result;
        }
        else {
            *status = 0;
            return result;
        }
    }
    else {
        result = huffman_decode(code->value.pair.left, id, &lstatus);
        if (lstatus == 1) {
            result.push_back(false);
            *status = 1;
            return result;
        }

        result = huffman_decode(code->value.pair.right, id, &lstatus);
        if (lstatus == 1) {
            result.push_back(true);
            *status = 1;
            return result;
        }

        *status = 0;
        return result;
    }   
}

std::vector<bool> read_file_as_bool(std::string path) {
    int c;
    std::vector<bool> x;

    FILE * fp = fopen(path.c_str(), "rb");
    while (1) {
        c = fgetc(fp);
        if (c == EOF) { break; }
        for (int i = 0; i < 8; i++) {
            x.push_back((c & (0x01 << i)) >> i);
        }
    }
    /*
    printf("read %d bits:\n", x.size());
    for (int i = 0; i < x.size(); i++) {
        printf("%c", x[i] ? '1' : '0');
    }
    printf("\n");
    */
    return x;
}
