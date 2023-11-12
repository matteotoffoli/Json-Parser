#include "json.hpp"
#include <iostream>

struct json::impl
{
    std::string string;
    double number;
    bool boolean;
    struct Node
    {
        json value;
        Node* next;
    };
    Node* list;

    struct DictNode
    {
        std::pair<std::string,json> value;
        DictNode* next;
    };
    DictNode *dictionary;
    Node* m_front=nullptr;
	Node* m_back=nullptr;
    //std::pair<std::string,json> dictionary;
};

//list_iterator
struct json::list_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = json;
	using pointer = json*;
	using reference = json&;

    list_iterator(json::impl::Node* p) : ptr(p) {};

	reference operator*() const{
        return ptr->value;
    };

	pointer operator->() const{
        return &(ptr->value);
    };

	list_iterator& operator++(){
        ptr = ptr->next;
        return *this;
    };

	list_iterator operator++(int){
        auto old_it {ptr};
        ++(*this);
        return old_it;
    };

	operator bool() const{
        return ptr != nullptr;
    };

	bool operator==(list_iterator const& x) const{
        return ptr == x.ptr;
    };

	bool operator!=(list_iterator const& x) const{
        return not(*this == x);
    };

	private:
        json::impl::Node* ptr;
};
json::list_iterator json::begin_list() {
    if (is_list()) {
        return list_iterator(pimpl->list);
    }
    throw json_exception{"JSON is not a list."};
}
json::list_iterator json::end_list(){
    return list_iterator(nullptr);
}

//const_list_iterator
struct json::const_list_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = json const;
	using pointer = json const*;
	using reference = json const&;

	const_list_iterator(json::impl::Node* p) : ptr(p) {}
	reference operator*() const{
        return ptr->value; 
    };
	pointer operator->() const{
        return &(ptr->value); 
    };
	const_list_iterator& operator++(){
        ptr = ptr->next;
	    return *this;
    };
	const_list_iterator operator++(int){
        auto old_it {ptr};
	    ++(*this);
	    return old_it;
    };
	operator bool() const{
        return ptr != nullptr;
    };
	bool operator==(const_list_iterator const& x) const{
        return ptr == x.ptr;
    };
	bool operator!=(const_list_iterator const& x) const{
        return not(*this == x);
    };

	private:

		json::impl::Node* ptr;
};
json::const_list_iterator json::begin_list() const {
    if (is_list()) {
        return const_list_iterator(pimpl->list);
    }
    throw json_exception{"JSON is not a list."};
}
json::const_list_iterator json::end_list() const {
    return const_list_iterator(nullptr);
}

//dictionary_iterator
struct json::dictionary_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::pair<std::string,json>;
	using pointer = std::pair<std::string,json>*;
	using reference = std::pair<std::string,json>&;

    dictionary_iterator(json::impl::DictNode* d) : dict(d) {};

	reference operator*() const{
        return dict->value;
    };

	pointer operator->() const{
        return &(dict->value);
    };

	dictionary_iterator& operator++(){
        dict=dict->next;
        return *this;
    };

	dictionary_iterator operator++(int){
        auto old_it {dict};
        ++(*this);
        return old_it;
    };

	bool operator==(dictionary_iterator const& x) const{
        return dict == x.dict;
    };

	bool operator!=(dictionary_iterator const& x) const{
        return !(*this == x);
    };

	private:
        json::impl::DictNode* dict;
};
json::dictionary_iterator json::begin_dictionary() {
    if (is_dictionary()) {
        return dictionary_iterator(pimpl->dictionary);
    } else {
        throw json_exception{"JSON is not a dictionary."};
    }
}
json::dictionary_iterator json::end_dictionary() {
    if (is_dictionary()) {
        // Poiché pimpl->dictionary è un solo elemento, il suo iteratore di fine sarà uguale al suo iteratore di inizio.
        return dictionary_iterator(nullptr);
    } else {
        throw json_exception{"JSON is not a dictionary."};
    }
}

//const_dictionary_iterator
struct json::const_dictionary_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::pair<std::string,json> const;
	using pointer = std::pair<std::string,json> const*;
	using reference = std::pair<std::string,json> const&;

    const_dictionary_iterator(json::impl::DictNode* d) : dict(d) {};

	reference operator*() const{
        return dict->value;
    };

	pointer operator->() const{
        return &(dict->value);
    };

	const_dictionary_iterator& operator++(){
        dict=dict->next;
        return *this;
    };

	const_dictionary_iterator operator++(int){
        auto old_it {dict};
        ++(*this);
        return old_it;
    };

	bool operator==(const_dictionary_iterator const& x) const{
        return dict == x.dict;
    };

	bool operator!=(const_dictionary_iterator const& x) const{
        return !(*this == x);
    };

	private:
        json::impl::DictNode* dict;
};
json::const_dictionary_iterator json::begin_dictionary() const {
    if (is_dictionary()) {
        return const_dictionary_iterator(pimpl->dictionary);
    } else {
        throw json_exception{"JSON is not a dictionary."};
    }
}
json::const_dictionary_iterator json::end_dictionary() const {
    if (is_dictionary()) {
        // Poiché pimpl->dictionary è un solo elemento, il suo iteratore di fine sarà uguale al suo iteratore di inizio.
        return const_dictionary_iterator(nullptr);
    } else {
        throw json_exception{"JSON is not a dictionary."};
    }
}



//costruttore di default
json::json() : pimpl(new impl()) {
    set_null();
}

//copy constructor
json::json(const json& other) : pimpl(new impl(*(other.pimpl))) {}

// Move constructor
json::json(json&& other) : pimpl(other.pimpl) {
    other.pimpl = nullptr;
}

// Destructor
json::~json() {
    delete pimpl;
}
// Copy assignment operator
json& json::operator=(json const& other) {
    if (this != &other) {
        delete pimpl;
        pimpl = new impl(*(other.pimpl));
    }
    return *this;
}

// Move assignment operator
json& json::operator=(json&& other) {
    if (this != &other) {
        delete pimpl;
        pimpl = other.pimpl;
        other.pimpl = nullptr;
    }
    return *this;
}

// Check the type of the json object
bool json::is_list() const {
    return pimpl->list != nullptr;
}

bool json::is_dictionary() const {
    return pimpl->dictionary!=nullptr;
}

bool json::is_string() const {
    return !pimpl->string.empty();
}

bool json::is_number() const {
    //return !is_list() && !is_dictionary() && !is_string() && !is_bool() && !is_null();
    return pimpl->number!=0.0;
}

bool json::is_bool() const {
    return pimpl->boolean;
}

bool json::is_null() const {
    return !is_list() && !is_dictionary() && !is_string() && !is_number() && !is_bool();
}

json const& json::operator[](std::string const& key) const {
    if (!is_dictionary()) {
        throw json_exception{"Operator[] can only be used on a dictionary JSON."};
    }

    impl::DictNode* current = pimpl->dictionary;
    while (current) {
        if (current->value.first == key) {
            return current->value.second;
        }
        current = current->next;
    }

    throw json_exception{"Key not found in dictionary."};
}

json& json::operator[](std::string const& key) {
    if (!is_dictionary()) {
        throw json_exception{"Operator[] can only be used on a dictionary JSON."};
    }

    if (!pimpl->dictionary) {
        pimpl->dictionary = new impl::DictNode{{key, json{}}, nullptr};
        return pimpl->dictionary->value.second;
    }

    impl::DictNode* current = pimpl->dictionary;
    while (current) {
        if (current->value.first == key) {
            return current->value.second;
        }
        if (!current->next) {
            current->next = new impl::DictNode{{key, json{}}, nullptr};
            return current->next->value.second;
        }
        current = current->next;
    }

    throw json_exception{"Dictionary access error."};
}

double& json::get_number() {
    if (is_number()) {
        return pimpl->number;
    }
    throw json_exception{"JSON is not a number."};
}

double const& json::get_number() const {
    if (is_number()) {
        return pimpl->number;
    }
    throw json_exception{"JSON is not a number."};
}

bool& json::get_bool() {
    if (is_bool()) {
        return pimpl->boolean;
    }
    throw json_exception{"JSON is not a boolean."};
}

bool const& json::get_bool() const {
    if (is_bool()) {
        return pimpl->boolean;
    }
    throw json_exception{"JSON is not a boolean."};
}

std::string& json::get_string() {
    if (is_string()) {
        return pimpl->string;
    }
    throw json_exception{"JSON is not a string."};
}

std::string const& json::get_string() const {
    if (is_string()) {
        return pimpl->string;
    }
    throw json_exception{"JSON is not a string."};
}

void json::set_string(std::string const& x) {
    pimpl->string = x;
    // Reset other types
    pimpl->number = 0.0;
    pimpl->boolean = false;
    pimpl->list = nullptr;
    pimpl->dictionary = nullptr;
}

void json::set_bool(bool value) {
    pimpl->boolean = value;
    // Reset other types
    pimpl->string.clear();
    pimpl->number = 0.0;
    pimpl->list = nullptr;
    pimpl->dictionary = nullptr;
}

void json::set_number(double value) {
    pimpl->number = value;
    // Reset other types
    pimpl->string.clear();
    pimpl->boolean = false;
    pimpl->list = nullptr;
    pimpl->dictionary = nullptr;
}

void json::set_null() {
    pimpl->string.clear();
    pimpl->number = 0.0;
    pimpl->boolean = false;
    pimpl->list = nullptr;
    pimpl->dictionary = nullptr;
}

void json::set_list() {
    // Release dictionary memory if present
    while (pimpl->dictionary) {
        impl::DictNode* temp = pimpl->dictionary;
        pimpl->dictionary = pimpl->dictionary->next;
        delete temp;
    }

    // Release list memory if present
    while (pimpl->list) {
        impl::Node* temp = pimpl->list;
        pimpl->list = pimpl->list->next;
        delete temp;
    }

    // Set as list
    pimpl->list = new impl::Node{json{}, nullptr};
    pimpl->dictionary = nullptr;

    // Reset other types
    pimpl->string.clear();
    pimpl->number = 0.0;
    pimpl->boolean = false;
}

void json::set_dictionary() {
    // Release dictionary memory if present
    while (pimpl->dictionary) {
        impl::DictNode* temp = pimpl->dictionary;
        pimpl->dictionary = pimpl->dictionary->next;
        delete temp;
    }

    // Release list memory if present
    while (pimpl->list) {
        impl::Node* temp = pimpl->list;
        pimpl->list = pimpl->list->next;
        delete temp;
    }

    // Set as dictionary
    pimpl->dictionary = new impl::DictNode{{"", json{}}, nullptr};
    pimpl->list = nullptr;

    // Reset other types
    pimpl->string.clear();
    pimpl->number = 0.0;
    pimpl->boolean = false;
}


void json::push_front(json const& value) {
    if (!is_list()) {
        throw json_exception{"push_front can only be used on a list JSON."};
    }

    impl::Node* newNode = new impl::Node{value, nullptr};
    if (pimpl->list) {
        newNode->next = pimpl->list;
    }
    pimpl->list = newNode;
}

void json::push_back(json const& value) {
    if (!is_list()) {
        throw json_exception{"push_back can only be used on a list JSON."};
    }

    if (!pimpl->list) {
        push_front(value); // Usa push_front se la lista è vuota
        return;
    }

    impl::Node* newNode = new impl::Node{value, nullptr};

    impl::Node* current = pimpl->list;
    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
}

void json::insert(std::pair<std::string, json> const& value) {
    if (!is_dictionary()) {
        throw json_exception{"insert can only be used on a dictionary JSON."};
    }

    impl::DictNode* newNode = new impl::DictNode{value, nullptr};

    if (!pimpl->dictionary) {
        pimpl->dictionary = newNode;
        return;
    }

    impl::DictNode* current = pimpl->dictionary;
    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
}

std::ostream& operator<<(std::ostream& lhs, const json& rhs) {
    if (rhs.is_null()) {
        lhs << "null";
    } else if (rhs.is_bool()) {
        lhs << (rhs.get_bool() ? "true" : "false");
    } else if (rhs.is_number()) {
        lhs << rhs.get_number();
    } else if (rhs.is_string()) {
        lhs << "\"" << rhs.get_string() << "\"";
    } else if (rhs.is_list()) {
        lhs << "[";
        bool first = true;
        for (json::const_list_iterator it = rhs.begin_list(); it != rhs.end_list(); ++it) {
            if (!first) {
                lhs << ",";
            }
            lhs << *it;
            first = false;
        }
        lhs << "]";
    } else if (rhs.is_dictionary()) {
        lhs << "{";
        bool first = true;
        for (json::const_dictionary_iterator it = rhs.begin_dictionary(); it != rhs.end_dictionary(); ++it) {
            if (!first) {
                lhs << ",";
            }
            lhs << "\"" << it->first << "\":" << it->second;
            first = false;
        }
        lhs << "}";
    }

    return lhs;
}

std::istream& operator>>(std::istream& lhs, json& rhs) {
    char c;
    lhs >> c;

    if (c == 'n') {
        // Lettura di "null"
        char buffer[4];
        lhs.read(buffer, 4);
        buffer[3] = '\0';
        if (std::string(buffer) == "null") {
            rhs.set_null();
        } else {
            throw json_exception{"Invalid JSON format: Expected 'null'."};
        }
    } else if (c == 't') {
        // Lettura di "true"
        char buffer[4];
        lhs.read(buffer, 4);
        buffer[3] = '\0';
        if (std::string(buffer) == "true") {
            rhs.set_bool(true);
        } else {
            throw json_exception{"Invalid JSON format: Expected 'true'."};
        }
    } else if (c == 'f') {
        // Lettura di "false"
        char buffer[5];
        lhs.read(buffer, 5);
        buffer[4] = '\0';
        if (std::string(buffer) == "false") {
            rhs.set_bool(false);
        } else {
            throw json_exception{"Invalid JSON format: Expected 'false'."};
        }
    } else if (c == '\"') {
        // Lettura di una stringa
        std::string jsonString;
        char prevChar = '\0';
        while (lhs.get(c)) {
            if (c == '\"' && prevChar != '\\') {
                break;
            }
            jsonString += c;
            prevChar = c;
        }
        rhs.set_string(jsonString);
    } else if (c == '[') {
        // Lettura di una lista
        rhs.set_list();
        while (true) {
            json item;
            lhs >> item;
            rhs.push_back(item);
            lhs >> c; // Legge la virgola o la parentesi chiusa
            if (c == ']') {
                break;
            }
        }
    } else if (c == '{') {
        // Lettura di un dizionario
        rhs.set_dictionary();
        while (true) {
            json key;
            lhs >> key;
            lhs >> c; // Deve essere il due punti
            if (c != ':') {
                throw json_exception{"Invalid JSON format: Expected ':' in dictionary."};
            }
            json value;
            lhs >> value;
            rhs.insert({key.get_string(), value});
            lhs >> c; // Legge la virgola o la parentesi chiusa
            if (c == '}') {
                break;
            }
        }
    } else {
        // Lettura di un numero
        lhs.putback(c);
        double number;
        lhs >> number;
        rhs.set_number(number);
    }

    return lhs;
}

