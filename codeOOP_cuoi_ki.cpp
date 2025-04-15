#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
using namespace std;

// ------------------------- Lớp Ngày tháng -------------------------
class NgayThangNam {
private:
    int ngay, thang, nam;

public:
    NgayThangNam(int n = 1, int t = 1, int na = 2000) : ngay(n), thang(t), nam(na) {}

    void Nhap() {
        cout << "Nhap ngay: "; cin >> ngay;
        cout << "Nhap thang: "; cin >> thang;
        cout << "Nhap nam: "; cin >> nam;
    }

    void Xuat() const {
        cout << setw(2) << setfill('0') << ngay << "/"
            << setw(2) << setfill('0') << thang << "/"
            << nam;
    }
};

// ------------------------- Lớp Giao dịch -------------------------
class GiaoDich {
private:
    string loaiGiaoDich;
    double soTien;
    string thoiGian;

public:
    GiaoDich(string loai, double tien, string time)
        : loaiGiaoDich(loai), soTien(tien), thoiGian(time) {
    }

    string ThongTin() const {
        return loaiGiaoDich + " - " + to_string(soTien) + " VND tại " + thoiGian;
    }
};

// ------------------------- Lớp Tài khoản -------------------------
class TaiKhoan {
protected:
    string soTaiKhoan;
    double soDu;
    NgayThangNam ngayMo;
    vector<GiaoDich> lichSu;

public:
    TaiKhoan(string soTK = "", double du = 0) : soTaiKhoan(soTK), soDu(du) {}

    virtual void RutTien(double tien) = 0;
    virtual void GuiTien(double tien) {
        soDu += tien;
        lichSu.emplace_back("Gui tien", tien, "Ngay hien tai");
    }

    virtual void TinhLai() = 0;
    virtual void XuatThongTin() const = 0;

    void ThemGiaoDich(const GiaoDich& gd) {
        lichSu.push_back(gd);
    }

    string LaySoTaiKhoan() const {
        return soTaiKhoan;
    }
};

// ------------------------- TK Tiết kiệm -------------------------
class TaiKhoanTietKiem : public TaiKhoan {
private:
    double laiSuat;

public:
    TaiKhoanTietKiem(string soTK, double du, double ls)
        : TaiKhoan(soTK, du), laiSuat(ls) {
    }

    void RutTien(double tien) override {
        if (soDu >= tien) {
            soDu -= tien;
            lichSu.emplace_back("Rut tien tiet kiem", tien, "Ngay hien tai");
        }
        else {
            cout << "Khong du tien de rut." << endl;
        }
    }

    void TinhLai() override {
        double lai = soDu * laiSuat / 100;
        soDu += lai;
        lichSu.emplace_back("Tinh lai tiet kiem", lai, "Ngay hien tai");
    }

    void XuatThongTin() const override {
        cout << "Tai khoan tiet kiem: " << soTaiKhoan << endl;
        cout << "So du: " << soDu << " VND" << endl;
        cout << "Lai suat: " << laiSuat << "%" << endl;
    }
};

// ------------------------- TK Thanh toán -------------------------
class TaiKhoanThanhToan : public TaiKhoan {
public:
    TaiKhoanThanhToan(string soTK, double du)
        : TaiKhoan(soTK, du) {
    }

    void RutTien(double tien) override {
        if (soDu >= tien) {
            soDu -= tien;
            lichSu.emplace_back("Rut tien thanh toan", tien, "Ngay hien tai");
        }
        else {
            cout << "Khong du tien de rut." << endl;
        }
    }

    void TinhLai() override {
        // Không tính lãi
    }

    void XuatThongTin() const override {
        cout << "Tai khoan thanh toan: " << soTaiKhoan << endl;
        cout << "So du: " << soDu << " VND" << endl;
    }
};

// ------------------------- Lớp Khách hàng -------------------------
class KhachHang {
private:
    string hoTen;
    string gioiTinh;
    NgayThangNam ngaySinh;
    string soCMND;
    string diaChi;
    string sdt;
    string email;
    vector<shared_ptr<TaiKhoan>> dsTaiKhoan;

public:
    void NhapThongTin() {
        cin.ignore(); // Xóa bộ đệm trước khi nhập chuỗi
        cout << "Nhap ho ten khach hang: ";
        getline(cin, hoTen);
        cout << "Nhap gioi tinh (Nam/Nu): ";
        getline(cin, gioiTinh);
        cout << "Nhap ngay sinh:\n";
        ngaySinh.Nhap();
        cin.ignore();
        cout << "Nhap so CMND/CCCD: ";
        getline(cin, soCMND);
        cout << "Nhap dia chi: ";
        getline(cin, diaChi);
        cout << "Nhap so dien thoai: ";
        getline(cin, sdt);
        cout << "Nhap email: ";
        getline(cin, email);
    }

    void ThemTaiKhoan(shared_ptr<TaiKhoan> tk) {
        if (tk != nullptr) {
            dsTaiKhoan.push_back(tk);
            cout << "Da them tai khoan " << tk->LaySoTaiKhoan() << " cho khach hang " << hoTen << endl;
        }
        else {
            cout << "Tai khoan khong hop le!" << endl;
        }
    }

    void XuatThongTin() const {
        cout << "\n=== THONG TIN KHACH HANG ===\n";
        cout << "Ho ten: " << hoTen << endl;
        cout << "Gioi tinh: " << gioiTinh << endl;
        cout << "Ngay sinh: ";
        ngaySinh.Xuat(); cout << endl;
        cout << "CMND/CCCD: " << soCMND << endl;
        cout << "Dia chi: " << diaChi << endl;
        cout << "So dien thoai: " << sdt << endl;
        cout << "Email: " << email << endl;
        cout << "So tai khoan: " << dsTaiKhoan.size() << endl;
        for (const auto& tk : dsTaiKhoan) {
            tk->XuatThongTin();
            cout << endl;
        }
    }

    string LayTen() const { return hoTen; }
};

// ------------------------- Quản lý ngân hàng -------------------------
class QuanLyNganHang {
private:
    vector<KhachHang> danhSachKH;

public:
    void ThemKhachHang() {
        KhachHang kh;
        kh.NhapThongTin();

        int n;
        cout << "Nhap so luong tai khoan muon tao: ";
        cin >> n;
        for (int i = 0; i < n; ++i) {
            int loai;
            cout << "\n1. Tai khoan tiet kiem\n2. Tai khoan thanh toan\nChon loai: ";
            cin >> loai;

            string stk;
            double tien;
            cout << "Nhap so tai khoan: ";
            cin >> stk;
            cout << "Nhap so du ban dau: ";
            cin >> tien;

            if (loai == 1) {
                double ls;
                cout << "Nhap lai suat (%): ";
                cin >> ls;
                kh.ThemTaiKhoan(make_shared<TaiKhoanTietKiem>(stk, tien, ls));
            }
            else {
                kh.ThemTaiKhoan(make_shared<TaiKhoanThanhToan>(stk, tien));
            }
        }

        danhSachKH.push_back(kh);
    }

    void TimKiemKhachHangTheoTen(const string& ten) const {
        for (const auto& kh : danhSachKH) {
            if (kh.LayTen() == ten) {
                kh.XuatThongTin();
                return;
            }
        }
        cout << "Khong tim thay khach hang ten: " << ten << endl;
    }

    void LietKeTatCaKhachHang() const {
        if (danhSachKH.empty()) {
            cout << "Chua co khach hang nao!" << endl;
            return;
        }
        for (const auto& kh : danhSachKH) {
            kh.XuatThongTin();
        }
    }
};

// ------------------------- Hàm chính -------------------------
int main() {
    QuanLyNganHang ql;
    int luaChon;

    do {
        cout << "\n=== MENU ===\n";
        cout << "1. Them khach hang moi\n";
        cout << "2. Tim khach hang theo ten\n";
        cout << "3. Liet ke tat ca khach hang\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        cin >> luaChon;

        switch (luaChon) {
        case 1:
            ql.ThemKhachHang();
            break;
        case 2: {
            string ten;
            cin.ignore();
            cout << "Nhap ten khach can tim: ";
            getline(cin, ten);
            ql.TimKiemKhachHangTheoTen(ten);
            break;
        }
        case 3:
            ql.LietKeTatCaKhachHang();
            break;
        case 0:
            cout << "Tam biet!" << endl;
            break;
        default:
            cout << "Lua chon khong hop le!" << endl;
        }

    } while (luaChon != 0);

    return 0;
}
