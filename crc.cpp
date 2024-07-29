#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Function that generates a random string of k bits
string generateBinaryString(int k){
    srand(time(NULL));
    // Stores the empty string
    string S = "";

    // Iterate over the range [0, N - 1]
    for (int i = 0; i < k; i++) {
        // Generate the random number
        int x = ((int)rand() % 2);

        // Append it to the string
        S += to_string(x);
    }
    return S;
}

// Adds noise to bit string that's being transmitted
string addNoise(const string& input, double errorRate) {
    cout<<endl<<"TRANSMISSION..."<<endl;
    string noisyString;
    const long max_rand = 1000000L;
    double lower_bound = 0.0;
    double upper_bound = 1.0;
    srand(time(NULL));

    for (char bit : input) {
        // Generate random double from 0.0 to 1.0
        double random_double = lower_bound + (upper_bound - lower_bound) * (rand() % max_rand) / max_rand;

        // If random number is less that BER
        if (random_double < errorRate) {
            // Flip the bit
            bit = (bit == '0') ? '1' : '0';
        }
        noisyString += bit;
    }
    cout << "Original: " << input << std::endl;
    cout << "Noisy:    " << noisyString << std::endl;
    return noisyString;
}

// Perform xor operation
string xor1(string a, string b){
    // Initialize result
    string result = "";
    int n = b.length();

    // Traverse all bits, if bits are same, then XOR is 0, else 1
    for (int i = 1; i < n; i++) {
        if (a[i] == b[i]) result += "0";
        else result += "1";
    }
    return result;
}

// Performs Modulo-2 division of two strings
string mod2div(string Z, string P){
    // Number of bits to be XORed at a time.
    int pick = P.length();

    // Slicing the dividend to appropriate length for particular step
    string tmp = Z.substr(0, pick);

    int n = Z.length();
    while (pick < n) {
        if (tmp[0] == '1')
            // Replace the dividend by the result of XOR and pull 1 bit down
            tmp = xor1(P, tmp) + Z[pick];
        else
            // If leftmost bit is '0'.
            // If the leftmost bit of the dividend (or the part used in each step) is 0, the step cannot use the regular divisor we need to use an all-0s divisor.
            tmp = xor1(string(pick, '0'), tmp) + Z[pick];
        // Increment pick to move further
        pick += 1;
    }

    // For the last n bits, we have to carry it out normally as increased value of pick will cause Index Out of Bounds.
    if (tmp[0] == '1') tmp = xor1(P, tmp);
    else tmp = xor1(string(pick, '0'), tmp);

    return tmp;
}

// Receiver checks if the message has any errors
void receiver(string T, string P, int n, int  k){
    cout<<endl<<"RECEIVER SIDE..."<<endl;
    cout<<"Received: "<<T<<endl;

    string currxor = mod2div(T.substr(0, P.size()),P);
    int curr = P.size();
    while (curr != T.size()) {
        if (currxor.size() != P.size()) {
            currxor.push_back(T[curr++]);
        }
        else {
            currxor = mod2div(currxor, P);
        }
    }

    if (currxor.size() == P.size()) {
        currxor = mod2div(currxor, P);
    }

    cout<<"Remainder in receiver : "<<currxor<<endl;
    if (currxor.find('1') != string::npos) {
        cout << "there is some error in data" << endl;
    }
    else {
        cout << "correct message received" << endl;
    }
}

// Sender computes FCS
string sender(int n, int k, string P){
    cout<<endl<<"SENDER SIDE..."<<endl;
    // Call function generateBinaryString to make random D sequence of k bits
    string D=generateBinaryString(k);
    cout<<"D : "<<D<<endl;

    // Put n-k zeros at the end of the bit string
    string Z=D;
    for(int i=0;i<n-k;i++)
        Z=Z + "0";
    cout<<"D+zeros : "<<Z<<endl;

    // Call function mod2div to divide D+zeros by P using modulo-2 division
    string remainder = mod2div(Z, P);

    // Make codeword using the remainder of the division
    string codeword = D + remainder;
    cout << "Remainder : " << remainder << endl;
    cout << "Encoded Data (Data + Remainder) :" << codeword << endl;
    return codeword;
}

int main(){
    // Determine n, k
    int n;
    int k;
    cout<<"Give n: "<<endl;
    cin>>n;
    cout<<"Give k: "<<endl;
    cin>>k;

    // Determine P (n-k+1 bits)
    int z=n-k+1;
    string P;
    cout<<"Give a binary number P as prototype (must be "<<z<<" bits): ";
    cin>>P;

    // Call function sender for: given n, k and P
    string codeword= sender(n, k, P);

    // Determine Bit Error Rate
    double BER = 1e-3;

    // Add noise to transmission
    string noisyString = addNoise(codeword, BER);

    // Call function receiver for: codeword that sender made and given n, k , P
    receiver(noisyString, P, n, k);
}
