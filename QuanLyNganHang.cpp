#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

struct NgayThangNam {
    int ngay, thang, nam;

    void Nhap(const string& moTa = "") {
        if (!moTa.empty()) cout << moTa << " (dd mm yyyy): ";
        cin >> ngay >> thang >> nam;
    }

    string ToString() const {
        ostringstream oss;
        oss << setw(2) << setfill('0') << ngay << "/"
            << setw(2) << setfill('0') << thang << "/"
            << nam;
        return oss.str();
    }

    static NgayThangNam TuChuoi(const string& str) {
        NgayThangNam d;
        sscanf(str.c_str(), "%d/%d/%d", &d.ngay, &d.thang, &d.nam);
        return d;
    }
};

struct GiaoDich {
    string loai;
    double soTien;
    time_t thoiGian;

    GiaoDich(string loai, double tien)
        : loai(loai), soTien(tien) {
        thoiGian = time(nullptr);
    }

    string ToString() const {
        char buf[80];
        strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", localtime(&thoiGian));
        return loai + " - " + to_string(soTien) + " VND - " + buf;
    }
};

class TaiKhoan {
protected:
    int soTK;
    string tenChuTK;
    NgayThangNam ngaySinh, ngayDangKy;
    double soDu;
    vector<GiaoDich> lichSuGD;

public:
    TaiKhoan(int so = 0, string ten = "", double du = 0)
        : soTK(so), tenChuTK(ten), soDu(du) {}

    virtual ~TaiKhoan() {}

    virtual void NhapThongTin() {
        cout << "Nhap so TK: "; cin >> soTK;
        cin.ignore();
        cout << "Nhap ten chu TK: "; getline(cin, tenChuTK);
        ngaySinh.Nhap("Ngay sinh");
        ngayDangKy.Nhap("Ngay dang ky");
        cout << "Nhap so du: "; cin >> soDu;
    }

    virtual void TinhLai() = 0;
    virtual string LoaiTK() const = 0;
    virtual void XuatCSV(ofstream& fout) const = 0;

    virtual void Xuat() const {
        cout << setw(8) << soTK << " | "
             << setw(20) << tenChuTK << " | "
             << setw(12) << ngaySinh.ToString() << " | "
             << setw(15) << ngayDangKy.ToString() << " | "
             << setw(12) << fixed << setprecision(2) << soDu << " | "
             << LoaiTK();
    }

    void GuiTien(double tien) {
        if (tien > 0) {
            soDu += tien;
            lichSuGD.emplace_back("Gui tien", tien);
        }
    }

    bool RutTien(double tien) {
        if (tien <= soDu) {
            soDu -= tien;
            lichSuGD.emplace_back("Rut tien", tien);
            return true;
        }
        return false;
    }

    void XemLichSuGD() const {
        if (lichSuGD.empty()) cout << "Khong co giao dich.\n";
        else for (const auto& gd : lichSuGD) cout << "- " << gd.ToString() << "\n";
    }

    int LaySoTK() const { return soTK; }
    string LayTenChuTK() const { return tenChuTK; }
    NgayThangNam LayNgaySinh() const { return ngaySinh; }
    NgayThangNam LayNgayDangKy() const { return ngayDangKy; }
    double LaySoDu() const { return soDu; }

    void SetNgaySinh(const NgayThangNam& d) { ngaySinh = d; }
    void SetNgayDangKy(const NgayThangNam& d) { ngayDangKy = d; }
};

class TaiKhoanTietKiem : public TaiKhoan {
    double laiSuat;

public:
    TaiKhoanTietKiem(int so = 0, string ten = "", double du = 0, double lai = 0)
        : TaiKhoan(so, ten, du), laiSuat(lai) {}

    void NhapThongTin() override {
        TaiKhoan::NhapThongTin();
        cout << "Nhap lai suat (%): "; cin >> laiSuat;
    }

    void TinhLai() override {
        double tienLai = soDu * laiSuat / 100;
        soDu += tienLai;
        lichSuGD.emplace_back("Tinh lai", tienLai);
    }

    string LoaiTK() const override { return "TietKiem"; }

    void Xuat() const override {
        TaiKhoan::Xuat();
        cout << " | " << laiSuat << "%\n";
    }

    void XuatCSV(ofstream& fout) const override {
        fout << "TietKiem," << soTK << "," << tenChuTK << ","
             << ngaySinh.ToString() << "," << ngayDangKy.ToString() << ","
             << soDu << "," << laiSuat << "\n";
    }
};

class TaiKhoanThanhToan : public TaiKhoan {
public:
    TaiKhoanThanhToan(int so = 0, string ten = "", double du = 0)
        : TaiKhoan(so, ten, du) {}

    void TinhLai() override {}

    string LoaiTK() const override { return "ThanhToan"; }

    void Xuat() const override {
        TaiKhoan::Xuat();
        cout << "\n";
    }

    void XuatCSV(ofstream& fout) const override {
        fout << "ThanhToan," << soTK << "," << tenChuTK << ","
             << ngaySinh.ToString() << "," << ngayDangKy.ToString() << ","
             << soDu << ",\n";
    }
};

class QuanLyTaiKhoan {
    vector<shared_ptr<TaiKhoan>> ds;

public:
    void ThemTaiKhoan() {
        int loai;
        cout << "1. Tiet kiem\n2. Thanh toan\nChon loai: "; cin >> loai;
        shared_ptr<TaiKhoan> tk = (loai == 1) ? make_shared<TaiKhoanTietKiem>() : make_shared<TaiKhoanThanhToan>();
        tk->NhapThongTin();
        ds.push_back(tk);
    }

    void HienThiDS() const {
        cout << left << setw(8) << "So TK" << " | "
             << setw(20) << "Chu TK" << " | "
             << setw(12) << "Ngay Sinh" << " | "
             << setw(15) << "Dang Ky" << " | "
             << setw(12) << "So Du" << " | Loai TK\n";
        cout << string(85, '-') << "\n";
        for (const auto& tk : ds) tk->Xuat();
    }

    void GuiTien() {
        int soTK; double tien;
        cout << "Nhap so TK: "; cin >> soTK;
        cout << "Nhap tien: "; cin >> tien;
        for (auto& tk : ds) if (tk->LaySoTK() == soTK) { tk->GuiTien(tien); return; }
        cout << "Khong tim thay!\n";
    }

    void RutTien() {
        int soTK; double tien;
        cout << "Nhap so TK: "; cin >> soTK;
        cout << "Nhap tien: "; cin >> tien;
        for (auto& tk : ds) {
            if (tk->LaySoTK() == soTK) {
                if (!tk->RutTien(tien)) cout << "So du khong du!\n";
                return;
            }
        }
        cout << "Khong tim thay!\n";
    }

    void TinhLaiChoTatCa() {
        for (auto& tk : ds) tk->TinhLai();
    }

    void XemLichSuGD() {
        int soTK;
        cout << "Nhap so TK: "; cin >> soTK;
        for (auto& tk : ds)
            if (tk->LaySoTK() == soTK) { tk->XemLichSuGD(); return; }
        cout << "Khong tim thay!\n";
    }

    void DocCSV(const string& tenFile) {
        ifstream fin(tenFile);
        if (!fin) { cout << "Khong mo duoc file!\n"; return; }
        string line; getline(fin, line); // tiêu đề
        while (getline(fin, line)) {
            stringstream ss(line);
            string loai, soTK_str, ten, ns, dk, du_str, lai_str;
            getline(ss, loai, ',');
            getline(ss, soTK_str, ',');
            getline(ss, ten, ',');
            getline(ss, ns, ',');
            getline(ss, dk, ',');
            getline(ss, du_str, ',');
            getline(ss, lai_str, ',');

            int soTK = stoi(soTK_str);
            double du = stod(du_str);
            NgayThangNam ngs = NgayThangNam::TuChuoi(ns);
            NgayThangNam ngdk = NgayThangNam::TuChuoi(dk);

            shared_ptr<TaiKhoan> tk;
            if (loai == "TietKiem") {
                double lai = stod(lai_str);
                auto t = make_shared<TaiKhoanTietKiem>(soTK, ten, du, lai);
                t->SetNgaySinh(ngs); t->SetNgayDangKy(ngdk);
                tk = t;
            } else {
                auto t = make_shared<TaiKhoanThanhToan>(soTK, ten, du);
                t->SetNgaySinh(ngs); t->SetNgayDangKy(ngdk);
                tk = t;
            }
            ds.push_back(tk);
        }
        cout << "Doc file CSV thanh cong!\n";
    }

    void GhiCSV(const string& tenFile) {
        ofstream fout(tenFile);
        fout << "Loai,SoTK,TenChuTK,NgaySinh,NgayDangKy,SoDu,LaiSuat\n";
        for (const auto& tk : ds) tk->XuatCSV(fout);
        cout << "Ghi file CSV thanh cong!\n";
    }
};

int main() {
    QuanLyTaiKhoan ql;
    int chon;
    do {
        cout << "\n==== MENU ====\n";
        cout << "1. Them tai khoan\n2. Hien thi danh sach\n3. Gui tien\n4. Rut tien\n";
        cout << "5. Tinh lai\n6. Xem lich su GD\n7. Doc CSV\n8. Ghi CSV\n0. Thoat\n";
        cout << "Chon: "; cin >> chon;
        switch (chon) {
            case 1: ql.ThemTaiKhoan(); break;
            case 2: ql.HienThiDS(); break;
            case 3: ql.GuiTien(); break;
            case 4: ql.RutTien(); break;
            case 5: ql.TinhLaiChoTatCa(); break;
            case 6: ql.XemLichSuGD(); break;
            case 7: {
                string f; cout << "Nhap file CSV: "; cin >> f;
                ql.DocCSV(f);
                break;
            }
            case 8: {
                string f; cout << "Nhap file CSV: "; cin >> f;
                ql.GhiCSV(f);
                break;
            }
        }
    } while (chon != 0);
    return 0;
}
