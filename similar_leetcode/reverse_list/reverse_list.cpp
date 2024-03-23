
#include <iostream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}

};

ListNode* ReverseList(ListNode* head) {
    if (head == nullptr || head->next == nullptr){
        return head;
    }
    ListNode* ret = nullptr;
    ListNode* cur = head;
    while (cur != nullptr)
    {
        ListNode* next = cur->next;//保存一个next
        cur->next = ret;
        ret = cur;
        cur = next;//类似cur=cur->next
    }
    return ret;
}

ListNode* ReverseListV2(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    ListNode* node = ReverseListV2(head->next);
    head->next->next = head;
    head->next = nullptr;
    return node;
}

int main() {
    //创建链表
    ListNode* head = new ListNode(1);
    ListNode* tmp = head;
    tmp->next = new ListNode(2);
    tmp = tmp->next;
    tmp->next = new ListNode(3);
    tmp = tmp->next;
    tmp->next = new ListNode(4);
    tmp = tmp->next;
    tmp->next = new ListNode(5);
    tmp = tmp->next;
    tmp->next = nullptr;
    std::cout << "before reverse:" << std::endl;
    tmp = head;
    while (tmp != nullptr){
        std::cout << tmp->val << " ";
        tmp = tmp->next;
    }
    std::cout << std::endl;

    std::cout << "after reverse:" << std::endl;
    ListNode* ret = ReverseList(head);
    tmp = ret;
    while (tmp != nullptr) {
        std::cout << tmp->val << " ";
        tmp = tmp->next;
    }
    std::cout << std::endl;

    std::cout << "after reverse v2:" << std::endl;
    ListNode* ret2 = ReverseListV2(ret);
    tmp = ret2;
    while (tmp != nullptr) {
        std::cout << tmp->val << " ";
        tmp = tmp->next;
    }
    return 0;
}
