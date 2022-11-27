#ifndef __IP_HACK_HPP__
#define __IP_HACK_HPP__

#define WIN32

#pragma comment(lib,"WinPcap/lib/wpcap.lib")
#include "WinPcap/Include/pcap.h"
#include <string>
#include <set>
#include "IP2Location.h"

namespace KYFunc {
    class IP_Hack {
    public:
        struct IP_Data {
            std::string IP;
            std::string Country, City;
            std::string Latitude, Longitude;
        };

    public:
        IP_Hack() {
            IP2LocationObj = IP2Location_open((char*)"IP2LOCATION-LITE-DB5.BIN");
        }
        ~IP_Hack() {
            IP2Location_close(IP2LocationObj);
        }

        IP2LocationRecord* get_ip_hack(const std::string& IP) {
            auto record = IP2Location_get_all(IP2LocationObj, (char*)IP.c_str());
            return record;
        }

        IP_Data get_IP_data(const std::string& IP) {
            IP_Data data_ip;
            auto record = IP2Location_get_all(IP2LocationObj, (char*)IP.c_str());

            data_ip.IP = IP;
            data_ip.Country = record->country_long;
            data_ip.City = record->city;
            data_ip.Latitude = std::to_string(record->latitude);
            data_ip.Longitude = std::to_string(record->longitude);
            return data_ip;
        }
    protected:
        IP2Location* IP2LocationObj;

    };

} // namespace KYFunc


#endif