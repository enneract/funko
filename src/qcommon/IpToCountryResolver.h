#ifndef IPTOCOUNTRYRESOLVER_H_INCLUDED
#define IPTOCOUNTRYRESOLVER_H_INCLUDED

#define FULL_NAME_LENGTH 64

typedef struct {
        unsigned int ipFrom;
        unsigned int ipTo;
        char name2[2+1]; //1 for zero character (line end)
        char name3[3+1];
        char nameFull[FULL_NAME_LENGTH];
} IpToCountryInfo;

typedef struct {
    unsigned int size;
    IpToCountryInfo *infoArray;
} IpToCountryList;

unsigned int ipstr_to_int (const char *cp_arg);
unsigned int ipbyte_to_int(unsigned int parts[4], unsigned int length);
unsigned int LoadIp2CountryInfo(const char *fileName, IpToCountryList *infoList, int camelCaseNames);
IpToCountryInfo *FindCountryInfoS(const char *ip_addr, IpToCountryList *infoList);
IpToCountryInfo *FindCountryInfoB(unsigned int parts[4], unsigned int length, IpToCountryList *infoList);
IpToCountryInfo *FindCountryInfoUI(unsigned int ip, IpToCountryList *infoList);

#endif // IPTOCOUNTRYRESOLVER_H_INCLUDED




