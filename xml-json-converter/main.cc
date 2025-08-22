#include <drogon/drogon.h>
using namespace drogon;
int main() {
    app().addListener("0.0.0.0", 8080);
    app().run();
    return 0;
}

// curl -X POST http://localhost:8080/pathconvert \
//      -H "Content-Type: application/json" \
//      -d '{"student": {"id": "12345", "fullname": "Oscar Ngombo", "age_dob": {"age": 28, "dob": "01-May-1997"} } } '


//      -d '{"person": {"name": "Alice", "age": 30}}'


// curl -X POST http://localhost:8080/pathconvert \
//      -H "Content-Type: application/xml" \
//      -d '<student id="12345">
//      <Fname>Oscar</Fname>
//      <Lname>Ngombo</Lname>
//      <Age>28</Age>
//      <DOB>01-May-1997</DOB>
//      </student>'

