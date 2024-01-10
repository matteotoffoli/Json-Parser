#include "json.hpp"

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
    else{
        throw json_exception{"JSON non è una lista"};
    }
}

json::list_iterator json::end_list()
{
    if(is_list){
        return list_iterator(nullptr);
    }
    else{
        throw json_exception{"JSON non è una lista"};
    }
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
    else{
        throw json_exception{"JSON non è una lista"};
    }
}

json::const_list_iterator json::end_list() const {
    if(is_list()){
        return const_list_iterator(nullptr);
    }
    else{
        throw json_exception{"JSON non è una lista"};
    }
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
        throw json_exception{"JSON non è un dizionario"};
    }
}
json::dictionary_iterator json::end_dictionary() {
    if (is_dictionary()) {
        // Poiché pimpl->dictionary è un solo elemento, il suo iteratore di fine sarà uguale al suo iteratore di inizio
        return dictionary_iterator(nullptr);
    } else {
        throw json_exception{"JSON non è un dizionario"};
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
    } 
    else{
        throw json_exception{"JSON non è un dizionario"};
    }
}
json::const_dictionary_iterator json::end_dictionary() const {
    if (is_dictionary()) {
        // Poiché pimpl->dictionary è un solo elemento, il suo iteratore di fine sarà uguale al suo iteratore di inizio.
        return const_dictionary_iterator(nullptr);
    } 
    else{
        throw json_exception{"JSON non è un dizionario"};
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
    throw json_exception{"JSON non è un numero"};
}

double const& json::get_number() const {
    if (is_number()) {
        return pimpl->number;
    }
    throw json_exception{"JSON non è un numero"};
}

bool& json::get_bool() {
    if (is_bool()) {
        return pimpl->boolean;
    }
    throw json_exception{"JSON non è un booleano"};
}

bool const& json::get_bool() const {
    if (is_bool()) {
        return pimpl->boolean;
    }
    throw json_exception{"JSON non è un booleano"};
}

std::string& json::get_string() {
    if (is_string()) {
        return pimpl->string;
    }
    throw json_exception{"JSON non è una stringa"};
}

std::string const& json::get_string() const {
    if (is_string()) {
        return pimpl->string;
    }
    throw json_exception{"JSON non è una stringa"};
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
        throw json_exception{"JSON non è una lista"};
    }

    impl::Node* newNode = new impl::Node{value, nullptr};
    if (pimpl->list) {
        newNode->next = pimpl->list;
    }
    pimpl->list = newNode;
}

void json::push_back(json const& value) {
    if (!is_list()) {
        throw json_exception{"JSON non è una lista"};
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

json parseList(std::istream &is);
json parseDictionary(std::istream &is);

std::string parse_string(std::istream &is)
{
    std::string str;
    char ch = 0;
    is >> ch;
    is.putback(ch);
    if (ch != '\"') {

    }
    while (is.get(ch) && ch != '\"') {
        if (ch == '\\') {

            if (is.peek() == '\"') {

                is.get(ch);
                str.push_back('\\');
                str.push_back(ch);
            } else {

                str.push_back(ch);
            }
        } else {
            str.push_back(ch);
        }
    }

    return str;
}

json parseString(std::istream& is) {
    std::string str;
    char ch = 0;
    is >> ch;
    is.putback(ch);
    if (ch != '\"') {
       // std::cout << ch;
    }
    while (is.get(ch) && ch != '\"') {
        //std::cout << ch;
        if (ch == '\\') {

            if (is.peek() == '\"') {

                is.get(ch);
                str.push_back('\\');
                str.push_back(ch);
            } else {

                str.push_back(ch);
            }
        } else {
            str.push_back(ch);
        }
    }
    json inseritore;
    inseritore.set_string(str);
    return inseritore;
}


json parseBool(std::istream &is)
{
    //std::cout<<"sono entrato fra"<<std::endl;
    char c = 0;
    is >> c;
    is.putback(c);
    std::string str;
    json parsing;
    // std::cout<<"sono entrato fra"<<std::endl;
    while (is.get(c) && c != ']' && c != '}' && c != ',' && c != ' ' && c != '\r' && c != '\n')
    {
        //std::cout<<"sono entrato fra"<<std::endl;
        str.push_back(c);

    }
    is.putback(c);
    if (str == "true")
    {
        parsing.set_bool(true);
    }
    else if(str == "false")
    {
        parsing.set_bool(false);
    }

    return parsing;
}
json parseNull(std::istream &is)
{
    std::string str;
    char c;
    is >> c;

    while(c != ']' && c != '}' && c != ',' && c != ' ' && c != '\r' && c != '\n' && !is.eof()){
        str.push_back(c);
        is.get(c);
    }
    if(str != "null")
        throw json_exception{"Il JSON non è in un formato valido: il valore null non è corretto."};

    is.putback(c);
    json parsing;
    parsing.set_null();
    return parsing;
}

json parseNumber(std::istream &is, char& c)
{
    std::string str;
    //char c;
    while (c != ']' && c != '}' && c != ',' && c != ' ' && c != '\r' && c != '\n' && !is.eof())
    {
        str.push_back(c);
        is.get(c);
        if((c < '0' || c > '9') && c != '.'&& c != ']' && c != '}' && c != ',' && c != ' ' && c != '\r' && c != '\n')
            throw json_exception{"Il JSON non è in un formato valido: il valore number non è corretto."};
        if (c == '.' && str.find('.') != str.npos)
            throw json_exception{"Il carattere '.' è già presente nel numero."};
    }

    //is.putback(c);
    double num = std::stod(str);
    json parsing;

    parsing.set_number(num);
    return parsing;

}

json parseList(std::istream &is)
{
    char c = 0;
    is >> c;
    json parsing;
    json inseritore;
    //is.putback(c);
    parsing.set_list();
    while (c != ']' && is.peek() != 44 && is.get(c) )
    {
        if (c == '{')
        {
            //is.putback(c);
            inseritore = parseDictionary(is);
            parsing.push_back(inseritore);
            is >> c;
            // std::cout << c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(dictionary) o la terminazione del dizionario."};
                }
                is.get(c);

            }
        }
        else if (c == '"')
        {
            inseritore = parseString(is);
            parsing.push_back(inseritore);
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    std::cout << c << std::endl;
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(string val)."};
                }
                is.get(c);
            }
        }
        else if (c == 'n')
        {
            is.putback(c);
            inseritore = parseNull(is);
            parsing.push_back(inseritore);
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(null val)."};
                }
                is.get(c);
            }
        }
        else if (c == 'f' || c == 't')
        {
            is.putback(c);
            inseritore = parseBool(is);
            parsing.push_back(inseritore);
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{' ) {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(bool val)."};
                }
                is.get(c);

            }
        }
        else if ((c >= '0' && c <= '9') || c == '-')
        {
            // is.putback(c);
            inseritore = parseNumber(is, c);
            parsing.push_back(inseritore);
            // std::cout<< is.peek() <<std::endl;
            if(c != ']' && c != '}') {
                while (c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{' ) {
                    if (c != ' ')
                    {
                        throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(num val)."};
                    }
                    is.get(c);

                }
            }
        }
        else if (c == '[') {
            is.putback(c);
            inseritore = parseList(is);
            parsing.push_back(inseritore);
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{' ) {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(list val)."};
                }
                is.get(c);

            }
        }
    }
    //std::cout<< c << std::endl;
    if(c != ']')
        throw json_exception{"il JSON non è in un formato valido(])"};

    return parsing;
}

std::istream &operator>>(std::istream &lhs, json &rhs)
{

    char c = 0;
    lhs >> c;
    if (c == '[' )
    {
        lhs.putback(c);
        rhs = parseList(lhs);
    }
    else if (c == '{')
    {
        //lhs.putback(c);
        rhs = parseDictionary(lhs);

    }
    else if (c == '"')
    {
       // lhs.putback(c);
        rhs = parseString(lhs);
    }
    else if (c == 'n')
    {
        lhs.putback(c);
        rhs = parseNull(lhs);
    }
    else if (c == 'f' || c == 't')
    {
        lhs.putback(c);
        rhs = parseBool(lhs);
    }
    else if ((c >= '0'&& c <= '9') || c == '-')
    {
       // lhs.putback(c);
        rhs = parseNumber(lhs, c);
    }else
    {
        throw json_exception{"Il JSON non è in un formato valido"};
    }


    return lhs;
}