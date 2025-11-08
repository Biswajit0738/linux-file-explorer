// Linux Console File Explorer (C++17)
// Build: g++ -std=gnu++17 -O2 main.cpp -o filex
// Run:   ./filex

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <sys/stat.h>

namespace fs = std::filesystem;

static std::string color(const std::string& txt, const std::string& code) {
    return "\033[" + code + "m" + txt + "\033[0m";
}

static std::string perms_to_string(fs::perms p) {
    auto has = [&](fs::perms bit){ return (p & bit) != fs::perms::none; };
    std::string s;
    s += has(fs::perms::owner_read)  ? 'r' : '-';
    s += has(fs::perms::owner_write) ? 'w' : '-';
    s += has(fs::perms::owner_exec)  ? 'x' : '-';
    s += has(fs::perms::group_read)  ? 'r' : '-';
    s += has(fs::perms::group_write) ? 'w' : '-';
    s += has(fs::perms::group_exec)  ? 'x' : '-';
    s += has(fs::perms::others_read)  ? 'r' : '-';
    s += has(fs::perms::others_write) ? 'w' : '-';
    s += has(fs::perms::others_exec)  ? 'x' : '-';
    return s;
}

static std::string human_size(uintmax_t bytes) {
    const char* u[] = {"B","KB","MB","GB","TB"};
    int i = 0; double b = static_cast<double>(bytes);
    while (b >= 1024.0 && i < 4) { b /= 1024.0; ++i; }
    std::ostringstream os;
    os << std::fixed << std::setprecision(i==0?0:1) << b << u[i];
    return os.str();
}

static void list_dir(const fs::path& dir) {
    std::vector<fs::directory_entry> items;
    try {
        for (auto& de : fs::directory_iterator(dir)) items.push_back(de);
    } catch (const std::exception& e) {
        std::cerr << color(std::string("ls error: ") + e.what(), "31") << "\n";
        return;
    }
    std::sort(items.begin(), items.end(),
              [](const auto& a, const auto& b){
                  return a.path().filename().string() < b.path().filename().string();
              });
    std::cout << color("Index  Type  Perms     Size    Name", "1;37") << "\n";
    int idx = 0;
    for (auto& de : items) {
        std::string type = de.is_directory() ? color("dir","1;34")
                          : (de.is_symlink() ? color("lnk","1;36") : "file");
        std::string perms = perms_to_string(fs::status(de).permissions());
        std::string name  = de.path().filename().string();
        if (de.is_directory()) name += "/";
        std::string size   = de.is_regular_file()? human_size(de.file_size()) : "-";
        std::cout << std::setw(5) << idx++ << "  "
                  << std::setw(4) << type << "  "
                  << std::setw(9) << perms << "  "
                  << std::setw(7) << size  << "  "
                  << name << "\n";
    }
}

static void copy_path(const fs::path& src, const fs::path& dst) {
    try {
        if (fs::is_directory(src)) {
            fs::create_directories(dst);
            fs::copy(src, dst, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        } else {
            fs::create_directories(dst.parent_path());
            fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
        }
        std::cout << color("copied", "32") << "\n";
    } catch (const std::exception& e) {
        std::cerr << color(std::string("cp error: ") + e.what(), "31") << "\n";
    }
}

static void move_path(const fs::path& src, const fs::path& dst) {
    try {
        fs::create_directories(dst.parent_path());
        fs::rename(src, dst);
        std::cout << color("moved", "32") << "\n";
    } catch (...) {
        try {
            if (fs::is_directory(src)) {
                fs::create_directories(dst);
                fs::copy(src, dst, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
                fs::remove_all(src);
            } else {
                fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
                fs::remove(src);
            }
            std::cout << color("moved (copy-fallback)", "33") << "\n";
        } catch (const std::exception& e) {
            std::cerr << color(std::string("mv error: ") + e.what(), "31") << "\n";
        }
    }
}

static void remove_path(const fs::path& p) {
    try {
        if (fs::is_directory(p)) {
            auto n = fs::remove_all(p);
            std::cout << color("removed " + std::to_string(n) + " entries", "32") << "\n";
        } else {
            fs::remove(p);
            std::cout << color("removed", "32") << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << color(std::string("rm error: ") + e.what(), "31") << "\n";
    }
}

static void make_file(const fs::path& p) {
    try {
        fs::create_directories(p.parent_path());
        std::ofstream f(p, std::ios::app);
        if (!f) throw std::runtime_error("cannot create file");
        f.close(); // touching by write-open is enough for our case
        std::cout << color("touched", "32") << "\n";
    } catch (const std::exception& e) {
        std::cerr << color(std::string("touch error: ") + e.what(), "31") << "\n";
    }
}

static void make_dir(const fs::path& p) {
    try {
        fs::create_directories(p);
        std::cout << color("mkdir ok", "32") << "\n";
    } catch (const std::exception& e) {
        std::cerr << color(std::string("mkdir error: ") + e.what(), "31") << "\n";
    }
}

static void show_permissions(const fs::path& p) {
    try {
        auto st = fs::status(p);
        std::cout << p << " " << perms_to_string(st.permissions()) << "\n";
    } catch (const std::exception& e) {
        std::cerr << color(std::string("perm error: ") + e.what(), "31") << "\n";
    }
}

static void chmod_octal(const fs::path& p, const std::string& oct) {
    try {
        if (oct.size() < 3 || oct.size() > 4 || !std::all_of(oct.begin(), oct.end(), ::isdigit))
            throw std::runtime_error("use 3-4 digit octal, e.g., 755 or 0644");
        mode_t m = std::stoul(oct, nullptr, 8);
        if (::chmod(p.c_str(), m) != 0) throw std::runtime_error("chmod failed");
        std::cout << color("chmod ok", "32") << "\n";
    } catch (const std::exception& e) {
        std::cerr << color(std::string("chmod error: ") + e.what(), "31") << "\n";
    }
}

static void find_name(const fs::path& root, const std::string& needle, bool case_sensitive) {
    size_t count = 0;
    auto norm = [&](std::string s) {
        if (!case_sensitive) for (auto& c: s) c = std::tolower(c);
        return s;
    };
    std::string nneedle = norm(needle);
    try {
        for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied);
             it != fs::recursive_directory_iterator(); ++it) {
            std::string name = it->path().filename().string();
            if (norm(name).find(nneedle) != std::string::npos) {
                std::cout << it->path().string() << "\n";
                ++count;
            }
        }
        std::cerr << color("found " + std::to_string(count) + " match(es)", "36") << "\n";
    } catch (const std::exception& e) {
        std::cerr << color(std::string("find error: ") + e.what(), "31") << "\n";
    }
}

static void help() {
    std::cout << color("Commands:", "1;37")
              << R"( 
  ls [path]                 List directory (default: current)
  cd <path>                 Change directory
  pwd                       Print current directory
  cp <src> <dst>            Copy file/dir (recursive)
  mv <src> <dst>            Move/Rename (copy fallback across devices)
  rm <path>                 Remove file or directory (recursive)
  touch <path>              Create file (or update timestamp)
  mkdir <path>              Create directory/directories
  find <root> <name> [i]    Search by name; add 'i' for case-insensitive
  perm <path>               Show rwx permissions
  chmod <octal> <path>      Set permissions (e.g., 755 or 0644)
  open <index>              cd into item by index from last ls
  help                      Show this help
  exit                      Quit
)" ;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<fs::path> last_ls;

    auto do_ls = [&](const fs::path& p){
        last_ls.clear();
        std::vector<fs::directory_entry> items;
        try {
            for (auto& de : fs::directory_iterator(p)) items.push_back(de);
        } catch (const std::exception& e) {
            std::cerr << color(std::string("ls error: ") + e.what(), "31") << "\n";
            return;
        }
        std::sort(items.begin(), items.end(),
                  [](const auto& a, const auto& b){
                      return a.path().filename().string() < b.path().filename().string();
                  });
        std::cout << color("Index  Type  Perms     Size    Name", "1;37") << "\n";
        int idx = 0;
        for (auto& de : items) {
            last_ls.push_back(de.path());
            std::string type = de.is_directory() ? color("dir","1;34")
                              : (de.is_symlink() ? color("lnk","1;36") : "file");
            std::string perms = perms_to_string(fs::status(de).permissions());
            std::string name  = de.path().filename().string();
            if (de.is_directory()) name += "/";
            std::string size   = de.is_regular_file()? human_size(de.file_size()) : "-";
            std::cout << std::setw(5) << idx++ << "  "
                      << std::setw(4) << type << "  "
                      << std::setw(9) << perms << "  "
                      << std::setw(7) << size  << "  "
                      << name << "\n";
        }
    };

    std::cout << color("Linux File Explorer • C++17", "1;32") << "\n";
    help();

    std::string line;
    while (true) {
        std::cout << color("\nfilex ", "1;32") << color(fs::current_path().string(), "36") << color(" > ", "1;37");
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd; iss >> cmd;

        try {
            if (cmd == "ls") {
                std::string p; std::getline(iss >> std::ws, p);
                fs::path target = p.empty() ? fs::current_path() : fs::weakly_canonical(fs::current_path() / p);
                do_ls(target);
            } else if (cmd == "open") {
                int index; if (!(iss >> index)) { std::cerr << color("usage: open <index>", "31") << "\n"; continue; }
                if (index < 0 || index >= (int)last_ls.size()) { std::cerr << color("invalid index", "31") << "\n"; continue; }
                fs::path p = fs::weakly_canonical(last_ls[index]);
                if (fs::is_directory(p)) fs::current_path(p);
                else std::cerr << color("not a directory", "31") << "\n";
                do_ls(fs::current_path());
            } else if (cmd == "cd") {
                std::string p; std::getline(iss >> std::ws, p);
                if (p.empty()) { std::cerr << color("usage: cd <path>", "31") << "\n"; continue; }
                fs::current_path(fs::weakly_canonical(fs::current_path() / p));
            } else if (cmd == "pwd") {
                std::cout << fs::current_path().string() << "\n";
            } else if (cmd == "cp") {
                std::string s,d; iss >> s >> d;
                if (s.empty() || d.empty()) { std::cerr << color("usage: cp <src> <dst>", "31") << "\n"; continue; }
                copy_path(fs::weakly_canonical(fs::current_path()/s), fs::weakly_canonical(fs::current_path()/d));
            } else if (cmd == "mv") {
                std::string s,d; iss >> s >> d;
                if (s.empty() || d.empty()) { std::cerr << color("usage: mv <src> <dst>", "31") << "\n"; continue; }
                move_path(fs::weakly_canonical(fs::current_path()/s), fs::weakly_canonical(fs::current_path()/d));
            } else if (cmd == "rm") {
                std::string p; iss >> p; if (p.empty()) { std::cerr << color("usage: rm <path>", "31") << "\n"; continue; }
                remove_path(fs::weakly_canonical(fs::current_path()/p));
            } else if (cmd == "touch") {
                std::string p; iss >> p; if (p.empty()) { std::cerr << color("usage: touch <path>", "31") << "\n"; continue; }
                make_file(fs::weakly_canonical(fs::current_path()/p));
            } else if (cmd == "mkdir") {
                std::string p; iss >> p; if (p.empty()) { std::cerr << color("usage: mkdir <path>", "31") << "\n"; continue; }
                make_dir(fs::weakly_canonical(fs::current_path()/p));
            } else if (cmd == "perm") {
                std::string p; iss >> p; if (p.empty()) { std::cerr << color("usage: perm <path>", "31") << "\n"; continue; }
                show_permissions(fs::weakly_canonical(fs::current_path()/p));
            } else if (cmd == "chmod") {
                std::string oct, p; iss >> oct >> p; if (p.empty()) { std::cerr << color("usage: chmod <octal> <path>", "31") << "\n"; continue; }
                chmod_octal(fs::weakly_canonical(fs::current_path()/p), oct);
            } else if (cmd == "find") {
                std::string root, name, flag; iss >> root >> name >> flag; 
                if (name.empty()) { std::cerr << color("usage: find <root> <name> [i]", "31") << "\n"; continue; }
                bool cs = !(flag == "i");
                find_name(fs::weakly_canonical(fs::current_path()/root), name, cs);
            } else if (cmd == "help") {
                help();
            } else if (cmd == "exit" || cmd == "quit") {
                break;
            } else {
                std::cerr << color("unknown command — try 'help'", "31") << "\n";
            }
        } catch (const std::exception& e) {
            std::cerr << color(std::string("error: ") + e.what(), "31") << "\n";
        }
    }
    return 0;
}
