#include "IpToCountryResolver.h"
#include "q_shared.h"

void convertToCamelCase(char *str)
{
    int upper = 1;
    char *pos = str;
    while (*pos != '\0') {
        if (*pos != ' ') {
            if (upper) {
                (*pos) = toupper(*pos);
                upper = 0;
            }
            else
                (*pos) = tolower(*pos);
        } else {
            upper = 1;
        }
        pos++;
    }
}

IpToCountryInfo *FindCountryInfoUI(unsigned int ip, IpToCountryList *infoList)
{
    int abound = 0;
    int zbound = infoList->size-1;
    int tbound = 0;
    IpToCountryInfo *telement;

    if (infoList->size == 0)
        return NULL;

    while (abound <= zbound)
    {
        tbound = (abound+zbound)/2;
        telement = &infoList->infoArray[tbound];

        if (ip < telement->ipFrom)
            zbound = tbound-1;
        else if (ip > telement->ipTo)
            abound = tbound+1;
        else
            return telement;
    }
    return NULL;
}

IpToCountryInfo *FindCountryInfoB(unsigned int parts[4], unsigned int length, IpToCountryList *infoList)
{
    return FindCountryInfoUI(ipbyte_to_int(parts, length),infoList);
}

IpToCountryInfo *FindCountryInfoS(const char *ip_addr, IpToCountryList *infoList)
{
    return FindCountryInfoUI(ipstr_to_int(ip_addr),infoList);
}

unsigned int ipbyte_to_int(unsigned int parts[4], unsigned int length)
{
    unsigned int val = parts[length-1];

    switch (length) {

	case 1:				/* a -- 32 bits */
		break;

	case 2:				/* a.b -- 8.24 bits */
		if (val > 0xffffff)
			return (0);
		val |= parts[0] << 24;
		break;

	case 3:				/* a.b.c -- 8.8.16 bits */
		if (val > 0xffff)
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16);
		break;

	case 4:				/* a.b.c.d -- 8.8.8.8 bits */
		if (val > 0xff)
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
		break;
	}

	return val;
}

unsigned int ipstr_to_int(const char *cp_arg)
{
	register unsigned long val;
	register int base, n;
	register unsigned char c;
	register unsigned const char *cp = (unsigned const char *) cp_arg;
	unsigned int parts[4];
	register unsigned int *pp = parts;

	for (;;) {
		/*
		 * Collect number up to ``.''.
		 * Values are specified as for C:
		 * 0x=hex, 0=octal, other=decimal.
		 */
		val = 0; base = 10;
		if (*cp == '0') {
			if (*++cp == 'x' || *cp == 'X')
				base = 16, cp++;
			else
				base = 8;
		}
		while ((c = *cp) != '\0') {
			if (isascii(c) && isdigit(c)) {
				val = (val * base) + (c - '0');
				cp++;
				continue;
			}
			if (base == 16 && isascii(c) && isxdigit(c)) {
				val = (val << 4) +
					(c + 10 - (islower(c) ? 'a' : 'A'));
				cp++;
				continue;
			}
			break;
		}
		if (*cp == '.') {
			/*
			 * Internet format:
			 *	a.b.c.d
			 *	a.b.c	(with c treated as 16-bits)
			 *	a.b	(with b treated as 24 bits)
			 */
			if (pp >= parts + 3 || val > 0xff)
				return (0);
			*pp++ = val, cp++;
		} else {
		    *pp = val;
			break;
		}
	}
	/*
	 * Check for trailing characters.
	 */
	if (*cp && (!isascii(*cp) || !isspace(*cp)))
		return (0);
	/*
	 * Concoct the address according to
	 * the number of parts specified.
	 */
	n = pp - parts + 1;

    return ipbyte_to_int(parts,n);
}
