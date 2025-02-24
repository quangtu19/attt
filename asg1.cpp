#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;

static const int SIZE = 5;
char matrix5x5[SIZE][SIZE];

// Hàm t?o ma tr?n 5×5 t? khóa:
// - G?p 'J' thành 'I'
// - Lo?i b? ký t? trùng l?p
// - B? sung các ch? cái còn thi?u (A-Z, b? qua 'J')
void createMatrix(const string& key) {
    vector<bool> used(26, false);
    string letters;

    // Thêm ký t? c?a key (l?y ch? cái, chuy?n in hoa, g?p J->I)
    for (char c : key) {
        if (isalpha(c)) {
            c = toupper(c);
            if (c == 'J') c = 'I';
            if (!used[c - 'A']) {
                used[c - 'A'] = true;
                letters.push_back(c);
            }
        }
    }
    // Thêm các ch? cái còn thi?u
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;
        if (!used[c - 'A']) {
            used[c - 'A'] = true;
            letters.push_back(c);
        }
    }
    // Nh?i các ký t? vào ma tr?n 5×5 theo th? t? t? trái sang ph?i, trên xu?ng d??i
    int index = 0;
    for (int r = 0; r < SIZE; r++) {
        for (int cc = 0; cc < SIZE; cc++) {
            matrix5x5[r][cc] = letters[index++];
        }
    }
}

// Hàm tìm v? trí c?a ký t? trong ma tr?n
void findPosition(char c, int& row, int& col) {
    if (c == 'J') c = 'I';
    for (int r = 0; r < SIZE; r++) {
        for (int cc = 0; cc < SIZE; cc++) {
            if (matrix5x5[r][cc] == c) {
                row = r;
                col = cc;
                return;
            }
        }
    }
}

// Hàm x? lý b?n rõ:
// - Lo?i b? ký t? không ph?i ch?
// - Chuy?n thành ch? in hoa và g?p 'J' -> 'I'
// - Chèn 'X' gi?a các ký t? li?n k? gi?ng nhau
// - N?u ?? dài l?, thêm 'X' ? cu?i
string prepareText(const string& text) {
    string temp;
    for (char c : text) {
        if (isalpha(c)) {
            c = toupper(c);
            if (c == 'J') c = 'I';
            temp.push_back(c);
        }
    }
    string processed;
    for (size_t i = 0; i < temp.size(); i++) {
        processed.push_back(temp[i]);
        // N?u ký t? hi?n t?i b?ng ký t? ti?p theo, chèn 'X'
        if (i + 1 < temp.size() && temp[i] == temp[i + 1]) {
            processed.push_back('X');
        }
    }
    if (processed.size() % 2 != 0) {
        processed.push_back('X');
    }
    return processed;
}

// Hàm mã hoá m?t c?p ký t? theo quy t?c Playfair
string encryptPair(char a, char b) {
    int r1, c1, r2, c2;
    findPosition(a, r1, c1);
    findPosition(b, r2, c2);

    // N?u cùng hàng: l?y ký t? bên ph?i (v?i vòng l?p)
    if (r1 == r2) {
        c1 = (c1 + 1) % SIZE;
        c2 = (c2 + 1) % SIZE;
        return string{ matrix5x5[r1][c1], matrix5x5[r1][c2] };
    }
    // N?u cùng c?t: l?y ký t? bên d??i (v?i vòng l?p)
    else if (c1 == c2) {
        r1 = (r1 + 1) % SIZE;
        r2 = (r2 + 1) % SIZE;
        return string{ matrix5x5[r1][c1], matrix5x5[r2][c1] };
    }
    // N?u khác hàng, khác c?t: ??i c?t (t?o hình ch? nh?t)
    else {
        return string{ matrix5x5[r1][c2], matrix5x5[r2][c1] };
    }
}

// Hàm mã hoá toàn b? b?n rõ v?i khóa cho tr??c
string encrypt(const string& plaintext, const string& key) {
    createMatrix(key);
    string prepared = prepareText(plaintext);
    string ciphertext;
    for (size_t i = 0; i < prepared.size(); i += 2) {
        ciphertext += encryptPair(prepared[i], prepared[i + 1]);
    }
    return ciphertext;
}

int main() {
    // Khóa m?c ??nh là "NETWORK SECURITY"
    string key = "NETWORK SECURITY";
    string plaintext;

    cout << "Key: " << key << endl;
    cout << "Nhap ban ro: ";
    getline(cin, plaintext);

    string cipherText = encrypt(plaintext, key);
    cout << "Ban ma: " << cipherText << endl;

    return 0;
}
