#include "json.hpp"

struct json::impl
{
    std::string type;
    std::string string;
    double number;
    bool boolean;
    struct NodeL
    {
        json value;
        NodeL* next;
    };
    typedef NodeL *ListNode;

    struct NodeD
    {
        std::pair<std::string,json> value;
        NodeD* next;
    };
    typedef NodeD *DictNode;

    ListNode headL;
    ListNode tailL;
    DictNode headD;
    DictNode tailD;

    void destroy(ListNode other);
    void destroy(DictNode other);

    ListNode copy(ListNode other);
    DictNode copy(DictNode other);
};
//metodi aggiuntivi utili
json::impl::ListNode json::impl::copy(json::impl::ListNode other)
{
    if(other==nullptr)
        return nullptr;
    else{
        json::impl::ListNode dest = new json::impl::NodeL;
        dest->value=other->value;
        dest->next=json::impl::copy(other->next);
        return dest;
    }
}

json::impl::DictNode json::impl::copy(json::impl::DictNode other)
{
    if(other==nullptr)
        return nullptr;
    else{
        json::impl::DictNode dest = new json::impl::NodeD;
        dest->value=other->value;
        dest->next=json::impl::copy(other->next);
        return dest;
    }
}

void json::impl::destroy(json::impl::ListNode x)
{
    if (x){
        destroy(x->next);
        delete x;
    }
}

void json::impl::destroy(json::impl::DictNode x)
{
    if (x){
        destroy(x->next);
        delete x;
    }
}

//list_iterator
struct json::list_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = json;
	using pointer = json*;
	using reference = json&;

    list_iterator(json::impl::NodeL *p) : ptr(p) {};

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
        json::impl::NodeL* ptr;
};

json::list_iterator json::begin_list() {
    if (is_list()) {
        return list_iterator(pimpl->headL);
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

	const_list_iterator(const json::impl::NodeL *p) : ptr(p) {}
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

		const json::impl::NodeL *ptr;
};

json::const_list_iterator json::begin_list() const {
    if (is_list()) {
        return const_list_iterator(pimpl->headL);
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

    dictionary_iterator(json::impl::NodeD *d) : dict(d) {};

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
        json::impl::NodeD *dict;
};

json::dictionary_iterator json::begin_dictionary() {
    if (is_dictionary()) {
        return dictionary_iterator(pimpl->headD);
    } else {
        throw json_exception{"JSON non è un dizionario"};
    }
}
json::dictionary_iterator json::end_dictionary() {
    if (is_dictionary()) {
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

    const_dictionary_iterator(const json::impl::NodeD *d) : dict(d) {};

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
        const json::impl::NodeD *dict;
};

json::const_dictionary_iterator json::begin_dictionary() const {
    if (is_dictionary()) {
        return const_dictionary_iterator(pimpl->headD);
    } 
    else{
        throw json_exception{"JSON non è un dizionario"};
    }
}
json::const_dictionary_iterator json::end_dictionary() const {
    if (is_dictionary()) {
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
json::json(const json& other) {
    pimpl = new impl;
    pimpl->string=other.pimpl->string;
    pimpl->number=other.pimpl->number;
    pimpl->boolean=other.pimpl->boolean;
    pimpl->type=other.pimpl->type;

    pimpl->headL = pimpl->copy(other.pimpl->headL);
    pimpl->headD = pimpl->copy(other.pimpl->headD);
    if(pimpl->headL != nullptr){
        json::impl::ListNode p1=pimpl->headL;
        while(p1->next!=nullptr)
            p1=p1->next;
        pimpl->tailL=p1;
    }
    if(pimpl->headD != nullptr){
        json::impl::DictNode p2=pimpl->headD;
        while(p2->next!=nullptr)
            p2=p2->next;
        pimpl->tailD=p2;
    }
}

// Move constructor
json::json(json&& other) : pimpl(other.pimpl) {
    other.pimpl = nullptr;
}

// Destructor
json::~json() {
    if(pimpl->headD){
        pimpl->destroy(pimpl->headD);
        pimpl->headD = nullptr;
        pimpl->tailD = nullptr;
    }
    if(pimpl->headL){
        pimpl->destroy(pimpl->headL);
        pimpl->headL = nullptr;
        pimpl->tailL = nullptr;
    }
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
    if(pimpl->type=="lista")
        return true;
    return false;
}

bool json::is_dictionary() const {
    if(pimpl->type=="dizionario")
        return true;
    return false;
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
        throw json_exception{"Non è possibile fare un inserimento nel dizionario tramite l'operatore [] const"};
    }

    auto it = begin_dictionary();
    while (it != end_dictionary())
    {
        if (it->first == key)
        {
            return it->second;
        }
        ++it;
    }
    throw json_exception{"sono l'operatore[] const, non posso effettuare alcun inserimento"};
}

json& json::operator[](std::string const& key) {
    if (!is_dictionary()) {
        throw json_exception{"JSON non è un dizionario"};
    }

    auto it = begin_dictionary();
    while (it != end_dictionary())
     {
        if (it->first == key)
        {
            return it->second;
        }
        ++it;
    }
    std::pair<std::string, json> nuova;
    nuova.first = key;
    nuova.second = json();
    this->insert(nuova);
    return pimpl->tailD->value.second;
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

void json::set_string(std::string const& value) {
    if (is_list())
    {
        if(pimpl->headL){
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary()){
        if(pimpl->headD){
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->string = value;
    pimpl->type = "stringa";
}

void json::set_bool(bool value) {
    if (is_list())
    {
        if(pimpl->headL){
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary()){
        if(pimpl->headD){
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->boolean = value;
    if (value == true)
    {
        pimpl->type = "true";
    }
    else
    {
        pimpl->type = "false";
    }
}

void json::set_number(double value) {
    if (is_list())
    {
        if(pimpl->headL){
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary()){
        if(pimpl->headD){
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->number = value;
    pimpl->type = std::to_string(value);
}

void json::set_null() {
    pimpl->string.clear();
    pimpl->number = 0.0;
    pimpl->boolean = false;
    if (is_list())
    {
        if(pimpl->headL){
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary()){
        if(pimpl->headD){
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->type="null";
}

void json::set_list() {
    if (is_list())
    {
        if(pimpl->headL){
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary()){
        if(pimpl->headD){
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }

    pimpl->type = "lista";
}

void json::set_dictionary() {
    if (is_list())
    {
        if(pimpl->headL){
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary()){
        if(pimpl->headD){
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }

    pimpl->type = "dizionario";
}


void json::push_front(json const& value) {
    if (!is_list()) {
        throw json_exception{"JSON non è una lista"};
    }

    json::impl::ListNode nuova = new json::impl::NodeL;
        nuova->value = value;
        nuova->next = nullptr;
        if (pimpl->headL == pimpl->tailL && pimpl->headL == nullptr)
        {
            pimpl->headL = pimpl->tailL = nuova;
        }
        else
        {
            nuova->next = pimpl->headL;
            pimpl->headL = nuova;
        }
}

void json::push_back(json const& value) {
    if (!is_list()) {
        throw json_exception{"JSON non è una lista"};
    }

    json::impl::ListNode nuova = new json::impl::NodeL;
        nuova->value = value;
        nuova->next = nullptr;
        if (pimpl->headL == nullptr)
        {
            pimpl->headL = pimpl->tailL = nuova;
        }
        else
        {
            pimpl->tailL->next = nuova;
            pimpl->tailL = nuova;
        }
}

void json::insert(std::pair<std::string, json> const& value) {
    if (!is_dictionary()) {
        throw json_exception{"JSON non è un dizionario"};
    }

    json::impl::DictNode nuova = new json::impl::NodeD;
    nuova->value = value;
    nuova->next = nullptr;
    if (pimpl->headD == pimpl->tailD && pimpl->headD == nullptr)
    {
        pimpl->headD = pimpl->tailD = nuova;
    }
    else
    {
        pimpl->tailD->next = nuova;
        pimpl->tailD = nuova;
    }
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
        auto it = rhs.begin_list();
        while(it != rhs.end_list()) {
            lhs << *it++;
            if (it != rhs.end_list())
                lhs << ",";
        }
        lhs << "]";
    } else if (rhs.is_dictionary()) {
        lhs << "{";
        auto it = rhs.begin_dictionary();
        while(it != rhs.end_dictionary())
        {
            lhs << it->first << ":" << it->second;
            if(++it != rhs.end_dictionary())
            {
                lhs << ",";
            }
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

json parseDictionary(std::istream &is)
{

    char c = 0;
    //is >> c;
    json parsing;
    std::pair<std::string, json> inseritore;
    parsing.set_dictionary();
    while (c != '}' && is.peek() != 44 && is.get(c))
    {
        while(c == ' ' || c == '\n' || c == '\r') {
            is >> c;
        }
        if(inseritore.first == "" && c == '"') {

            is >> c;
            is.putback(c);
            inseritore.first = parse_string(is);
            is >> c;
            if (c != ':') {
                throw json_exception{"Il JSON non è in un formato valido: ':' mancante dopo la chiave."};
            }

        }

        else if(inseritore.first == "" && c != '"' && c != '{' && c != '\r' && c != '}' && c != ',' && c != ']' && c != '[')
        {
            throw json_exception{"Il JSON non è in un formato valido: chiave non stringa."};
        }
        else if (c == '{')
        {
            //is.putback(c);
            inseritore.second = parseDictionary(is);
            parsing.insert(inseritore);
            is >> c;
            is.putback(c);
            is >> c;
            if(c == '\n')
                throw json_exception{"Il JSON non è in un formato valido: la parentesi } non è presente."};
            if(c == '}' && is.peek() == 44)
                break;
            inseritore.first = "";
        }
        else if (c == '"')
        {
            inseritore.second = parseString(is);
            parsing.insert(inseritore);
            inseritore.first = "";
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(string val in dictionary)."};
                }
                is.get(c);

            }
        }
        else if (c == 'n')
        {
            is.putback(c);
            inseritore.second = parseNull(is);
            parsing.insert(inseritore);
            inseritore.first = "";
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    // std::cout<< c<<std::endl;
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(null val in dictionary)."};
                }
                is.get(c);
            }
        }
        else if (c == 'f' || c == 't')
        {
            is.putback(c);
            inseritore.second = parseBool(is);
            parsing.insert(inseritore);
            inseritore.first = "";
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(bool val in dictionary)."};
                }
                is.get(c);

            }
        }
        else if ((c >= '0' && c <= '9' )|| c == '-')
        {
            //is.putback(c);
            inseritore.second = parseNumber(is, c);
            parsing.insert(inseritore);
            inseritore.first = "";
            //is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(num val in dictionary)."};
                }
                is.get(c);

            }
        }
        else if (c == '[')
        {
            is.putback(c);
            inseritore.second = parseList(is);
            parsing.insert(inseritore);
            inseritore.first = "";
            is >> c;
            while(c != ',' && is.peek() != 10 && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') {
                if (c != ' ') {
                    throw json_exception{"Il JSON non è in un formato valido: non è presente il separatore(list val)."};
                }
                is.get(c);

            }

        }

    }
    if(c != '}')
        throw json_exception{"il Json non è in un formato valido(})"};
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