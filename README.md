# huffman

Huffman coding.

## About

[Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) is used in compression algorithms. This is a simple implementation to explore the concept.

The basic idea is that you encode symbols as paths in a tree, where 0 is a left turn and 1 is a right turn. The way the data is compressed is by letting more frequent symbols have shorter paths in the tree and therefore shorter bit strings.

For example, if you want to compress the string "hello", the most frequent symbol is the letter l. If the letter l can be reached in the tree with a single right turn, then it can be encoded as a single bit 1. The letter h on the other hand which is less frequent may be a series of left turns, resulting in three 0 bits.

The tree is required for both encoding and decoding, so the encoded data needs to include some representation of the tree.