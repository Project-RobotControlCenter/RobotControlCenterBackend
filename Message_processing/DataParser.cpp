//
// Created by elejandc on 11/15/24.
//

#include "DataParser.h"

std::string DataParser::getMessageTypeFromJson(const std::string& jsonString) {
    try {
        // Parsowanie JSON
        boost::json::value jsonValue = boost::json::parse(jsonString);
        const boost::json::object& jsonObject = jsonValue.as_object();

        // Sprawdzanie, czy istnieje pole "message_type" i czy jest typu string
        if (jsonObject.contains("message_type") && jsonObject.at("message_type").is_string()) {
            return jsonObject.at("message_type").as_string().c_str();
        } else {
            return ""; // Zwracamy pusty ciąg, jeśli "message_type" nie istnieje lub nie jest ciągiem
        }
    } catch (const boost::json::system_error& e) {
        // Obsługa błędu parsowania JSON
        std::cerr << "Błąd parsowania JSON: " << e.what() << std::endl;
        return "";
    }
}

st_getAllRobotsOrder DataParser::parseJsonToStrucGetAllRobotsOrdert(const std::string &json) {
    // Parsowanie JSON
    boost::json::value jsonValue = boost::json::parse(json);
    const boost::json::object& jsonObject = jsonValue.as_object();

    // Tworzenie instancji struktury i przypisanie wartości
    st_getAllRobotsOrder result;
    if (jsonObject.contains("message_type") && jsonObject.at("message_type").is_string()) {
        result.message_type = jsonObject.at("message_type").as_string().c_str();
    }

    // Możesz dodać dodatkowe mapowanie danych, jeśli istnieją w sekcji 'data'

    return result;
}

st_connectToRobotOrder DataParser::parseJsonToStructConnectToRobotOrder(const std::string &jsonString) {
    // Parsowanie JSON
    boost::json::value jsonValue = boost::json::parse(jsonString);
    const boost::json::object& jsonObject = jsonValue.as_object();

    st_connectToRobotOrder result;

    // Przypisanie wartości do pól struktury
    if (jsonObject.contains("message_type") && jsonObject.at("message_type").is_string()) {
        result.message_type = jsonObject.at("message_type").as_string().c_str();
    }

    if (jsonObject.contains("data") && jsonObject.at("data").is_object()) {
        const boost::json::object& dataObject = jsonObject.at("data").as_object();
        if (dataObject.contains("mac_address") && dataObject.at("mac_address").is_string()) {
            result.data.mac_address = dataObject.at("mac_address").as_string().c_str();
        }
    }

    return result;
}

std::string DataParser::parseStructToJson(const st_AllRobotsInfo &allRobotsInfo) {
    // Tworzenie tablicy JSON z obiektów st_robotInfo
    boost::json::array robotsArray;
    for (const auto& robot : allRobotsInfo.data.robots_info) {
        robotsArray.push_back(toJson(robot));
    }

    // Tworzenie obiektu JSON dla st_AllRobotsInfo
    boost::json::object jsonObject;
    jsonObject["message_type"] = allRobotsInfo.message_type;
    jsonObject["data"] = boost::json::object{
            {"robots_info", robotsArray}
    };

    // Serializacja obiektu JSON do std::string
    return boost::json::serialize(jsonObject);
}

boost::json::value DataParser::toJson(const st_robotInfo &robotInfo) {
    return boost::json::object{
                {"mac_address", robotInfo.mac_address},
                {"robot_name", robotInfo.robot_name},
                {"robot_ip", robotInfo.robot_ip},
                {"robot_port", robotInfo.robot_port} // Konwersja unsigned char do int zostanie wykonana automatycznie
    };
}
