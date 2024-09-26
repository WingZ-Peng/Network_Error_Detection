#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string.h>
#include <algorithm>

using namespace std;

// CRC 
string XOR(string a, string b){
    string ret = "";
    // for loop from i=1, to skip xor each first item
    // to handle two cases: 
    // 1. when both two strings are in the same size that start with 1, then to skip append the 0 to ret
    // 2. when two strings not in the same size, which means tmp_dividend.size() = divisor.size() + 1. tmp_dividend start with a zero
    for (int i = 1; i < a.size(); ++i){
        if (a[i] == b[i])
            ret += '0';
        else
            ret += '1';
    }
    return ret;
}

string calculateCRC(string dividend, string divisor){
    int idx = divisor.size();
    string tmp_dividend = dividend.substr(0, idx);

    while (idx < dividend.size()){
        if (tmp_dividend[0] == '1'){
            tmp_dividend = XOR(tmp_dividend, divisor) + dividend[idx];
        }
        // tmp_dividend[0] == 0, which means we need to append twice to reach the same size with divisor
        // solution: use the same size substr that all filled with zero to do XOR in order to keep the original data
        else {
            tmp_dividend = XOR(tmp_dividend, string(divisor.size(), '0')) + dividend[idx];
        }
        ++idx;
    }
    // handle the last n size
    if (tmp_dividend[0] == '1')
        tmp_dividend = XOR(tmp_dividend, divisor);
    else
        tmp_dividend = XOR(tmp_dividend, string(divisor.size(), '0'));

    return tmp_dividend;
}

string CRC(string data){
    string CRC16 = "10000010000010001"; 
    int length_of_zero = CRC16.size() - 1;
    // append correct numbers of zero to data
    string appended_data = data + string(length_of_zero, '0');
    string check_code = calculateCRC(appended_data, CRC16);

    return check_code;
}

// CheckSum
vector<string> separateToParts(string data, int partSize = 16) {
    vector<string> parts;
    for (int i = 0; i < 4; ++i) {
        parts.push_back(data.substr(i * partSize, partSize));
    }
    return parts;
}

vector<int> convertBinaryToDecimal(vector<string> binaryParts) {
    vector<int> decimalParts;
    for (const auto& part : binaryParts) {
        decimalParts.push_back(bitset<16>(part).to_ulong());
    }
    return decimalParts;
}

int calculateSum(vector<int> numbers) {
    int sum = 0;
    for (const auto& n : numbers) {
        sum += n;
    }
    return sum;
}

string calculateChecksum(int sum, int divisor = 65536) {
    int quotient = sum / divisor;
    int remainder = sum % divisor + quotient;
    string binary = bitset<16>(remainder).to_string();
    
    string checksum = "";
    for (const auto& bit : binary) {
        checksum += (bit == '1') ? '0' : '1';
    }
    return checksum;
}

string CheckSum(string data) {
    vector<string> binary_parts = separateToParts(data);
    vector<int> decimal_parts = convertBinaryToDecimal(binary_parts);
    int bit16_sum = calculateSum(decimal_parts);
    string check_code = calculateChecksum(bit16_sum);
    
    return check_code;
}

string introduceErrorBits(string data, string errorBits){
    string polluted_data = data;
    for (int i = 0; i < data.size(); ++i){
        if (errorBits[i] == '1') {
            polluted_data[i] = (polluted_data[i] == '0') ? '1' : '0';
        }
    }
    return polluted_data;
}

void printResult(string data, string errorBits){
    // Append the check codes to the data following the corresponding rules
    string crc_code = CRC(data);
    string checksum_code = CheckSum(data);
    string crc_data = data + crc_code;
    string checksum_data = data + checksum_code;

    // Introduce error, length is 80 bits
    string polluted_crc = introduceErrorBits(crc_data, errorBits);
    string polluted_checksum = introduceErrorBits(checksum_data, errorBits);

    // recalculate the remainder of CRC & CheckSum
    string polluted_crc_code = CRC(polluted_crc.substr(0, data.size()));
    string polluted_checksum_code = CheckSum(polluted_checksum.substr(0, data.size()));

    // Print result for CRC
    cout << "CRC-16" << endl;
    cout << "CRC: " << crc_code << "; \t\tResult: " 
         << ((crc_code == polluted_crc_code) ? "Pass" : "Not Pass") 
         << endl;

    // Print results for checksum
    cout << "Checksum" << endl;
    cout << "Checksum: " << checksum_code << "; \tResult: " 
         << ((checksum_code == polluted_checksum_code) ? "Pass" : "Not Pass") 
         << endl;
}

int main(){
    ifstream myfile;
    myfile.open("data.txt");

    if (myfile.is_open() == 0){
        cout<<"File does not exist.";
    }
    else{
        string input, data, errorBits;
        
        while (getline(myfile, input)){
            for (int i = 0; i < input.length(); ++i){
                if (input[i] == ' '){
                    data = input.substr(0, i);
                    errorBits = input.substr(i+1);
                    cout << "=================================" << endl;
                    cout << "Data: " << data << endl;
                    cout << "Error: " << errorBits << endl;
                    printResult(data, errorBits);
                    break;
                }
            }
        }
    }

    return 0;
}

