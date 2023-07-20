#include "main.h"
#define MAX_TREE_HT 70

int ct = 1;
struct Human {
    int id, region, result, num;
    Human() {
        id = region = result = num = 0;
    }
    Human(int id, int result, int region, int num) {
        this->id = id;
        this->result = result;
        this->region = region;
        this->num = num;
    }
};

struct Node {
    struct Node *next;
    struct Node *pre;
    int key;
    Human value;
    Node(int key, Human value) {
        this->key = key;
        this->value = value;
        this->next = NULL;
        this->pre = NULL;
    }
};

struct HashTable {
    Node *tail = NULL;
    Node *head = NULL;
    void push(int key, Human value) {
        Node *newNode = new Node(key, value);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->pre = tail;
            tail = newNode;
        }
    }
};

// HashTable *M = new HashTable();
//  HashTable *Customer = new HashTable();
map<char, string> M;

struct Node2 {
    char data;
    int freq;
    int weight;

    Node2 *left, *right;

    // constructor
    Node2(char d, int f, int w, Node2 *l = nullptr, Node2 *r = nullptr) {
        data = d;
        freq = f;
        weight = w;
        left = l;
        right = r;
    }
};

class compare {
   public:
    bool operator()(Node2 *a, Node2 *b) {
        if (a->freq == b->freq) {
            if (a->weight == b->weight) return (a->data > b->data);
            return (a->weight > b->weight);
        }
        return (a->freq > b->freq);
    }
};

void print_codes(Node2 *node, string str) {
    if (node != nullptr) {
        if (node->data != '*')
            M[node->data] = str;
        // cout << node->data << ": " << str << endl;

        print_codes(node->left, str + '0');
        print_codes(node->right, str + '1');
    }
}

void cleanup(Node2 *node) {
    // recurse till all the elements are exhausted
    if (node != nullptr) {
        cleanup(node->left);
        cleanup(node->right);

        delete node;
    }
}

void HuffmanCode(char *arr, int *freq, size_t n) {
    Node2 *left, *right, *top;
    priority_queue<Node2 *, vector<Node2 *>, compare> q;

    // first fill the priority queue with data
    for (size_t i = 0; i < n; i++) {
        q.push(new Node2(arr[i], freq[i], 0));
    }

    /*while (q.size()) {
        top = q.top();
        cout << top->data << " " << top->freq << " " << top->weight << "\n";
        q.pop();
    }*/
    // time to extract minimum element, combine them and reinsert again
    while (q.size() > 1) {
        left = q.top();
        q.pop();

        right = q.top();
        q.pop();

        // '*' acts as dummy character
        top = new Node2('*', left->freq + right->freq, ct++, left, right);
        q.push(top);
    }

    print_codes(q.top(), "");

    // free up the heap space
    cleanup(q.top());
}

int binaryToDecimal(string x) {
    int ans = 0, ct = 0;
    for (int i = x.size() - 1; i >= 0; i--) ans += (int)(x[i] - '0' - 0) * (1 << (ct++));
    return ans;
}

/*.......................................................................................*/

struct Node1 {
    Human data;
    Node1 *left;
    Node1 *right;
    int height;
};

int getHeight(Node1 *root) {
    if (root == NULL)
        return 0;
    // return 1 + max(getHeight(root->left), getHeight(root->right));
    return root->height;
}

Node1 *rightRotate(Node1 *root) {
    Node1 *x = root->left;

    // Bắt đầu quay phải
    root->left = x->right;
    x->right = root;

    // Thiết lập chiều cao
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    // Return x - trả về root mới
    return x;
}

Node1 *leftRotate(Node1 *root) {
    Node1 *y = root->right;

    // Bắt đầu quay trái
    root->right = y->left;
    y->left = root;

    // Thiết lập chiều cao
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    // Return y - trả về root mới
    return y;
}
// Insertion - AVL Tree
Node1 *Insert(Node1 *root, Human value) {
    // 1. Insert
    if (root == NULL)
        return new Node1{value, NULL, NULL, 1};  // Trả về Node có height = 1
    if (value.result >= root->data.result)
        root->right = Insert(root->right, value);
    else if (value.result < root->data.result)
        root->left = Insert(root->left, value);

    // 2. Set height
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // 3. Rotate
    int valBalance = getHeight(root->left) - getHeight(root->right);

    // Kiểm tra 4 TH xảy ra:
    // 4.1. Left left
    if (valBalance > 1 && value.result < root->left->data.result)
        return rightRotate(root);

    // 4.2. Right Right
    if (valBalance < -1 && value.result > root->right->data.result)
        return leftRotate(root);

    // 4.3. Left Right
    if (valBalance > 1 && value.result > root->left->data.result) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // 4.4. Right Left
    if (valBalance < -1 && value.result < root->right->data.result) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Giá trị cân bằng - Value Balance
int valueBalance(Node1 *root) {
    if (root == NULL)
        return 0;
    return getHeight(root->left) - getHeight(root->right);
}

Node1 *maxValueNode(Node1 *root) {
    Node1 *current = root;

    // Tìm kiếm Node có giá trị lớn nhất
    while (current->right != NULL)
        current = current->right;

    return current;
}

// Deletion - AVL Tree
// xoa theo result
Node1 *deleteNode(Node1 *root, int key) {
    // 1. XÓA NODE - DELETE
    if (root == NULL)
        return root;

    if (key > root->data.result)
        root->right = deleteNode(root->right, key);
    else if (key < root->data.result)
        root->left = deleteNode(root->left, key);

    // Nếu key có giá trị bằng với root->data
    // Thì đây chính là Node cần xóa
    else {
        // Trường hợp 0 con hoặc 1 con
        if (root->left == NULL || root->right == NULL) {
            // Sử dụng Node temp để kiểm tra
            Node1 *temp = root->left;
            if (root->right != NULL)
                temp = root->right;

            // TH: 0 con - No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
                delete temp;
            }
            // TH: 1 con - One child case
            else {
                // Gán tất cả các giá trị (bao gồm left, right, height) của temp vào root
                *root = *temp;
                delete temp;
            }
        }
        // Trường hợp 2 con - Two child case
        else {
            // Tìm Node lớn nhất bên trái (nhỏ nhất bên phải)
            // successor - biggest in the left subtree
            Node1 *temp = maxValueNode(root->left);

            // Đưa data của temp vào root
            root->data = temp->data;

            // Xóa temp như 2 TH trên - Delete the inorder successor
            root->right = deleteNode(root->right, temp->data.result);
        }
    }

    // Nếu không còn gì thì trả về luôn !!
    if (root == NULL)
        return root;

    // 2. CẬP NHẬT CHIỀU CAO - UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // 3. CÂN BẰNG CÂY - GET THE BALANCE FACTOR
    int valBalance = valueBalance(root);

    // Kiểm tra 4 TH xảy ra - There are 4 cases

    // Case 1: Left left - Trái trái
    if (valBalance > 1 && valueBalance(root->left) >= 0)
        return rightRotate(root);

    // Case 2: Right right - Phải phải
    if (valBalance < -1 && valueBalance(root->right) <= 0)
        return leftRotate(root);

    // Case 3: Left right - Trái phải
    if (valBalance > 1 && valueBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Case 4: Right left - Phải trái
    if (valBalance < -1 && valueBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
// In - Print 2D ra console
void print2DUtil(Node1 *root) {
    if (root == NULL) return;
    queue<Node1> q;
    q.push(*root);
    while (q.size()) {
        Node1 root1 = q.front();
        q.pop();
        cout << root1.data.id << "-" << root1.data.result << "-" << root1.data.num << "\n";
        if (root1.left != NULL) q.push(*root1.left);
        if (root1.right != NULL) q.push(*root1.right);
    }
    /*cout << root->data.id << "-" << root->data.result << "-" << root->data.num << "\n";
    if (root->left != NULL) print2DUtil(root->left);
    if (root->right != NULL) print2DUtil(root->right);*/
    return;
}

void swap(pair<int, int> *x, pair<int, int> *y);

// A class for Min Heap
class MinHeap {
    pair<int, int> *harr;  // pointer to array of elements in heap
    int capacity;          // maximum possible size of min heap
    int heap_size;         // Current number of elements in min heap
   public:
    // Constructor
    MinHeap(int capacity);

    // to heapify a subtree with the root at given index
    void MinHeapify(int);

    int parent(int i) { return (i - 1) / 2; }

    // to get index of left child of node at index i
    int left(int i) { return (2 * i + 1); }

    // to get index of right child of node at index i
    int right(int i) { return (2 * i + 2); }

    // to extract the root which is the minimum element
    int extractMin();

    // Decreases key value of key at index i to new_val
    void decreaseKey(int i, int new_val);

    // Returns the minimum key (key at root) from min heap
    int getMin() { return harr[0].second; }

    // Deletes a key stored at index i
    void deleteKey(int i);

    // Inserts a new key 'k'
    void insertKey(Human k);

    void printtree(int pos);

    int deleteValue(int pos, int val);
};

// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap) {
    heap_size = 0;
    capacity = cap;
    harr = new pair<int, int>[cap];
}

// Inserts a new key 'k'
void MinHeap::insertKey(Human k) {
    if (heap_size == capacity) {
        return;
    }

    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr[i] = {k.id, k.num};

    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)].second > harr[i].second) {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

// Decreases value of key at index 'i' to new_val. It is assumed that
// new_val is smaller than harr[i].
void MinHeap::decreaseKey(int i, int new_val) {
    harr[i].second = new_val;
    while (i != 0 && harr[parent(i)].second > harr[i].second) {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

// Method to remove minimum element (or root) from min heap
int MinHeap::extractMin() {
    if (heap_size <= 0)
        return INT_MAX;
    if (heap_size == 1) {
        heap_size--;
        return harr[0].second;
    }

    // Store the minimum value, and remove it from heap
    int root = harr[0].second;
    harr[0] = harr[heap_size - 1];
    heap_size--;
    MinHeapify(0);

    return root;
}

// This function deletes key at index i. It first reduced value to minus
// infinite, then calls extractMin()
void MinHeap::deleteKey(int i) {
    decreaseKey(i, INT_MIN);
    extractMin();
}

// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i) {
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;
    if (smallest != i) {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}

// A utility function to swap two elements
void swap(pair<int, int> *x, pair<int, int> *y) {
    pair<int, int> temp = *x;
    *x = *y;
    *y = temp;
}

int MinHeap::deleteValue(int pos, int key) {
    int l = left(pos);
    int r = right(pos);
    int flag = 0;
    if (harr[pos].second == key) {
        deleteKey(pos);
        return 1;
    }
    if (flag == 0 && l < heap_size) {
        flag = deleteValue(l, key);
    }
    if (flag == 0 && r < heap_size) {
        flag = deleteValue(r, key);
    }
    return flag;
}

void MinHeap::printtree(int pos) {
    int l = left(pos);
    int r = right(pos);
    cout << harr[pos].first << "-" << harr[pos].second << "\n";
    if (l < heap_size) printtree(l);
    if (r < heap_size) printtree(r);
    return;
}

#define pii pair<int, int>
#define fi first
#define se second

bool cmp(pair<int, Human> a, pair<int, Human> b) {
    if (a.fi == b.fi)
        return (a.se.num < b.se.num);
    else
        return (a.fi < b.fi);
}

char kitu[75];
int dd[75], customerReg1, customerReg2;
int freq[75];
int table[75];
// queue de xu ly query FIFO
queue<Human> q;
// deque de xu ly query LRCO
deque<Human> dq;

// HashTable *M = new HashTable();
HashTable *Customer = new HashTable();

MinHeap h(MAXSIZE);
Node1 *tree = NULL;
/*..............................................................................................*/
void reg(ifstream &read);
void cle(ifstream &read);
void printHT();
void printAVL();
void printMH();

void simulate(string filename) {
    // Tạo input stream giống như cin, nhưng khác ở chỗ nó đọc từ file chứ ko đọc từ bàn phím
    ifstream read(filename);

    // Chuỗi này để đọc operation ở đầu mỗi dòng trong file
    string operation = "";

    // Đọc đến khi hết file
    while (!read.eof()) {
        // cout << operation << "\n";
        if (operation == "") read >> operation;
        if (operation == "REG") {
            // Truyền cái input stream vào hàm reg để đọc tiếp thông tin của khách hàng
            // Lưu ý mình chỉ duy trì 1 input stream, nên nó sẽ đọc tiếp từ vị trí trước đó
            reg(read);
            operation = "";
        } else if (operation == "CLE") {
            cle(read);
            operation = "";
        } else if (operation == "PrintHT") {
            printHT();
            operation = "";
        } else if (operation == "PrintAVL") {
            printAVL();
            operation = "";
        } else if (operation == "PrintMH") {
            printMH();
            operation = "";
        }
    }
    // cout << "hdz";
    return;
}

bool cmp1(pair<char, int> a, pair<char, int> b) {
    if (a.se == b.se)
        return a.fi < b.fi;
    else
        return a.se < b.se;
}
void reg(ifstream &read) {
    string name;
    read >> name;
    ct = 0;
    memset(dd, 0, sizeof(dd));
    for (size_t i = 0; i < name.size(); i++) {
        int num = (int)(name[i] - 'A' - 0);
        // cout << name[i] << " " << num << endl;
        if (!dd[num]) kitu[ct++] = name[i];
        dd[num]++;
    }
    int dem = 0;
    for (int i = 0; i < ct; i++) {
        int num = (int)(kitu[i] - 'A' - 0);
        freq[dem++] = dd[num];
    }
    vector<pair<char, int> > v;
    for (int i = 0; i < ct; i++) v.push_back({kitu[i], freq[i]});
    sort(v.begin(), v.end(), cmp1);
    for (int i = 0; i < ct; i++) kitu[i] = v[i].fi, freq[i] = v[i].se;
    // for (int i = 0; i < ct; i++) cout << kitu[i] << " " << freq[i] << "\n";
    // return;
    HuffmanCode(kitu, freq, ct);

    /*map<char, string>::iterator it;
    for (it = M.begin(); it != M.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }
    return;*/

    int result = 0;
    string temp = "";
    if (name.size() == 1)
        result = 1;
    else {
        for (size_t i = 0; i < name.size(); i++) temp = temp + M[name[i]];
        while (temp.size() > 15) temp.erase(temp.begin(), temp.begin() + 1);
        // reverse(temp.begin(), temp.end());
        //  cout << temp << "\n";
        result = binaryToDecimal(temp);
        // cout << name << ": " << temp << "  result:  " << result << "\n";
    }

    int region = ((result & 1) ? 1 : 2);
    // cout << Region;

    int id = result % MAXSIZE + 1;
    // cout << id << "\n";
    // cout << name << ": " << result << "\n";
    Human hu(id, result, region, 1);

    if (table[id] == result) {
        for (size_t i = 0; i < dq.size(); i++) {
            if (dq[i].result == result) {
                hu = dq[i];
                dq.erase(dq.begin() + i, dq.begin() + i + 1);
                break;
            }
        }
        hu.num++;
        dq.push_back(hu);
    } else {
        dem = 0;
        while (table[hu.id]) {
            dem++;
            hu.id = (hu.id + 1) % MAXSIZE;
            if (dem == MAXSIZE + 1) break;
        }

        if (ct == MAXSIZE + 1) {
            // duoi khach cu them khach moi
            int opt = result % 3;
            if (opt == 0) {
                Human cus = q.front();
                table[cus.id] = 0;
                if (cus.region == 2) {
                    tree = deleteNode(tree, cus.result);
                    customerReg2--;
                } else {
                    Node *pt = Customer->head;
                    Customer->head = Customer->head->next;
                    delete pt;
                    customerReg1--;
                }
            } else if (opt == 1) {
                Human cusOut = dq.front();
                table[cusOut.id] = 0;
                dq.pop_front();
                if (cusOut.region == 2)
                    tree = deleteNode(tree, cusOut.result);
                else {
                    Node *pt = Customer->head;
                    while (1) {
                        if (pt == NULL) break;
                        if (pt->value.result == cusOut.result) {
                            Node *ptPre = pt->pre;
                            Node *ptNext = pt->next;
                            if (ptPre == NULL || ptNext == NULL) {
                                delete pt;
                                break;
                            } else {
                                ptPre->next = ptNext;
                                ptNext->pre = ptPre;
                                delete pt;
                                break;
                            }
                        }
                        pt = pt->next;
                    }
                }
            } else if (opt == 2) {
                h.extractMin();
            }

        } else if (region == 1 && customerReg1 == MAXSIZE / 2)
            region = 2;
        else if (region == 2 && customerReg2 == MAXSIZE / 2)
            region = 1;

        dq.push_back(hu);
        // cout << hu.id << " " << hu.result << " " << hu.region << "\n";
        table[hu.id] = result;
        h.insertKey(hu);
        if (region == 1) {
            q.push(hu);
            Customer->push(result % (MAXSIZE / 2), hu);
            customerReg1++;
        } else {
            q.push(hu);
            tree = Insert(tree, hu);
            customerReg2++;
        }
    }
}

void cle(ifstream &read) {
    string x;
    read >> x;
    int num = stoi(x);
    // cout << "asdasd " << num << " " << table[num] << endl;
    if (num < 1) {
        customerReg1 = 0;
        deque<Human> dqTemp;
        for (size_t i = 0; i < dq.size(); i++) {
            if (dq[i].region == 2)
                dqTemp.push_back(dq[i]);
            else {
                table[dq[i].id] = 0;
                h.deleteValue(0, dq[i].num);
            }
        }

        dq.clear();
        for (size_t i = 0; i < dqTemp.size(); i++) dq.push_back(dqTemp[i]);

        Node *pt = Customer->head;
        while (1) {
            Node *pt1 = pt;
            delete pt1;
            if (pt->next != NULL)
                pt = pt->next;
            else
                break;
        }
    } else if (num > MAXSIZE) {
        customerReg2 = 0;
        deque<Human> dqTemp;
        for (size_t i = 0; i < dq.size(); i++) {
            if (dq[i].region == 1)
                dqTemp.push_back(dq[i]);
            else {
                h.deleteValue(0, dq[i].num);
                tree = deleteNode(tree, dq[i].result);
                table[dq[i].id] = 0;
            }
        }
        dq.clear();
        for (size_t i = 0; i < dqTemp.size(); i++) dq.push_back(dqTemp[i]);
    } else {
        // cout << "asdasd " << table[num] << endl;
        int owner = table[num];
        if (table[num] == 0) return;
        for (size_t i = 0; i < dq.size(); i++) {
            if (dq[i].result == owner) {
                h.deleteValue(0, dq[i].num);
                if (dq[i].region == 2) {
                    tree = deleteNode(tree, dq[i].result);
                } else {
                    Node *pt = Customer->head;
                    while (1) {
                        if (pt == NULL) break;
                        if (pt->value.result == dq[i].result) {
                            Node *ptPre = pt->pre;
                            Node *ptNext = pt->next;
                            if (ptPre == NULL || ptNext == NULL) {
                                delete pt;
                                break;
                            } else {
                                ptPre->next = ptNext;
                                ptNext->pre = ptPre;
                                delete pt;
                                break;
                            }
                        }
                        pt = pt->next;
                    }
                }
            }
        }
    }
}

void printHT() {
    vector<pair<int, Human> > v;
    Node *pt = Customer->head;
    while (1) {
        if (pt == NULL) break;
        v.push_back({pt->key, pt->value});
        pt = pt->next;
    }
    sort(v.begin(), v.end(), cmp);
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i].se.id << "-" << v[i].se.result << "-" << v[i].se.num << "\n";
    }
    return;
}

void printAVL() {
    print2DUtil(tree);
}

void printMH() {
    h.printtree(0);
}
// 111001110110001010010111101010010111011010000011000110011100000111110110001100000111001111100011101
