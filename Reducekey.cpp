#include<bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
using namespace std;

const char subCandidates[15][50] = {
	" - 0x10000000000000000000000000000000",
	" - 0x20000000000000000000000000000000",
	" - 0x30000000000000000000000000000000",
	" - 0x40000000000000000000000000000000",
	" - 0x50000000000000000000000000000000",
	" - 0x60000000000000000000000000000000",
	" - 0x70000000000000000000000000000000",
	" - 0x80000000000000000000000000000000",
	" - 0x90000000000000000000000000000000",
	" - 0xA0000000000000000000000000000000",
	" - 0xB0000000000000000000000000000000",
	" - 0xC0000000000000000000000000000000",
	" - 0xD0000000000000000000000000000000",
	" - 0xE0000000000000000000000000000000",
	" - 0xF0000000000000000000000000000000",
};

FILE *file;
char hexVal[280], op[140], out[140], order[5], key[130];
void run_command(char* command){
	system(command);
	return;	
}

void selectSubtractedValue (){

    int numBits = sizeof(hexVal) * 8;

    unsigned long long decimal = 0;
    for (int i = 0; i < numBits; i++) {
        decimal += (hexVal[i/4] & (0xF << ((i%4)*4))) >> ((i%4)*4) << i;
    }

    int degrees = round((decimal * 14) / (pow(2, numBits) - 1)) + 1;
    char s[280];
    strcpy(s, subCandidates[degrees]);
    int len = strlen(hexVal);
    strcpy(op, "");
    if(len+4<strlen(s)) for(int i=0;i<len+4;i++) op[i] = s[i]; 
    else strcpy(op, s);
}

void _blkmk_bin2hex(char *out, char* data, size_t datasz) {
	char* datac = data;
	static char hex[] = "0123456789abcdef";
	out[datasz * 2] = '\0';
	for (size_t i = 0; i < datasz; ++i)
	{
		out[ i*2   ] = hex[datac[i] >> 4];
		out[(i*2)+1] = hex[datac[i] & 15];
	}
}

static uint8_t tolowercase(uint8_t ch)
{
    if ((ch >= 'a') && (ch <= 'f'))
        return ch + 0x20;  
    return ch;
}

static int8_t parseHexDigit(uint8_t digit)
{
    digit = tolowercase(digit);
    if (isdigit(digit))
        return (int8_t)digit - '0';
    if ((digit >= 'a') && (digit <= 'f'))
        return (int8_t)digit + 0xA - 'a';
    return -1; 
}

static int hexstring_parse(const char *hexstr, uint8_t *buf, size_t *buflen)
{
    size_t hexstrlen = *buflen * 2;
    size_t i;

    if (hexstrlen & 0x1)
    {
        return 1;
    }

    if (*buflen < hexstrlen/2)
    {
        return 1;
    }

    for (i=0;i<hexstrlen;i+=2)
    {
        int8_t a, b;
        if (-1 == (a = parseHexDigit(hexstr[i])))
        {
            return 1;
        }
        if (-1 == (b = parseHexDigit(hexstr[i+1])))
        {
            return 1;
        }
        *buf++ = (a << 4) | b;
    }

    *buflen = hexstrlen/2;

    return 0;
}

bool _blkmk_hex2bin(char *o, const char *x, size_t len) {
	char *oc = o;
	char c, hc = 0x10;
	len *= 2;
	char arr[33];
	uint8_t buf[8];
	size_t buflen = sizeof(buf);
	int k = hexstring_parse(o, buf, &buflen);
	while (len)
	{
		switch (x[k]) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			c = x[k] - '0';
			break;
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			c = x[k] - 'A' + 10;
			break;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			c = x[k] - 'a' + 10;
			break;
		default:
			return false;
		}
		++x;
		if (hc < 0x10)
		{
			(oc++)[0] = (hc << 4) | c;
			hc = 0x10;
		}
		else
			hc = c;
		--len;
	}
	_blkmk_bin2hex(oc, oc, 1024);
	return !x[0];
}

bool run_keyMath(){
    char cmd[1005];
    strcpy(cmd, "keymath.exe ");
	strcat(cmd, hexVal);
	strcat(cmd, op);
	system(cmd);
	
	char line[140];
	file = fopen("keymath_result.txt", "r");
    if (file == NULL) {
        printf("Unable to open the file.\n");
        return 0;
    }
    strcpy(out, "");
    while (fgets(line, sizeof(line), file)) {
		for(int i = 0;i<strlen(line);i++){
		 	if(line[i]!='@' && line[i]!='\n') out[i] = line[i];
		}
        break;
    }
    strcpy(hexVal, out);
    fclose(file);
}

void printOut(){
	char cmd[1005];
    strcpy(cmd, "PrintReducedKey.exe ");
    strcat(order, " ");
    strcat(cmd, order);
    strcat(cmd, hexVal);
    run_command(cmd);
    return;
}

void start(){
	if(strcmp(order, "2") == 0){
		printOut();
		return;
	}
	selectSubtractedValue();
	for(int i=0;i<10;i++) run_keyMath();
	
	char va[280];
	strcpy(va, hexVal);
	int len = strlen(key);
	strcpy(hexVal, "");
	for(int i = 0;i<len;i++){
		int id = i * 2;
		hexVal[ id ] = key[i] , id++;
		hexVal[id] = va[i];
	}
	printOut();
	return;
}



int main() {
	char line[140];
	
    file = fopen("input.txt", "r");

    if (file == NULL) {
        printf("Unable to open the file.\n");
        return 0;
    }
	
	
	strcpy(order, "");
	strcpy(key, "");
	
    while (fgets(line, sizeof(line), file)) {
        order[0] = line[0];
		for(int i = 2;i<strlen(line);i++){
		 	if(line[i]!='@' && line[i]!='\n') key[i-2] = line[i];
		}
        break;
    }

    strcpy(hexVal, key);
    start();
    fclose(file);
    

    return 0;
}
