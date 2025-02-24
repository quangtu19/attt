#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;

static const int SIZE = 5;
char matrix5x5[SIZE][SIZE];

// H�m t?o ma tr?n 5�5 t? kh�a:
// - G?p 'J' th�nh 'I'
// - Lo?i b? k� t? tr�ng l?p
// - B? sung c�c ch? c�i c�n thi?u (A-Z, b? qua 'J')
void createMatrix(const string& key) {
    vector<bool> used(26, false);
    string letters;

    // Th�m k� t? c?a key (l?y ch? c�i, chuy?n in hoa, g?p J->I)
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
    // Th�m c�c ch? c�i c�n thi?u
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;
        if (!used[c - 'A']) {
            used[c - 'A'] = true;
            letters.push_back(c);
        }
    }
    // Nh?i c�c k� t? v�o ma tr?n 5�5 theo th? t? t? tr�i sang ph?i, tr�n xu?ng d??i
    int index = 0;
    for (int r = 0; r < SIZE; r++) {
        for (int cc = 0; cc < SIZE; cc++) {
            matrix5x5[r][cc] = letters[index++];
        }
    }
}

// H�m t�m v? tr� c?a k� t? trong ma tr?n
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

// H�m x? l� b?n r�:
// - Lo?i b? k� t? kh�ng ph?i ch?
// - Chuy?n th�nh ch? in hoa v� g?p 'J' -> 'I'
// - Ch�n 'X' gi?a c�c k� t? li?n k? gi?ng nhau
// - N?u ?? d�i l?, th�m 'X' ? cu?i
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
        // N?u k� t? hi?n t?i b?ng k� t? ti?p theo, ch�n 'X'
        if (i + 1 < temp.size() && temp[i] == temp[i + 1]) {
            processed.push_back('X');
        }
    }
    if (processed.size() % 2 != 0) {
        processed.push_back('X');
    }
    return processed;
}

// H�m m� ho� m?t c?p k� t? theo quy t?c Playfair
string encryptPair(char a, char b) {
    int r1, c1, r2, c2;
    findPosition(a, r1, c1);
    findPosition(b, r2, c2);

    // N?u c�ng h�ng: l?y k� t? b�n ph?i (v?i v�ng l?p)
    if (r1 == r2) {
        c1 = (c1 + 1) % SIZE;
        c2 = (c2 + 1) % SIZE;
        return string{ matrix5x5[r1][c1], matrix5x5[r1][c2] };
    }
    // N?u c�ng c?t: l?y k� t? b�n d??i (v?i v�ng l?p)
    else if (c1 == c2) {
        r1 = (r1 + 1) % SIZE;
        r2 = (r2 + 1) % SIZE;
        return string{ matrix5x5[r1][c1], matrix5x5[r2][c1] };
    }
    // N?u kh�c h�ng, kh�c c?t: ??i c?t (t?o h�nh ch? nh?t)
    else {
        return string{ matrix5x5[r1][c2], matrix5x5[r2][c1] };
    }
}

// H�m m� ho� to�n b? b?n r� v?i kh�a cho tr??c
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
    // Kh�a m?c ??nh l� "NETWORK SECURITY"
    string key = "NETWORK SECURITY";
    string plaintext;

    cout << "Key: " << key << endl;
    cout << "Nhap ban ro: ";
    getline(cin, plaintext);

    string cipherText = encrypt(plaintext, key);
    cout << "Ban ma: " << cipherText << endl;

    return 0;
}
