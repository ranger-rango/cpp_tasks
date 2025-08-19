#include "xmlJsonConverterController.h"
#include <pugixml.hpp>
#include <json/json.h>
#include <string>
using namespace pugi;
using namespace Json;

string converter(const Value& json, const string& rootName = "root")
{
    xml_document doc;
    auto root = doc.append_child(rootName.c_str());

    function<void (const Value&, xml_node&)> buildXml = [&](const Value& val, xml_node& node)
    {
        if (val.isObject())
        {
            for (const auto &key : val.getMemberNames())
            {
                auto child = node.append_child(key.c_str());
                buildXml(val[key], child);
            }
        }
        else if (val.isArray())
        {
            for (const auto &item : val)
            {
                auto child = node.append_child("item");
                buildXml(item, child);
            }
        }
        else
        {
            node.text().set(val.asString().c_str());
        }
    };

    buildXml(json, root);

    ostringstream oss;
    doc.save(oss, " ");
    return oss.str();
}

Value converter(const xml_node& node)
{
    Value json;
    if (node.first_child() && node.first_child().type() == node_pcdata)
    {
        return Value(node.text().as_string());
    }

    for (auto& child : node.children())
    {
        Value childJson = converter(child);
        if (json.isMember(child.name()))
        {
            if (!json[child.name()].isArray())
            {
                Value temp = json[child.name()];
                json[child.name()] = arrayValue;
                json[child.name()].append(temp);
            }
            json[child.name()].append(childJson);
        }
        else
        {
            json[child.name()] = childJson;
        }
    }

    return json;
}

void xmlJsonConverterController::xmlJsonConverter(const HttpRequestPtr& req, function <void (const HttpResponsePtr& )> &&callback)
{
    auto contentType = req->getHeader("Content-Type");
    auto bodyView = req->getBody();
    string body(bodyView);

    // cout << "Content-Type: " << contentType << endl << "body: " << body << endl;
    if (contentType.find("application/json") != string::npos)
    {
        CharReaderBuilder rbuilder;
        Value jsonData;
        string errs;
        istringstream iss(body);
        if (!parseFromStream(rbuilder, iss, &jsonData, &errs))
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);
            resp->setBody("Invalid JSON: " + errs);
            callback(resp);
            return;
        }

        string xml = converter(jsonData);
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_XML);
        resp->setBody(xml);
        callback(resp);

    }
    else if (contentType.find("application/xml") != string::npos)
    {
        xml_document doc;
        if (!doc.load_string(body.c_str()))
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);
            resp->setBody("Invalid XML");
            callback(resp);
            return;
        }

        Value jsonData = converter(doc.document_element());
        // Value jsonData = converter(doc.select_node("/*").node());
        StreamWriterBuilder wbuilder;
        string jsonStr = writeString(wbuilder, jsonData);
        auto resp = HttpResponse::newHttpJsonResponse(jsonData);
        callback(resp);
    }
    else
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k415UnsupportedMediaType);
        resp->setBody("Content-Type must be application/json or application/xml");
        callback(resp);
    }
}


// Notes.
//     Json Verification.
//         CharReaderBuilder - create a parser configuration.
//         istringstream iss(body) - treat the request body as a stream for the parser.
//         parseFromStream(...) - parse the JSON text into jsonData
//             Returns false of invalid JSON.
//             Errors are stored in errs.
        
//     Json -> XML conversion.
//         xml_document - the XML DOM tree container from pugixml.
//         doc.append_child(...) - creates an element, e.g <root>
//         val.getMemberNames() - loop over keys, returns vector of strings.
//         doc.save(...) - serialise XML to a string with identaion "  ".

//     XML Verification.
//         doc.load_string(body.c_str())
//             parses the XML string
//             Returns false if invalid XML.

//     XML -> Json conversion.
//         xml_node& node - a DOM node from pugixml.
//         if the node's first child is text (node_pcdata)
//             return the text value as a Json::Value (string).
        