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

string calculateCRC(string& dividend, string& divisor){
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

string CRC(string& data){
    string CRC16 = "10000010000010001"; 
    int length_of_zero = CRC16.size() - 1;
    // append correct numbers of zero to data
    string appended_data = data + string(length_of_zero, '0');
    string check_code = calculateCRC(appended_data, CRC16);

    return check_code;
}

// CheckSum
vector<string> separateToParts(const string& data, int partSize = 16) {
    vector<string> parts;
    for (int i = 0; i < 4; ++i) {
        parts.push_back(data.substr(i * partSize, partSize));
    }
    return parts;
}

vector<int> convertBinaryToDecimal(const vector<string>& binaryParts) {
    vector<int> decimalParts;
    for (const auto& part : binaryParts) {
        decimalParts.push_back(bitset<16>(part).to_ulong());
    }
    return decimalParts;
}

int calculateSum(const vector<int>& numbers) {
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

string CheckSum(string& data) {
    vector<string> binary_parts = separateToParts(data);
    vector<int> decimal_parts = convertBinaryToDecimal(binary_parts);
    int bit16_sum = calculateSum(decimal_parts);
    string check_code = calculateChecksum(bit16_sum);
    
    return check_code;
}

int main(){
    ifstream myfile;
    myfile.open("data.txt");

    if (myfile.is_open() == 0){
        cout<<"File does not exist.";
    }
    else{
        string input, data, error;
        
        while (getline(myfile, input)){
            for (int i = 0; i < input.length(); ++i){
                if (input[i] == ' '){
                    data = input.substr(0, i);
                    error = input.substr(i+1);
                    // cout << data << "---" << error << endl;
                    
                    string a = CRC(data);
                    string b = CheckSum(data);
                    cout << "CRC remainder is: " << a << endl;
                    cout << "Checksum remainder is: " << b << endl;
                    break;
                }
            }
        }
    }

    return 0;
}

