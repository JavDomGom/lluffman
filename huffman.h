#include <vector>
#include <string>

#define HUFFMAN_LEAF 0
#define HUFFMAN_PAIR 1

typedef int leaf_t;

typedef struct huffman_pair {
    struct huffman_node * left;
    struct huffman_node * right;
} huffman_pair_t;

typedef struct huffman_node {
    int tag;
    int weight;
    union {
        leaf_t leaf;
        struct huffman_pair pair;
    } value;
} huffman_node_t;

huffman_node * make_huffman_code(std::vector<int> weights);

void free_huffman_code(huffman_node_t * code);
int huffman_encode(huffman_node_t * code, std::vector<bool> bit_source,
                   int * p_source, leaf_t * p_decode);
std::vector<bool> huffman_decode(huffman_node_t * code, leaf_t id, int * status);
std::vector<bool> read_file_as_bool(std::string path);
