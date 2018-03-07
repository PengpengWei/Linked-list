#pragma once
#define DEFAULT_INITIAL_VOLUMN 200000	//默认初始化链表容量
#include<cstdlib>

typedef int listptr;	//listptr表示链表指针

//以下的class T应当具备大小关系的、可比较的
template <class T>
struct Node {
	T data;
	listptr previous;		//指向前一个结点
	listptr next;			//指向后一个结点
};

template <class T>
class Priority_Linklist {
private:
	Node<T>* m_list_entity;			//链表数组实体
	listptr empty_list_h;			//空链表头指针，表头本身也存储数据。
	listptr full_list_h;			//满链表头，表头本身也存储数据。
	int m_size;						//链表实体内非空单元的个数
	int m_volumn;					//链表容量

public:
	Priority_Linklist(int initial_volumn = DEFAULT_INITIAL_VOLUMN);
	//待定义的构造函数，需要完成以下工作：
	//1.确定一个初始容量initial_volumn
	//2.让空链表头和满链表头的p域和n域都指向自身
	//3.对空链表头的n域：为n域申请空间，然后为n域指向结点的n域申请空间，以此类推，直至个数满足要求
	//4.调整空链表，使各指针正确作用。主要是调整空链表的p域。
	//5.填写m_size=0, m_volumn=initial_volumn.

	~Priority_Linklist();		//待定义的析构函数

	T top();					//返回满链表块链的链头

	int push(T element);		//从空链表链中取出一个空链表块，把element拷贝到链表块的d域后，将它移动到满链表队列的恰当位置
								//如果空链表队列为空，则插入失败
								//如果元素插入失败，返回-1.成功插入,m_size++, 返回0.

	int pop();					//删除满链表链的链头，size--
								//如果原本满链表链就为空，则返回-1，否则返回0.

	bool empty();				//如果满链表链为空，则返回true，否则返回false.

	//void swap(Priority_Linklist<T> &other);	//交换两链表对象。如果用不到，可以不实现这一方法。（直接返回）

	int size() { return(m_size); }
	int volumn() { return(m_volumn); }
};

#include<iostream>

using std::cin;
using std::cout;
using std::cerr;
using std::ostream;
using std::istream;
using std::endl;
using std::operator <;
using std::operator >;
using std::operator ==;

template<class T>
Priority_Linklist<T>::Priority_Linklist(int initial_volumn) {
	try {
		m_list_entity = new Node<T>[initial_volumn];
	}
	catch (std::bad_alloc) {
		cerr << "Exception: Bad_alloc" << endl;
		cerr << "Allocation failed." << endl;
		m_list_entity = NULL;
		return;
	}

	//链接所有空单元
	for (int i = 0; i < initial_volumn; i++) {
		m_list_entity[i].previous = i - 1;
		m_list_entity[i].next = i + 1;
	}
	m_list_entity[0].previous = initial_volumn - 1;
	m_list_entity[initial_volumn - 1].next = 0;

	m_volumn = initial_volumn;	//链表容量按设定值走
	empty_list_h = 0;	//空链表头为0号单元
	full_list_h = -1;	//满链表头暂时不存在，记为-1
	m_size = 0;			//链表初始大小为0

	return;
}

template<class T>
Priority_Linklist<T>::~Priority_Linklist() {
	delete[] m_list_entity;
	return;
}

template<class T>
T Priority_Linklist<T>::top() {
	T temp;
	if (m_list_entity == NULL) return(temp);
	if (full_list_h == -1) return(temp);
	return(m_list_entity[full_list_h].data);
}

template<class T>
bool Priority_Linklist<T>::empty() {
	if (m_list_entity == NULL) return(true);
	if (m_size == 0) return(true);
	return(false);
}

template<class T>
int Priority_Linklist<T>::push(T element) {
	if (m_list_entity == NULL) return(-1);

	//如果插入失败
	if (m_size >= m_volumn) {
		cout << "Full!" << endl;
		return(-1);
	}

	//允许插入，更新size域
	m_size++;

	//摘取一个空单元
	listptr empty_unit;
	listptr pre, next;
	empty_unit = empty_list_h;		//空链表队首交付索引号
	pre = m_list_entity[empty_list_h].previous;		//记录待交付单元的前一位
	next = m_list_entity[empty_list_h].next;		//记录待交付单元的后一位
	m_list_entity[pre].next = next;	//短路待交付单元
	m_list_entity[next].previous = pre;
	if (m_size == m_volumn) {
		empty_list_h = -1;			//空链表单元用完
	}
	else {
		empty_list_h = next;
	}

	//写入数据
	m_list_entity[empty_unit].data = element;

	//把它挂在恰当的位置
	if (full_list_h == -1) {
		//如果链表全空
		full_list_h = empty_unit;
		m_list_entity[full_list_h].previous = full_list_h;
		m_list_entity[full_list_h].next = full_list_h;
		return(0);
	}
	//如果链表非空
	listptr cur = full_list_h;
	do {
		//如果发现待插入元素比当前元素要大
		if (m_list_entity[empty_unit].data > m_list_entity[cur].data) {
			pre = m_list_entity[cur].previous;	//插入后的前一位
			next = cur;						//插入后的后一位
			m_list_entity[next].previous = empty_unit;
			m_list_entity[pre].next = empty_unit;
			m_list_entity[empty_unit].previous = pre;
			m_list_entity[empty_unit].next = next;
			if (cur == full_list_h) {
				//如果插入的元素应该位于链表头
				full_list_h = empty_unit;
			}
			return(0);
		}
		else {
			cur = m_list_entity[cur].next;
		}
	} while (cur != full_list_h);
	//如果待插入元素是最小的，那么插到最末位
	pre = m_list_entity[cur].previous;	//插入后的前一位
	next = cur;						//插入后的后一位
	m_list_entity[next].previous = empty_unit;
	m_list_entity[pre].next = empty_unit;
	m_list_entity[empty_unit].previous = pre;
	m_list_entity[empty_unit].next = next;
	return(0);

}

template<class T>
int Priority_Linklist<T>::pop() {
	if (m_list_entity == NULL) return(-1);
	if (full_list_h == -1) return(-1);
	listptr pre, cur, next;

	//先把满链表头解挂
	pre = m_list_entity[full_list_h].previous;
	next = m_list_entity[full_list_h].next;
	cur = full_list_h;
	m_list_entity[pre].next = next;
	m_list_entity[next].previous = pre;
	full_list_h = next;
	m_size--;	//长度减1

	//检查一下是不是没有满链表单元了
	if (m_size == 0) {
		full_list_h = -1;
	}

	//然后把cur挂到空链表队列里面去
	//如果空链表头为-1
	if (empty_list_h == -1) {
		empty_list_h = cur;
		m_list_entity[cur].previous = cur;
		m_list_entity[cur].next = cur;
		return(0);
	}
	//如果空链表队列存在
	pre = m_list_entity[empty_list_h].previous;
	next = empty_list_h;
	m_list_entity[cur].previous = pre;
	m_list_entity[cur].next = next;
	m_list_entity[pre].next = cur;
	m_list_entity[next].previous = cur;
	return(0);
}