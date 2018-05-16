#include <iostream>
#include <fstream>
using namespace std;

template<class T>
class Node {
public:
	T data;
	Node* next;
	Node() : data{ 0 }, next{ nullptr } {}
	Node(T data_) : data{ data_ }, next{ nullptr } {}
	Node(T data_, Node* next_) : data{ data_ }, next{ next_ } {}
	Node(const Node & node) : data{ node.data }, next{ node.next } {}
};

template <class T>
class Queue {
public:
	Queue() : head{}, size_{ 0 } {}
	Queue(initializer_list<T> list) {
		for (auto cur : list)
			this->push(cur);
	}
	Queue(const Queue & q) {
		this->~Queue();
		for (Node<T>* cur = q.head; cur != nullptr; cur = cur->next)
			this->push(cur->data);
	}
	auto swap(Queue & q) -> void {
		Node<T>* tmp_head = q.head;
		size_t tmp_size = q.size_;
		q.head = this->head;
		q.size_ = this->size_;
		this->head = tmp_head;
		this->size_ = tmp_size;
	}
	auto operator=(const Queue & q) -> Queue & {
		this->swap(Queue{ q });
		return *this;
	}
	auto empty() -> bool {
		return (head == nullptr);
	}
	auto size() -> std::size_t {
		return size_;
	}
	auto push(T val) -> void {
		size_++;
		if (head == nullptr) {
			head = new Node<T>{ val };
		}
		else {
			Node<T>* cur = head;
			while (cur->next != nullptr) cur = cur->next;
			cur->next = new Node<T>{ val };
		}
	}
	auto pop() -> void {
		if (head == nullptr) return;
		size_--;
		Node<T>*cur = head;
		head = head->next;
		cur = nullptr;
	}
	auto front() const -> T & {
		if (head == nullptr)
			throw logic_error("exception");
		else
			return head->data;
	}
	auto back() const -> T & {
		Node<T>*cur = head;
		if(cur==nullptr)
			throw logic_error("exception");
		else {
			while (cur->next != nullptr) cur = cur->next;
			return cur->data;
		}
	}
	bool operator==(const Queue & q) {
		Node<T>* cur_this{ head };
		Node<T>* cur_q{ q.head };
		while (cur_this != nullptr && cur_q != nullptr && cur_this->data == cur_q->data) {
			cur_this = cur_this->next;
			cur_q = cur_q->next;
		}
		return (cur_this == nullptr && cur_q == nullptr);
	}
	friend auto operator<<(ostream &,  const Queue &)->ostream &;
	friend auto operator>>(istream &, Queue &)->istream &;
	~Queue() {
		Node<T> * cur = head;
		while (cur != nullptr)
		{
			Node<T>* tmp = cur->next;
			delete cur;
			cur = tmp;
		}
		head = nullptr;
		size_ = 0;
	}
private:
	Node<T>* head;
	size_t size_;
};

template<class T>
auto operator<<(ostream & out,  const Queue<T> & q)->ostream & {
	Node<T>* cur = q.head;
	while (cur != nullptr) {
		out << cur.data << ' ';
		cur = cur->next;
	}
	return out;
}

template<class T>
auto operator>>(istream & in, Queue<T> & q)->istream & {
	T val;
	while (in >> val) q.push(val);
	return in;
}

int main()
{
	Queue<int> q1;
	Queue<int> q2 = { 1,2,3,4 };
	Queue<int> q3{ q2 };

	cout << q2 << endl;
	cout << q3 << endl;

	q1.swap(q3);
	cout << q1 << endl;

	cout << boolalpha << q3.empty() << endl;

	cout << q1.size() << endl;

	q3.push(5);
	cout << q3 << endl;

	ofstream output{ "queue.bin" };
	output << q3;

	Queue<int> q4;
	ifstream input{ "queue.bin" };
	input >> q4;

	cout << (q4 == q3) << endl;
	try {
		cout << q4.front() << endl;
		cout << q4.back() << endl;
	}
	catch (logic_error er) { cout << er.what() << endl; }

	q4.pop();
	cout << q4 << endl;

	try {
		q4.front() = 6;
	}
	catch (logic_error er) {cout << er.what() << endl; }

	return 0;
}
