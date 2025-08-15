#include <drogon/drogon.h>
using namespace drogon;
int main() {
    app().addListener("0.0.0.0", 8080);
    app().run();
    return 0;
}

// curl -X POST http://localhost:8080/convert \
//      -H "Content-Type: application/json" \
//      -d '{"person": {"name": "Alice", "age": 30}}'


// curl -X POST http://localhost:8080/convert \
//      -H "Content-Type: application/xml" \
//      -d '<person><name>Alice</name><age>30</age></person>'
