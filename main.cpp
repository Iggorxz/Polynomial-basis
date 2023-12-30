#include <iostream>
#include <string>
#include <bitset>
#include <chrono>
#include <algorithm>
#define DIMENSION 233
#define DDIMENSION 466

using namespace std;
using namespace chrono;

class PB {

    bitset<DIMENSION> bits;
    const size_t bit_length;

public:
    PB() : bits{0b0}, bit_length{DIMENSION} {}

    PB(const bitset<DIMENSION>& bits) : bit_length{DIMENSION}, bits(bits) {}

    PB(const string& stringx) : bit_length{DIMENSION}, bits(0) {
        if (stringx.size() > DIMENSION) {
            throw invalid_argument("String length exceeds bitset size");
        }

        for (size_t i = 0; i < stringx.size(); ++i) {
            if (stringx[i] != '0' && stringx[i] != '1') {
                throw invalid_argument("Invalid character in string");
            }
            bits[stringx.size() - i - 1] = stringx[i] == '1';
        }
    }

    string to_string() const {
        size_t first_one = bits.size() - 1;
        while (first_one > 0 && !bits.test(first_one)) {
            --first_one;
        }

        if (!bits.test(first_one)) {
            return "0";
        }

        string res;
        for (int i = first_one; i >= 0; --i) {
            res += bits.test(i) ? '1' : '0';
        }
        return res;
    }

    bitset<DDIMENSION> increaseBitsetSize(const bitset<DIMENSION>& st) {
        return bitset<DDIMENSION>(st.to_string());
    }

    bitset<DIMENSION> cutToSmallerBitset(const bitset<DDIMENSION>& st) {
        return bitset<DIMENSION>(st.to_string().substr(DDIMENSION - DIMENSION));
    }


    void mod(bitset<DDIMENSION>& value) {
        const bitset<DDIMENSION> p = bitset<DDIMENSION>(1) | (bitset<DDIMENSION>(1) << 1) | (bitset<DDIMENSION>(1) << 4) | (bitset<DDIMENSION>(1) << 9) | (bitset<DDIMENSION>(1) << DIMENSION);

        for (int i = DDIMENSION - 1; i >= DIMENSION; --i) {
            if (value.test(i)) {
                value ^= p << (i - DIMENSION);
            }
        }
    }

    PB operator+(const PB& other) const{
        PB result;

        result.bits = this->bits ^ other.bits;

        return result;
    }

    PB operator*(const PB& right){
        bitset<DDIMENSION> mrc(0);

        bitset<DDIMENSION> a = increaseBitsetSize(this->bits);
        bitset<DDIMENSION> b = increaseBitsetSize(right.bits);

        int shift = 0;

        while (shift < DDIMENSION)
        {
            if (b[shift])
            {
                mrc ^= (a << shift);
            }
            shift++;
        }

        mod(mrc);

        return PB(cutToSmallerBitset(mrc));
    }

    bool operator==(const PB& x) const{
        int i = 0;
        while (i < DIMENSION) {
            if (this->bits[i] != x.bits[i])
            {
                return false;
            }
            i++;
        }
        return true;
    }

    PB& operator=(const PB& other){
        if (this != &other)
        {
            bits = other.bits;
        }
        return *this;
    }

    bitset<DIMENSION> str(const string& x){
        bitset<DIMENSION> res;
        int len = x.size()-1;
        if (len > DIMENSION) {
            throw invalid_argument("String length exceeds bitset size");
        }

        size_t i = 0;
        while (i <= len) {
            char currentChar = x[len - i];
            if (currentChar == '1') {
                res.set(i);
            } else {
                res[i] = 0;
            }
            i++;
        }
        return res;
    }

    PB square() {
        bitset<DDIMENSION> res;

        for (int i = 0; i < DIMENSION; ++i) {
            if (this->bits.test(i)) {
                res.set(i * 2);
            }
        }

        mod(res);

        return PB(cutToSmallerBitset(res));
    }

    void in_square() {
        PB square_result = this->square();
        this->bits = square_result.bits;
    }

    PB trace(const PB& pb){
        PB result = pb;
        PB currentSquaredPB = pb;
        int i = 1;

        do
        {
            currentSquaredPB = currentSquaredPB.square();
            result.bits ^= currentSquaredPB.bits;
            i++;
        } while (i < 233);

        return result;
    }

    PB inverse() const{
        PB result("1");
        int i = 1;

        while (i < DIMENSION)
        {
            result = result * (*this);
            result.in_square();
            i++;
        }

        return result;
    }

    PB power(const string& pow) {
        bitset<DIMENSION> c_pow = str(pow);
        PB result("1");

        int i = 232;
        while (i >= 0) {
            result = c_pow[i] ? result * *this : result;
            (i > 0) ? result.in_square() : void();
            --i;
        }

        return result;
    }

private:

};

template <typename Func>
void measureAndPrint(const string& label, Func func, const string& expectedOutput) {
    auto start = high_resolution_clock::now();
    PB result = func();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    string actualOutput = result.to_string();
    cout << label << ": " << actualOutput << endl;
    cout << "Result is " << (actualOutput == expectedOutput ? "correct" : "incorrect") << endl;
    cout << "Time: " << duration.count() << " microseconds" << endl << endl;
}

//bool checkPowerProperty(const PB& d) {
//    PB result = d;
//    result = result.power("233");
//    return result == PB("1");
//}



int main() {

    string a1 = "11010111010010111001011011110001000100110111101110001010010000111011111011110010000110001110111001011101000011110010010100111000111011110001010000101101111110100111100100100111100111011000010011110000100011001010100001111010001001101";
    string b1 = "11110100110111100010000101100010000010101100111000011100110101100000001010000101111000001010111011001111111110011111001011100111010100011100110111001100101001001011110001101011000000000101101110100111001111101110110111000111111001111";
    string N = "00100000100000010000010100110111101001111000110000000101010001101011010110001000100100110111100100000011110011011111010010111000010010111110010111101100101011001101101100011000101111011111100101011011001010010101111100110111111001101";
    cout << endl << "Input A: " << a1 << endl << endl;
    cout << "Input B: " << b1 << endl << endl;
    cout << "Input N: " << N << endl << endl;
    PB a(a1);
    PB b(b1);

    string expectedOutputAAddB = "100011100101011011011110010011000110011011010110010110100101011011110001110111111110000100000010010010111101101101011111011111101111101101100111100001010111101100010101001100100111011101111101010111101100100100010110111101110000010";
    string expectedOutputAMulB = "101100110111111010100100100010001100001000101011111101001001100100111101010100101100101000101101111011000000101101111000110110001011111001110000001010110011110100101111100001001111011100001101101000001000011001010001010001110010010";
    string expectedOutputTrA = "0";
    string expectedOutputASquare = "11101010010001000011111010110001010100001101000110001101000110011011101111011110011010010011000111001110101001010001000001010011100100001001001110000111111001001001011111101101101100010001011100000100110010010010011100001101101010000";
    string expectedOutputAPowN = "11111101001111001000000001100010000110111010110101001001111001010111000100000101000100010100001110101010010101110110110110001101001100011000101100111111011110011011100110100010001010110000100001011111100101011000100110111110110111000";
    string expectedOutputAInv = "1010000111111101101111101000101110101111001111101011111100011111111110101011110000100111100100010101101110101111100101010100001011011000011010000101100010111111011100110110100100000110011010011001010001001111001000010111000010110111";

    measureAndPrint("A + B", [&]() { return a + b; }, expectedOutputAAddB);
    measureAndPrint("A * B", [&]() { return a * b; }, expectedOutputAMulB);
    measureAndPrint("Tr(A)", [&]() { return a.trace(a); }, expectedOutputTrA);
    measureAndPrint("A^2", [&]() { return a.square(); }, expectedOutputASquare);
    measureAndPrint("A^N", [&]() { return a.power(N); }, expectedOutputAPowN);
    measureAndPrint("A^-1", [&]() { return a.inverse(); }, expectedOutputAInv);


//    PB d("101010111");
//    if (checkPowerProperty(d)) {
//        std::cout << "Power property test passed." << std::endl;
//    } else {
//        std::cout << "Power property test failed." << std::endl;
//    }

}