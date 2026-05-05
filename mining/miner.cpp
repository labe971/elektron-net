/**
 * Standalone CPU miner for Elektron Net (C++).
 *
 * Conforms to the standard Bitcoin mining protocol:
 * - Fetches block templates via RPC (getblocktemplate)
 * - Mines using multi-threaded CPU SHA-256d
 * - Submits solved blocks via RPC (submitblock)
 *
 * Build:
 *   mkdir build && cd build && cmake .. && cmake --build .
 *
 * Usage:
 *   ./elektron_miner ../config.json
 */

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <curl/curl.h>

#include <openssl/evp.h>
#include <openssl/sha.h>

#if defined(_WIN32)
#include <winsock2.h>
#endif

// ---------------------------------------------------------------------------
// SHA-256d helpers
// ---------------------------------------------------------------------------

static void sha256d(const uint8_t *data, size_t len, uint8_t out[32]) {
    uint8_t hash1[32];
    SHA256(data, len, hash1);
    SHA256(hash1, 32, out);
}

static bool hash_le_target(const uint8_t hash[32], const uint8_t target[32]) {
    // Compare in little-endian order (bytes are already LE)
    for (int i = 31; i >= 0; --i) {
        if (hash[i] < target[i]) return true;
        if (hash[i] > target[i]) return false;
    }
    return true; // equal
}

static std::string hex_encode(const uint8_t *data, size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i)
        oss << std::setw(2) << static_cast<int>(data[i]);
    return oss.str();
}

static std::vector<uint8_t> hex_decode(const std::string &hex) {
    std::vector<uint8_t> out;
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        std::string byte = hex.substr(i, 2);
        out.push_back(static_cast<uint8_t>(std::stoul(byte, nullptr, 16)));
    }
    return out;
}

// Reverse byte order (LE <-> BE for 256-bit values)
static void reverse256(uint8_t *data) {
    for (int i = 0; i < 16; ++i) std::swap(data[i], data[31 - i]);
}

// ---------------------------------------------------------------------------
// JSON helpers (minimal, no external dependency)
// ---------------------------------------------------------------------------

static std::string json_escape(const std::string &s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\b') out += "\\b";
        else if (c == '\f') out += "\\f";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else out += c;
    }
    return out;
}

static std::string json_rpc(const std::string &method,
                            const std::vector<std::string> &params) {
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
    return std::stoll(json.substr(pos));
}

// ---------------------------------------------------------------------------
// HTTP / RPC
// ---------------------------------------------------------------------------

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

class RpcClient {
public:
    std::string url;
    std::string user;
    std::string password;

    RpcClient(const std::string &u, const std::string &usr, const std::string &pwd)
        : url(u), user(usr), password(pwd) {}

    std::string call(const std::string &method,
                     const std::vector<std::string> &params = {}) {
        CURL *curl = curl_easy_init();
        if (!curl) throw std::runtime_error("curl init failed");

        std::string payload = json_rpc(method, params);
        std::string readBuffer;
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string creds = user + ":" + password;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERPWD, creds.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK)
            throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(res));

        return readBuffer;
    }
};

// ---------------------------------------------------------------------------
// Config
// ---------------------------------------------------------------------------

struct Config {
    std::string rpc_url = "http://127.0.0.1:6032";
    std::string rpc_user = "user";
    std::string rpc_password = "password";
    std::string mining_address;
    int threads = 4;
    int target_spacing = 60;
    bool pool_enabled = false;
    std::string pool_url;
    std::string pool_user;
    std::string pool_password;

    void load(const std::string &path) {
        std::ifstream f(path);
        if (!f.is_open()) {
            std::cerr << "Warning: cannot open " << path << ", using defaults.\n";
            return;
        }
        std::string json((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());

        rpc_url = extract_json_string(json, "url");
        rpc_user = extract_json_string(json, "user");
        rpc_password = extract_json_string(json, "password");
        mining_address = extract_json_string(json, "address");
        threads = static_cast<int>(extract_json_int(json, "threads"));
        target_spacing = static_cast<int>(extract_json_int(json, "target_spacing"));
        pool_enabled = json.find("\"enabled\":true") != std::string::npos;
        pool_url = extract_json_string(json, "url");
        pool_user = extract_json_string(json, "user");
        pool_password = extract_json_string(json, "password");

        if (threads <= 0) threads = 4;
    }
};

// ---------------------------------------------------------------------------
// Mining
// ---------------------------------------------------------------------------

static std::atomic<bool> g_stop{false};
static std::atomic<uint32_t> g_nonce_found{0};
static std::atomic<bool> g_found{false};

struct BlockTemplate {
    int version = 1;
    std::string prev_blockhash;
    std::string merkleroot;
    uint32_t curtime = 0;
    uint32_t bits = 0;
    std::string coinbase_tx_hex;
    std::vector<std::string> tx_hexes;
};

static uint32_t bits_to_target(uint32_t n_bits, uint8_t target[32]) {
    uint32_t exponent = (n_bits >> 24) & 0xff;
    uint32_t coefficient = n_bits & 0x007fffff;
    std::memset(target, 0, 32);
    target[exponent - 3] = (coefficient >> 0) & 0xff;
    if (exponent >= 2) target[exponent - 2] = (coefficient >> 8) & 0xff;
    if (exponent >= 1) target[exponent - 1] = (coefficient >> 16) & 0xff;
    return 0;
}

static void mine_thread(int tid, const uint8_t header[76],
                        const uint8_t target[32],
                        uint32_t start_nonce) {
    uint8_t hash[32];
    uint8_t local_header[80];
    std::memcpy(local_header, header, 76);

    for (uint32_t nonce = start_nonce; nonce < 0xffffffff && !g_found.load(); ++nonce) {
        local_header[76] = nonce & 0xff;
        local_header[77] = (nonce >> 8) & 0xff;
        local_header[78] = (nonce >> 16) & 0xff;
        local_header[79] = (nonce >> 24) & 0xff;

        sha256d(local_header, 80, hash);

        if (hash_le_target(hash, target)) {
            bool expected = false;
            if (g_found.compare_exchange_strong(expected, true)) {
                g_nonce_found.store(nonce);
                std::cout << "\n[thread " << tid << "] FOUND nonce=" << nonce
                          << " hash=" << hex_encode(hash, 32) << "\n";
            }
            return;
        }

        if ((nonce % 1000000) == 0 && tid == 0) {
            std::cout << "  [thread 0] tried " << nonce << " nonces...\n";
        }
    }
}

static BlockTemplate parse_template(const std::string &json) {
    BlockTemplate tmpl;
    tmpl.version = static_cast<int>(extract_json_int(json, "version"));
    tmpl.prev_blockhash = extract_json_string(json, "previousblockhash");
    tmpl.merkleroot = extract_json_string(json, "merkleroot");
    tmpl.curtime = static_cast<uint32_t>(extract_json_int(json, "curtime"));

    std::string bits_str = extract_json_string(json, "bits");
    if (!bits_str.empty()) tmpl.bits = static_cast<uint32_t>(std::stoul(bits_str, nullptr, 16));

    // Extract coinbase transaction hex from the first transaction
    size_t coinbase_pos = json.find("\"coinbasetxn\"");
    if (coinbase_pos != std::string::npos) {
        tmpl.coinbase_tx_hex = extract_json_string(json.substr(coinbase_pos, 2000), "data");
    } else {
        size_t txs_pos = json.find("\"transactions\"");
        if (txs_pos != std::string::npos) {
            size_t data_pos = json.find("\"data\"", txs_pos);
            if (data_pos != std::string::npos) {
                size_t q1 = json.find('"', data_pos + 6);
                size_t q2 = json.find('"', q1 + 1);
                if (q1 != std::string::npos && q2 != std::string::npos)
                    tmpl.coinbase_tx_hex = json.substr(q1 + 1, q2 - q1 - 1);
            }
        }
    }
    return tmpl;
}

static std::vector<uint8_t> build_header(const BlockTemplate &tmpl, uint32_t nonce) {
    std::vector<uint8_t> header(80);
    // version (LE)
    header[0] = tmpl.version & 0xff;
    header[1] = (tmpl.version >> 8) & 0xff;
    header[2] = (tmpl.version >> 16) & 0xff;
    header[3] = (tmpl.version >> 24) & 0xff;

    // prev block hash (LE)
    auto prev = hex_decode(tmpl.prev_blockhash);
    std::reverse(prev.begin(), prev.end());
    std::copy(prev.begin(), prev.end(), header.begin() + 4);

    // merkle root (LE)
    auto merkle = hex_decode(tmpl.merkleroot);
    std::reverse(merkle.begin(), merkle.end());
    std::copy(merkle.begin(), merkle.end(), header.begin() + 36);

    // timestamp (LE)
    header[68] = tmpl.curtime & 0xff;
    header[69] = (tmpl.curtime >> 8) & 0xff;
    header[70] = (tmpl.curtime >> 16) & 0xff;
    header[71] = (tmpl.curtime >> 24) & 0xff;

    // bits (LE)
    header[72] = tmpl.bits & 0xff;
    header[73] = (tmpl.bits >> 8) & 0xff;
    header[74] = (tmpl.bits >> 16) & 0xff;
    header[75] = (tmpl.bits >> 24) & 0xff;

    // nonce (LE)
    header[76] = nonce & 0xff;
    header[77] = (nonce >> 8) & 0xff;
    header[78] = (nonce >> 16) & 0xff;
    header[79] = (nonce >> 24) & 0xff;

    return header;
}

static std::string assemble_block(const BlockTemplate &tmpl, uint32_t nonce,
                                  const std::vector<std::string> &extra_txs) {
    auto header = build_header(tmpl, nonce);
    std::string block = hex_encode(header.data(), header.size());

    // transaction count (varint)
    size_t tx_count = 1 + extra_txs.size();
    if (tx_count < 0xfd) {
        block += hex_encode(reinterpret_cast<const uint8_t *>(&tx_count), 1);
    } else {
        uint8_t vi[3] = {0xfd, static_cast<uint8_t>(tx_count), static_cast<uint8_t>(tx_count >> 8)};
        block += hex_encode(vi, 3);
    }

    block += tmpl.coinbase_tx_hex;
    for (const auto &tx : extra_txs) block += tx;
    return block;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main(int argc, char *argv[]) {
#if defined(_WIN32)
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    std::string config_path = (argc > 1) ? argv[1] : "config.json";
    Config cfg;
    cfg.load(config_path);

    std::cout << "Elektron Net Miner (C++)\n";
    std::cout << "RPC: " << cfg.rpc_url << "\n";
    std::cout << "Threads: " << cfg.threads << "\n";

    RpcClient rpc(cfg.rpc_url, cfg.rpc_user, cfg.rpc_password);
    curl_global_init(CURL_GLOBAL_DEFAULT);

    while (true) {
        try {
            std::cout << "\nFetching block template...\n";
            std::string tmpl_json = rpc.call("getblocktemplate", {"{\"rules\":[\"segwit\"]}"});

            if (tmpl_json.find("\"error\":") != std::string::npos &&
                tmpl_json.find("\"error\":null") == std::string::npos) {
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

            std::cout << "Target bits: " << std::hex << tmpl.bits << std::dec << "\n";

            uint8_t target[32];
            bits_to_target(tmpl.bits, target);
            std::cout << "Target: " << hex_encode(target, 32) << "\n";

            auto header = build_header(tmpl, 0);
            uint8_t raw_header[76];
            std::copy(header.begin(), header.end(), raw_header);

            g_found = false;
            g_nonce_found = 0;

            std::vector<std::thread> threads;
            for (int i = 0; i < cfg.threads; ++i) {
                threads.emplace_back(mine_thread, i, raw_header, target, i);
            }

            for (auto &t : threads) t.join();

            if (g_found.load()) {
                uint32_t nonce = g_nonce_found.load();
                std::cout << "Submitting block with nonce=" << nonce << "\n";
                std::string block_hex = assemble_block(tmpl, nonce, {});
                std::string result = rpc.call("submitblock", {"\"" + block_hex + "\""});
                std::cout << "Submit result: " << result << "\n";
            } else {
                std::cout << "No nonce found (template expired?).\n";
            }
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        std::cout << "Press Ctrl+C to stop, or waiting 1s before next round...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    curl_global_cleanup();
#if defined(_WIN32)
    WSACleanup();
#endif
    return 0;
}
