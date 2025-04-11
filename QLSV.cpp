#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

class SINHVIEN {  
protected:
    int MSSV;
    string Ten;
    string He;
    float OOP, VDK, KTMT;
    float DTB;

public:
    SINHVIEN() : MSSV(0), Ten(""), He(""), OOP(0), VDK(0), KTMT(0), DTB(0) {}

    SINHVIEN(int mssv, string ten, string he, float oop, float vdk, float ktmt)
        : MSSV(mssv), Ten(ten), He(he), OOP(oop), VDK(vdk), KTMT(ktmt), DTB(0) {
    }

    virtual ~SINHVIEN() {}

    virtual void Nhap() {
        cout << "MSSV: "; cin >> MSSV;
        cin.ignore();
        cout << "Ho va ten: "; 
        getline(cin, Ten);
        cout << "Diem OOP: "; 
        cin >> OOP;
        cout << "Diem VDK: "; 
        cin >> VDK;
        cout << "Diem KTMT: "; 
        cin >> KTMT;
    }

    virtual void Xuat() const {
        cout << "| " << setw(6) << MSSV << " | " << setw(12) << Ten << " | " << setw(6) << He << " | "
            << setw(6) << fixed << setprecision(1) << OOP << " | "
            << setw(6) << VDK << " | " << setw(6) << KTMT << " | ";
    }

    virtual float TinhDTB() {
        return 0.0;
    }

    virtual string ToCsvString() const {
        return "";
    }

    int LayMSSV() const { 
        return MSSV; }
    float LayDTB() const { 
        return DTB; }
    float LayOOP() const { 
        return OOP; }
    float LayVDK() const { 
        return VDK; }
    float LayKTMT() const { 
        return KTMT; }
    string LayTen() const { 
        return Ten; }
    string LayHe() const { 
        return He; }

    virtual float LayDiemMon(int mon) const {
        switch (mon) {
        case 1: return OOP;
        case 2: return VDK;
        case 3: return KTMT;
        default: return 0;
        }
    }

    void CapNhatDTB(float dtb) { DTB = dtb; }
};

class SV_CLC : public SINHVIEN {
private:
    float Matlab, DSP, TTS;

public:
    SV_CLC() : SINHVIEN(), Matlab(0), DSP(0), TTS(0) {}

    SV_CLC(int mssv, string ten, string he, float oop, float vdk, float ktmt, float matlab, float dsp, float tts)
        : SINHVIEN(mssv, ten, he, oop, vdk, ktmt), Matlab(matlab), DSP(dsp), TTS(tts) {
        DTB = TinhDTB();
    }

    void Nhap() override {
        SINHVIEN::Nhap();
        He = "CLC";
        cout << "Diem Matlab: "; 
        cin >> Matlab;
        cout << "Diem DSP: "; 
        cin >> DSP;
        cout << "Diem TTS: "; 
        cin >> TTS;
        DTB = TinhDTB();
    }

    void Xuat() const override {
        SINHVIEN::Xuat();
        cout << setw(6) << fixed << setprecision(1) << Matlab << " | "
            << setw(6) << DSP << " | " << setw(6) << TTS << " | "
            << setw(6) << "" << " | " << setw(6) << "" << " | "
            << setw(6) << DTB << " |";
    }

    float TinhDTB() override {
        float dtb = (OOP + VDK + KTMT + Matlab + DSP + TTS) / 6;
        return round(dtb * 10) / 10;
    }

    float LayDiemMon(int mon) const override {
        if (mon <= 3) return SINHVIEN::LayDiemMon(mon);
        switch (mon) {
        case 4: return Matlab;
        case 5: return DSP;
        case 6: return TTS;
        default: return 0;
        }
    }

    float LayMatlab() const { return Matlab; }
    float LayDSP() const { return DSP; }
    float LayTTS() const { return TTS; }

    string ToCsvString() const override {
        stringstream ss;
        ss << MSSV << "," << Ten << ",CLC," << OOP << "," << VDK << "," << KTMT << ","
            << Matlab << "," << DSP << "," << TTS << ",,," << DTB;
        return ss.str();
    }
};

class SV_DT : public SINHVIEN {
private:
    float Java, CSDL;

public:
    SV_DT() : SINHVIEN(), Java(0), CSDL(0) {}

    SV_DT(int mssv, string ten, string he, float oop, float vdk, float ktmt, float java, float csdl)
        : SINHVIEN(mssv, ten, he, oop, vdk, ktmt), Java(java), CSDL(csdl) {
        DTB = TinhDTB();
    }

    void Nhap() override {
        SINHVIEN::Nhap();
        He = "DT";
        cout << "Diem Java: "; 
        cin >> Java;
        cout << "Diem CSDL: "; 
        cin >> CSDL;
        DTB = TinhDTB();
    }

    void Xuat() const override {
        SINHVIEN::Xuat();
        cout << setw(6) << "" << " | " << setw(6) << "" << " | " << setw(6) << "" << " | "
            << setw(6) << fixed << setprecision(1) << Java << " | "
            << setw(6) << CSDL << " | "
            << setw(6) << DTB << " |";
    }

    float TinhDTB() override {
        float dtb = (OOP + VDK + KTMT + 2 * (Java + CSDL)) / 7;
        return round(dtb * 10) / 10;
    }

    float LayDiemMon(int mon) const override {
        if (mon <= 3) return SINHVIEN::LayDiemMon(mon);
        switch (mon) {
        case 7: return Java;
        case 8: return CSDL;
        default: return 0;
        }
    }

    float LayJava() const { return Java; }
    float LayCSDL() const { return CSDL; }

    string ToCsvString() const override {
        stringstream ss;
        ss << MSSV << "," << Ten << ",DT," << OOP << "," << VDK << "," << KTMT
            << ",,,," << Java << "," << CSDL << "," << DTB;
        return ss.str();
    }
};

class QuanLySV {
private:
    vector<SINHVIEN*> dsSV;

    void InTieuDe() const {
        cout << "| " << setw(6) << "MSSV" << " | " << setw(12) << "Ho va ten" << " | " << setw(6) << "He" << " | "
            << setw(6) << "OOP" << " | " << setw(6) << "VDK" << " | " << setw(6) << "KTMT" << " | "
            << setw(6) << "Matlab" << " | " << setw(6) << "DSP" << " | " << setw(6) << "TTS" << " | "
            << setw(6) << "Java" << " | " << setw(6) << "CSDL" << " | " << setw(6) << "DTB" << " |" << endl;
    }

public:
    ~QuanLySV() {
        for (auto sv : dsSV) {
            delete sv;
        }
    }

    void Nhap() {
        int n;
        cout << "Nhap so luong SV: "; 
        cin >> n;
        dsSV.clear();
        for (int i = 0; i < n; i++) {
            cout << "\nSinh vien " << i + 1 << ":\n";
            cout << "He (1-CLC, 2-DT): ";
            int he; cin >> he;
            if (he == 1) {
                SV_CLC* sv = new SV_CLC();
                sv->Nhap();
                dsSV.push_back(sv);
            }
            else {
                SV_DT* sv = new SV_DT();
                sv->Nhap();
                dsSV.push_back(sv);
            }
        }
    }

    void Xuat() const {
        if (dsSV.empty()) {
            cout << "Danh sach trong!\n";
            return;
        }
        InTieuDe();
        for (const auto& sv : dsSV) {
            sv->Xuat();
            cout << endl;
        }
    }

    void ChinhSua() {
        int mssv;
        cout << "Nhap MSSV can sua: "; 
        cin >> mssv;
        bool found = false;
        for (auto& sv : dsSV) {
            if (sv->LayMSSV() == mssv) {
                cout << "Nhap thong tin moi:\n";
                sv->Nhap();
                sv->CapNhatDTB(sv->TinhDTB());
                cout << "Da cap nhat!\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "Khong tim thay MSSV!\n";
    }

    void Them() {
        cout << "He (1-CLC, 2-DT): ";
        int he; cin >> he;
        if (he == 1) {
            SV_CLC* sv = new SV_CLC();
            sv->Nhap();
            dsSV.push_back(sv);
        }
        else {
            SV_DT* sv = new SV_DT();
            sv->Nhap();
            dsSV.push_back(sv);
        }
        cout << "Da them sinh vien!\n";
    }

    void TimDiemCaoThap() const {
        if (dsSV.empty()) {
            cout << "Danh sach trong!\n";
            return;
        }

        SINHVIEN* maxSV = dsSV[0];
        SINHVIEN* minSV = dsSV[0];
        for (const auto& sv : dsSV) {
            if (sv->LayDTB() > maxSV->LayDTB()) maxSV = sv;
            if (sv->LayDTB() < minSV->LayDTB()) minSV = sv;
        }

        cout << "SV diem cao nhat:\n";
        InTieuDe();
        maxSV->Xuat();
        cout << endl;

        cout << "SV diem thap nhat:\n";
        minSV->Xuat();
        cout << endl;
    }

    void SapXep() {
        if (dsSV.empty()) {
            cout << "Danh sach trong!\n";
            return;
        }

        int tieuChi, thuTu;
        cout << "Tieu chi (1-MSSV, 2-OOP, 3-VDK, 4-KTMT, 5-Matlab, 6-DSP, 7-TTS, 8-Java, 9-CSDL, 10-DTB): ";
        cin >> tieuChi;
        if (tieuChi < 1 || tieuChi > 10) {
            cout << "Tieu chi khong hop le!\n";
            return;
        }

        cout << "Thu tu (1-Tang, 2-Giam): ";
        cin >> thuTu;
        if (thuTu != 1 && thuTu != 2) {
            cout << "Thu tu khong hop le!\n";
            return;
        }

        // Hàm so sánh với kiểm tra đầy đủ mọi trường hợp
        auto compare = [tieuChi, thuTu](SINHVIEN* a, SINHVIEN* b) {
            bool tang = (thuTu == 1);
            float valueA, valueB;

            if (tieuChi == 1) { // MSSV
                return tang ? a->LayMSSV() < b->LayMSSV() : a->LayMSSV() > b->LayMSSV();
            }
            else if (tieuChi == 10) { // DTB
                valueA = a->LayDTB();
                valueB = b->LayDTB();
            }
            else { // Các môn học (2-9)
                // Chuyển tiêu chí từ 2-9 về 1-8 cho LayDiemMon
                int mon = tieuChi - 1;
                if (tieuChi >= 5) mon = tieuChi - 1; // Điều chỉnh cho Matlab(4), DSP(5), TTS(6), Java(7), CSDL(8)
                valueA = a->LayDiemMon(mon);
                valueB = b->LayDiemMon(mon);
            }

            // Nếu giá trị chính bằng nhau (bao gồm cả 0), so sánh MSSV
            if (valueA == valueB) {
                return tang ? a->LayMSSV() < b->LayMSSV() : a->LayMSSV() > b->LayMSSV();
            }
            return tang ? valueA < valueB : valueA > valueB;
            };

        sort(dsSV.begin(), dsSV.end(), compare);

        cout << "Danh sach sau khi sap xep:\n";
        Xuat();
    }

    void Xoa() {
        if (dsSV.empty()) {
            cout << "Danh sach trong!\n";
            return;
        }
        int mssv;
        cout << "Nhap MSSV can xoa: "; 
        cin >> mssv;
        for (auto it = dsSV.begin(); it != dsSV.end(); ++it) {
            if ((*it)->LayMSSV() == mssv) {
                delete* it;
                dsSV.erase(it);
                cout << "Da xoa!\n";
                return;
            }
        }
        cout << "Khong tim thay MSSV!\n";
    }

    void LietKeDiemMonTren5() {
        if (dsSV.empty()) {
            cout << "Danh sach trong!\n";
            return;
        }

        int mon;
        cout << "Chon mon (1-OOP, 2-VDK, 3-KTMT, 4-Matlab, 5-DSP, 6-TTS, 7-Java, 8-CSDL): ";
        cin >> mon;

        if (mon < 1 || mon > 8) {
            cout << "Mon khong hop le!\n";
            return;
        }

        InTieuDe();
        bool found = false;
        for (const auto& sv : dsSV) {
            if (sv->LayDiemMon(mon) >= 5) {
                sv->Xuat();
                cout << endl;
                found = true;
            }
        }
        if (!found) cout << "| Khong co SV nao thoa dieu kien!                                      |\n";
    }

    void LietKeDiemTBTren() {
        if (dsSV.empty()) {
            cout << "Danh sach trong!\n";
            return;
        }
        float x;
        cout << "Nhap diem TB can so sanh: "; 
        cin >> x;
        InTieuDe();
        bool found = false;
        for (const auto& sv : dsSV) {
            if (sv->LayDTB() >= x) {
                sv->Xuat();
                cout << endl;
                found = true;
            }
        }
        if (!found) cout << "| Khong co SV nao thoa dieu kien!                                      |\n";
    }

    void GhiFileCSV() {
        if (dsSV.empty()) {
            cout << "Danh sach trong!\n";
            return;
        }
        string tenFile;
        cout << "Nhap ten file: "; cin >> tenFile;
        ofstream outFile(tenFile + ".csv");
        outFile << "MSSV,Ho va ten,He,OOP,VDK,KTMT,Matlab,DSP,TTS,Java,CSDL,DTB\n";
        for (const auto& sv : dsSV) {
            outFile << sv->ToCsvString() << "\n";
        }
        outFile.close();
        cout << "Da ghi file " << tenFile << ".csv\n";
    }

    void DocFileCSV() {
        string tenFile;
        cout << "Nhap ten file: "; cin >> tenFile;
        ifstream inFile(tenFile);
        if (!inFile) {
            cout << "Khong mo duoc file!\n";
            return;
        }

        dsSV.clear();

        string line;
        getline(inFile, line); // Bỏ qua dòng tiêu đề
        while (getline(inFile, line)) {
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ',')) tokens.push_back(token);

            int mssv = stoi(tokens[0]);
            string ten = tokens[1];
            string he = tokens[2];
            float oop = stof(tokens[3]);
            float vdk = stof(tokens[4]);
            float ktmt = stof(tokens[5]);

            if (he == "CLC") {
                float matlab = stof(tokens[6]);
                float dsp = stof(tokens[7]);
                float tts = stof(tokens[8]);
                SV_CLC* sv = new SV_CLC(mssv, ten, he, oop, vdk, ktmt, matlab, dsp, tts);
                dsSV.push_back(sv);
            }
            else {
                float java = stof(tokens[9]);
                float csdl = stof(tokens[10]);
                SV_DT* sv = new SV_DT(mssv, ten, he, oop, vdk, ktmt, java, csdl);
                dsSV.push_back(sv);
            }
        }
        inFile.close();
        cout << "Da doc file!\n";
    }
};

class Menu {
private:
    QuanLySV qlsv;

public:
    void HienThi() {
        int luaChon;
        do {
            cout << "\n===== QUAN LY SINH VIEN =====\n";
            cout << "1. Nhap danh sach sinh vien\n";
            cout << "2. Xuat danh sach sinh vien\n";
            cout << "3. Chinh sua thong tin sinh vien\n";
            cout << "4. Them sinh vien moi\n";
            cout << "5. Tim SV diem cao nhat/thap nhat\n";
            cout << "6. Sap xep danh sach\n";
            cout << "7. Xoa sinh vien\n";
            cout << "8. Liet ke SV diem mon hoc >= 5\n";
            cout << "9. Liet ke SV diem TB >= X\n";
            cout << "10. Ghi danh sach SV ra file CSV\n";
            cout << "11. Doc danh sach SV tu file CSV\n";
            cout << "0. Thoat\n";
            cout << "Lua chon: "; cin >> luaChon;

            switch (luaChon) {
            case 1: qlsv.Nhap();
                break;
            case 2: qlsv.Xuat(); 
                break;
            case 3: qlsv.ChinhSua(); 
                break;
            case 4: qlsv.Them(); 
                break;
            case 5: qlsv.TimDiemCaoThap(); 
                break;
            case 6: qlsv.SapXep(); 
                break;
            case 7: qlsv.Xoa(); 
                break;
            case 8: qlsv.LietKeDiemMonTren5(); 
                break;
            case 9: qlsv.LietKeDiemTBTren(); 
                break;
            case 10: qlsv.GhiFileCSV(); 
                break;
            case 11: qlsv.DocFileCSV(); 
                break;
            case 0: cout << "Thoat\n"; 
                break;
            default: cout << "Lua chon khong hop le!\n";
            }
        } while (luaChon != 0);
    }
};

int main() {
    Menu menu;
    menu.HienThi();
    return 0;
}
