#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

class HtmlElement
{
protected:
    string name;
    vector<pair<string, string>> attributes; // pair<name, value>
public:
    HtmlElement(const string& name) : name(name) {};
    virtual void add_attribute(const string& name, const string& value)
    {
        attributes.push_back({name, value});
    }
};



class CssElement : public HtmlElement
{
    friend class Printer;
protected:
    string css_type;
public:
    CssElement(const string& name, const string& css_type = "")
    : HtmlElement(name), css_type(css_type) {};

};



class TagElement : public HtmlElement
{
    friend class Printer;
protected:
    string text;
    vector<TagElement> children;
    vector<CssElement> css_children;
public:
    TagElement(const string& name, const string& text = "")
    : HtmlElement(name), text(text) {};

    TagElement& add_child(TagElement e) {
        children.push_back(e);
        return *this;
    }

    TagElement& add_child(const string& name, const string& text = "") {
        TagElement e(name, text);
        return add_child(e);
    }

    TagElement& add_css_child(CssElement e) {
        css_children.push_back(e);
        return *this;
    }

    TagElement& jump_down() {return children.back(); }
};

// Printer:
// string print(TagElement)
// string print(CssElement)
// save()

struct Printer
{
    int indent_size = 2;

    string print(TagElement tag, int indent = 0) 
    {
        ostringstream oss;
        string indent_space (indent_size*indent, ' ');

        oss << indent_space << "<" << tag.name;
        for (auto a : tag.attributes) oss << " " << a.first << "=" << "\"" << a.second << "\"";
        oss << ">" << endl;

        if (tag.text.size())
            oss << string(indent_size*(indent+1), ' ') << tag.text << endl;
        
        for (auto e : tag.children) oss << print(e, indent+1);
        for (auto e : tag.css_children) oss << print(e, indent+1);

        oss << indent_space << "</" << tag.name << ">" << endl;

        return oss.str();
    }

    string print(CssElement css, int indent = 0) {
        ostringstream oss;
        string indent_space (indent_size*indent, ' ');

        oss << indent_space << css.css_type << css.name << " " << "{" << endl;

        for (auto e : css.attributes)
            oss << string(indent_size*(indent+1), ' ') << e.first
                << ": \"" << e.second << "\"" << endl;
        
        oss << indent_space << "}" << endl;

        return oss.str();
    }

    void save(const string& data) {
        ofstream out;
        out.open("index.html");
        out << data;
        out.close();
    }
};

int main() {
    // Head
    TagElement title("title", "Sample \"Hello, World\" Application");

    CssElement p("p");
    p.add_attribute("color", "brown");

    TagElement style("style");
    style.add_css_child(p);

    TagElement head("head");
    head.add_child(title).add_child(style);


// Body
    TagElement table("table");
    table.add_child("tr").jump_down()
         .add_child("td").jump_down()
         .add_child("h1", "Sample \"Hello, World\" Application");

    TagElement ul("ul");
    ul.add_child("li", "To a JSP page")
      .add_child("li", "To a servlet");

    TagElement body("body");
    body.add_child(table)
        .add_child("p", "This is the home page for the HelloWorld Web application.")
        .add_child("p", "To prove that they work, you can execute either of the following ways:")
        .add_child(ul)
        .add_attribute("bgcolor", "white");
    
    TagElement html("html");
    html.add_child(head)
        .add_child(body);
    
    Printer printer;
    printer.save(printer.print(html));
}