#ifndef _X_TRIE_H_
#define _X_TRIE_H_

#include <array>
#include <memory>
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>

namespace trie{
    
class unknow_word_error:public std::exception{
    public:
        unknow_word_error(){}
        ~unknow_word_error(){}
        
        virtual const char *what()const noexcept{
            return "Trie Error:Unknow word";
        }
};
    
template<class Type>
struct TrieNode{
    TrieNode();
    TrieNode(const TrieNode &);
    TrieNode(TrieNode &&);
    ~TrieNode();
    
    TrieNode& operator=(const TrieNode<Type> &);
    TrieNode& operator=(TrieNode<Type> &&);
    
    unsigned int counter;
    Type data;
    std::array<std::unique_ptr<TrieNode<Type>>,256> nodes;
};

template<class Type>
class Trie{
    public:
        Trie();
        Trie(const Trie &);
        Trie(Trie &&);
        ~Trie();
        
        Trie &operator=(const Trie &);
        Trie &operator=(Trie &&);
        
        void add_word(std::string word,Type data);
        Type data(std::string word);
        unsigned int counter(std::string word);
        bool in(std::string word);
        void clear();
        void print()noexcept;
        
    protected:
    private:
        TrieNode<Type> m_root;
        
        void m_print(std::unique_ptr<TrieNode<Type>> &ptr,std::string word);
};

//TrieNode
template<class Type>
TrieNode<Type>::TrieNode() = default;

template<class Type>
TrieNode<Type>::TrieNode(const TrieNode<Type> &node){
    for(int i = 0;i < 256;++i){
        if(node.nodes[i]){
            this->nodes[i] = std::make_unique<TrieNode<Type>>(*(node.nodes[i]));
        }
    }
    counter = node.counter;
    data = node.data;
}

template<class Type>
TrieNode<Type>::TrieNode(TrieNode<Type> &&node){
    for(int i = 0;i < 256;++i){
        this->nodes[i] = std::move(node.nodes[i]);
    }
    counter = node.counter;
    data = std::move(node.data);
}

template<class Type>
TrieNode<Type>::~TrieNode() = default;

template<class Type>
TrieNode<Type>& TrieNode<Type>::operator=(const TrieNode<Type> &node){
    for(int i = 0;i < 256;++i){
        if(node.nodes[i]){
            this->nodes[i] = std::make_unique<TrieNode<Type>>(*(node.nodes[i]));
        }
    }
    counter = node.counter;
    data = node.data;
    return *this;
}

template<class Type>
TrieNode<Type>& TrieNode<Type>::operator=(TrieNode<Type> &&node){
    for(int i = 0;i < 256;++i){
        this->nodes[i] = std::move(node.nodes[i]);
    }
    counter = node.counter;
    data = std::move(node.data);
    return *this;
}

//Trie(proxy class)
template<class Type>
Trie<Type>::Trie() = default;

template<class Type>
Trie<Type>::Trie(const Trie<Type> &trie)
    :m_root(trie.m_root){}

template<class Type>
Trie<Type>::Trie(Trie<Type> &&trie)
    :m_root(std::move(trie.m_root)){}

template<class Type>
Trie<Type>::~Trie() = default;

template<class Type>
Trie<Type> &Trie<Type>::operator=(const Trie &trie){
    m_root = trie.m_root;
    return *this;
}

template<class Type>
Trie<Type> &Trie<Type>::operator=(Trie &&trie){
    m_root = std::move(trie.m_root);
    return *this;
}

template<class Type>
void Trie<Type>::add_word(std::string word,Type data){
    std::unique_ptr<TrieNode<Type>> *node = &(m_root.nodes[word[0]]);
    if(*node == nullptr){
        *node = std::make_unique<TrieNode<Type>>();
    }
    for(auto itr = word.begin()+1;itr < word.end();++itr){
        node = &((*node)->nodes[*itr]);
        if(*node == nullptr){
            *node = std::make_unique<TrieNode<Type>>();
        }
    }
    ++(*node)->counter;
    (*node)->data = data;
}

template<class Type>
unsigned int Trie<Type>::counter(std::string word){
    std::unique_ptr<TrieNode<Type>> *node = &(m_root.nodes[word[0]]);
    if(*node == nullptr){
        return 0;
    }
    for(auto itr = word.begin()+1;itr < word.end();++itr){
        node = &((*node)->nodes[*itr]);
        if(*node == nullptr){
            return 0;
        }
    }
    return (*node)->counter;
}

template<class Type>
Type Trie<Type>::data(std::string word){
    std::unique_ptr<TrieNode<Type>> *node = &(m_root.nodes[word[0]]);
    if(*node == nullptr){
        throw unknow_word_error();
    }
    for(auto itr = word.begin()+1;itr < word.end();++itr){
        node = &((*node)->nodes[*itr]);
        if(*node == nullptr){
            throw unknow_word_error();
        }
    }
    return (*node)->data;
}

template<class Type>
bool Trie<Type>::in(std::string word){
    return counter(std::move(word)) != 0;
}

template<class Type>
void Trie<Type>::clear(){
    for(auto &ptr:m_root.nodes){
        ptr.reset();
    }
}

template<class Type>
void Trie<Type>::print()noexcept{
    std::string s;
    for(int i = 0;i < 256;++i){
        if(m_root.nodes[i]){
            s.clear();
            s.push_back(static_cast<char>(i));
            m_print(m_root.nodes[i],s);
        }
    }
    std::cout<<std::endl;
}

template<class Type>
void Trie<Type>::m_print(std::unique_ptr<TrieNode<Type>> &ptr,std::string word){
    if(ptr->counter != 0){
        std::cout<<word<<std::endl;
    }
    std::string s;
    for(int i = 0;i < 256;++i){
        if(ptr->nodes[i]){
            s.clear();
            s = word;
            s.push_back(static_cast<char>(i));
            m_print(ptr->nodes[i],s);
        }
    }
}

}

#endif
