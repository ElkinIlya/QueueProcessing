#include <iostream>
#include <ctime>
using namespace std;

template<typename TElement>
struct qnode {
	qnode* prev;
	TElement data;
	qnode* next;

	qnode() {}
	qnode(TElement val) : data(val), prev(nullptr), next(nullptr) {}

	TElement getData() {
		return data;
	}
};

template<typename TElement>
struct qlist {
	qnode<TElement>* front;
	int size;
	qlist() : front(nullptr), size(0) {}

	int get_size() {
		return size;
	}

	bool if_empty() {
		if (front == nullptr)
			return true;
		else
			return false;
	}

	TElement getFront() {
		if (front != nullptr)
			return front->getData();
		else {
			cerr << "Queue is empty" << endl;
		}
	}

	void pop() {
		if (front != nullptr) {
			if (front->prev != nullptr) {
				front = front->prev;
				front->next = nullptr;
				size--;
			}
			else {
				front = nullptr;
			}
		}
		else {
			cerr << "Queue is empty" << endl;
		}
	}

	void push(TElement el) {
		qnode<TElement>* new_node = new qnode<TElement>(el);
		if (front != nullptr) {
			auto tmp = front;
			while (tmp->prev != nullptr)
				tmp = tmp->prev;
			tmp->prev = new_node;
			new_node->next = tmp;
			size++;
		}
		else {
			front = new_node;
		}
	}
};

struct request {
	int id;
	int cycles;
	float t1, t2;
	request() {}
	request(int id, float t1, float t2) : id(id), cycles(1), t1(t1), t2(t2) {}
};

float get_rand(float t1, float t2) {
	return (float(rand()) / RAND_MAX) * (t2 - t1) + t1;
}

int main() {

	int cnt = 0;
	int in = 0;
	request reqs[1000];
	reqs[0] = request(0, get_rand(0.f, 10.f), get_rand(0.f, 2.f));
	for (int i = 1; i < 1000; ++i) {
		reqs[i] = request(i, get_rand(0.f, 10.f) + reqs[i - 1].t1, get_rand(0.f, 2.f));
	}
	qlist<request> q;
	float beg = clock();
	float last_action = 0;
	float idle = 0;
	float max_time = 0;
	bool fl[11];
	for (int i = 0; i < 11; ++i) fl[i] = false;
	while (cnt < 1000) {
		if (in < 1000 && (clock() - beg) >= reqs[in].t1) {
			q.push(reqs[in]);
			max_time = max(max_time, reqs[in].t2);
			idle += clock() - last_action;
			in++;
		}
		if (!q.if_empty() && ((clock() - beg) >= q.getFront().t1 + q.getFront().t2)) {
			auto tmp = q.getFront();
			q.pop();
			tmp.t1 = clock() - beg;
			tmp.cycles++;
			if (tmp.cycles <= 4) {
				q.push(tmp);
				if (q.get_size() == 0) {
					last_action = clock();
				}
			}
			else {
				cnt++;
			}
		}
		if ((cnt % 100) == 0 && !fl[cnt / 100]) {
			cout << "Processed: " << cnt << ", requests in: " << in << ", current queue size: " << q.get_size() << ", time: " << clock() - beg <<
				" idle: " << idle << ", max time for request: " << max_time << endl;
			fl[cnt / 100] = true;
		}
	}
	return 0;
}