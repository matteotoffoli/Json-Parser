//Toffoli Matteo 892313
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct json::impl
{
    std::string type="null";
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
json::impl::ListNode json::impl::copy(json::impl::ListNode x)
{
    if(x==nullptr)
    {
        return nullptr;
    }
    else
    {
        json::impl::ListNode dest = new json::impl::NodeL;
        dest->value=x->value;
        dest->next=json::impl::copy(x->next);
        return dest;
    }
}

json::impl::DictNode json::impl::copy(json::impl::DictNode x)
{
    if(x==nullptr)
    {
        return nullptr;
    }
    else
    {
        json::impl::DictNode dest = new json::impl::NodeD;
        dest->value=x->value;
        dest->next=json::impl::copy(x->next);
        return dest;
    }
}

void json::impl::destroy(json::impl::ListNode x)
{
    while(x!=nullptr)
    {
        json::impl::ListNode tmp=x;
        x=x->next;
        delete tmp;
    }
}

void json::impl::destroy(json::impl::DictNode x)
{
    while(x!=nullptr)
    {
        json::impl::DictNode tmp=x;
        x=x->next;
        delete tmp;
    }
}

//list_iterator
struct json::list_iterator
{
	using iterator_category = std::forward_iterator_tag;
	using value_type = json;
	using pointer = json*;
	using reference = json&;

    list_iterator(json::impl::NodeL *p) : ptr(p) {};

	reference operator*() const
    {
        return ptr->value;
    };

	pointer operator->() const
    {
        return &(ptr->value);
    };

	list_iterator& operator++()
    {
        ptr = ptr->next;
        return *this;
    };

	list_iterator operator++(int)
    {
        auto old_it {ptr};
        ++(*this);
        return old_it;
    };

	operator bool() const
    {
        return ptr != nullptr;
    };

	bool operator==(list_iterator const& x) const
    {
        return ptr == x.ptr;
    };

	bool operator!=(list_iterator const& x) const
    {
        return !(*this == x);
    };

	private:
        json::impl::NodeL* ptr;
};

json::list_iterator json::begin_list() 
{
    if (is_list()) 
    {
        return list_iterator(pimpl->headL);
    }
    else
    {
        throw json_exception{"JSON non è una lista"};
    }
}

json::list_iterator json::end_list()
{
    if(is_list())
    {
        return list_iterator(nullptr);
    }
    else
    {
        throw json_exception{"JSON non è una lista"};
    }
}

//const_list_iterator
struct json::const_list_iterator
{
	using iterator_category = std::forward_iterator_tag;
	using value_type = json const;
	using pointer = json const*;
	using reference = json const&;

	const_list_iterator(const json::impl::NodeL *p) : ptr(p) {}
	reference operator*() const
    {
        return ptr->value; 
    };
	pointer operator->() const
    {
        return &(ptr->value); 
    };
	const_list_iterator& operator++()
    {
        ptr = ptr->next;
	    return *this;
    };
	const_list_iterator operator++(int)
    {
        auto old_it {ptr};
	    ++(*this);
	    return old_it;
    };
	operator bool() const
    {
        return ptr != nullptr;
    };
	bool operator==(const_list_iterator const& x) const
    {
        return ptr == x.ptr;
    };
	bool operator!=(const_list_iterator const& x) const
    {
        return !(*this == x);
    };

	private:

		const json::impl::NodeL *ptr;
};

json::const_list_iterator json::begin_list() const 
{
    if (is_list()) 
    {
        return const_list_iterator(pimpl->headL);
    }
    else
    {
        throw json_exception{"JSON non è una lista"};
    }
}

json::const_list_iterator json::end_list() const 
{
    if(is_list())
    {
        return const_list_iterator(nullptr);
    }
    else
    {
        throw json_exception{"JSON non è una lista"};
    }
}

//dictionary_iterator
struct json::dictionary_iterator
{
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::pair<std::string,json>;
	using pointer = std::pair<std::string,json>*;
	using reference = std::pair<std::string,json>&;

    dictionary_iterator(json::impl::NodeD *d) : dict(d) {};

	reference operator*() const
    {
        return dict->value;
    };

	pointer operator->() const
    {
        return &(dict->value);
    };

	dictionary_iterator& operator++()
    {
        dict=dict->next;
        return *this;
    };

	dictionary_iterator operator++(int)
    {
        auto old_it {dict};
        ++(*this);
        return old_it;
    };

	bool operator==(dictionary_iterator const& x) const
    {
        return dict == x.dict;
    };

	bool operator!=(dictionary_iterator const& x) const
    {
        return !(*this == x);
    };

	private:
        json::impl::NodeD *dict;
};

json::dictionary_iterator json::begin_dictionary() 
{
    if (is_dictionary()) 
    {
        return dictionary_iterator(pimpl->headD);
    } 
    else 
    {
        throw json_exception{"JSON non è un dizionario"};
    }
}
json::dictionary_iterator json::end_dictionary() 
{
    if (is_dictionary()) 
    {
        return dictionary_iterator(nullptr);
    } 
    else 
    {
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

	reference operator*() const
    {
        return dict->value;
    };

	pointer operator->() const
    {
        return &(dict->value);
    };

	const_dictionary_iterator& operator++()
    {
        dict=dict->next;
        return *this;
    };

	const_dictionary_iterator operator++(int)
    {
        auto old_it {dict};
        ++(*this);
        return old_it;
    };

	bool operator==(const_dictionary_iterator const& x) const
    {
        return dict == x.dict;
    };

	bool operator!=(const_dictionary_iterator const& x) const
    {
        return !(*this == x);
    };

	private:
        const json::impl::NodeD *dict;
};

json::const_dictionary_iterator json::begin_dictionary() const 
{
    if (is_dictionary()) 
    {
        return const_dictionary_iterator(pimpl->headD);
    } 
    else
    {
        throw json_exception{"JSON non è un dizionario"};
    }
}
json::const_dictionary_iterator json::end_dictionary() const 
{
    if (is_dictionary()) 
    {
        return const_dictionary_iterator(nullptr);
    } 
    else
    {
        throw json_exception{"JSON non è un dizionario"};
    }
}



//costruttore di default
json::json()
{
    pimpl=new impl;
    pimpl->number=0.0;
    pimpl->string="";
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
    pimpl->type = "null";
}

//copy constructor
json::json(const json& other) 
{
    pimpl = new impl;
    pimpl->string=other.pimpl->string;
    pimpl->number=other.pimpl->number;
    pimpl->boolean=other.pimpl->boolean;
    pimpl->type=other.pimpl->type;

    pimpl->headL = pimpl->copy(other.pimpl->headL);
    if(pimpl->headL != nullptr){
        json::impl::ListNode l=pimpl->headL;
        while(l->next!=nullptr){
            l=l->next;
        }
        pimpl->tailL=l;
    }
    pimpl->headD = pimpl->copy(other.pimpl->headD);
    if(pimpl->headD != nullptr){
        json::impl::DictNode d=pimpl->headD;
        while(d->next!=nullptr)
            d=d->next;
        pimpl->tailD=d;
    }
}

// Move constructor
json::json(json&& other)
{
    pimpl = new impl;
    pimpl->type="null";
    pimpl->number = 0.0;
    pimpl->string="";
    pimpl->headL= nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
    *this = std::move(other);
}

// Destructor
json::~json() 
{
    pimpl->number=0.0;
    pimpl->type="null";
    pimpl->boolean=false;
    pimpl->string="";
    if(pimpl->headL != nullptr){
        pimpl->destroy(pimpl->headL);
        pimpl->headL = nullptr;
        pimpl->tailL = nullptr;
    }
    if(pimpl->headD != nullptr){
        pimpl->destroy(pimpl->headD);
        pimpl->headD = nullptr;
        pimpl->tailD = nullptr;
    }
    delete pimpl;
}
// Copy assignment operator
json& json::operator=(json const& other) 
{
    if (this != &other)
    {
        pimpl->type = other.pimpl->type;
        if(is_string())
        {
            pimpl->string = other.pimpl->string;
        }
        else
        {
            if(is_number())
            {
                pimpl->number = other.pimpl->number;
            }
            else
            {
                if(is_bool())
                {
                    pimpl->boolean = other.pimpl->boolean;        
                }
                else
                {
                    if(is_list())
                    {
                        pimpl->destroy(pimpl->headL);
                        pimpl->headL = nullptr;
                        pimpl->tailL = nullptr;
                        for(auto it = other.begin_list(); it != other.end_list(); ++it)
                        {
                            push_back(*it);
                        }
                    }
                    else
                    {
                        if(is_dictionary())
                        {
                            pimpl->destroy(pimpl->headD);
                            pimpl->headD = nullptr;
                            pimpl->tailD = nullptr;
                            for(auto it = other.begin_dictionary(); it != other.end_dictionary(); ++it)
                            {
                                insert(*it);
                            }
                        }
                    }
                }
            }
        }
    }
    return *this;
}

// Move assignment operator
json& json::operator=(json&& other) 
{
    if (this != &other)
    {
    
        pimpl->number = other.pimpl->number;
        pimpl->boolean = other.pimpl->boolean;
        pimpl->string = std::move(other.pimpl->string);
        pimpl->type = std::move(other.pimpl->type);


        pimpl->destroy(pimpl->headL);
        pimpl->headL = nullptr;
        pimpl->tailL = nullptr;
        pimpl->headL = other.pimpl->headL;
        pimpl->tailL = other.pimpl->tailL;
        other.pimpl->headL = other.pimpl->tailL = nullptr;

        pimpl->destroy(pimpl->headD);
        pimpl->headD = nullptr;
        pimpl->tailD = nullptr;
        pimpl->headD = other.pimpl->headD;
        pimpl->tailD = other.pimpl->tailD;
        other.pimpl->headD = other.pimpl->tailD = nullptr;
    }
    return *this;
}



bool json::is_list() const 
{
    if(pimpl->type=="lista")
    {
        return true;
    }
    return false;
}

bool json::is_dictionary() const 
{
    if(pimpl->type=="dizionario")
    {
        return true;
    }
    return false;
}

bool json::is_string() const 
{
    if(pimpl->type=="stringa")
    {
        return true;
    }
    return false;
}

bool json::is_number() const 
{
    if (pimpl->type=="numero")
    {
        return true;
    }
    return false;
}

bool json::is_bool() const 
{
    if (pimpl->type == "bool")
    {
        return true;
    }
    return false;
}

bool json::is_null() const 
{
    if (pimpl->type == "null")
    {
        return true;
    }
    return false;
}


json const& json::operator[](std::string const& key) const 
{
    if (!is_dictionary()) 
    {
        throw json_exception{"Non è possibile fare un inserimento nel dizionario tramite operator[] const"};
    }
    json::impl::DictNode it = pimpl->headD;
    while (it != nullptr && it->value.first != key)
    {
        it = it->next;
    }

    if (it != nullptr)
    {
        return it->value.second;
    }
    else
    {
        throw json_exception{"value non trovato"};
    }
}

json& json::operator[](std::string const& key) 
{
    if (!is_dictionary()) 
    {
        throw json_exception{"JSON non è un dizionario"};
    }
    json::impl::DictNode it = pimpl->headD;
    while (it != nullptr && it->value.first != key)
    {
        it = it->next;
    }

    if (it != nullptr)
    {
        return it->value.second;
    }
    else
    {
        std::pair<std::string, json> newPair;
        newPair.first = key;
        newPair.second = json();
        this->insert(newPair);
        return pimpl->tailD->value.second;
    }
}

double& json::get_number() 
{
    if (is_number()) 
    {
        return pimpl->number;
    }
    throw json_exception{"JSON non è un numero"};
}

double const& json::get_number() const 
{
    if (is_number()) 
    {
        return pimpl->number;
    }
    throw json_exception{"JSON non è un numero"};
}

bool& json::get_bool() 
{
    if (is_bool()) 
    {
        return pimpl->boolean;
    }
    throw json_exception{"JSON non è un booleano"};
}

bool const& json::get_bool() const 
{
    if (is_bool()) 
    {
        return pimpl->boolean;
    }
    throw json_exception{"JSON non è un booleano"};
}

std::string& json::get_string() 
{
    if (is_string()) 
    {
        return pimpl->string;
    }
    throw json_exception{"JSON non è una stringa"};
}

std::string const& json::get_string() const 
{
    if (is_string()) 
    {
        return pimpl->string;
    }
    throw json_exception{"JSON non è una stringa"};
}


//metodi per settare contenuto JSON
void json::set_string(std::string const& value) 
{
    pimpl->number=0.0;
    pimpl->boolean=false;
    if (is_list())
    {
        if(pimpl->headL)
        {
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary())
    {
        if(pimpl->headD)
        {
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->string = value;
    pimpl->type="stringa";
}

void json::set_bool(bool value) 
{
    pimpl->number=0.0;
    pimpl->string="";
    if (is_list())
    {
        if(pimpl->headL != nullptr)
        {
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary())
    {
        if(pimpl->headD != nullptr)
        {
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->boolean = value;
    pimpl->type="bool";
}

void json::set_number(double value) 
{
    pimpl->boolean=false;
    pimpl->string="";
    if (is_list())
    {
        if(pimpl->headL)
        {
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary())
    {
        if(pimpl->headD)
        {
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->number = value;
    pimpl->type = "numero";
}

void json::set_null() 
{
    pimpl->string="";
    pimpl->number = 0.0;
    pimpl->boolean = false;
    if (is_list())
    {
        if(pimpl->headL)
        {
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary())
    {
        if(pimpl->headD)
        {
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }
    pimpl->type="null";
}

void json::set_list() 
{
    pimpl->boolean=false;
    pimpl->number=0.0;
    pimpl->string="";
    if (is_list())
    {
        if(pimpl->headL)
        {
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary())
    {
        if(pimpl->headD)
        {
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }

    pimpl->type = "lista";
}

void json::set_dictionary() 
{
    pimpl->boolean=false;
    pimpl->number=0.0;
    pimpl->string="";
    if (is_list())
    {
        if(pimpl->headL)
        {
            pimpl->destroy(pimpl->headL);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }
    }else if(is_dictionary())
    {
        if(pimpl->headD)
        {
            pimpl->destroy(pimpl->headD);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }
    }

    pimpl->type = "dizionario";
}


void json::push_front(json const& value) 
{
    if (!is_list()) 
    {
        throw json_exception{"JSON non è una lista"};
    }
    json::impl::ListNode newL = new json::impl::NodeL;
    newL->value = value;
    newL->next = pimpl->headL;
    if (pimpl->headL == nullptr)
    {
        pimpl->tailL=newL;
    }
    pimpl->headL=newL;
}

void json::push_back(json const& value) 
{
    if (!is_list()) 
    {
        throw json_exception{"JSON non è una lista"};
    }
    json::impl::ListNode newL = new json::impl::NodeL;
    newL->value = value;
    newL->next = nullptr;
    if (pimpl->tailL == nullptr)
    {
        pimpl->headL=pimpl->tailL=newL;
    }
    else
    {
        pimpl->tailL->next = newL;
        pimpl->tailL = newL;
    }
}

void json::insert(std::pair<std::string, json> const& value) 
{
    if (!is_dictionary()) 
    {
        throw json_exception{"JSON non è un dizionario"};
    }
    json::impl::DictNode newD = new json::impl::NodeD;
    newD->value = value;
    newD->next = nullptr;
    if (pimpl->tailD == nullptr)
    {
        pimpl->headD = pimpl->tailD = newD;
    }
    else
    {
        pimpl->tailD->next = newD;
        pimpl->tailD = newD;
    }
}

std::ostream& operator<<(std::ostream& lhs, const json& rhs) 
{
    if (rhs.is_null()) 
    {
        lhs << "null";
    } else if (rhs.is_bool()) 
    {
        if(rhs.get_bool())
        {
            lhs << "true";
        }
        else
        {
            lhs<< "false";
        }
    } else if (rhs.is_number()) 
    {
        lhs << rhs.get_number();
    } else if (rhs.is_string()) 
    {
        lhs << "\"";
        lhs << rhs.get_string();
        lhs << "\"";
    } else if (rhs.is_list()) 
    {
        lhs << "[";
        auto it = rhs.begin_list();
        while(it != rhs.end_list()) 
        {
            lhs << *it++;
            if (it != nullptr)
            {
                lhs << ",";
            }
        }
        lhs << "]";
    } else if (rhs.is_dictionary()) 
    {
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

json parseList(std::istream &lhs);
json parseDictionary(std::istream &lhs);

std::string parse_string(std::istream &lhs)
{
    std::string str;
    char ch = 0;
    lhs >> ch;
    lhs.putback(ch);
    while (lhs.get(ch) && ch != '\"') 
    {
        if (ch == '\\') 
        {

            if (lhs.peek() == '\"') 
            {

                lhs.get(ch);
                str.push_back('\\');
                str.push_back(ch);
            } 
            else 
            {
                str.push_back(ch);
            }
        } 
        else 
        {
            str.push_back(ch);
        }
    }
    return str;
}

json parseString(std::istream& lhs) 
{
    std::string str;
    json rhs;
    char ch = 0;
    lhs >> ch;
    lhs.putback(ch);
    while (lhs.get(ch) && ch != '\"') 
    {
        if (ch == '\\') 
        {
            if (lhs.peek() == '\"') 
            {
                lhs.get(ch);
                str.push_back('\\');
                str.push_back(ch);
            } 
            else 
            {
                str.push_back(ch);
            }
        } 
        else 
        {
            str.push_back(ch);
        }
    }
    rhs.set_string(str);
    return rhs;
}


json parseBool(std::istream &lhs)
{
    char c = 0;
    json newJ;
    lhs >> c;
    if(c=='t')
    {
        lhs>>c;
        if(c!='r')
            throw json_exception{"errore in json \"true\""};
        lhs>>c;
        if(c!='u')
            throw json_exception{"errore in json \"true\""};
        lhs>>c;
        if(c!='e')
            throw json_exception{"errore in json \"true\""};

        newJ.set_bool(true);
    }
    else
    {
        lhs>>c;
        if(c!='a')
            throw json_exception{"errore in json \"false\""};
        lhs>>c;
        if(c!='l')
            throw json_exception{"errore in json \"false\""};
        lhs>>c;
        if(c!='s')
            throw json_exception{"errore in json \"false\""};
        lhs>>c;
        if(c!='e')
            throw json_exception{"errore in json \"false\""};

        newJ.set_bool(false);
    }

    return newJ;
}
json parseNull(std::istream &lhs)
{
    std::string str;
    json newJ;
    char c;
    lhs >> c;
    while(c != ']' && c != '}' && c != ',' && c != ' ' && c != '\r' && c != '\n' && !lhs.eof())
    {
        str.push_back(c);
        lhs.get(c);
    }
    if(str != "null")
    {
        throw json_exception{"JSON non è in un formato valido: null non è corretto"};
    }
    lhs.putback(c);
    newJ.set_null();
    return newJ;
}

json parseNumber(std::istream &lhs, char& c)
{
    std::string str;
    json newJ;
    while (c != ']' && c != '}' && c != ',' && c != ' ' && c != '\r' && c != '\n' && !lhs.eof())
    {
        str.push_back(c);
        lhs.get(c);
        if((c < '0' || c > '9') && c != '.'&& c != ']' && c != '}' && c != ',' && c != ' ' && c != '\r' && c != '\n')
        {
            throw json_exception{"JSON non è in un formato valido: number non è corretto"};
        }
        if (c == '.' && str.find('.') != str.npos)
        {
            throw json_exception{"'.' è già presente in number"};
        }
    }
    double num = std::stod(str);
    newJ.set_number(num);
    return newJ;
}


json parseList(std::istream &lhs)
{
    char c = 0;
    lhs >> c;
    json newJson;
    json rhs;
    newJson.set_list();
    while (c != ']' && lhs.peek() != ',' && lhs.get(c))
    {
        if (c == '{')
        {
            rhs = parseDictionary(lhs);
            newJson.push_back(rhs);
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c == '"')
        {
            rhs = parseString(lhs);
            newJson.push_back(rhs);
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    std::cout << c << std::endl;
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c == 'n')
        {
            lhs.putback(c);
            rhs = parseNull(lhs);
            newJson.push_back(rhs);
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c == 'f' || c == 't')
        {
            lhs.putback(c);
            rhs = parseBool(lhs);
            newJson.push_back(rhs);
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{' ) 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='-')
        {
            rhs = parseNumber(lhs, c);
            newJson.push_back(rhs);
            if(c != ']' && c != '}') {
                while (c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{' ) {
                    if (c != ' ')
                    {
                        throw json_exception{"JSON non è in un formato valido"};
                    }
                    lhs.get(c);
                }
            }
        }
        else if (c == '[') 
        {
            lhs.putback(c);
            rhs = parseList(lhs);
            newJson.push_back(rhs);
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{' ) 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
    }
    if(c != ']')
    {
        throw json_exception{"JSON non è in un formato valido"};
    }
    return newJson;
}

json parseDictionary(std::istream &lhs)
{
    char c = 0;
    json newJ;
    std::pair<std::string, json> rhs;
    newJ.set_dictionary();
    while (c != '}' && lhs.peek() != ',' && lhs.get(c))
    {
        while(c == ' ' || c == '\n' || c == '\r') 
        {
            lhs >> c;
        }
        if(rhs.first == "" && c == '"') 
        {
            lhs >> c;
            lhs.putback(c);
            rhs.first = parse_string(lhs);
            lhs >> c;
            if (c != ':') 
            {
                throw json_exception{"JSON non è in un formato valido"};
            }
        }
        else if(rhs.first == "" && c != '"' && c != '{' && c != '\r' && c != '}' && c != ',' && c != ']' && c != '[')
        {
            throw json_exception{"JSON non è in un formato valido"};
        }
        else if (c == '{')
        {
            rhs.second = parseDictionary(lhs);
            newJ.insert(rhs);
            lhs >> c;
            lhs.putback(c);
            lhs >> c;
            if(c == '\n')
            {
                throw json_exception{"JSON non è in un formato valido"};
            }
            if(c == '}' && lhs.peek() == ',')
                break;
            rhs.first = "";
        }
        else if (c == '"')
        {
            rhs.second = parseString(lhs);
            newJ.insert(rhs);
            rhs.first = "";
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c == 'n')
        {
            lhs.putback(c);
            rhs.second = parseNull(lhs);
            newJ.insert(rhs);
            rhs.first = "";
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c == 't' || c == 'f')
        {
            lhs.putback(c);
            rhs.second = parseBool(lhs);
            newJ.insert(rhs);
            rhs.first = "";
            lhs >> c;
            while(c != ',' && lhs.peek() !='\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='-')
        {
            rhs.second = parseNumber(lhs, c);
            newJ.insert(rhs);
            rhs.first = "";
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
        else if (c == '[')
        {
            lhs.putback(c);
            rhs.second = parseList(lhs);
            newJ.insert(rhs);
            rhs.first = "";
            lhs >> c;
            while(c != ',' && lhs.peek() != '\n' && c != ']' && c != '\n' && c != '[' && c != '}' && c != '{') 
            {
                if (c != ' ') 
                {
                    throw json_exception{"JSON non è in un formato valido"};
                }
                lhs.get(c);
            }
        }
    }
    if(c != '}')
        throw json_exception{"JSON non è in un formato valido"};
    return newJ;
}

std::istream &operator>>(std::istream &lhs, json &rhs)
{

    char c = 0;
    lhs >> c;
    if(c==' ')
    {
        throw json_exception{"json vuoto"};
    }
    if (c == '[' )
    {
        lhs.putback(c);
        rhs = parseList(lhs);
    }
    else if (c == '{')
    {
        rhs = parseDictionary(lhs);

    }
    else if (c == '"')
    {
        rhs = parseString(lhs);
    }
    else if (c == 'n')
    {
        lhs.putback(c);
        rhs = parseNull(lhs);
    }
    else if (c == 't' || c == 'f')
    {
        lhs.putback(c);
        rhs = parseBool(lhs);
    }
    else if (c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='-')
    {
        rhs = parseNumber(lhs, c);
    }else
    {
        throw json_exception{"Il JSON non è in un formato valido"};
    }

    return lhs;
}
int main() {

    std::string newFile = "";

    for (int i=1; i<101;i++){
        newFile = "json_test/"+std::to_string(i);
        newFile += ".txt";
        std::cout<<""<<std::endl;
        std::cout<<""<<std::endl;
        std::cout<<"Apro il file "<<i<<std::endl;
        std::ifstream file(newFile);
        if (!file.is_open()) {
            std::cout << "Failed to open the file." << std::endl;
            return 1;
        }
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        json data;

        ss >> data;
        std::cout << data << std::endl;
    }
    std::cout<<"Il Parser e' andato a buon fine"<<std::endl;
    return 0;
}