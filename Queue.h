#ifndef TRY_QUEUE_H
#define TRY_QUEUE_H

#include <queue>
#include <mutex>

class Element {
public:

    void Init(std::pair<int, int> p, int node) {
        first = (int*) numa_alloc_onnode(sizeof(int), node);
        *first = p.first;
        second = (int*) numa_alloc_onnode(sizeof(int), node);
        *second = p.second;
        next = nullptr;
    }

    void SetNext(Element* e) {
        next.store(e);
    }

    int* GetFirst() {
        return first;
    }

    int* GetSecond() {
        return second;
    }

    Element* GetNext() {
        return next.load();
    }

    ~Element() {
        numa_free(first, sizeof(int));
        numa_free(second, sizeof(int));
    }

private:
    int* first;
    int* second;
    std::atomic<Element*> next;
};

// какая-то очень быстро написанная очередь
// ее надо бы проверить и пофиксить как минимум мьютекс
class Queue {
public:
    void Init(int node) {
        this->node = node;
        head = nullptr;
        tail = head;
    }

    void Push(std::pair<int, int> p) {
        m.lock();
        auto e = (Element*) numa_alloc_onnode(sizeof(Element), node);
        e->Init(p, node);

        if (tail == nullptr) {
            head = e;
            tail = e;
        } else {
            tail->SetNext(e);
            tail = e;
        }
        m.unlock();
    }

    std::vector<std::pair<int, int>> List() {
        std::cerr << "in Queue List \n";
        m.lock();
        std::vector<std::pair<int, int>> result;
        while (!empty()) {
            std::cerr << "in while \n";
            auto p = pop();
            std::cerr << "p poped \n";
            if (p == nullptr) {
                std::cerr << "p is null \n";
                break;
            }
            result.emplace_back(std::make_pair(*p->GetFirst(), *p->GetSecond()));
            std::cerr << "result updated \n";
            std::cerr << *p->GetFirst() <<  " " << *p->GetSecond() << "\n";
        }
        m.unlock();
        return result;
    }

private:
    Element* pop() {
        std::cerr << "in pop \n";
        auto e = head;
        std::cerr << "e == head \n";
        if (e == nullptr) {
            std::cerr << "e is null \n";
        } else {
            head = head->GetNext();
            if (head == nullptr) {
                tail = nullptr;
            }
        }
        return e;
    }

    bool empty() {
        if (head == nullptr) {
            return true;
        }
        return false;
    }

    int node;
    Element* head;
    Element* tail;
    std::mutex m;
};

#endif //TRY_QUEUE_H
