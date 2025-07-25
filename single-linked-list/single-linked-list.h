#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>

template <typename Type>
class SingleLinkedList {
    
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) noexcept
        : node_(node) {}

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Type;
            using difference_type = std::ptrdiff_t;
            using pointer = ValueType*;
            using reference = ValueType&;

            BasicIterator() = default;

            BasicIterator(const BasicIterator<Type>& other) noexcept 
            : node_(other.node_) {}

            BasicIterator& operator=(const BasicIterator& rhs) = default;

            [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
                return node_ == rhs.node_;
            }

            [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
                return node_ != rhs.node_;
            }

            [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
                return node_ == rhs.node_;
            }

            [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
                return node_ != rhs.node_;
            }

            BasicIterator& operator++() noexcept {
                if (node_) {
                    node_ = node_->next_node;
                }
                return *this;
            }

            BasicIterator operator++(int) noexcept {
                BasicIterator prev = *this;
                node_ = node_->next_node;
                return prev;
            }

            [[nodiscard]] reference operator*() const noexcept {
                assert(node_ != nullptr);
                return node_->value;
            }

            [[nodiscard]] pointer operator->() const noexcept {
                assert(node_ != nullptr);
                return &node_->value;
            }

        private:
            Node* node_ = nullptr;
    };

    public:

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        SingleLinkedList<Type> temp;
        for (auto it = std::rbegin(values); it != std::rend(values); ++it) {
            temp.PushFront(*it);
        }
        this->swap(temp);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Node* current = &head_;
        for (Node* node = other.head_.next_node; node != nullptr; node = node->next_node) {
            current->next_node = new Node(node->value, nullptr);
            current = current->next_node;
            ++size_;
        }
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* node = pos.node_;
        Node* new_node = new Node(value, node->next_node);
        node->next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        Node* current = head_.next_node;
        while (current != nullptr) {
            Node* next = current->next_node;
            delete current;
            current = next;
        }
        head_.next_node = nullptr;
        size_ = 0;
    }

    void PopFront() noexcept {
        if (head_.next_node) {
            Node* temp = head_.next_node;
            head_.next_node = temp->next_node;
            delete temp;
            --size_;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);
        Node* node = pos.node_;
        Node* target = node->next_node;

        if (target) {
            node->next_node = target->next_node;
            delete target;
            --size_;
        }
        return Iterator(node->next_node);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }

   
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    ~SingleLinkedList() {
        Clear();
    }

private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
} 