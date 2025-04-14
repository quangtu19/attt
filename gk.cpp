#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

//=============== THƯ VIỆN THUẬT TOÁN AES ===============
namespace AES {

    //---------- Các hằng số và bảng sử dụng trong AES ----------
    // Ví dụ: bảng S-box (chỉ để minh họa; cần cài đặt đầy đủ trong thực tế)
    const unsigned char AES_SBOX[256] = {
        // Giá trị mẫu, cần thay thế bảng S-box hoàn chỉnh
    };

    //---------- Các hàm nội bộ của AES ----------
    void SubBytes(std::vector<unsigned char>& trangThai) {
        // Ví dụ: thay thế mỗi byte bằng giá trị trong S-box
        for (size_t i = 0; i < trangThai.size(); ++i) {
            // Dùng bảng S-box nếu có, ví dụ:
            // trangThai[i] = AES_SBOX[trangThai[i]];
        }
    }

    void ShiftRows(std::vector<unsigned char>& trangThai) {
        // Cài đặt dịch hàng theo định nghĩa AES
        // Ví dụ: thay đổi vị trí các byte của các hàng thứ 2, 3, 4...
    }

    void MixColumns(std::vector<unsigned char>& trangThai) {
        // Cài đặt phép trộn cột (MixColumns) dựa trên phép nhân trong GF(2^8)
    }

    void AddRoundKey(std::vector<unsigned char>& trangThai, const std::vector<unsigned char>& keySlice) {
        // keySlice phải có kích thước bằng với trangThai (16 byte)
        for (size_t i = 0; i < trangThai.size(); ++i) {
            trangThai[i] ^= keySlice[i];
        }
    }

    // Hàm mở rộng khóa (Key Expansion) mẫu
    // Đối với AES-128, ta cần 11 khóa vòng, mỗi khóa 16 byte => tổng 176 byte
    std::vector<unsigned char> KeyExpansion(const std::vector<unsigned char>& khoaBanDau) {
        std::vector<unsigned char> khoaMoRong;
        // Với mục đích minh họa, nhân bản khóa ban đầu 11 lần
        for (int round = 0; round < 11; round++) {
            khoaMoRong.insert(khoaMoRong.end(), khoaBanDau.begin(), khoaBanDau.end());
        }
        return khoaMoRong;
    }

    // Hàm mã hóa một khối 16 byte (128-bit) với AES
    std::vector<unsigned char> EncryptBlock(const std::vector<unsigned char>& blockRong,
        const std::vector<unsigned char>& khoaBanDau) {
        std::vector<unsigned char> trangThai = blockRong;

        // Lấy khóa mở rộng với kích thước 11*16 = 176 byte
        std::vector<unsigned char> expandedKey = KeyExpansion(khoaBanDau);

        // Vòng thêm khóa ban đầu: dùng 16 byte đầu tiên
        AddRoundKey(trangThai, std::vector<unsigned char>(expandedKey.begin(), expandedKey.begin() + 16));

        // 9 vòng lặp xử lý
        for (int round = 1; round <= 9; round++) {
            SubBytes(trangThai);
            ShiftRows(trangThai);
            MixColumns(trangThai);
            // Lấy khóa vòng tương ứng (mỗi 16 byte)
            std::vector<unsigned char> roundKey(expandedKey.begin() + round * 16,
                expandedKey.begin() + (round + 1) * 16);
            AddRoundKey(trangThai, roundKey);
        }
        // Vòng cuối cùng (không MixColumns)
        SubBytes(trangThai);
        ShiftRows(trangThai);
        std::vector<unsigned char> finalRoundKey(expandedKey.begin() + 10 * 16,
            expandedKey.begin() + 11 * 16);
        AddRoundKey(trangThai, finalRoundKey);

        return trangThai;
    }

    // Hàm đảo ngược các bước của AES (dạng mẫu, cần cài đặt chi tiết)
    void InvSubBytes(std::vector<unsigned char>& trangThai) {
        // Thay thế byte theo bảng inverse S-box
    }

    void InvShiftRows(std::vector<unsigned char>& trangThai) {
        // Thực hiện dịch hàng ngược lại theo định nghĩa AES
    }

    void InvMixColumns(std::vector<unsigned char>& trangThai) {
        // Thực hiện đảo ngược phép trộn cột
    }

    std::vector<unsigned char> DecryptBlock(const std::vector<unsigned char>& blockMaHoa,
        const std::vector<unsigned char>& khoaBanDau) {
        std::vector<unsigned char> trangThai = blockMaHoa;
        std::vector<unsigned char> expandedKey = KeyExpansion(khoaBanDau);

        // Vòng cuối cùng của giải mã (không có bước MixColumns)
        AddRoundKey(trangThai, std::vector<unsigned char>(expandedKey.begin() + 10 * 16,
            expandedKey.begin() + 11 * 16));
        InvShiftRows(trangThai);
        InvSubBytes(trangThai);

        // 9 vòng giải mã đảo ngược
        for (int round = 9; round >= 1; round--) {
            // Sử dụng khóa vòng tương ứng
            std::vector<unsigned char> roundKey(expandedKey.begin() + round * 16,
                expandedKey.begin() + (round + 1) * 16);
            AddRoundKey(trangThai, roundKey);
            InvMixColumns(trangThai);
            InvShiftRows(trangThai);
            InvSubBytes(trangThai);
        }
        // Thêm khóa ban đầu (16 byte đầu tiên)
        AddRoundKey(trangThai, std::vector<unsigned char>(expandedKey.begin(),
            expandedKey.begin() + 16));

        return trangThai;
    }

    //---------- Hàm mã hóa/giải mã toàn bộ chuỗi văn bản ----------
    std::string Encrypt(const std::string& banRo, const std::vector<unsigned char>& khoa) {
        std::string banMaHoa;
        size_t doDai = banRo.size();
        // Xử lý chia nhỏ văn bản theo từng khối 16 byte, thêm padding '\0' nếu chưa đủ
        for (size_t i = 0; i < doDai; i += 16) {
            std::vector<unsigned char> block(16, 0);
            size_t soByte = std::min(static_cast<size_t>(16), doDai - i);
            std::copy(banRo.begin() + i, banRo.begin() + i + soByte, block.begin());
            std::vector<unsigned char> blockMaHoa = EncryptBlock(block, khoa);
            banMaHoa.append(blockMaHoa.begin(), blockMaHoa.end());
        }
        return banMaHoa;
    }

    std::string Decrypt(const std::string& banMaHoa, const std::vector<unsigned char>& khoa) {
        std::string banGiaiMa;
        size_t doDai = banMaHoa.size();
        // Với mã hóa AES, kích thước chuỗi mã hóa phải là bội số của 16
        if (doDai % 16 != 0) {
            std::cerr << "Lỗi: Dữ liệu mã hóa không hợp lệ (không chia hết cho 16)!\n";
            return "";
        }
        for (size_t i = 0; i < doDai; i += 16) {
            std::vector<unsigned char> block(16, 0);
            std::copy(banMaHoa.begin() + i, banMaHoa.begin() + i + 16, block.begin());
            std::vector<unsigned char> blockRo = DecryptBlock(block, khoa);
            banGiaiMa.append(blockRo.begin(), blockRo.end());
        }
        return banGiaiMa;
    }

} // end namespace AES

//=============== THƯ VIỆN THUẬT TOÁN DES ===============
namespace DES {
    // Tương tự, cài đặt DES sẽ được viết theo quy tắc, ví dụ sau:
    // (Các hàm dưới đây chỉ mang tính chất mẫu và cần hoàn thiện)
    std::vector<int> initialPermutation(const std::vector<int>& block64) {
        std::vector<int> blockDaHoanVi(64, 0);
        // Thực hiện hoán vị theo bảng IP (placeholder)
        return blockDaHoanVi;
    }

    std::vector<int> finalPermutation(const std::vector<int>& block64) {
        std::vector<int> blockDaHoanVi(64, 0);
        // Thực hiện hoán vị theo bảng IP-1 (placeholder)
        return blockDaHoanVi;
    }

    std::vector<int> expansion(const std::vector<int>& nuaBlock32) {
        std::vector<int> blockMoRong(48, 0);
        // Cài đặt mở rộng (placeholder)
        return blockMoRong;
    }

    std::vector<int> substitution(const std::vector<int>& block48) {
        std::vector<int> blockSauSBox(32, 0);
        // Áp dụng S-box (placeholder)
        return blockSauSBox;
    }

    std::vector<int> permutation(const std::vector<int>& block32) {
        std::vector<int> blockHoanVi(32, 0);
        // Cài đặt hoán vị (placeholder)
        return blockHoanVi;
    }

    std::vector< std::vector<int> > generateSubKeys(const std::vector<int>& khoa64) {
        std::vector< std::vector<int> > danhSachKhoa(16, std::vector<int>(48, 0));
        // Sinh khóa con từ khóa 64 bit (placeholder)
        return danhSachKhoa;
    }

    std::pair<std::vector<int>, std::vector<int>> DESRound(const std::vector<int>& trai, const std::vector<int>& phai, const std::vector<int>& khoaCon) {
        std::vector<int> phaiMoRong = expansion(phai);
        for (size_t i = 0; i < phaiMoRong.size(); ++i) {
            phaiMoRong[i] ^= khoaCon[i];
        }
        std::vector<int> sauSBox = substitution(phaiMoRong);
        std::vector<int> kqHoanVi = permutation(sauSBox);

        std::vector<int> phaiMoi(32, 0);
        for (size_t i = 0; i < trai.size(); ++i) {
            phaiMoi[i] = trai[i] ^ kqHoanVi[i];
        }
        return std::make_pair(phai, phaiMoi);
    }

    std::vector<int> EncryptBlock(const std::vector<int>& block64, const std::vector<int>& khoa64) {
        std::vector<int> blockDaHoanVi = initialPermutation(block64);
        std::vector<int> L(blockDaHoanVi.begin(), blockDaHoanVi.begin() + 32);
        std::vector<int> R(blockDaHoanVi.begin() + 32, blockDaHoanVi.end());
        std::vector< std::vector<int> > danhSachKhoa = generateSubKeys(khoa64);
        for (int vong = 0; vong < 16; vong++) {
            auto capMoi = DESRound(L, R, danhSachKhoa[vong]);
            L = R;
            R = capMoi.second;
        }
        std::vector<int> ketHop;
        ketHop.insert(ketHop.end(), R.begin(), R.end());
        ketHop.insert(ketHop.end(), L.begin(), L.end());
        std::vector<int> blockMaHoa = finalPermutation(ketHop);
        return blockMaHoa;
    }

    std::vector<int> DecryptBlock(const std::vector<int>& blockMaHoa, const std::vector<int>& khoa64) {
        std::vector<int> blockDaHoanVi = initialPermutation(blockMaHoa);
        std::vector<int> L(blockDaHoanVi.begin(), blockDaHoanVi.begin() + 32);
        std::vector<int> R(blockDaHoanVi.begin() + 32, blockDaHoanVi.end());
        std::vector< std::vector<int> > danhSachKhoa = generateSubKeys(khoa64);
        for (int vong = 15; vong >= 0; vong--) {
            auto capMoi = DESRound(L, R, danhSachKhoa[vong]);
            L = R;
            R = capMoi.second;
        }
        std::vector<int> ketHop;
        ketHop.insert(ketHop.end(), R.begin(), R.end());
        ketHop.insert(ketHop.end(), L.begin(), L.end());
        std::vector<int> banRo = finalPermutation(ketHop);
        return banRo;
    }

    std::string Encrypt(const std::string& banRo, const std::vector<int>& khoa64) {
        std::string banMaHoa;
        // Xử lý chia thành các block 64 bit và mã hóa từng block (placeholder)
        return banMaHoa;
    }

    std::string Decrypt(const std::string& banMaHoa, const std::vector<int>& khoa64) {
        std::string banGiaiMa;
        // Xử lý chia block và giải mã (placeholder)
        return banGiaiMa;
    }

} // end namespace DES

//=============== THƯ VIỆN XỬ LÝ FILE (FILE I/O) ===============
namespace FileIO {

    std::string docDuLieu(const std::string& duongDanFile) {
        std::ifstream tepDoc(duongDanFile);
        std::stringstream noiDung;
        if (tepDoc.is_open()) {
            noiDung << tepDoc.rdbuf();
            tepDoc.close();
        }
        return noiDung.str();
    }

    void ghiDuLieu(const std::string& duongDanFile, const std::string& noiDung) {
        std::ofstream tepGhi(duongDanFile);
        if (tepGhi.is_open()) {
            tepGhi << noiDung;
            tepGhi.close();
        }
    }

} // end namespace FileIO

//=============== HÀM MAIN ===============
int main() {
    std::string duongDanInput = "input.txt";
    std::string duongDanOutput = "output.txt";

    std::string noiDungInput = FileIO::docDuLieu(duongDanInput);
    std::istringstream docStream(noiDungInput);
    std::string cheDo;
    std::getline(docStream, cheDo); // Dòng đầu: chế độ xử lý

    std::string duLieu;
    std::getline(docStream, duLieu, '\0'); // Phần nội dung cần xử lý

    std::string ketQua;

    if (cheDo == "AES_ENCRYPT") {
        // Khóa mẫu 16 byte cho AES-128
        std::vector<unsigned char> khoaAES = { 'k','h','a','u','A','E','S','1','2','3','4','5','6','7','8','9' };
        ketQua = AES::Encrypt(duLieu, khoaAES);
    }
    else if (cheDo == "AES_DECRYPT") {
        std::vector<unsigned char> khoaAES = { 'k','h','a','u','A','E','S','1','2','3','4','5','6','7','8','9' };
        ketQua = AES::Decrypt(duLieu, khoaAES);
    }
    else if (cheDo == "DES_ENCRYPT") {
        std::vector<int> khoaDES = {
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1,
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1,
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1,
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1
        };
        ketQua = DES::Encrypt(duLieu, khoaDES);
    }
    else if (cheDo == "DES_DECRYPT") {
        std::vector<int> khoaDES = {
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1,
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1,
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1,
            1,0,1,0, 0,1,0,1, 1,0,1,0, 0,1,0,1
        };
        ketQua = DES::Decrypt(duLieu, khoaDES);
    }
    else {
        ketQua = "Chế độ xử lý không hợp lệ!";
    }

    FileIO::ghiDuLieu(duongDanOutput, ketQua);
    return 0;
}
