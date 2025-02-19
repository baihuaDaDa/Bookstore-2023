#ifndef INC_2074_MEMORY_H
#define INC_2074_MEMORY_H

#include "memoryRiver.h"
#include <iostream>
#include <vector>

template<class INDEX, class VALUE>
struct Pair {

    INDEX index;
    VALUE value;

    Pair() : index(), value() {}

    Pair(const INDEX &index, const VALUE &value) : index(index), value(value) {}
};

template<class INDEX, class VALUE,
        int (*CmpIndex)(const INDEX &, const INDEX &),
        int (*CmpValue)(const VALUE &, const VALUE &)>
class memory {

private:
    static constexpr int SIZE_OF_BLOCK = 500;
    int num_of_block;

    struct BlockNode {

        Pair<INDEX, VALUE> element;
        int address;
        int size;
        int pre;
        int next;

        BlockNode() : element(), address(-1), size(0), pre(-1), next(-1) {}

        BlockNode(const Pair<INDEX, VALUE> &element, int address, int size, int pre, int next) : element(element), address(address), size(size), pre(pre), next(next) {}

        ~BlockNode() = default;

    }; // maintain the block link directly in the file

    BlockNode head, tail;
    int head_pos = sizeof(int), tail_pos = sizeof(int) + sizeof(BlockNode);

    static int cmp_pair(const Pair<INDEX, VALUE> &a, const Pair<INDEX, VALUE> &b) {
        int flag = CmpIndex(a.index, b.index);
        if (flag) {
            return flag;
        }
        return CmpValue(a.value, b.value);
    }

    int FindElement(const Pair<INDEX, VALUE> &element) {
        BlockNode node;
        int pos = head_pos;
        memory_BlockNode.read(node, pos);
        while (pos != tail_pos) {
//            std::cout << node.size << node.pre << std::endl;
//            if (node.index == "") std::cout << 1 <<  std::endl;
            if (cmp_pair(node.element, element) == 1) {
                break;
            }
            pos = node.next;
            memory_BlockNode.read(node, pos);
        }
        return node.pre;
    }

    int BinarySearchPair(const Pair<INDEX, VALUE> &element, const Pair<INDEX, VALUE> *data, const BlockNode &block) {
        int l = 0, r = block.size - 1, mid;
        while (l <= r) {
            mid = l + (r - l) / 2;
            int flag = cmp_pair(data[mid], element);
            if (flag == -1) {
                l = mid + 1;
            } else if (flag == 1) {
                r = mid - 1;
            } else {
                return mid;
            }
        }
        return l;
    }

    memoryRiver<BlockNode, 1> memory_BlockNode;
    memoryRiver<Pair<INDEX, VALUE>[SIZE_OF_BLOCK], 0> memory_element;
    string element_file_name;

public:
    memory() = default;

    ~memory() = default;

    memory(const string &blockNode_file_name, const string &element_file_name)
            : memory_BlockNode(blockNode_file_name), memory_element(element_file_name), element_file_name(element_file_name) {
        if (!memory_BlockNode.file_exist()) {
            memory_BlockNode.initialise();
            BlockNode head_node, tail_node;
            head_node.next = tail_pos;
            tail_node.pre = head_pos;
            memory_BlockNode.write(head_node);
            memory_BlockNode.write(tail_node);
        }
        if (!memory_element.file_exist()) {
            memory_element.initialise();
        } // Check if the file exist. If not, then initialise it.
        memory_BlockNode.read(head, head_pos);
        memory_BlockNode.read(tail, tail_pos);
        memory_BlockNode.get_info(num_of_block, 1);
    }

    void Insert(const INDEX &index, VALUE &value) {
        Pair<INDEX, VALUE> element(index, value);
        int pos = FindElement(element);
        BlockNode now;
        Pair<INDEX, VALUE> data[SIZE_OF_BLOCK] = {};
        fstream file;
        if (pos == head_pos) {
            memory_BlockNode.read(head, head_pos);
            pos = head.next;
            if (pos == tail_pos) {
                BlockNode new_block(element, sizeof(Pair<INDEX, VALUE>[SIZE_OF_BLOCK]) * (num_of_block++), 1, head_pos, tail_pos);
                memory_BlockNode.write_info(num_of_block, 1);
                head.next = memory_BlockNode.write(new_block);
                tail.pre = head.next;
                memory_BlockNode.update(head, head_pos);
                memory_BlockNode.update(tail, tail_pos);
                file.open(element_file_name, std::ios::out | std::ios::in);
                file.seekp(new_block.address);
                file.write(reinterpret_cast<char *>(&element), sizeof(Pair<INDEX, VALUE>));
                file.close();
                return;
            }
            memory_BlockNode.read(now, pos);
            if (now.size + 1 <= SIZE_OF_BLOCK) {
                file.open(element_file_name, std::ios::in);
                file.seekg(now.address);
                file.read(reinterpret_cast<char *>(data + 1), sizeof(Pair<INDEX, VALUE>) * now.size);
                file.close();
                data[0] = element;
                now.element = element;
                now.size++;
                memory_BlockNode.update(now, pos);
                file.open(element_file_name, std::ios::out | std::ios::in);
                file.seekp(now.address);
                file.write(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
                file.close();
            } else {
                BlockNode new_block(element, sizeof(Pair<INDEX, VALUE>[SIZE_OF_BLOCK]) * (num_of_block++), SIZE_OF_BLOCK / 2, now.pre, pos);
                now.size /= 2;
                Pair<INDEX, VALUE> new_data[SIZE_OF_BLOCK] = {};
                file.open(element_file_name, std::ios::in);
                file.seekg(now.address);
                file.read(reinterpret_cast<char *>(new_data + 1), sizeof(Pair<INDEX, VALUE>) * new_block.size);
                file.seekg(now.address + new_block.size * sizeof(Pair<INDEX, VALUE>));
                file.read(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
                file.close();
                new_data[0] = element;
                now.element = data[0];
                new_block.size++;
                BlockNode pre;
                memory_BlockNode.read(pre, now.pre);
                pre.next = memory_BlockNode.write(new_block);
                now.pre = pre.next;
                memory_BlockNode.update(pre, new_block.pre);
                memory_BlockNode.update(now, pos);
                memory_BlockNode.write_info(num_of_block, 1);
                file.open(element_file_name, std::ios::out | std::ios::in);
                file.seekp(now.address);
                file.write(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
                file.seekp(new_block.address);
                file.write(reinterpret_cast<char *>(new_data), sizeof(Pair<INDEX, VALUE>) * new_block.size);
                file.close();
            } // 裂块
            return;
        } // 首个块的第一个位置插入
        memory_BlockNode.read(now, pos);
        file.open(element_file_name, std::ios::in);
        file.seekg(now.address);
        file.read(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
        file.close();
        int element_pos = BinarySearchPair(element, data, now);
        if (element_pos < now.size) {
            if (cmp_pair(data[element_pos], element) == 0) {
                return;
            }
        }
        if (now.size + 1 <= SIZE_OF_BLOCK) {
            for (int i = now.size - 1; i >= element_pos; i--) {
                data[i + 1] = data[i];
            }
            data[element_pos] = element;
            now.size++;
            memory_BlockNode.update(now, pos);
            file.open(element_file_name, std::ios::out | std::ios::in);
            file.seekp(now.address);
            file.write(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
            file.close();
        } else {
            const int half_size = SIZE_OF_BLOCK / 2;
            BlockNode new_block(now.element, sizeof(Pair<INDEX, VALUE>[SIZE_OF_BLOCK]) * (num_of_block++), half_size, now.pre, pos);
            now.size = half_size;
            Pair<INDEX, VALUE> new_data[SIZE_OF_BLOCK] = {};
            if (element_pos < half_size) {
                for (int i = 0; i < element_pos; i++) {
                    new_data[i] = data[i];
                }
                for (int i = half_size; i > element_pos; i--) {
                    new_data[i] = data[i - 1];
                }
                new_data[element_pos] = element;
                for (int i = 0; i < half_size; i++) {
                    data[i] = data[half_size + i];
                }
                new_block.size++;
                now.element = data[0];
            } else {
                for (int i = 0; i < half_size; i++) {
                    new_data[i] = data[i];
                }
                for (int i = 0; i < element_pos - half_size; i++) {
                    data[i] = data[i + half_size];
                }
                for (int i = half_size; i > element_pos - half_size; i--) {
                    data[i] = data[i + half_size - 1];
                }
                data[element_pos - half_size] = element;
                now.element = data[0];
                now.size++;
            };
            BlockNode pre;
            memory_BlockNode.read(pre, now.pre);
            pre.next = memory_BlockNode.write(new_block);
            now.pre = pre.next;
            memory_BlockNode.update(pre, new_block.pre);
            memory_BlockNode.update(now, pos);
            memory_BlockNode.write_info(num_of_block, 1);
            file.open(element_file_name, std::ios::out | std::ios::in);
            file.seekp(now.address);
            file.write(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
            file.seekp(new_block.address);
            file.write(reinterpret_cast<char *>(new_data), sizeof(Pair<INDEX, VALUE>) * new_block.size);
            file.close();
        } // 裂块
        // 其余位置插入
    }

    void Delete(const INDEX &index, const VALUE &value) {
        Pair<INDEX, VALUE> element(index, value);
        int pos = FindElement(element);
        if (pos == head_pos) {
            return;
        }
        BlockNode now;
        Pair<INDEX, VALUE> data[SIZE_OF_BLOCK] = {};
        fstream file;
        memory_BlockNode.read(now, pos);
        file.open(element_file_name, std::ios::in);
        file.seekg(now.address);
        file.read(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
        file.close();
        int element_pos = BinarySearchPair(element, data, now);
        if (element_pos < now.size) {
            if (cmp_pair(data[element_pos], element) == 0) {
                if (now.size > 1) {
                    for (int i = element_pos; i < now.size - 1; i++) {
                        data[i] = data[i + 1];
                    }
                    now.size--;
                    now.element = data[0];
                    memory_BlockNode.update(now, pos);
                    file.open(element_file_name, std::ios::out | std::ios::in);
                    file.seekp(now.address);
                    file.write(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
                    file.close();
                } else {
                    BlockNode pre, next;
                    memory_BlockNode.read(pre, now.pre);
                    memory_BlockNode.read(next, now.next);
                    pre.next = now.next;
                    next.pre = now.pre;
                    memory_BlockNode.update(pre, now.pre);
                    memory_BlockNode.update(next, now.next);
                } // 删除空块
            }
        }
    }

    std::vector<Pair<INDEX, VALUE>> Find(const INDEX &index) {
        std::vector<Pair<INDEX, VALUE>> out;
        Pair<INDEX, VALUE> element(index, VALUE());
        int pos = FindElement(element);
        BlockNode now;
        if (pos == head_pos) {
            memory_BlockNode.read(head, head_pos);
            pos = head.next;
            if (pos == tail_pos) {
                return out;
            }
        }
        fstream file;
        Pair<INDEX, VALUE> data[SIZE_OF_BLOCK] = {};
        int i;
        bool flag;
        while (pos != tail_pos) {
            flag = false;
            memory_BlockNode.read(now, pos);
            file.open(element_file_name, std::ios::in);
            file.seekg(now.address);
            file.read(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
            file.close();
            for (i = 0; i < now.size; i++) {
                if (CmpIndex(data[i].index, index) == 0) {
                    flag = true;
                    out.push_back(data[i]);
                } else if (flag) {
                    break;
                }
            }
            if (i != now.size) {
                break;
            }
            pos = now.next;
        }
        return out;
    }

    void Output() {
        memory_BlockNode.read(head, head_pos);
        int pos = head.next;
        BlockNode now;
        fstream file;
        Pair<INDEX, VALUE> data[SIZE_OF_BLOCK] = {};
        while (pos != tail_pos) {
            memory_BlockNode.read(now, pos);
            file.open(element_file_name, std::ios::in);
            file.seekg(now.address);
            file.read(reinterpret_cast<char *>(data), sizeof(Pair<INDEX, VALUE>) * now.size);
            file.close();
            for (int i = 0; i < now.size; i++) {
                std::cout << data[i].index << '\t' << data[i].value << '\n';
            }
            pos = now.next;
        }
    }
};

#endif //INC_2074_MEMORY_H
