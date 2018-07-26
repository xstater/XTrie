# XTrie
Trie

## Trie
    namespace trie
    Trie<Type> which Type is the additional data.

### functions
```
void add_word(std::string word,Type data);
    add a word into trie with additional data.
```
```
Type data(std::string word);
    search for a word and return its additional data.
    if failed,it will throw an unknow_word_error exception which derived from std::exception.
```
```
unsigned int counter(std::string word);
    search for a word and return its value of counter.
    if failed,it will return 0 and will not throw any thing.
```
```
bool in(std::string word);
    return true if a word in trie and return false if not.
```
```
void clear();
    clear all the words in trie.
```
```
void print();
    print all the words in trie.
```
