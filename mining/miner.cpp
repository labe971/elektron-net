/**
 * Standalone CPU miner for Elektron Net (C++).
 *
 * Zero external dependencies:
 *   - Windows: uses WinHTTP (system API)
 *   - Linux/macOS: uses POSIX sockets
 *
 * Build:
 *   cmake -B build -S . && cmake --build build --config Release
 * Or directly with MSVC:
 *   cl /O2 /EHsc /std:c++20 miner.cpp /Fe:elektron_miner.exe
 */

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#else
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif

// ---------------------------------------------------------------------------
// SHA-256 (public domain implementation)
// ---------------------------------------------------------------------------
struct SHA256_CTX {
    uint32_t state[8];
    uint64_t bitcount;
    uint8_t buffer[64];
    size_t buffer_used;
};

static const uint32_t K256[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static void sha256_transform(SHA256_CTX *ctx, const uint8_t data[64]) {
    uint32_t a,b,c,d,e,f,g,h,t1,t2;
    uint32_t w[64];
    for (int i=0;i<16;i++) {
        w[i] = ((uint32_t)data[i*4]<<24)|((uint32_t)data[i*4+1]<<16)|((uint32_t)data[i*4+2]<<8)|((uint32_t)data[i*4+3]);
    }
    for (int i=16;i<64;i++) {
        uint32_t s0 = (w[i-15]>>7|w[i-15]<<25)^(w[i-15]>>18|w[i-15]<<14)^(w[i-15]>>3);
        uint32_t s1 = (w[i-2]>>17|w[i-2]<<15)^(w[i-2]>>19|w[i-2]<<13)^(w[i-2]>>10);
        w[i] = w[i-16]+s0+w[i-7]+s1;
    }
    a=ctx->state[0]; b=ctx->state[1]; c=ctx->state[2]; d=ctx->state[3];
    e=ctx->state[4]; f=ctx->state[5]; g=ctx->state[6]; h=ctx->state[7];
    for (int i=0;i<64;i++) {
        uint32_t S1 = (e>>6|e<<26)^(e>>11|e<<21)^(e>>25|e<<7);
        t1 = h + S1 + ((e&f)^((~e)&g)) + K256[i] + w[i];
        uint32_t S0 = (a>>2|a<<30)^(a>>13|a<<19)^(a>>22|a<<10);
        t2 = S0 + ((a&b)^(a&c)^(b&c));
        h=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
    }
    ctx->state[0]+=a; ctx->state[1]+=b; ctx->state[2]+=c; ctx->state[3]+=d;
    ctx->state[4]+=e; ctx->state[5]+=f; ctx->state[6]+=g; ctx->state[7]+=h;
}

static void sha256_init(SHA256_CTX *ctx) {
    ctx->state[0]=0x6a09e667; ctx->state[1]=0xbb67ae85; ctx->state[2]=0x3c6ef372; ctx->state[3]=0xa54ff53a;
    ctx->state[4]=0x510e527f; ctx->state[5]=0x9b05688c; ctx->state[6]=0x1f83d9ab; ctx->state[7]=0x5be0cd19;
    ctx->bitcount=0; ctx->buffer_used=0;
}

static void sha256_update(SHA256_CTX *ctx, const uint8_t *data, size_t len) {
    for (size_t i=0;i<len;i++) {
        ctx->buffer[ctx->buffer_used++] = data[i];
        ctx->bitcount += 8;
        if (ctx->buffer_used == 64) {
            sha256_transform(ctx, ctx->buffer);
            ctx->buffer_used = 0;
        }
    }
}

static void sha256_final(SHA256_CTX *ctx, uint8_t hash[32]) {
    uint64_t bitcount = ctx->bitcount;
    size_t padlen = (ctx->buffer_used < 56) ? (56 - ctx->buffer_used) : (120 - ctx->buffer_used);
    uint8_t pad[1] = {0x80};
    sha256_update(ctx, pad, 1);
    uint8_t zero[64] = {0};
    if (padlen > 1) sha256_update(ctx, zero, padlen-1);
    uint8_t lenbuf[8];
    for (int i=0;i<8;i++) lenbuf[i] = (uint8_t)(bitcount >> (56-i*8));
    sha256_update(ctx, lenbuf, 8);
    for (int i=0;i<8;i++) {
        hash[i*4]   = (uint8_t)(ctx->state[i]>>24);
        hash[i*4+1] = (uint8_t)(ctx->state[i]>>16);
        hash[i*4+2] = (uint8_t)(ctx->state[i]>>8);
        hash[i*4+3] = (uint8_t)(ctx->state[i]);
    }
}

static void sha256(const uint8_t *data, size_t len, uint8_t hash[32]) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, len);
    sha256_final(&ctx, hash);
}

static void sha256d(const uint8_t *data, size_t len, uint8_t out[32]) {
    uint8_t hash1[32];
    sha256(data, len, hash1);
    sha256(hash1, 32, out);
}

// ---------------------------------------------------------------------------
// Base64
// ---------------------------------------------------------------------------
static std::string base64_encode(const std::string &in) {
    static const char *b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(b[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(b[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

// ---------------------------------------------------------------------------
// Hex helpers
// ---------------------------------------------------------------------------
static std::string hex_encode(const uint8_t *data, size_t len) {
    static const char *hex = "0123456789abcdef";
    std::string out;
    out.reserve(len * 2);
    for (size_t i = 0; i < len; ++i) {
        out.push_back(hex[data[i] >> 4]);
        out.push_back(hex[data[i] & 0xf]);
    }
    return out;
}
static std::string hex_encode(const std::vector<uint8_t> &data) {
    return hex_encode(data.data(), data.size());
}
static std::vector<uint8_t> hex_decode(const std::string &hex) {
    std::vector<uint8_t> out;
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        std::string byte = hex.substr(i, 2);
        out.push_back(static_cast<uint8_t>(std::stoul(byte, nullptr, 16)));
    }
    return out;
}

// ---------------------------------------------------------------------------
// JSON helpers (minimal)
// ---------------------------------------------------------------------------
static std::string json_rpc(const std::string &method, const std::vector<std::string> &params) {
    std::ostringstream oss;
    oss << "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"" << method << "\",\"params\":[";
    for (size_t i = 0; i < params.size(); ++i) {
        if (i) oss << ",";
        oss << params[i];
    }
    oss << "]}";
    return oss.str();
}

static std::string extract_json_string(const std::string &json, const std::string &key) {
    std::string quoted = "\"" + key + "\"";
    size_t pos = json.find(quoted);
    if (pos == std::string::npos) return "";
    pos = json.find('"', pos + quoted.size());
    if (pos == std::string::npos) return "";
    size_t end = json.find('"', pos + 1);
    if (end == std::string::npos) return "";
    return json.substr(pos + 1, end - pos - 1);
}

static int64_t extract_json_int(const std::string &json, const std::string &key) {
    std::string quoted = "\"" + key + "\"";
    size_t pos = json.find(quoted);
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos + quoted.size());
    if (pos == std::string::npos) return 0;
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) ++pos;
    size_t end = pos;
    while (end < json.size() && (json[end] == '-' || (json[end] >= '0' && json[end] <= '9'))) ++end;
    try {
        return std::stoll(json.substr(pos, end - pos));
    } catch (...) {
        return 0;
    }
}

// ---------------------------------------------------------------------------
// HTTP (cross-platform)
// ---------------------------------------------------------------------------
#if defined(_WIN32)
static std::string http_post(const std::string &url, const std::string &user, const std::string &pass, const std::string &payload) {
    std::string u = url;
    std::string path = "/";
    if (u.rfind("http://", 0) == 0) u = u.substr(7);
    else if (u.rfind("https://", 0) == 0) u = u.substr(8);
    size_t slash = u.find('/');
    if (slash != std::string::npos) {
        path = u.substr(slash);
        u = u.substr(0, slash);
    }
    std::string host = u;
    INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT;
    size_t colon = host.find(':');
    if (colon != std::string::npos) {
        port = static_cast<INTERNET_PORT>(std::stoi(host.substr(colon + 1)));
        host = host.substr(0, colon);
    }

    HINTERNET hSession = WinHttpOpen(L"ElektronMiner/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) throw std::runtime_error("WinHttpOpen failed");

    std::wstring whost(host.begin(), host.end());
    HINTERNET hConnect = WinHttpConnect(hSession, whost.c_str(), port, 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); throw std::runtime_error("WinHttpConnect failed"); }

    std::wstring wpath(path.begin(), path.end());
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", wpath.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); throw std::runtime_error("WinHttpOpenRequest failed"); }

    std::string creds = user + ":" + pass;
    std::string auth = "Authorization: Basic " + base64_encode(creds) + "\r\n";
    std::string headers = "Content-Type: application/json\r\n" + auth;
    std::wstring wheaders(headers.begin(), headers.end());

    BOOL sent = WinHttpSendRequest(hRequest, wheaders.c_str(), (DWORD)headers.size(), (LPVOID)payload.data(), (DWORD)payload.size(), (DWORD)payload.size(), 0);
    if (!sent) {
        WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
        throw std::runtime_error("WinHttpSendRequest failed");
    }

    WinHttpReceiveResponse(hRequest, NULL);
    std::string response;
    DWORD size = 0;
    do {
        DWORD downloaded = 0;
        WinHttpQueryDataAvailable(hRequest, &size);
        if (!size) break;
        std::vector<char> buf(size);
        WinHttpReadData(hRequest, buf.data(), size, &downloaded);
        response.append(buf.data(), downloaded);
    } while (size > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return response;
}
#else
static std::string http_post(const std::string &url, const std::string &user, const std::string &pass, const std::string &payload) {
    std::string u = url;
    std::string path = "/";
    if (u.rfind("http://", 0) == 0) u = u.substr(7);
    else if (u.rfind("https://", 0) == 0) u = u.substr(8);
    size_t slash = u.find('/');
    if (slash != std::string::npos) {
        path = u.substr(slash);
        u = u.substr(0, slash);
    }
    std::string host = u;
    int port = 80;
    size_t colon = host.find(':');
    if (colon != std::string::npos) {
        port = std::stoi(host.substr(colon + 1));
        host = host.substr(0, colon);
    }

    struct addrinfo hints = {}, *res = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0)
        throw std::runtime_error("getaddrinfo failed");

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) { freeaddrinfo(res); throw std::runtime_error("socket failed"); }
    if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
        close(sock); freeaddrinfo(res);
        throw std::runtime_error("connect failed");
    }
    freeaddrinfo(res);

    std::string creds = user + ":" + pass;
    std::string auth = base64_encode(creds);
    std::string request = "POST " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Authorization: Basic " + auth + "\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + std::to_string(payload.size()) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += payload;

    if (send(sock, request.data(), request.size(), 0) < 0) {
        close(sock);
        throw std::runtime_error("send failed");
    }

    std::string response;
    char buffer[4096];
    ssize_t n;
    while ((n = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, static_cast<size_t>(n));
    }
    close(sock);

    size_t pos = response.find("\r\n\r\n");
    if (pos == std::string::npos) pos = response.find("\n\n");
    if (pos != std::string::npos)
        return response.substr(pos + ((response[pos] == '\r') ? 4 : 2));
    return response;
}
#endif

// ---------------------------------------------------------------------------
// RPC client
// ---------------------------------------------------------------------------
class RpcClient {
public:
    std::string url, user, password;
    RpcClient(const std::string &u, const std::string &usr, const std::string &pwd)
        : url(u), user(usr), password(pwd) {}
    std::string call(const std::string &method, const std::vector<std::string> &params = {}) {
        std::string payload = json_rpc(method, params);
        return http_post(url, user, password, payload);
    }
};

// ---------------------------------------------------------------------------
// Bech32
// ---------------------------------------------------------------------------
static int bech32_polymod(const std::vector<int> &values) {
    const int GEN[5] = {0x3b6a57b2, 0x26508e6d, 0x1ea119fa, 0x3d4233dd, 0x2a1462b3};
    int chk = 1;
    for (int v : values) {
        int b = chk >> 25;
        chk = ((chk & 0x1ffffff) << 5) ^ v;
        for (int i = 0; i < 5; ++i) chk ^= GEN[i] * ((b >> i) & 1);
    }
    return chk;
}

static std::vector<int> bech32_hrp_expand(const std::string &hrp) {
    std::vector<int> ret;
    for (char c : hrp) ret.push_back(static_cast<int>(c) >> 5);
    ret.push_back(0);
    for (char c : hrp) ret.push_back(static_cast<int>(c) & 31);
    return ret;
}

static int bech32_verify_checksum(const std::string &hrp, const std::vector<int> &data) {
    std::vector<int> values = bech32_hrp_expand(hrp);
    values.insert(values.end(), data.begin(), data.end());
    int polymod = bech32_polymod(values);
    if (polymod == 1) return 0;
    if (polymod == 0x2bc830a3) return 1;
    return -1;
}

static std::vector<int> bech32_convert_bits(const std::vector<int> &data, int from_bits, int to_bits, bool pad = true) {
    int acc = 0, bits = 0;
    int maxv = (1 << to_bits) - 1;
    int max_acc = (1 << (from_bits + to_bits - 1)) - 1;
    std::vector<int> ret;
    for (int value : data) {
        if (value < 0 || (value >> from_bits)) return {};
        acc = ((acc << from_bits) | value) & max_acc;
        bits += from_bits;
        while (bits >= to_bits) {
            bits -= to_bits;
            ret.push_back((acc >> bits) & maxv);
        }
    }
    if (pad && bits) ret.push_back((acc << (to_bits - bits)) & maxv);
    return ret;
}

static std::tuple<std::string, std::vector<int>, bool> bech32_decode(const std::string &bech) {
    for (char c : bech) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc < 33 || uc > 126) return {"", {}, false};
    }
    bool has_lower = false, has_upper = false;
    for (char c : bech) {
        if (c >= 'a' && c <= 'z') has_lower = true;
        if (c >= 'A' && c <= 'Z') has_upper = true;
    }
    if (has_lower && has_upper) return {"", {}, false};
    std::string s = bech;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    size_t pos = s.rfind('1');
    if (pos < 1 || pos + 7 > s.size()) return {"", {}, false};
    std::string hrp = s.substr(0, pos);
    std::vector<int> data;
    const std::string BECH32_CHARSET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
    for (char c : s.substr(pos + 1)) {
        size_t idx = BECH32_CHARSET.find(c);
        if (idx == std::string::npos) return {"", {}, false};
        data.push_back(static_cast<int>(idx));
    }
    bool valid = bech32_verify_checksum(hrp, data) >= 0;
    data.resize(data.size() - 6);
    return {hrp, data, valid};
}

// ---------------------------------------------------------------------------
// Base58
// ---------------------------------------------------------------------------
static std::vector<uint8_t> base58_decode(const std::string &s) {
    const char *alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    int map[256];
    std::fill(std::begin(map), std::end(map), -1);
    for (int i = 0; i < 58; ++i) map[static_cast<unsigned char>(alphabet[i])] = i;

    std::vector<uint8_t> result;
    for (char c : s) {
        int carry = map[static_cast<unsigned char>(c)];
        if (carry == -1) return {};
        for (size_t i = 0; i < result.size(); ++i) {
            carry += 58 * result[i];
            result[i] = carry % 256;
            carry /= 256;
        }
        while (carry > 0) {
            result.push_back(carry % 256);
            carry /= 256;
        }
    }
    for (char c : s) {
        if (c == '1') result.push_back(0);
        else break;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// ---------------------------------------------------------------------------
// Address -> scriptPubKey
// ---------------------------------------------------------------------------
static std::vector<uint8_t> address_to_scriptpubkey(const std::string &addr) {
    std::string lower = addr;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower.rfind("be1", 0) == 0 || lower.rfind("tb1", 0) == 0 || lower.rfind("bcrt1", 0) == 0) {
        auto [hrp, data, valid] = bech32_decode(lower);
        if (data.empty()) throw std::runtime_error("Invalid bech32 address");
        if (!valid) std::cerr << "WARNING: bech32 checksum mismatch for " << addr << " -- using anyway\n";
        if (data.size() < 1) throw std::runtime_error("Invalid bech32 address (no data)");
        int witness_version = data[0];
        std::vector<int> program_chars(data.begin() + 1, data.end());
        auto program = bech32_convert_bits(program_chars, 5, 8, true);
        if (program.empty()) throw std::runtime_error("Invalid bech32 address (convert failed)");
        int total_bits = static_cast<int>(program_chars.size()) * 5;
        int last_byte_bits = total_bits % 8;
        if (last_byte_bits != 0) program.back() >>= (8 - last_byte_bits);
        std::vector<uint8_t> witness_program;
        for (int v : program) witness_program.push_back(static_cast<uint8_t>(v));
        if (witness_version == 0) {
            if (witness_program.size() == 20) {
                std::vector<uint8_t> out = {0x00, 0x14};
                out.insert(out.end(), witness_program.begin(), witness_program.end());
                return out;
            } else if (witness_program.size() == 32) {
                std::vector<uint8_t> out = {0x00, 0x20};
                out.insert(out.end(), witness_program.begin(), witness_program.end());
                return out;
            }
        } else if (witness_version == 1 && witness_program.size() == 32) {
            std::vector<uint8_t> out = {0x51, 0x20};
            out.insert(out.end(), witness_program.begin(), witness_program.end());
            return out;
        }
    } else {
        auto decoded = base58_decode(addr);
        if (decoded.size() != 25) throw std::runtime_error("Invalid base58 address length");
        uint8_t prefix = decoded[0];
        if (prefix == 0x00 || prefix == 0x6f) {
            std::vector<uint8_t> out = {0x76, 0xa9, 0x14};
            out.insert(out.end(), decoded.begin() + 1, decoded.begin() + 21);
            out.push_back(0x88);
            out.push_back(0xac);
            return out;
        } else if (prefix == 0x05 || prefix == 0xc4) {
            std::vector<uint8_t> out = {0xa9, 0x14};
            out.insert(out.end(), decoded.begin() + 1, decoded.begin() + 21);
            out.push_back(0x87);
            return out;
        }
    }
    throw std::runtime_error("Unsupported address type");
}

// ---------------------------------------------------------------------------
// Transaction / Script helpers
// ---------------------------------------------------------------------------
static std::vector<uint8_t> write_compact_size(uint64_t n) {
    std::vector<uint8_t> out;
    if (n < 0xfd) {
        out.push_back(static_cast<uint8_t>(n));
    } else if (n <= 0xffff) {
        out.push_back(0xfd);
        out.push_back(n & 0xff);
        out.push_back((n >> 8) & 0xff);
    } else if (n <= 0xffffffff) {
        out.push_back(0xfe);
        out.push_back(n & 0xff);
        out.push_back((n >> 8) & 0xff);
        out.push_back((n >> 16) & 0xff);
        out.push_back((n >> 24) & 0xff);
    } else {
        out.push_back(0xff);
        for (int i = 0; i < 8; ++i) out.push_back((n >> (8*i)) & 0xff);
    }
    return out;
}

static void push_u32_le(std::vector<uint8_t> &v, uint32_t x) {
    v.push_back(x & 0xff); v.push_back((x >> 8) & 0xff);
    v.push_back((x >> 16) & 0xff); v.push_back((x >> 24) & 0xff);
}

static void push_u64_le(std::vector<uint8_t> &v, uint64_t x) {
    for (int i = 0; i < 8; ++i) v.push_back((x >> (8*i)) & 0xff);
}

static std::vector<uint8_t> script_num(int64_t n) {
    if (n == -1) return {0x4f};
    if (n == 0) return {0x00};
    if (n >= 1 && n <= 16) return {static_cast<uint8_t>(0x50 + n)};
    bool neg = n < 0;
    uint64_t v = static_cast<uint64_t>(neg ? -n : n);
    std::vector<uint8_t> result;
    while (v > 0) {
        result.push_back(v & 0xff);
        v >>= 8;
    }
    if (result.back() & 0x80) {
        if (neg) result.push_back(0x80);
        else result.push_back(0x00);
    } else if (neg) {
        result.back() |= 0x80;
    }
    result.insert(result.begin(), static_cast<uint8_t>(result.size()));
    return result;
}

// ---------------------------------------------------------------------------
// Config
// ---------------------------------------------------------------------------
struct Config {
    std::string rpc_url = "http://127.0.0.1:8332";
    std::string rpc_user = "user";
    std::string rpc_password = "password";
    std::string mining_address;
    int threads = 4;
    std::vector<uint8_t> script_pubkey;

    void load(const std::string &path) {
        std::ifstream f(path);
        if (!f.is_open()) {
            std::cerr << "Warning: cannot open " << path << ", using defaults.\n";
            return;
        }
        std::string json((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        rpc_url = extract_json_string(json, "url");
        rpc_user = extract_json_string(json, "user");
        rpc_password = extract_json_string(json, "password");
        mining_address = extract_json_string(json, "address");
        int t = static_cast<int>(extract_json_int(json, "threads"));
        if (t > 0) threads = t;

        // fallback to nested objects (e.g. {"rpc": {"url": ...}, "mining": {"address": ...}})
        if (rpc_url.empty() || rpc_user.empty() || rpc_password.empty()) {
            size_t rpc_pos = json.find("\"rpc\"");
            if (rpc_pos != std::string::npos) {
                size_t brace = json.find('{', rpc_pos);
                if (brace != std::string::npos) {
                    size_t end = json.find('}', brace);
                    if (end != std::string::npos) {
                        std::string rpc_obj = json.substr(brace, end - brace + 1);
                        if (rpc_url.empty()) rpc_url = extract_json_string(rpc_obj, "url");
                        if (rpc_user.empty()) rpc_user = extract_json_string(rpc_obj, "user");
                        if (rpc_password.empty()) rpc_password = extract_json_string(rpc_obj, "password");
                    }
                }
            }
        }
        if (mining_address.empty()) {
            size_t mining_pos = json.find("\"mining\"");
            if (mining_pos != std::string::npos) {
                size_t brace = json.find('{', mining_pos);
                if (brace != std::string::npos) {
                    size_t end = json.find('}', brace);
                    if (end != std::string::npos) {
                        std::string mining_obj = json.substr(brace, end - brace + 1);
                        if (mining_address.empty()) mining_address = extract_json_string(mining_obj, "address");
                        if (t <= 0) {
                            int t2 = static_cast<int>(extract_json_int(mining_obj, "threads"));
                            if (t2 > 0) threads = t2;
                        }
                    }
                }
            }
        }

        if (!mining_address.empty()) {
            script_pubkey = address_to_scriptpubkey(mining_address);
        }
    }
};

// ---------------------------------------------------------------------------
// Block template parsing
// ---------------------------------------------------------------------------
struct BlockTemplate {
    int version = 1;
    std::string prev_blockhash;
    uint32_t curtime = 0;
    uint32_t bits = 0;
    int64_t height = 0;
    int64_t coinbasevalue = 0;
    std::string default_witness_commitment;
    bool has_witness_commitment = false;
};

struct TxInfo { std::string txid; std::string data; };

static BlockTemplate parse_template(const std::string &json) {
    BlockTemplate tmpl;
    tmpl.version = static_cast<int>(extract_json_int(json, "version"));
    tmpl.prev_blockhash = extract_json_string(json, "previousblockhash");
    tmpl.curtime = static_cast<uint32_t>(extract_json_int(json, "curtime"));
    tmpl.height = extract_json_int(json, "height");
    tmpl.coinbasevalue = extract_json_int(json, "coinbasevalue");
    std::string bits_str = extract_json_string(json, "bits");
    if (!bits_str.empty()) tmpl.bits = static_cast<uint32_t>(std::stoul(bits_str, nullptr, 16));
    tmpl.has_witness_commitment = json.find("\"default_witness_commitment\"") != std::string::npos;
    if (tmpl.has_witness_commitment) tmpl.default_witness_commitment = extract_json_string(json, "default_witness_commitment");
    return tmpl;
}

static std::vector<TxInfo> parse_transactions(const std::string &json) {
    std::vector<TxInfo> result;
    size_t pos = json.find("\"transactions\"");
    if (pos == std::string::npos) return result;
    size_t arr_start = json.find('[', pos);
    if (arr_start == std::string::npos) return result;
    size_t i = arr_start + 1;
    int depth = 1;
    while (i < json.size() && depth > 0) {
        if (json[i] == '[') depth++;
        else if (json[i] == ']') depth--;
        if (depth == 1 && json[i] == '{') {
            size_t obj_end = json.find('}', i);
            if (obj_end == std::string::npos) break;
            std::string obj = json.substr(i, obj_end - i + 1);
            std::string txid = extract_json_string(obj, "txid");
            std::string data = extract_json_string(obj, "data");
            if (!txid.empty() && !data.empty()) result.push_back({txid, data});
            i = obj_end + 1;
        } else {
            i++;
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Coinbase
// ---------------------------------------------------------------------------
struct CoinbaseResult {
    std::vector<uint8_t> tx;
    std::vector<uint8_t> tx_no_witness;
    std::vector<uint8_t> txid;
};

static CoinbaseResult build_coinbase_tx(const BlockTemplate &tmpl, const std::vector<uint8_t> &script_pubkey) {
    std::vector<uint8_t> script_sig = script_num(tmpl.height);
    if (script_sig.size() < 2) script_sig.push_back(0x00);

    std::vector<uint8_t> inputs(32, 0);
    push_u32_le(inputs, 0xFFFFFFFF);
    auto ss = write_compact_size(script_sig.size());
    inputs.insert(inputs.end(), ss.begin(), ss.end());
    inputs.insert(inputs.end(), script_sig.begin(), script_sig.end());
    push_u32_le(inputs, 0xFFFFFFFE);

    std::vector<uint8_t> outputs;
    push_u64_le(outputs, static_cast<uint64_t>(tmpl.coinbasevalue));
    auto spk_size = write_compact_size(script_pubkey.size());
    outputs.insert(outputs.end(), spk_size.begin(), spk_size.end());
    outputs.insert(outputs.end(), script_pubkey.begin(), script_pubkey.end());

    int output_count = 1;
    if (tmpl.has_witness_commitment && !tmpl.default_witness_commitment.empty()) {
        auto wc_script = hex_decode(tmpl.default_witness_commitment);
        push_u64_le(outputs, 0);
        auto wc_size = write_compact_size(wc_script.size());
        outputs.insert(outputs.end(), wc_size.begin(), wc_size.end());
        outputs.insert(outputs.end(), wc_script.begin(), wc_script.end());
        output_count = 2;
    }

    std::vector<uint8_t> tx_no_witness;
    push_u32_le(tx_no_witness, 2);
    auto ic = write_compact_size(1);
    tx_no_witness.insert(tx_no_witness.end(), ic.begin(), ic.end());
    tx_no_witness.insert(tx_no_witness.end(), inputs.begin(), inputs.end());
    auto oc = write_compact_size(output_count);
    tx_no_witness.insert(tx_no_witness.end(), oc.begin(), oc.end());
    tx_no_witness.insert(tx_no_witness.end(), outputs.begin(), outputs.end());
    push_u32_le(tx_no_witness, static_cast<uint32_t>(tmpl.height - 1));

    std::vector<uint8_t> tx = tx_no_witness;
    tx.insert(tx.begin() + 4, {0x00, 0x01});
    std::vector<uint8_t> witness;
    auto ws1 = write_compact_size(1);
    witness.insert(witness.end(), ws1.begin(), ws1.end());
    auto ws32 = write_compact_size(32);
    witness.insert(witness.end(), ws32.begin(), ws32.end());
    witness.insert(witness.end(), 32, 0);
    tx.insert(tx.end() - 4, witness.begin(), witness.end());

    std::vector<uint8_t> txid(32);
    sha256d(tx_no_witness.data(), tx_no_witness.size(), txid.data());

    return {tx, tx_no_witness, txid};
}

// ---------------------------------------------------------------------------
// Merkle root
// ---------------------------------------------------------------------------
static std::vector<uint8_t> compute_merkle_root(std::vector<std::vector<uint8_t>> hashes) {
    if (hashes.empty()) return std::vector<uint8_t>(32, 0);
    while (hashes.size() > 1) {
        if (hashes.size() % 2 == 1) hashes.push_back(hashes.back());
        std::vector<std::vector<uint8_t>> next;
        for (size_t i = 0; i < hashes.size(); i += 2) {
            std::vector<uint8_t> cat;
            cat.insert(cat.end(), hashes[i].begin(), hashes[i].end());
            cat.insert(cat.end(), hashes[i+1].begin(), hashes[i+1].end());
            std::vector<uint8_t> h(32);
            sha256d(cat.data(), cat.size(), h.data());
            next.push_back(std::move(h));
        }
        hashes = std::move(next);
    }
    return hashes[0];
}

// ---------------------------------------------------------------------------
// Block assembly
// ---------------------------------------------------------------------------
static std::vector<uint8_t> assemble_block(const BlockTemplate &tmpl, uint32_t nonce,
                                            const std::vector<uint8_t> &coinbase_tx,
                                            const std::vector<TxInfo> &transactions,
                                            const std::vector<uint8_t> &merkle_root_le) {
    std::vector<uint8_t> header(80);
    header[0] = tmpl.version & 0xff;
    header[1] = (tmpl.version >> 8) & 0xff;
    header[2] = (tmpl.version >> 16) & 0xff;
    header[3] = (tmpl.version >> 24) & 0xff;

    auto prev = hex_decode(tmpl.prev_blockhash);
    std::reverse(prev.begin(), prev.end());
    std::copy(prev.begin(), prev.end(), header.begin() + 4);

    std::copy(merkle_root_le.begin(), merkle_root_le.end(), header.begin() + 36);

    header[68] = tmpl.curtime & 0xff;
    header[69] = (tmpl.curtime >> 8) & 0xff;
    header[70] = (tmpl.curtime >> 16) & 0xff;
    header[71] = (tmpl.curtime >> 24) & 0xff;

    header[72] = tmpl.bits & 0xff;
    header[73] = (tmpl.bits >> 8) & 0xff;
    header[74] = (tmpl.bits >> 16) & 0xff;
    header[75] = (tmpl.bits >> 24) & 0xff;

    header[76] = nonce & 0xff;
    header[77] = (nonce >> 8) & 0xff;
    header[78] = (nonce >> 16) & 0xff;
    header[79] = (nonce >> 24) & 0xff;

    std::vector<uint8_t> block = header;
    auto tx_count = write_compact_size(1 + transactions.size());
    block.insert(block.end(), tx_count.begin(), tx_count.end());
    block.insert(block.end(), coinbase_tx.begin(), coinbase_tx.end());
    for (const auto &tx : transactions) {
        auto txdata = hex_decode(tx.data);
        block.insert(block.end(), txdata.begin(), txdata.end());
    }
    return block;
}

// ---------------------------------------------------------------------------
// Mining
// ---------------------------------------------------------------------------
static std::atomic<bool> g_stop{false};
static std::atomic<bool> g_found{false};
static std::atomic<uint32_t> g_nonce_found{0};

static void bits_to_target(uint32_t n_bits, uint8_t target[32]) {
    uint32_t exponent = (n_bits >> 24) & 0xff;
    uint32_t coefficient = n_bits & 0x007fffff;
    std::memset(target, 0, 32);
    target[exponent - 3] = (coefficient >> 0) & 0xff;
    if (exponent >= 2) target[exponent - 2] = (coefficient >> 8) & 0xff;
    if (exponent >= 1) target[exponent - 1] = (coefficient >> 16) & 0xff;
}

static bool hash_le_target(const uint8_t hash[32], const uint8_t target[32]) {
    for (int i = 31; i >= 0; --i) {
        if (hash[i] < target[i]) return true;
        if (hash[i] > target[i]) return false;
    }
    return true;
}

static void mine_thread(int tid, const uint8_t header[76], const uint8_t target[32], uint32_t start_nonce, uint32_t step) {
    uint8_t hash[32];
    uint8_t local_header[80];
    std::memcpy(local_header, header, 76);
    for (uint32_t nonce = start_nonce; nonce < 0xffffffff && !g_found.load(); nonce += step) {
        local_header[76] = nonce & 0xff;
        local_header[77] = (nonce >> 8) & 0xff;
        local_header[78] = (nonce >> 16) & 0xff;
        local_header[79] = (nonce >> 24) & 0xff;
        sha256d(local_header, 80, hash);
        if (hash_le_target(hash, target)) {
            bool expected = false;
            if (g_found.compare_exchange_strong(expected, true)) {
                g_nonce_found.store(nonce);
                uint8_t rev[32];
                std::memcpy(rev, hash, 32);
                std::reverse(std::begin(rev), std::end(rev));
                std::cout << "\n[thread " << tid << "] FOUND nonce=" << nonce << " hash=" << hex_encode(rev, 32) << "\n";
            }
            return;
        }
        if ((nonce % 1000000) == 0 && tid == 0) {
            std::cout << "  [thread 0] tried " << nonce << " nonces...\n";
        }
    }
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    std::string config_path = (argc > 1) ? argv[1] : "config.json";
    Config cfg;
    cfg.load(config_path);

    if (cfg.mining_address.empty()) {
        std::cerr << "ERROR: No payout address specified. Set 'address' in config.json\n";
        return 1;
    }
    if (cfg.script_pubkey.empty()) {
        std::cerr << "ERROR: Failed to derive scriptPubKey from address.\n";
        return 1;
    }

    RpcClient rpc(cfg.rpc_url, cfg.rpc_user, cfg.rpc_password);

    std::cout << "Elektron Net Miner (C++)\n";
    std::cout << "RPC: " << cfg.rpc_url << "\n";
    std::cout << "Address: " << cfg.mining_address << "\n";
    std::cout << "Threads: " << cfg.threads << "\n";

    while (true) {
        try {
            std::cout << "\nFetching block template...\n";
            std::string tmpl_json = rpc.call("getblocktemplate", {"{\"rules\":[\"segwit\"]}"});

            if (tmpl_json.find("\"error\":") != std::string::npos && tmpl_json.find("\"error\":null") == std::string::npos) {
                std::cerr << "RPC error: " << tmpl_json << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            BlockTemplate tmpl = parse_template(tmpl_json);
            if (tmpl.bits == 0) {
                std::cerr << "Failed to parse block template.\n";
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            std::cout << "Height: " << tmpl.height << "\n";
            std::cout << "Target bits: " << std::hex << tmpl.bits << std::dec << "\n";

            auto coinbase = build_coinbase_tx(tmpl, cfg.script_pubkey);
            auto transactions = parse_transactions(tmpl_json);

            // Merkle root
            std::vector<std::vector<uint8_t>> tx_hashes;
            tx_hashes.push_back(coinbase.txid);
            for (const auto &tx : transactions) {
                auto hash = hex_decode(tx.txid);
                std::reverse(hash.begin(), hash.end());
                tx_hashes.push_back(std::move(hash));
            }
            auto merkle_root = compute_merkle_root(tx_hashes);

            // Build header (without nonce)
            uint8_t raw_header[76];
            {
                std::vector<uint8_t> header(80);
                header[0] = tmpl.version & 0xff;
                header[1] = (tmpl.version >> 8) & 0xff;
                header[2] = (tmpl.version >> 16) & 0xff;
                header[3] = (tmpl.version >> 24) & 0xff;
                auto prev = hex_decode(tmpl.prev_blockhash);
                std::reverse(prev.begin(), prev.end());
                std::copy(prev.begin(), prev.end(), header.begin() + 4);
                std::copy(merkle_root.begin(), merkle_root.end(), header.begin() + 36);
                header[68] = tmpl.curtime & 0xff;
                header[69] = (tmpl.curtime >> 8) & 0xff;
                header[70] = (tmpl.curtime >> 16) & 0xff;
                header[71] = (tmpl.curtime >> 24) & 0xff;
                header[72] = tmpl.bits & 0xff;
                header[73] = (tmpl.bits >> 8) & 0xff;
                header[74] = (tmpl.bits >> 16) & 0xff;
                header[75] = (tmpl.bits >> 24) & 0xff;
                std::copy(header.begin(), header.begin() + 76, raw_header);
            }

            uint8_t target[32];
            bits_to_target(tmpl.bits, target);
            std::cout << "Target: " << hex_encode(target, 32) << "\n";

            g_found = false;
            g_nonce_found = 0;

            std::vector<std::thread> threads;
            for (int i = 0; i < cfg.threads; ++i) {
                threads.emplace_back(mine_thread, i, raw_header, target, i, static_cast<uint32_t>(cfg.threads));
            }
            for (auto &t : threads) t.join();

            if (g_found.load()) {
                uint32_t nonce = g_nonce_found.load();
                std::cout << "Submitting block with nonce=" << nonce << "\n";
                auto block = assemble_block(tmpl, nonce, coinbase.tx, transactions, merkle_root);
                std::string block_hex = hex_encode(block.data(), block.size());

                // local sanity check
                uint8_t hash_check[32];
                sha256d(block.data(), 80, hash_check);
                std::reverse(std::begin(hash_check), std::end(hash_check));
                std::cout << "Block hash (local): " << hex_encode(hash_check, 32) << "\n";

                std::string result = rpc.call("submitblock", {"\"" + block_hex + "\""});
                std::cout << "Submit result: " << result << "\n";

                bool accepted = result.empty() || result == "null" || result.find("\"result\":null") != std::string::npos;
                if (!accepted) {
                    std::cerr << "ERROR: Block was rejected by the node. Waiting 10s before retry to avoid waste.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                } else {
                    std::cout << "Block accepted. Waiting for next height before continuing...\n";
                    int64_t submitted_height = tmpl.height;
                    for (int wait = 0; wait < 60; ++wait) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        try {
                            std::string info = rpc.call("getblockchaininfo", {});
                            int64_t current_height = extract_json_int(info, "blocks");
                            if (current_height > submitted_height) {
                                std::cout << "New height detected: " << current_height << "\n";
                                break;
                            }
                        } catch (...) {}
                    }
                    continue;
                }
            } else {
                std::cout << "No nonce found (template expired?).\n";
            }
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        std::cout << "Waiting before next round...\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
